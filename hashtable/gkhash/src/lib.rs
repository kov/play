use std::hash::{DefaultHasher, Hash, Hasher};

pub struct GkHash<K, V> {
    pub(crate) buckets: Box<[Entry<K, V>]>,
    pub(crate) used: usize,
}

pub enum Entry<K, V> {
    Empty,
    Node(Node<K, V>),
}

pub struct Node<K, V> {
    pub(crate) key: K,
    pub(crate) value: V,
    pub(crate) next: Box<Entry<K, V>>,
}

impl<K: Hash + Eq, V> GkHash<K, V> {
    fn new() -> Self {
        GkHash {
            buckets: Box::new([]),
            used: 0,
        }
    }

    fn maybe_grow(&mut self) {
        // Figure out if we do not need to grow, and return early.
        if self.buckets.len() > 0 {
            let usage = self.used as f64 / self.buckets.len() as f64;
            if usage < 0.7 {
                return;
            }
        }

        // Minimum of 16 slots as we may be starting from 0, and that gives
        // us a reasonable size to avoid 100% collisions from the start.
        let new_size = (self.buckets.len() * 2).max(16);
        let old_buckets = std::mem::replace(
            &mut self.buckets,
            (0..new_size)
                .map(|_| Entry::Empty)
                .collect::<Vec<_>>()
                .into_boxed_slice(),
        );

        // Move data from old buckets to the new.
        for mut bucket in old_buckets {
            let index;
            match &mut bucket {
                Entry::Empty => continue,
                Entry::Node(node) => {
                    index = self.bucket_index(&node.key);

                    let mut sub_bucket = std::mem::replace(&mut node.next, Box::new(Entry::Empty));
                    while let Entry::Node(sub_node) = &mut *sub_bucket {
                        let next_bucket =
                            std::mem::replace(&mut sub_node.next, Box::new(Entry::Empty));

                        let index = self.bucket_index(&sub_node.key);
                        let mut target = &mut self.buckets[index];
                        while let Entry::Node(target_node) = target {
                            target = &mut target_node.next;
                        }
                        *target = *sub_bucket;

                        sub_bucket = next_bucket;
                    }
                }
            }

            let mut target = &mut self.buckets[index];
            while let Entry::Node(target_node) = target {
                target = &mut target_node.next;
            }
            *target = bucket;
        }
    }

    fn bucket_index(&self, key: &K) -> usize {
        let mut hasher = DefaultHasher::new();
        key.hash(&mut hasher);
        (hasher.finish() % self.buckets.len() as u64) as usize
    }

    fn get(&self, key: &K) -> Option<&V> {
        let index = self.bucket_index(key);
        let mut bucket = &self.buckets[index];
        while let Entry::Node(node) = bucket {
            if node.key == *key {
                return Some(&node.value);
            }
            bucket = &node.next;
        }
        None
    }

    fn insert(&mut self, key: K, value: V) -> Option<V> {
        self.maybe_grow();

        let index = self.bucket_index(&key);
        let mut bucket = &mut self.buckets[index];
        while let Entry::Node(node) = bucket {
            if node.key == key {
                return Some(std::mem::replace(&mut node.value, value));
            }
            bucket = &mut node.next;
        }

        *bucket = Entry::Node(Node {
            key,
            value,
            next: Box::new(Entry::Empty),
        });

        self.used += 1;

        None
    }
}

pub struct GkHashIter<'a, K, V> {
    buckets: &'a [Entry<K, V>],
    index: usize,
    entry: Option<&'a Entry<K, V>>,
}

impl<'a, K, V> GkHash<K, V> {
    fn iter(&'a self) -> GkHashIter<'a, K, V> {
        let (index, entry) = match self
            .buckets
            .iter()
            .position(|b| matches!(b, Entry::Node(_)))
        {
            Some(index) => (index, Some(&self.buckets[index])),
            None => (0, None),
        };
        GkHashIter {
            buckets: &self.buckets,
            index,
            entry,
        }
    }
}

impl<'a, K, V> Iterator for GkHashIter<'a, K, V> {
    type Item = (&'a K, &'a V);

    fn next(&mut self) -> Option<Self::Item> {
        match self.entry? {
            Entry::Empty => return None,
            Entry::Node(node) => {
                // Save references we'll return for this round
                let (key, value) = (&node.key, &node.value);

                // Try to find the next item, starting from our node linked list.
                if let Entry::Node(_) = &*node.next {
                    self.entry = Some(&node.next);
                } else {
                    self.index += 1;
                    self.entry = None;

                    // Try to find the next bucket with a node.
                    while self.index < self.buckets.len() {
                        if let Entry::Node(_) = &self.buckets[self.index] {
                            self.entry = Some(&self.buckets[self.index]);
                            break;
                        }

                        self.index += 1;
                    }
                };

                // Go ahead and return the ones we saved
                Some((key, value))
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_grow() {
        let mut map = GkHash::new();

        assert_eq!(map.buckets.len(), 0);
        assert_eq!(map.used, 0);

        let key = "kov".to_string();
        map.insert(key.clone(), 42isize);

        // We should now have buckets set up with the minimum 16 entries.
        assert_eq!(map.buckets.len(), 16);
        assert_eq!(map.used, 1);

        let mut hasher = DefaultHasher::new();
        key.hash(&mut hasher);
        let hash = hasher.finish();

        let index = (hash % map.buckets.len() as u64) as usize;
        let Entry::Node(node) = &map.buckets[index] else {
            panic!("Bucket was empty");
        };

        assert_eq!(node.key, key);
        assert_eq!(node.value, 42);
        assert!(matches!(*node.next, Entry::Empty));

        assert_eq!(map.get(&key).unwrap(), &42);
    }

    #[test]
    fn test_grow_again() {
        let mut map = GkHash::new();

        assert_eq!(map.buckets.len(), 0);
        assert_eq!(map.used, 0);

        (0..100).for_each(|i| {
            map.insert(format!("{i}"), i);
        });

        // 100 is >70% of 128, so we've had to grow to 256 by this point
        assert_eq!(map.buckets.len(), 256);
        assert_eq!(map.used, 100);

        assert_eq!(map.get(&"99".to_owned()).unwrap(), &99);
        assert!(map.get(&"100".to_owned()).is_none());

        let mut map_check = vec![false; 100];
        for (k, v) in map.iter() {
            assert!(!map_check[*v as usize]);
            map_check[*v as usize] = true;

            assert_eq!(k, &v.to_string());
        }

        // Test that all items showed up on the iterator.
        for b in map_check.into_iter() {
            assert_eq!(b, true);
        }
    }

    #[test]
    fn test_iter() {
        let mut map = GkHash::new();

        assert_eq!(map.buckets.len(), 0);
        assert_eq!(map.used, 0);

        (0..100).for_each(|i| {
            map.insert(format!("{i}"), i);
        });

        let mut map_check = vec![false; 100];
        for (k, v) in map.iter() {
            let i = *v as usize;
            assert!(!map_check[i]);
            map_check[i] = true;

            assert_eq!(k, &v.to_string());
        }

        // Test that all items showed up on the iterator.
        for b in map_check.into_iter() {
            assert_eq!(b, true);
        }
    }
}

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
        for bucket in old_buckets {
            match bucket {
                Entry::Empty => (),
                Entry::Node(ref node) => {
                    let index = self.bucket_index(&node.key);
                    self.buckets[index] = bucket;
                }
            }
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
    }
}

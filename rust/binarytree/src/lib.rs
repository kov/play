#![allow(unused)]
use std::cmp::Ordering;

struct BinaryTree<K, V> {
    root: Option<Box<Node<K, V>>>,
}

struct Node<K, V> {
    key: K,
    value: V,
    left: Option<Box<Node<K, V>>>,
    right: Option<Box<Node<K, V>>>,
}

impl<K, V> Node<K, V> {
    fn new(key: K, value: V) -> Box<Self> {
        Box::new(Node {
            key,
            value,
            left: None,
            right: None,
        })
    }
}

impl<K, V> BinaryTree<K, V> {
    fn new() -> Self {
        BinaryTree { root: None }
    }

    fn insert(&mut self, key: K, value: V)
    where
        K: Ord,
    {
        if self.root.is_none() {
            self.root = Some(Node::new(key, value));
            return;
        }

        self.at_key_mut(key, move |key, node| {
            if let Some(node) = node {
                assert!(key == node.key);
                node.value = value;
            } else {
                *node = Some(Node::new(key, value));
            }
        });
    }

    fn get(&self, key: &K) -> Option<&V>
    where
        K: Ord,
    {
        self.find_node(key).map(|n| &n.value)
    }

    fn find_node(&self, key: &'_ K) -> Option<&Node<K, V>>
    where
        K: Ord,
    {
        let mut node = self.root.as_ref();
        while let Some(n) = node {
            match key.cmp(&n.key) {
                Ordering::Less => node = n.left.as_ref(),
                Ordering::Greater => node = n.right.as_ref(),
                Ordering::Equal => return Some(n),
            }
        }
        None
    }

    fn at_key_mut(&mut self, key: K, func: impl FnOnce(K, &mut Option<Box<Node<K, V>>>))
    where
        K: Ord,
    {
        let mut node = &mut self.root;
        loop {
            if node.is_none() {
                func(key, node);
                return;
            }

            let ordering = key.cmp(&node.as_ref().unwrap().key);
            match ordering {
                Ordering::Less => node = &mut node.as_mut().unwrap().left,
                Ordering::Greater => node = &mut node.as_mut().unwrap().right,
                Ordering::Equal => {
                    func(key, node);
                    return;
                }
            }
        }
    }

    fn rotate_left(node: &mut Option<Box<Node<K, V>>>) {
        assert!(!node.is_none());
        if let Some(mut child) = node.as_mut().unwrap().right.take() {
            node.as_mut().unwrap().right = child.left.take();
            child.left = node.take();
            *node = Some(child);
        }
    }

    fn rotate_right(node: &mut Option<Box<Node<K, V>>>) {
        assert!(!node.is_none());
        if let Some(mut child) = node.as_mut().unwrap().left.take() {
            node.as_mut().unwrap().left = child.right.take();
            child.right = node.take();
            *node = Some(child);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basics() {
        let mut binary_tree = BinaryTree::new();
        binary_tree.insert(format!("kov"), 42u64);
        binary_tree.insert(format!("lala"), 13);
        assert_eq!(binary_tree.get(&format!("kov")), Some(&42));
        assert_eq!(binary_tree.get(&format!("lala")), Some(&13));
        assert_eq!(binary_tree.get(&format!("qwe")), None);
    }

    #[test]
    fn test_rotate_left_does_nothing() {
        let mut binary_tree = BinaryTree::new();

        // This should create a very imbalanced tree, very left-leaning.
        //       3
        //     2
        //   1
        // 0
        for i in (0..4).rev() {
            binary_tree.insert(i, i);
        }

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        for i in (0..3).rev() {
            node = node.left.as_ref().unwrap();
            assert_eq!(node.key, i);
            assert!(node.right.is_none());
        }

        for i in (0..4) {
            // Such that rotating left on any of the nodes...
            binary_tree.at_key_mut(i, |_, node| {
                assert_eq!(node.as_ref().unwrap().key, i);
                BinaryTree::rotate_left(node);
            });

            /// ... will do nothing
            let mut node = binary_tree.root.as_ref().unwrap();
            assert_eq!(node.key, 3);
            assert!(node.right.is_none());

            for i in (0..3).rev() {
                node = node.left.as_ref().unwrap();
                assert_eq!(node.key, i);
                assert!(node.right.is_none());
            }
        }
    }

    #[test]
    fn test_rotate_left() {
        let mut binary_tree = BinaryTree::new();

        // This should create a very imbalanced tree, very right-leaning.
        // 0
        //   1
        //     2
        //       3
        for i in 0..4 {
            binary_tree.insert(i, i);
        }

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 0);
        assert!(node.left.is_none());

        for i in 1..4 {
            node = node.right.as_ref().unwrap();
            assert_eq!(node.key, i);
            assert!(node.left.is_none());
        }

        // Rotating on 1 should leave us with the following:
        // 0
        //    2
        //  1   3
        BinaryTree::rotate_left(&mut binary_tree.root.as_mut().unwrap().right);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 0);
        assert!(node.left.is_none());

        node = node.right.as_ref().unwrap();
        assert_eq!(node.key, 2);

        assert_eq!(node.left.as_ref().unwrap().key, 1);
        assert_eq!(node.right.as_ref().unwrap().key, 3);

        // Rotating left on 0 should leave us with the following:
        //     2
        //  0     3
        //    1
        BinaryTree::rotate_left(&mut binary_tree.root);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 2);

        let left = node.left.as_ref().unwrap();
        assert_eq!(left.key, 0);
        assert!(left.left.is_none());
        assert_eq!(left.right.as_ref().unwrap().key, 1);

        let right = node.right.as_ref().unwrap();
        assert_eq!(right.key, 3);
        assert!(right.left.is_none());
        assert!(right.right.is_none());
    }

    #[test]
    fn test_rotate_right() {
        let mut binary_tree = BinaryTree::new();

        // This should create a very imbalanced tree, very left-leaning.
        //       3
        //     2
        //   1
        // 0
        for i in (0..4).rev() {
            binary_tree.insert(i, i);
        }

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        for i in (0..3).rev() {
            node = node.left.as_ref().unwrap();
            assert_eq!(node.key, i);
            assert!(node.right.is_none());
        }

        // Rotating on 2 should leave us with the following:
        //       3
        //    1
        //  0   2
        BinaryTree::rotate_right(&mut binary_tree.root.as_mut().unwrap().left);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        node = node.left.as_ref().unwrap();
        assert_eq!(node.key, 1);

        assert_eq!(node.left.as_ref().unwrap().key, 0);
        assert_eq!(node.right.as_ref().unwrap().key, 2);

        // Rotating right on 3 should leave us with the following:
        //     1
        //  0     3
        //      2
        BinaryTree::rotate_right(&mut binary_tree.root);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 1);

        let left = node.left.as_ref().unwrap();
        assert_eq!(left.key, 0);
        assert!(left.left.is_none());
        assert!(left.right.is_none());

        let right = node.right.as_ref().unwrap();
        assert_eq!(right.key, 3);
        assert_eq!(right.left.as_ref().unwrap().key, 2);
        assert!(right.right.is_none());
    }

    #[test]
    fn test_rotate_there_and_back_again() {
        let mut binary_tree = BinaryTree::new();

        // This should create a very imbalanced tree, very left-leaning.
        //       3
        //     2
        //   1
        // 0
        for i in (0..4).rev() {
            binary_tree.insert(i, i);
        }

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        for i in (0..3).rev() {
            node = node.left.as_ref().unwrap();
            assert_eq!(node.key, i);
            assert!(node.right.is_none());
        }

        // Rotating right on 2 should leave us with the following:
        //       3
        //    1
        //  0   2
        BinaryTree::rotate_right(&mut binary_tree.root.as_mut().unwrap().left);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        node = node.left.as_ref().unwrap();
        assert_eq!(node.key, 1);

        assert_eq!(node.left.as_ref().unwrap().key, 0);
        assert_eq!(node.right.as_ref().unwrap().key, 2);

        // Rotating left on 1 should bring us back to where we were:
        //       3
        //     2
        //   1
        // 0
        BinaryTree::rotate_left(&mut binary_tree.root.as_mut().unwrap().left);

        let mut node = binary_tree.root.as_ref().unwrap();
        assert_eq!(node.key, 3);
        assert!(node.right.is_none());

        for i in (0..3).rev() {
            node = node.left.as_ref().unwrap();
            assert_eq!(node.key, i);
            assert!(node.right.is_none());
        }
    }
}

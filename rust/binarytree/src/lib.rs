#![allow(unused)]
use std::cmp::Ordering;

struct BinaryTree<K, V> {
    root: Option<Box<Node<K, V>>>,
    length: usize,
}

#[derive(Debug, PartialEq, Eq)]
enum BalanceAction {
    Nothing,
    RotateLeft,
    RotateRight,
}

enum AtKeyAction {
    Nothing,
    AddNode,
    RemoveNode,
}

struct Node<K, V> {
    key: K,
    value: V,
    height: usize,
    left: Option<Box<Node<K, V>>>,
    right: Option<Box<Node<K, V>>>,
}

impl<K, V> Node<K, V> {
    fn new(key: K, value: V) -> Box<Self> {
        Box::new(Node {
            key,
            value,
            height: 1,
            left: None,
            right: None,
        })
    }

    fn update_height(&mut self) {
        match (&self.left, &self.right) {
            (Some(left), Some(right)) => {
                self.height = 1 + left.height.max(right.height);
            }
            (None, Some(right)) => {
                self.height = 1 + right.height;
            }
            (Some(left), None) => {
                self.height = 1 + left.height;
            }
            (None, None) => {
                self.height = 1;
            }
        }
    }

    fn check_balance(&self) -> BalanceAction {
        let left_height = self.left.as_ref().map_or(0, |n| n.height);
        let right_height = self.right.as_ref().map_or(0, |n| n.height);

        let balance_factor = left_height as isize - right_height as isize;

        if balance_factor < -1 {
            BalanceAction::RotateLeft
        } else if balance_factor > 1 {
            BalanceAction::RotateRight
        } else {
            BalanceAction::Nothing
        }
    }
}

impl<K, V> BinaryTree<K, V> {
    fn new() -> Self {
        BinaryTree {
            root: None,
            length: 0,
        }
    }

    fn len(&self) -> usize {
        self.length
    }

    fn insert(&mut self, key: K, value: V)
    where
        K: Ord,
    {
        if self.root.is_none() {
            self.root = Some(Node::new(key, value));
            return;
        }

        let action = self.at_key_mut(key, move |key, node, visited_nodes| {
            if let Some(node) = node {
                assert!(key == node.key);
                node.value = value;

                AtKeyAction::Nothing
            } else {
                *node = Some(Node::new(key, value));

                // After insert, go through all the nodes we visited updating their heights and
                // checking their balance, rotating as necessary.
                for &node_ptr in visited_nodes.iter().rev() {
                    // Safety: the pointers we collected are of Options we went through when visiting
                    // nodes to get here. We have exclusive access to the tree (&mut self) throughout
                    // the whole process. The tree owns these values. Nothing we do moves or destroys
                    // the Options, so these are safe to dereference.
                    let node: &mut Option<Box<Node<K, V>>> = unsafe { &mut *node_ptr };

                    let action = if let Some(node) = node {
                        node.update_height();
                        node.check_balance()
                    } else {
                        unreachable!("These nodes can't be None, as we walked through them")
                    };

                    match action {
                        BalanceAction::RotateLeft => BinaryTree::rotate_left(node),
                        BalanceAction::RotateRight => BinaryTree::rotate_right(node),
                        BalanceAction::Nothing => (),
                    }
                }

                AtKeyAction::AddNode
            }
        });

        match action {
            AtKeyAction::AddNode => self.length += 1,
            AtKeyAction::RemoveNode => unreachable!(),
            AtKeyAction::Nothing => (),
        };
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

    fn at_key_mut(
        &mut self,
        key: K,
        func: impl FnOnce(
            K,
            &mut Option<Box<Node<K, V>>>,
            &Vec<*mut Option<Box<Node<K, V>>>>,
        ) -> AtKeyAction,
    ) -> AtKeyAction
    where
        K: Ord,
    {
        let mut node = &mut self.root;
        let mut visited_nodes = vec![];
        loop {
            if node.is_none() {
                return func(key, node, &visited_nodes);
            }

            // We will need to revisit the nodes we went through to update
            // their height tracking and potentially rotate, in case of unbalance.
            visited_nodes.push(node as *mut _);

            let ordering = key.cmp(&node.as_ref().unwrap().key);
            match ordering {
                Ordering::Less => {
                    node = &mut node.as_mut().unwrap().left;
                }
                Ordering::Greater => node = &mut node.as_mut().unwrap().right,
                Ordering::Equal => {
                    return func(key, node, &visited_nodes);
                }
            }
        }
    }

    fn rotate_left(node: &mut Option<Box<Node<K, V>>>) {
        assert!(!node.is_none());
        if let Some(mut child) = node.as_mut().unwrap().right.take() {
            node.as_mut().unwrap().right = child.left.take();
            child.left = node.take();

            // After rotation, update heights: first the old root (now left child), then new root
            if let Some(child_left) = child.left.as_mut() {
                child_left.update_height();
            }
            child.update_height();

            *node = Some(child);
        }
    }

    fn rotate_right(node: &mut Option<Box<Node<K, V>>>) {
        assert!(!node.is_none());
        if let Some(mut child) = node.as_mut().unwrap().left.take() {
            node.as_mut().unwrap().left = child.right.take();
            child.right = node.take();

            // After rotation, update heights: first the old root (now right child), then new root
            if let Some(child_right) = child.right.as_mut() {
                child_right.update_height();
            }

            child.update_height();
            *node = Some(child);
        }
    }

    fn iter(&self) -> Iter<'_, K, V> {
        // The height of the root node is effectively the max depth a node can be at.
        let max_depth = self.root.as_deref().map(|root| root.height).unwrap_or(0);
        let mut stack = Vec::with_capacity(max_depth);

        // Begin by going all the way to the left of the tree, pushing each node onto the stack.
        let mut link = self.root.as_deref();
        while let Some(node) = link {
            stack.push(node);
            link = node.left.as_deref();
        }

        Iter { stack }
    }

    fn iter_mut(&mut self) -> IterMut<'_, K, V> {
        // The height of the root node is effectively the max depth a node can be at.
        let max_depth = self.root.as_deref().map(|root| root.height).unwrap_or(0);
        let mut stack = Vec::with_capacity(max_depth);

        // Begin by going all the way to the left of the tree, pushing each node onto the stack.
        let mut link = self.root.as_deref_mut();
        while let Some(node) = link {
            stack.push(node as *mut _);
            link = node.left.as_deref_mut();
        }

        IterMut { tree: self, stack }
    }
}

struct Iter<'a, K, V> {
    stack: Vec<&'a Node<K, V>>,
}

impl<'a, K, V> Iterator for Iter<'a, K, V> {
    type Item = (&'a K, &'a V);

    fn next(&mut self) -> Option<Self::Item> {
        // Pop the node we'll return...
        let node = self.stack.pop()?;

        // Then descend the tree using the right child of the node, pushing onto
        // the stack. This makes sure we'll visit all nodes in order.
        let mut next = node.right.as_deref();
        while let Some(node) = next {
            self.stack.push(node);
            next = node.left.as_deref();
        }

        Some((&node.key, &node.value))
    }
}

struct IterMut<'a, K, V> {
    tree: &'a mut BinaryTree<K, V>,
    stack: Vec<*mut Node<K, V>>,
}

impl<'a, K, V> Iterator for IterMut<'a, K, V> {
    type Item = (&'a K, &'a mut V);

    fn next(&mut self) -> Option<Self::Item> {
        // Pop the node we'll return...
        // Safety: we hold an exclusive reference to the tree, who owns all the nodes we are visiting.
        // Only one instance of next() can be running at a time, as it takes an exclusive reference to
        // the iterator. We do nothing that moves or destroys nodes, so dereferencing is safe.
        let node = unsafe { &mut *self.stack.pop()? };

        // Then descend the tree using the right child of the node, pushing onto
        // the stack. This makes sure we'll visit all nodes in order.
        let mut next = node.right.as_deref_mut();
        while let Some(node) = next {
            self.stack.push(node as *mut _);
            next = node.left.as_deref_mut();
        }

        Some((&node.key, &mut node.value))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn in_order_keys<K: Ord + Clone, V: Clone>(tree: &BinaryTree<K, V>) -> Vec<K> {
        fn walk<K: Ord + Clone, V: Clone>(node: &Option<Box<Node<K, V>>>, out: &mut Vec<K>) {
            if let Some(n) = node {
                walk(&n.left, out);
                out.push(n.key.clone());
                walk(&n.right, out);
            }
        }
        let mut out = Vec::new();
        walk(&tree.root, &mut out);
        out
    }

    fn is_balanced<K, V>(node: &Option<Box<Node<K, V>>>) -> bool {
        fn check<K, V>(node: &Option<Box<Node<K, V>>>) -> (bool, isize) {
            if let Some(n) = node {
                let (lb, lh) = check(&n.left);
                let (rb, rh) = check(&n.right);
                let balanced = lb && rb && (lh - rh).abs() <= 1;
                (balanced, 1 + lh.max(rh))
            } else {
                (true, 0)
            }
        }
        check(node).0
    }

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
    fn test_balanced_after_sequential_inserts() {
        let mut tree = BinaryTree::new();
        for i in 0..100 {
            tree.insert(i, i);
        }
        assert!(is_balanced(&tree.root));
        let keys = in_order_keys(&tree);
        assert_eq!(keys, (0..100).collect::<Vec<_>>());
    }

    #[test]
    fn test_balanced_after_reverse_inserts() {
        let mut tree = BinaryTree::new();
        for i in (0..100).rev() {
            tree.insert(i, i);
        }
        assert!(is_balanced(&tree.root));
        let keys = in_order_keys(&tree);
        assert_eq!(keys, (0..100).collect::<Vec<_>>());
    }

    #[test]
    fn test_balanced_after_mixed_inserts() {
        let mut tree = BinaryTree::new();
        let data = [10, 20, 5, 15, 25, 2, 7, 12, 17, 22, 27];
        for &x in &data {
            tree.insert(x, x);
        }
        assert!(is_balanced(&tree.root));
        let mut sorted = data.to_vec();
        sorted.sort();
        let keys = in_order_keys(&tree);
        assert_eq!(keys, sorted);
    }

    #[test]
    fn test_manual_rotations_on_unbalanced_tree() {
        // Manually construct an unbalanced right-leaning tree: 10 -> 20 -> 30
        let mut tree = BinaryTree::new();
        tree.root = Some(Box::new(Node {
            key: 10,
            value: 10,
            height: 3, // intentionally unbalanced
            left: None,
            right: Some(Box::new(Node {
                key: 20,
                value: 20,
                height: 2,
                left: None,
                right: Some(Box::new(Node {
                    key: 30,
                    value: 30,
                    height: 1,
                    left: None,
                    right: None,
                })),
            })),
        }));

        // Assert initial structure and unbalance
        let root = tree.root.as_ref().unwrap();
        assert_eq!(root.key, 10);
        assert!(root.left.is_none());
        assert_eq!(root.right.as_ref().unwrap().key, 20);
        assert_eq!(root.right.as_ref().unwrap().right.as_ref().unwrap().key, 30);
        assert!(!is_balanced(&tree.root));
        assert_eq!(root.check_balance(), BalanceAction::RotateLeft);

        // Rotate left at root
        BinaryTree::rotate_left(&mut tree.root);
        let root = tree.root.as_ref().unwrap();
        assert_eq!(root.key, 20);
        assert_eq!(root.left.as_ref().unwrap().key, 10);
        assert_eq!(root.right.as_ref().unwrap().key, 30);

        // After rotation, should be balanced
        assert!(is_balanced(&tree.root));

        // Rotate right at root to restore original unbalanced structure
        BinaryTree::rotate_right(&mut tree.root);
        let root = tree.root.as_ref().unwrap();
        assert_eq!(root.key, 10);
        assert!(root.left.is_none());
        assert_eq!(root.right.as_ref().unwrap().key, 20);
        assert_eq!(root.right.as_ref().unwrap().right.as_ref().unwrap().key, 30);

        // Should be unbalanced again
        assert!(!is_balanced(&tree.root));
        assert_eq!(root.check_balance(), BalanceAction::RotateLeft);
    }

    #[test]
    fn test_iterator_in_order() {
        let mut tree = BinaryTree::new();

        let mut data = vec![15, 3, 20, 7, 1, 18, 25, 5, 10];

        for &x in &data {
            tree.insert(x, x * 2);
        }

        // Sort our reference vector.
        data.sort();

        // Create iterators with matching structure and compare them.
        let expected = data.iter().map(|&k| (k, k * 2));
        let actual = tree.iter().map(|(k, v)| (*k, *v));
        assert!(expected.eq(actual));
    }

    #[test]
    fn test_iterator_mut_in_order_and_mutation() {
        let mut tree = BinaryTree::new();

        let mut data = vec![15, 3, 20, 7, 1, 18, 25, 5, 10];

        for &x in &data {
            tree.insert(x, x * 2);
        }

        data.sort();

        // First, mutate all values via the mutable iterator
        for (k, v) in tree.iter_mut() {
            *v += 1000;
        }

        // Now, check that the keys are in order and values are as expected
        let expected = data.iter().map(|&k| (k, k * 2 + 1000));
        let actual = tree.iter().map(|(k, v)| (*k, *v));
        assert!(expected.eq(actual));
    }
}

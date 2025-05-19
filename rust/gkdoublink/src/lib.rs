use std::{
    cell::{RefCell, RefMut},
    rc::Rc,
};

use anyhow::bail;

pub struct GkDoublink<T> {
    pub(crate) head: Link<T>,
    pub(crate) tail: Link<T>,
}

type Link<T> = Option<Rc<RefCell<Node<T>>>>;

struct Node<T> {
    pub(crate) value: T,
    pub(crate) prev: Link<T>,
    pub(crate) next: Link<T>,
}

impl<T> Node<T> {
    fn new(value: T, prev: Link<T>, next: Link<T>) -> Rc<RefCell<Self>> {
        Rc::new(RefCell::new(Node { value, prev, next }))
    }
}

impl<T> GkDoublink<T> {
    pub fn new() -> Self {
        GkDoublink {
            head: None,
            tail: None,
        }
    }

    pub fn add_first(&mut self, value: T) {
        assert!(self.head.is_none());
        assert!(self.tail.is_none());
        let link = Node::new(value, None, None);
        self.head.replace(link.clone());
        self.tail.replace(link);
    }

    pub fn append(&mut self, value: T) {
        match self.tail.take() {
            None => self.add_first(value),
            Some(link) => {
                let new_link = Node::new(value, Some(link.clone()), None);
                link.borrow_mut().next = Some(new_link.clone());
                self.tail = Some(new_link);
            }
        }
    }

    pub fn prepend(&mut self, value: T) {
        match self.head.take() {
            None => self.add_first(value),
            Some(link) => {
                let new_link = Node::new(value, None, Some(link.clone()));
                link.borrow_mut().prev = Some(new_link.clone());
                self.head = Some(new_link);
            }
        }
    }

    pub fn walk(&mut self) -> Cursor<T> {
        let current = self.head.clone();
        Cursor {
            doublink: self,
            current,
        }
    }
}

pub struct Cursor<'a, T> {
    pub(crate) doublink: &'a mut GkDoublink<T>,
    pub(crate) current: Link<T>,
}

impl<'a, T> Cursor<'_, T> {
    fn forward(&mut self) -> Option<RefMut<T>> {
        match self.current.take() {
            Some(current) => {
                self.current = current.borrow().next.clone().take();
                self.get()
            }
            None => None,
        }
    }

    fn backward(&mut self) -> Option<RefMut<T>> {
        match self.current.take() {
            Some(current) => {
                self.current = current.borrow().prev.clone().take();
                self.get()
            }
            None => None,
        }
    }

    fn get(&mut self) -> Option<RefMut<T>> {
        self.current
            .as_mut()
            .map(|c| RefMut::map(c.borrow_mut(), |c| &mut c.value))
    }

    fn insert_at(&mut self, value: T) {
        let new_link = Node::new(value, None, None);
        match self.current.take() {
            None => self.current = Some(new_link),
            Some(current) => {
                if self.doublink.head.as_ref().unwrap().as_ptr() == current.as_ptr() {
                    self.doublink.head.replace(new_link.clone());
                }

                current.borrow_mut().prev = Some(new_link.clone());
                new_link.borrow_mut().next = Some(current);
                self.current = Some(new_link);
            }
        }
    }

    fn insert_after(&mut self, value: T) -> anyhow::Result<()> {
        let Some(current) = self.current.clone() else {
            bail!("Tried to insert after empty node");
        };

        let next = current.borrow_mut().next.take();
        let new_link = Node::new(value, self.current.clone(), next.clone());

        if self.doublink.tail.as_ref().unwrap().as_ptr() == current.as_ptr() {
            self.doublink.tail.replace(new_link.clone());
        }

        current.borrow_mut().next = Some(new_link);

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basics() {
        let mut doublink = GkDoublink::new();
        doublink.append(42);
        doublink.append(43);
        doublink.append(44);

        doublink.prepend(41);
        doublink.prepend(40);

        doublink.append(45);
        doublink.append(46);

        assert_eq!(doublink.head.as_ref().unwrap().borrow().value, 40);
        assert_eq!(doublink.tail.as_ref().unwrap().borrow().value, 46);
    }

    #[test]
    fn test_cursor() {
        let mut doublink = GkDoublink::new();
        doublink.append(42);
        doublink.append(43);
        doublink.append(44);

        doublink.prepend(41);
        doublink.prepend(40);

        doublink.append(45);
        doublink.append(46);

        assert_eq!(doublink.head.as_ref().unwrap().borrow().value, 40);
        assert_eq!(doublink.tail.as_ref().unwrap().borrow().value, 46);

        let mut cursor = doublink.walk();

        assert_eq!(cursor.current.as_ref().unwrap().borrow().value, 40);

        assert_eq!(cursor.forward().as_deref(), Some(&41));
        assert_eq!(cursor.forward().as_deref(), Some(&42));
        assert_eq!(cursor.forward().as_deref(), Some(&43));
        assert_eq!(cursor.forward().as_deref(), Some(&44));
        assert_eq!(cursor.forward().as_deref(), Some(&45));
        assert_eq!(cursor.forward().as_deref(), Some(&46));
        assert_eq!(cursor.forward().as_deref(), None);

        let mut cursor = doublink.walk();
        cursor.insert_at(39);

        assert_eq!(cursor.doublink.head.as_ref().unwrap().borrow().value, 39);

        assert_eq!(cursor.forward().as_deref(), Some(&40));
        assert_eq!(cursor.forward().as_deref(), Some(&41));
        assert_eq!(cursor.forward().as_deref(), Some(&42));
        assert_eq!(cursor.forward().as_deref(), Some(&43));
        assert_eq!(cursor.backward().as_deref(), Some(&42));

        assert!(cursor.insert_after(0).is_ok());

        assert_eq!(cursor.forward().as_deref(), Some(&0));
        assert_eq!(cursor.backward().as_deref(), Some(&42));
    }
}

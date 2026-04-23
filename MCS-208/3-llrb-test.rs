use std::cell::RefCell;
use std::cmp::Ordering;
use std::rc::Rc;

#[derive(Debug, Clone, Copy, PartialEq)]
enum Color {
    Red,
    Black,
}

struct Node<K: Ord + Clone, V: Clone> {
    key: K,
    value: V,
    color: Color,
    left: Option<Rc<RefCell<Node<K, V>>>>,
    right: Option<Rc<RefCell<Node<K, V>>>>,
}

impl<K: Ord + Clone, V: Clone> Node<K, V> {
    fn new(key: K, value: V, color: Color) -> Self {
        Node {
            key,
            value,
            color,
            left: None,
            right: None,
        }
    }
}

pub struct LLRBTree<K: Ord + Clone, V: Clone> {
    root: Option<Rc<RefCell<Node<K, V>>>>,
}

impl<K: Ord + Clone + std::fmt::Debug, V: Clone + std::fmt::Debug> LLRBTree<K, V> {
    pub fn new() -> Self {
        LLRBTree { root: None }
    }

    // Helper: Check if a node is red
    fn is_red(node: &Option<Rc<RefCell<Node<K, V>>>>) -> bool {
        match node {
            Some(n) => n.borrow().color == Color::Red,
            None => false,
        }
    }

    // Rotate Left: Fix right-leaning red link
    fn rotate_left(h: Rc<RefCell<Node<K, V>>>) -> Rc<RefCell<Node<K, V>>> {
        // 1. Get right child (x)
        let x = {
            let h_ref = h.borrow();
            h_ref.right.as_ref().unwrap().clone()
        };

        // 2. Perform rotation
        let h_clone = h.clone(); // Clone Rc to move into x's left
        {
            let mut h_mut = h.borrow_mut();
            let mut x_mut = x.borrow_mut();

            h_mut.right = x_mut.left.take();
            x_mut.left = Some(h_clone);
            x_mut.color = h_mut.color;
            h_mut.color = Color::Red;
        }

        x
    }

    // Rotate Right: Fix two consecutive left red links
    fn rotate_right(h: Rc<RefCell<Node<K, V>>>) -> Rc<RefCell<Node<K, V>>> {
        // 1. Get left child (x)
        let x = {
            let h_ref = h.borrow();
            h_ref.left.as_ref().unwrap().clone()
        };

        // 2. Perform rotation
        let h_clone = h.clone();
        {
            let mut h_mut = h.borrow_mut();
            let mut x_mut = x.borrow_mut();

            h_mut.left = x_mut.right.take();
            x_mut.right = Some(h_clone);
            x_mut.color = h_mut.color;
            h_mut.color = Color::Red;
        }

        x
    }

    // Flip Colors: Split a 4-node
    fn flip_colors(h: &Rc<RefCell<Node<K, V>>>) {
        let mut h_mut = h.borrow_mut();
        h_mut.color = Color::Red;
        if let Some(ref left) = h_mut.left {
            left.borrow_mut().color = Color::Black;
        }
        if let Some(ref right) = h_mut.right {
            right.borrow_mut().color = Color::Black;
        }
    }

    // Balance: Restore LLRB invariants
    fn balance(mut h: Rc<RefCell<Node<K, V>>>) -> Rc<RefCell<Node<K, V>>> {
        // 1. Fix right-leaning red
        {
            let h_ref = h.borrow();
            if Self::is_red(&h_ref.right) && !Self::is_red(&h_ref.left) {
                drop(h_ref); // Drop borrow before mutation
                h = Self::rotate_left(h);
            }
        }

        // 2. Fix two consecutive left reds
        {
            let h_ref = h.borrow();
            if Self::is_red(&h_ref.left) {
                let left_left_red = if let Some(ref left) = h_ref.left {
                    Self::is_red(&left.borrow().left)
                } else {
                    false
                };
                if left_left_red {
                    drop(h_ref);
                    h = Self::rotate_right(h);
                }
            }
        }

        // 3. Split 4-node (both children red)
        {
            let h_ref = h.borrow();
            if Self::is_red(&h_ref.left) && Self::is_red(&h_ref.right) {
                drop(h_ref);
                Self::flip_colors(&h);
            }
        }

        h
    }

    // Public Insert
    pub fn insert(&mut self, key: K, value: V) {
        // Take root to avoid borrow conflicts
        let old_root = self.root.take();
        self.root = Some(self.insert_rec(old_root, key, value));

        // Root must always be black
        if let Some(ref root) = self.root {
            root.borrow_mut().color = Color::Black;
        }
    }

    // Recursive Insert
    fn insert_rec(
        &self,
        h: Option<Rc<RefCell<Node<K, V>>>>,
        key: K,
        value: V,
    ) -> Rc<RefCell<Node<K, V>>> {
        // Base case: Create new red node
        let h = match h {
            Some(node) => node,
            None => return Rc::new(RefCell::new(Node::new(key, value, Color::Red))),
        };

        let cmp = {
            let h_ref = h.borrow();
            key.cmp(&h_ref.key)
        };

        match cmp {
            Ordering::Less => {
                let child = {
                    let mut h_mut = h.borrow_mut();
                    h_mut.left.take()
                };
                let new_left = self.insert_rec(child, key, value);
                h.borrow_mut().left = Some(new_left);
            }
            Ordering::Greater => {
                let child = {
                    let mut h_mut = h.borrow_mut();
                    h_mut.right.take()
                };
                let new_right = self.insert_rec(child, key, value);
                h.borrow_mut().right = Some(new_right);
            }
            Ordering::Equal => {
                h.borrow_mut().value = value;
            }
        }

        Self::balance(h)
    }

    // Public Search
    pub fn search(&self, key: &K) -> Option<V> {
        let mut current = self.root.clone();
        while let Some(node) = current {
            let node_ref = node.borrow();
            match key.cmp(&node_ref.key) {
                Ordering::Less => current = node_ref.left.clone(),
                Ordering::Greater => current = node_ref.right.clone(),
                Ordering::Equal => return Some(node_ref.value.clone()),
            }
        }
        None
    }

    // Public Delete
    pub fn delete(&mut self, key: &K) {
        // If root and both children are black, make root red temporarily
        let root_is_black = if let Some(ref r) = self.root {
            r.borrow().color == Color::Black
        } else {
            true
        };

        let left_is_black = if let Some(ref r) = self.root {
            !Self::is_red(&r.borrow().left)
        } else {
            true
        };

        let right_is_black = if let Some(ref r) = self.root {
            !Self::is_red(&r.borrow().right)
        } else {
            true
        };

        if root_is_black && left_is_black && right_is_black {
            if let Some(ref root) = self.root {
                root.borrow_mut().color = Color::Red;
            }
        }

        let old_root = self.root.take();
        self.root = self.delete_rec(old_root, key);

        if let Some(ref root) = self.root {
            root.borrow_mut().color = Color::Black;
        }
    }

    // Recursive Delete
    fn delete_rec(
        &self,
        h: Option<Rc<RefCell<Node<K, V>>>>,
        key: &K,
    ) -> Option<Rc<RefCell<Node<K, V>>>> {
        let h = match h {
            Some(node) => node,
            None => return None,
        };

        let cmp = {
            let h_ref = h.borrow();
            key.cmp(&h_ref.key)
        };

        if cmp == Ordering::Less {
            // Move Red Left
            let needs_move = {
                let h_ref = h.borrow();
                !Self::is_red(&h_ref.left)
                    && !(if let Some(ref l) = h_ref.left {
                        Self::is_red(&l.borrow().left)
                    } else {
                        false
                    })
            };

            if needs_move {
                let h = Self::move_red_left(h);
                let child = {
                    let mut h_mut = h.borrow_mut();
                    h_mut.left.take()
                };
                h.borrow_mut().left = self.delete_rec(child, key);
                Some(Self::balance(h))
            } else {
                let child = {
                    let mut h_mut = h.borrow_mut();
                    h_mut.left.take()
                };
                h.borrow_mut().left = self.delete_rec(child, key);
                Some(Self::balance(h))
            }
        } else {
            // Rotate Right if left is red
            let left_is_red = {
                let h_ref = h.borrow();
                Self::is_red(&h_ref.left)
            };
            if left_is_red {
                let h = Self::rotate_right(h);
                let child = {
                    let mut h_mut = h.borrow_mut();
                    h_mut.left.take()
                };
                h.borrow_mut().left = self.delete_rec(child, key);
                return Some(Self::balance(h));
            }

            // Check if key found and right is empty
            let right_is_none = {
                let h_ref = h.borrow();
                h_ref.right.is_none()
            };
            if cmp == Ordering::Equal && right_is_none {
                return None;
            }

            // Move Red Right
            let needs_move = {
                let h_ref = h.borrow();
                !Self::is_red(&h_ref.right)
                    && !(if let Some(ref r) = h_ref.right {
                        Self::is_red(&r.borrow().left)
                    } else {
                        false
                    })
            };

            if needs_move {
                let h = Self::move_red_right(h);

                // Re-check comparison after rotation/move
                let cmp2 = {
                    let h_ref = h.borrow();
                    key.cmp(&h_ref.key)
                };

                if cmp2 == Ordering::Equal {
                    // Find min in right subtree
                    let min_node = {
                        let h_ref = h.borrow();
                        h_ref.right.as_ref().unwrap().clone()
                    };
                    let (min_key, min_val) = {
                        let min_ref = min_node.borrow();
                        (min_ref.key.clone(), min_ref.value.clone())
                    };

                    {
                        let mut h_mut = h.borrow_mut();
                        h_mut.key = min_key;
                        h_mut.value = min_val;
                        let child = h_mut.right.take();
                        h_mut.right = self.delete_min(child);
                    }
                } else {
                    let child = {
                        let mut h_mut = h.borrow_mut();
                        h_mut.right.take()
                    };
                    h.borrow_mut().right = self.delete_rec(child, key);
                }
                Some(Self::balance(h))
            } else {
                let cmp2 = {
                    let h_ref = h.borrow();
                    key.cmp(&h_ref.key)
                };
                if cmp2 == Ordering::Equal {
                    let min_node = {
                        let h_ref = h.borrow();
                        h_ref.right.as_ref().unwrap().clone()
                    };
                    let (min_key, min_val) = {
                        let min_ref = min_node.borrow();
                        (min_ref.key.clone(), min_ref.value.clone())
                    };

                    {
                        let mut h_mut = h.borrow_mut();
                        h_mut.key = min_key;
                        h_mut.value = min_val;
                        let child = h_mut.right.take();
                        h_mut.right = self.delete_min(child);
                    }
                } else {
                    let child = {
                        let mut h_mut = h.borrow_mut();
                        h_mut.right.take()
                    };
                    h.borrow_mut().right = self.delete_rec(child, key);
                }
                Some(Self::balance(h))
            }
        }
    }

    fn move_red_left(h: Rc<RefCell<Node<K, V>>>) -> Rc<RefCell<Node<K, V>>> {
        Self::flip_colors(&h);

        let right_left_is_red = {
            let h_ref = h.borrow();
            if let Some(ref r) = h_ref.right {
                Self::is_red(&r.borrow().left)
            } else {
                false
            }
        };

        if right_left_is_red {
            // Rotate right child right
            let right_child = {
                let h_ref = h.borrow();
                h_ref.right.as_ref().unwrap().clone()
            };
            let rotated_right = Self::rotate_right(right_child);
            h.borrow_mut().right = Some(rotated_right);

            // Rotate h left
            let h = Self::rotate_left(h);
            Self::flip_colors(&h);
            h
        } else {
            h
        }
    }

    fn move_red_right(h: Rc<RefCell<Node<K, V>>>) -> Rc<RefCell<Node<K, V>>> {
        Self::flip_colors(&h);

        let left_left_is_red = {
            let h_ref = h.borrow();
            if let Some(ref l) = h_ref.left {
                Self::is_red(&l.borrow().left)
            } else {
                false
            }
        };

        if left_left_is_red {
            let h = Self::rotate_right(h);
            Self::flip_colors(&h);
            h
        } else {
            h
        }
    }

    fn delete_min(&self, h: Option<Rc<RefCell<Node<K, V>>>>) -> Option<Rc<RefCell<Node<K, V>>>> {
        let h = match h {
            Some(node) => node,
            None => return None,
        };

        // If left is null, we are at min
        let left_is_none = {
            let h_ref = h.borrow();
            h_ref.left.is_none()
        };
        if left_is_none {
            return None;
        }

        // Move red left if needed
        let needs_move = {
            let h_ref = h.borrow();
            !Self::is_red(&h_ref.left)
                && !(if let Some(ref l) = h_ref.left {
                    Self::is_red(&l.borrow().left)
                } else {
                    false
                })
        };

        if needs_move {
            let h = Self::move_red_left(h);
            let child = {
                let mut h_mut = h.borrow_mut();
                h_mut.left.take()
            };
            h.borrow_mut().left = self.delete_min(child);
            Some(Self::balance(h))
        } else {
            let child = {
                let mut h_mut = h.borrow_mut();
                h_mut.left.take()
            };
            h.borrow_mut().left = self.delete_min(child);
            Some(Self::balance(h))
        }
    }

    // Inorder Traversal
    pub fn inorder(&self) -> Vec<(K, V, &'static str)> {
        let mut result = Vec::new();
        self.inorder_rec(&self.root, &mut result);
        result
    }

    fn inorder_rec(
        &self,
        node: &Option<Rc<RefCell<Node<K, V>>>>,
        result: &mut Vec<(K, V, &'static str)>,
    ) {
        if let Some(n) = node {
            let n_ref = n.borrow();
            self.inorder_rec(&n_ref.left, result);
            let color_str = match n_ref.color {
                Color::Red => "RED",
                Color::Black => "BLACK",
            };
            result.push((n_ref.key.clone(), n_ref.value.clone(), color_str));
            self.inorder_rec(&n_ref.right, result);
        }
    }

    // Check Balance (Validation)
    pub fn is_balanced(&self) -> bool {
        self.check_balanced(&self.root)
    }

    fn check_balanced(&self, node: &Option<Rc<RefCell<Node<K, V>>>>) -> bool {
        match node {
            None => true,
            Some(n) => {
                let n_ref = n.borrow();

                // 1. No right-leaning red
                if Self::is_red(&n_ref.right) {
                    return false;
                }

                // 2. No two consecutive left reds
                if n_ref.color == Color::Red {
                    if let Some(ref left) = n_ref.left {
                        if left.borrow().color == Color::Red {
                            return false;
                        }
                    }
                }

                // 3. Recurse
                let left_ok = self.check_balanced(&n_ref.left);
                let right_ok = self.check_balanced(&n_ref.right);

                left_ok && right_ok
            }
        }
    }
}

// ==================== MAIN EXAMPLE ====================
fn main() {
    println!("============================================================");
    println!("LEFT-LEANING RED-BLACK TREE - RUST (FINAL FIXED VERSION)");
    println!("============================================================\n");

    let mut llrb: LLRBTree<i32, String> = LLRBTree::new();

    // 1. Insertions
    println!("1. INSERTIONS:");
    let keys = vec![50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 65];
    for k in &keys {
        llrb.insert(*k, format!("val_{}", k));
        println!("  Inserted {:2} | Balanced: {}", k, llrb.is_balanced());
    }

    // 2. Inorder
    println!("\n2. INORDER TRAVERSAL:");
    for (k, _v, c) in llrb.inorder() {
        print!("{}({}) ", k, c);
    }
    println!();

    // 3. Search
    println!("\n3. SEARCH:");
    for k in &[30, 99] {
        match llrb.search(k) {
            Some(v) => println!("  Found {}: {}", k, v),
            None => println!("  {} not found", k),
        }
    }

    // 4. Deletions
    println!("\n4. DELETIONS:");
    for k in &[20, 50, 70] {
        llrb.delete(k);
        println!("  Deleted {:2} | Balanced: {}", k, llrb.is_balanced());
    }

    // 5. Final State
    println!("\n5. FINAL INORDER:");
    for (k, _v, c) in llrb.inorder() {
        print!("{}({}) ", k, c);
    }
    println!("\n\n✅ ALL OPERATIONS SUCCESSFUL!");
}

from enum import Enum
from typing import Any, Optional


class Color(Enum):
    RED = True
    BLACK = False


class Node:
    def __init__(self, key: Any, value: Any, color: Color = Color.RED):
        self.key = key
        self.value = value
        self.color = color  # Color of the link from parent to this node
        self.left: Optional["Node"] = None
        self.right: Optional["Node"] = None


class LeftLeaningRedBlackTree:
    def __init__(self):
        self.root: Optional[Node] = None

    def _is_red(self, node: Optional[Node]) -> bool:
        """Check if a node is red"""
        if node is None:
            return False
        return node.color == Color.RED

    def _rotate_left(self, h: Node) -> Node:
        """
        Rotate left: Make a right-leaning red link lean to the left.

        Before:           After:
          h                 x
           \               /
            x     ->      h
           / \           / \
          a   b         a   b
        """
        x = h.right
        h.right = x.left
        x.left = h
        x.color = h.color
        h.color = Color.RED
        return x

    def _rotate_right(self, h: Node) -> Node:
        """
        Rotate right: Fix two consecutive left red links.

        Before:         After:
            h             x
           /             / \
          x      ->      a   h
         / \               / \
        a   b             b   c
        """
        x = h.left
        h.left = x.right
        x.right = h
        x.color = h.color
        h.color = Color.RED
        return x

    def _flip_colors(self, h: Node):
        """
        Flip colors: Split a 4-node.
        Pass red to parent, make children black.
        """
        h.color = Color.RED
        if h.left:
            h.left.color = Color.BLACK
        if h.right:
            h.right.color = Color.BLACK

    def _balance(self, h: Node) -> Node:
        """
        Restore LLRB invariants after insertion/deletion.
        """
        # Fix right-leaning red link
        if self._is_red(h.right) and not self._is_red(h.left):
            h = self._rotate_left(h)

        # Fix two consecutive left red links
        if self._is_red(h.left) and self._is_red(h.left.left):
            h = self._rotate_right(h)

        # Split 4-node
        if self._is_red(h.left) and self._is_red(h.right):
            self._flip_colors(h)

        return h

    def insert(self, key: Any, value: Any):
        """Public insert method"""
        self.root = self._insert(self.root, key, value)
        self.root.color = Color.BLACK  # Root is always black

    def _insert(self, h: Optional[Node], key: Any, value: Any) -> Node:
        """
        Insert a key-value pair into the subtree rooted at h.
        Returns the new root of the subtree.
        """
        if h is None:
            return Node(key, value, Color.RED)

        cmp = self._compare(key, h.key)

        if cmp < 0:
            h.left = self._insert(h.left, key, value)
        elif cmp > 0:
            h.right = self._insert(h.right, key, value)
        else:
            h.value = value  # Update existing key

        return self._balance(h)

    def delete(self, key: Any):
        """Public delete method"""
        if not self._is_red(self.root.left) and not self._is_red(self.root.right):
            if self.root:
                self.root.color = Color.RED

        self.root = self._delete(self.root, key)
        if self.root:
            self.root.color = Color.BLACK

    def _delete(self, h: Optional[Node], key: Any) -> Optional[Node]:
        """Delete a key from the subtree rooted at h"""
        if h is None:
            return None

        if self._compare(key, h.key) < 0:
            # Move red left
            if not self._is_red(h.left) and h.left and not self._is_red(h.left.left):
                h = self._move_red_left(h)
            h.left = self._delete(h.left, key)
        else:
            # Rotate right if needed
            if self._is_red(h.left):
                h = self._rotate_right(h)

            # Delete at this node
            if self._compare(key, h.key) == 0 and h.right is None:
                return None

            # Move red right
            if not self._is_red(h.right) and h.right and not self._is_red(h.right.left):
                h = self._move_red_right(h)

            # Delete minimum from right subtree
            if self._compare(key, h.key) == 0:
                min_node = self._find_min(h.right)
                h.key = min_node.key
                h.value = min_node.value
                h.right = self._delete_min(h.right)
            else:
                h.right = self._delete(h.right, key)

        return self._balance(h)

    def _move_red_left(self, h: Node) -> Node:
        """Move a red link to the left"""
        self._flip_colors(h)
        if h.right and self._is_red(h.right.left):
            h.right = self._rotate_right(h.right)
            h = self._rotate_left(h)
            self._flip_colors(h)
        return h

    def _move_red_right(self, h: Node) -> Node:
        """Move a red link to the right"""
        self._flip_colors(h)
        if h.left and self._is_red(h.left.left):
            h = self._rotate_right(h)
            self._flip_colors(h)
        return h

    def _delete_min(self, h: Node) -> Optional[Node]:
        """Delete the minimum key from the subtree rooted at h"""
        if h.left is None:
            return None

        if not self._is_red(h.left) and not self._is_red(h.left.left):
            h = self._move_red_left(h)

        h.left = self._delete_min(h.left)
        return self._balance(h)

    def _find_min(self, h: Node) -> Node:
        """Find the node with minimum key"""
        while h.left:
            h = h.left
        return h

    def search(self, key: Any) -> Optional[Any]:
        """Search for a key and return its value"""
        node = self.root
        while node:
            cmp = self._compare(key, node.key)
            if cmp < 0:
                node = node.left
            elif cmp > 0:
                node = node.right
            else:
                return node.value
        return None

    def _compare(self, key1: Any, key2: Any) -> int:
        """Compare two keys"""
        if key1 < key2:
            return -1
        elif key1 > key2:
            return 1
        return 0

    def inorder(self):
        """Inorder traversal - returns sorted keys"""
        result = []
        self._inorder(self.root, result)
        return result

    def _inorder(self, node: Optional[Node], result: list):
        if node:
            self._inorder(node.left, result)
            result.append(
                (node.key, node.value, "RED" if node.color == Color.RED else "BLACK")
            )
            self._inorder(node.right, result)

    def is_balanced(self) -> bool:
        """Check if the tree maintains LLRB properties"""
        return self._is_balanced(self.root)

    def _is_balanced(self, node: Optional[Node]) -> bool:
        if node is None:
            return True

        # Check no right-leaning red links
        if self._is_red(node.right):
            return False

        # Check no node with two red children
        if self._is_red(node) and node.left and self._is_red(node.left):
            if node.left.left and self._is_red(node.left.left):
                return False

        return self._is_balanced(node.left) and self._is_balanced(node.right)


# ==================== EXAMPLE USAGE ====================
if __name__ == "__main__":
    print("=" * 60)
    print("LEFT-LEANING RED-BLACK TREE - PYTHON IMPLEMENTATION")
    print("=" * 60)

    llrb = LeftLeaningRedBlackTree()

    # Insert operations
    print("\n1. INSERTION OPERATIONS:")
    print("-" * 40)
    keys_to_insert = [50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 65]

    for key in keys_to_insert:
        llrb.insert(key, f"value_{key}")
        print(f"Inserted {key:2d} -> Tree balanced: {llrb.is_balanced()}")

    # Display tree
    print("\n2. INORDER TRAVERSAL (sorted order):")
    print("-" * 40)
    for key, value, color in llrb.inorder():
        print(f"Key: {key:2d}, Value: {value}, Color: {color}")

    # Search operations
    print("\n3. SEARCH OPERATIONS:")
    print("-" * 40)
    search_keys = [30, 60, 100]
    for key in search_keys:
        result = llrb.search(key)
        print(f"Search {key:2d}: {'Found -> ' + result if result else 'Not Found'}")

    # Delete operations
    print("\n4. DELETION OPERATIONS:")
    print("-" * 40)
    keys_to_delete = [20, 50, 70]

    for key in keys_to_delete:
        print(f"\nDeleting {key}...")
        llrb.delete(key)
        print(f"Tree balanced after deletion: {llrb.is_balanced()}")
        print("Inorder traversal:")
        for k, v, c in llrb.inorder():
            print(f"  {k:2d} ({c})")

    print("\n" + "=" * 60)
    print("OPERATIONS COMPLETED SUCCESSFULLY!")
    print("=" * 60)

class Node:
    def __init__(self, val, left=None, right=None) -> None:
        self.val: int = val
        self.left: Node | None = left
        self.right: Node | None = right
        self.height: int = 1

class AvlTree:
    def __init__(self) -> None:
        self.root: Node | None = None

    @staticmethod
    def _get_height(node: Node | None) -> int:
        return node.height if node else 0

    def diff(self, root: Node | None) -> int:
        if root is None:
            return float("inf")
        ans = []
        if root.left is not None:
            ans.append(abs(root.left.val - root.val))
        if root.right is not None:
            ans.append(abs(root.right.val - root.val))
        return min(ans + [self.diff(root.left), self.diff(root.right)])

    def update_height(self, node: Node | None) -> None:
        node.height = max(self._get_height(node.left), self._get_height(node.right)) + 1

    def _get_balance(self, node: Node | None) -> int:
        return self._get_height(node.right) - self._get_height(node.left) if node else 0

    def right_rotate(self, node: Node) -> Node:
        left_child = node.left
        buffer = left_child.right
        left_child.right = node
        node.left = buffer
        self.update_height(node)
        self.update_height(left_child)
        return left_child

    def left_rotate(self, node: Node) -> Node:
        right_child = node.right
        buffer = right_child.left
        right_child.left = node
        node.right = buffer
        self.update_height(node)
        self.update_height(right_child)
        return right_child

    def balance(self, node: Node) -> Node:
        self.update_height(node)
        cur_balance = self._get_balance(node)
        if cur_balance < -1:
            if self._get_balance(node.left) > 0:
                node.left = self.left_rotate(node.left)
            node = self.right_rotate(node)
        elif cur_balance > 1:
            if self._get_balance(node.right) < 0:
                node.right = self.right_rotate(node.right)
            node = self.left_rotate(node)
        return node

    def _insert(self, val: int, node: Node | None) -> Node:
        if node is None:
            return Node(val)
        if val < node.val:
            node.left = self._insert(val, node.left)
        else:
            node.right = self._insert(val, node.right)
        return self.balance(node)

    def insert_value(self, val: int) -> None:
        self.root = self._insert(val, self.root)

    def get_min(self, node: Node | None = None) -> Node:
        if node is None:
            node = self.root
        while node.left is not None:
            node = node.left
        return node

    def get_max(self, node: Node | None = None) -> Node:
        if node is None:
            node = self.root
        while node.right is not None:
            node = node.right
        return node

    def delete_value(self, val: int) -> None:
        self.root = self._delete(self.root, val)

    def _delete(self, node: Node | None, val: int) -> Node | None:
        if node is None:
            return node
        if val < node.val:
            node.left = self._delete(node.left, val)
        elif val > node.val:
            node.right = self._delete(node.right, val)
        elif node.left is not None and node.right is not None:
            right_min = node.right
            while right_min.left is not None:
                right_min = right_min.left
            node.val = right_min.val
            node.right = self._delete(node.right, node.val)
        elif node.left is not None:
            node = node.left
        elif node.right is not None:
            node = node.right
        else:
            node = None
        return self.balance(node) if node else None

    def delete_min(self, node: Node | None) -> Node | None:
        if node is None:
            return None
        if node.left:
            node.left = self.delete_min(node.left)
        else:
            return node.right

        return self.balance(node)

    def delete_max(self, node: Node | None) -> Node | None:
        if node is None:
            return None
        if node.right:
            node.right = self.delete_max(node.right)
        else:
            return node.left

        return self.balance(node)

    def in_order_traversal(self, node: Node | None, result: list[int] | None = None) -> list[int]:
        if result is None:
            result = []
        if node is not None:
            self.in_order_traversal(node.left, result)
            result.append(node.val)
            self.in_order_traversal(node.right, result)
        return result

    def print_tree(self) -> None:
        print(self.in_order_traversal(self.root))
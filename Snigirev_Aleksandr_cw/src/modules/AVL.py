class Node:
    def __init__(self, key, value, left=None, right=None) -> None:
        self.val: str = value
        self.key: list[int] = key
        self.left: Node | None = left
        self.right: Node | None = right
        self.height: int = 1

class AVL:
    def __init__(self) -> None:
        self.root: Node | None = None

    @staticmethod
    def _get_height(node: Node | None) -> int:
        return node.height if node else 0

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
    
    def diff(self, root: Node | None) -> int:
        if root is None:
            return float("inf")
        ans = []
        if root.left is not None:
            ans.append(abs(root.left.val - root.val))
        if root.right is not None:
            ans.append(abs(root.right.val - root.val))
        return min(ans + [self.diff(root.left), self.diff(root.right)])

    def in_order_traversal(self, node: Node | None, result: list[int] | None = None) -> list[int]:
        if result is None:
            result = []
        if node is not None:
            self.in_order_traversal(node.left, result)
            result.append([node.key, node.val])
            self.in_order_traversal(node.right, result)
        return result
          
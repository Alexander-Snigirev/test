from __future__ import annotations
import random
#import pygraphviz

class Node:
    def __init__(self, value) -> None:
        self.value = value
        self.parent: Node | None = None
        self.left: Node | None = None
        self.right: Node | None = None
        self.color: bool = True
        self.height: int = 1

    def set_black(self) -> None:
        self.color = False

    def set_red(self) -> None:
        self.color = True

    def is_red(self) -> bool:
        return self.color

    def is_black(self) -> bool:
        return not self.color

    def fix_height(self) -> None:
        self.height = max(Node.get_height(self.left), Node.get_height(self.right)) + 1

    @staticmethod
    def get_height(node: Node | None) -> int:
        return 0 if node is None else node.height

    @staticmethod
    def get_nill() -> Node:
        node = Node(None)
        node.set_black()
        return node


class RBTree:
    def __init__(self, debug_mode: bool = False) -> None:
        self.NILL = Node.get_nill()
        self.root = self.NILL
        self.debug_mode = debug_mode

    def in_order_traversal(self) -> list:
        values = []

        def traversal(root: Node | None) -> None:
            if root.value is None:
                return
            traversal(root.left)
            values.append(root.value)
            traversal(root.right)

        traversal(self.root)
        return values

    def insert(self, value: int) -> None:
        node = Node(value)
        node.left = self.NILL
        node.right = self.NILL
        node.set_red()

        y = None
        x = self.root
        while x.value is not None:
            y = x
            if node.value < x.value:
                x = x.left
            elif node.value > x.value:
                x = x.right
            else:
                return

        node.parent = y
        if y is None:
            self.root = node
        elif node.value < y.value:
            y.left = node
        else:
            y.right = node

        if node.parent is None:
            node.set_black()
            return
        if node.parent.parent is None:
            return
        self._fix_insert(node)

        if self.debug_mode:
            print(f"Insert RB. Is balanced: {self.is_rb_tree()}")
            print(f"RB data: {self.in_order_traversal()}")

    def delete(self, value: int) -> None:
        z = self.NILL
        node = self.root
        while node.value is not None:
            if node.value == value:
                z = node
                break
            if node.value < value:
                node = node.right
            else:
                node = node.left

        if z.value is None:
            return

        y = z
        y_original_color = y.color
        if z.left.value is None:
            x = z.right
            self._rb_transplant(z, z.right)
        elif z.right.value is None:
            x = z.left
            self._rb_transplant(z, z.left)
        else:
            y = self._minimum(z.right)
            y_original_color = y.color
            x = y.right
            if y.parent == z:
                x.parent = y
            else:
                self._rb_transplant(y, y.right)
                y.right = z.right
                y.right.parent = y

            self._rb_transplant(z, y)
            y.left = z.left
            y.left.parent = y
            y.color = z.color

        if not y_original_color:
            self._delete_fix(x)

        if self.debug_mode:
            print(f"Delete RB. Is balanced: {self.is_rb_tree()}")
            print(f"RB data: {self.in_order_traversal()}")

    def visualize(self, filename: str | None = None) -> bytes | None:
        graph = pygraphviz.AGraph(strict=True, directed=True)
        if self.root != self.NILL:
            self._add_edges(graph, self.root)
        graph.layout(prog="dot")
        return graph.draw(filename)

    def _add_edges(self, graph, node: Node):
        node_id = node.value
        graph.add_node(
            node_id,
            label=str(node.value),
            fillcolor="red" if node.is_red() else "black",
            fontcolor="white",
            style="filled"
        )
        if node.left and node.left.value is not None:
            graph.add_edge(node.value, node.left.value)
            self._add_edges(graph, node.left)
        if node.right and node.right.value is not None:
            graph.add_edge(node.value, node.right.value)
            self._add_edges(graph, node.right)

    def _rb_transplant(self, u: Node, v: Node) -> None:
        if u.parent is None:
            self.root = v
        elif u == u.parent.left:
            u.parent.left = v
        else:
            u.parent.right = v
        v.parent = u.parent

    def _minimum(self, node: Node = None) -> Node:
        if node is None:
            node = self.root
        if node.value is None:
            return self.NILL
        while node.left.value is not None:
            node = node.left
        return node

    def _left_rotate(self, x: Node) -> None:
        y = x.right
        x.right = y.left
        if y.left.value is not None:
            y.left.parent = x
        y.parent = x.parent
        if x.parent is None:
            self.root = y
        elif x == x.parent.left:
            x.parent.left = y
        else:
            x.parent.right = y
        y.left = x
        x.parent = y

    def _right_rotate(self, x: Node) -> None:
        y = x.left
        x.left = y.right
        if y.right.value is not None:
            y.right.parent = x
        y.parent = x.parent
        if x.parent is None:
            self.root = y
        elif x == x.parent.right:
            x.parent.right = y
        else:
            x.parent.left = y
        y.right = x
        x.parent = y

    def _fix_insert(self, node: Node) -> None:
        while node.parent.is_red():
            if node.parent == node.parent.parent.right:
                uncle = node.parent.parent.left
                if uncle.is_red():
                    uncle.set_black()
                    node.parent.set_black()
                    node.parent.parent.set_red()
                    node = node.parent.parent
                else:
                    if node == node.parent.left:
                        node = node.parent
                        self._right_rotate(node)
                    node.parent.set_black()
                    node.parent.parent.set_red()
                    self._left_rotate(node.parent.parent)
            else:
                uncle = node.parent.parent.right
                if uncle.is_red():
                    uncle.set_black()
                    node.parent.set_black()
                    node.parent.parent.set_red()
                    node = node.parent.parent
                else:
                    if node == node.parent.right:
                        node = node.parent
                        self._left_rotate(node)
                    node.parent.set_black()
                    node.parent.parent.set_red()
                    self._right_rotate(node.parent.parent)
            if node == self.root:
                break
        self.root.set_black()

    def _delete_fix(self, x: Node) -> None:
        while x != self.root and x.is_black():
            if x == x.parent.left:
                s = x.parent.right
                if s.is_red():
                    s.set_black()
                    x.parent.set_red()
                    self._left_rotate(x.parent)
                    s = x.parent.right
                if s.left.is_black() and s.right.is_black():
                    s.set_red()
                    x = x.parent
                else:
                    if s.right.is_black():
                        s.left.set_black()
                        s.set_red()
                        self._right_rotate(s)
                        s = x.parent.right

                    s.color = x.parent.color
                    x.parent.set_black()
                    s.right.set_black()
                    self._left_rotate(x.parent)
                    x = self.root
            else:
                s = x.parent.left
                if s.is_red():
                    s.set_black()
                    x.parent.set_red()
                    self._right_rotate(x.parent)
                    s = x.parent.left
                if s.left.is_black() and s.right.is_black():
                    s.set_red()
                    x = x.parent
                else:
                    if s.left.is_black():
                        s.right.set_black()
                        s.set_red()
                        self._left_rotate(s)
                        s = x.parent.left

                    s.color = x.parent.color
                    x.parent.set_black()
                    s.left.set_black()
                    self._right_rotate(x.parent)
                    x = self.root
        x.set_black()

    def is_rb_tree(self) -> bool:
        def get_height(node: Node) -> int:
            if node.value is None:
                return 1
            return 1 + max(get_height(node.left), get_height(node.right))

        def get_black(node: Node) -> int:
            if node.value is None:
                return 1
            count = get_black(node.left)
            if count == -1 or count != get_black(node.right):
                return -1
            return count + (1 if node.is_black() else 0)

        def is_node_balanced(node: Node) -> bool:
            if node.value is None:
                return True
            h1 = get_height(node.left)
            h2 = get_height(node.right)
            return all(
                (
                    max(h1, h2) / min(h1, h2) <= 2,
                    get_black(node) != -1,
                    node.left.is_black() and node.right.is_black() if node.is_red() else True,
                    is_node_balanced(node.left),
                    is_node_balanced(node.right)
                )
            )

        return is_node_balanced(self.root)
    

A = RBTree()

for i in range(100):
    A.insert(random.randint(0,100))

print(A.in_order_traversal())    

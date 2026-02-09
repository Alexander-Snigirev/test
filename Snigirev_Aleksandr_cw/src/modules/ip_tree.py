from .AVL import AVL, Node


def cmp_ip(ip1, ip2):
    for i in range(len(ip1)):
        if ip1[i]>ip2[i]:
            return 1
        elif ip1[i]<ip2[i]:
            return -1
    return 0 

class Tree_ip(AVL):
    def _insert(self, val: str, key: list[int], node: Node | None) -> Node:
        if node is None:
            return Node(key, val)
        if cmp_ip(key, node.key)==-1:
            node.left = self._insert(val, key, node.left)
        elif cmp_ip(key, node.key)==1:
            node.right = self._insert(val, key, node.right)   
        elif cmp_ip(key, node.key)==0:
            raise ValueError("Такая пара уже существует")    
        return self.balance(node)

    def insert_value(self, val: str, key: list[int]) -> None:
        self.root = self._insert(val, key, self.root)

    def delete_value(self, key: list[int]) -> None:
        self.root = self._delete(self.root, key)

    def _delete(self, node: Node | None, key: list[int]) -> Node | None:
        if node is None:
            return node
        if cmp_ip(key, node.key)==-1:
            node.left = self._delete(node.left, key)
        elif cmp_ip(key, node.key)==1:
            node.right = self._delete(node.right, key)
        elif node.left is not None and node.right is not None:
            right_min = node.right
            while right_min.left is not None:
                right_min = right_min.left
            node.key = right_min.key
            node.val = right_min.val
            node.right = self._delete(node.right, node.key)
        elif node.left is not None:
            node = node.left
        elif node.right is not None:
            node = node.right
        else:
            node = None
        return self.balance(node) if node else None

    def search(self, key: list[int]) -> str | None:
        current = self.root
        while current is not None:
            comparison = cmp_ip(key, current.key)
            if comparison == 0:
                return current.val  # Найден нужный узел
            elif comparison == -1:
                current = current.left  # Идем влево
            else:
                current = current.right  # Идем вправо
        return None
    
    def print_tree(self) -> None:
        curr_list = self.in_order_traversal(self.root)
        for elem in curr_list:
            print(".".join(list(map(str,elem[0]))), " - ", elem[1]) 






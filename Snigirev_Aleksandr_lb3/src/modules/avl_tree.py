import graphviz as gv

class Node:
    def __init__(self, key) -> None:
        self.balance = 0
        self.height = 0
        self.right = None
        self.left = None
        self.parent = None
        self.value = key

class AVLTree:
    def __init__(self):
        self.root = None

    def search_place(self, key) -> Node:
        curr = self.root
        parent = None
        while curr is not None:
            parent = curr
            if key < curr.value:
                curr = curr.left
            elif key > curr.value:
                curr = curr.right
            else:
                return curr
        return parent

    def insert(self, key):
        new = Node(key)
        if self.root is None:
            self.root = new
            return
        
        parent = self.search_place(key)

        if parent and parent.value == key:
            return

        new.parent = parent
        if key < parent.value:
            parent.left = new
        else:
            parent.right = new

        self.__update_balances_iterative(new)

    def delete(self, key):
        node_to_delete = self.search_place(key)
        if node_to_delete is None or node_to_delete.value != key:
            return

        if node_to_delete.left is None or node_to_delete.right is None:
            self.__replace_node_in_parent(node_to_delete, node_to_delete.left or node_to_delete.right)
        else:
            successor = self._get_min_value_node(node_to_delete.right)
            node_to_delete.value = successor.value
            self.__replace_node_in_parent(successor, successor.left)

        parent = node_to_delete.parent
        self.__update_balances_iterative(parent)

    def __replace_node_in_parent(self, node, new_child):
        if node.parent:
            if node == node.parent.left:
                node.parent.left = new_child
            else:
                node.parent.right = new_child
        else:
            self.root = new_child
        if new_child:
            new_child.parent = node.parent

    def _get_min_value_node(self, node):
        current = node
        while current.left is not None:
            current = current.left
        return current

    def __update_balances_iterative(self, node):
        while node:
            self.__update_height_balance(node)
            self.__restore_balance__(node)
            node = node.parent

    def __restore_balance__(self, node):
        if node is None:
            return node

        if node.balance < -1:
            if node.left and node.left.balance <= 0:
                return self.__rotate_r__(node)
            elif node.left and node.left.balance > 0:
                node.left = self.__rotate_l__(node.left)
                return self.__rotate_r__(node)
        elif node.balance > 1:
            if node.right and node.right.balance >= 0:
                return self.__rotate_l__(node)
            elif node.right and node.right.balance < 0:
                node.right = self.__rotate_r__(node.right)
                return self.__rotate_l__(node)
        
        return node

    def __rotate_r__(self, node):
        if node is None or node.left is None:
            return node

        y = node.left
        node.left = y.right
        if y.right is not None:
            y.right.parent = node
        y.parent = node.parent
        if node.parent is None:
            self.root = y
        elif node == node.parent.right:
            node.parent.right = y
        else:
            node.parent.left = y
        y.right = node
        node.parent = y
        self.__update_height_balance(node)
        self.__update_height_balance(y)
        return y

    def __rotate_l__(self, node):
        if node is None or node.right is None:
            return node

        y = node.right
        node.right = y.left
        if y.left is not None:
            y.left.parent = node
        y.parent = node.parent
        if node.parent is None:
            self.root = y
        elif node == node.parent.left:
            node.parent.left = y
        else:
            node.parent.right = y
        y.left = node
        node.parent = y
        self.__update_height_balance(node)
        self.__update_height_balance(y)
        return y

    def __update_height_balance(self, node):
        left_height = node.left.height if node.left else -1
        right_height = node.right.height if node.right else -1
        node.height = max(left_height, right_height) + 1
        node.balance = right_height - left_height

    def in_order(self, curr, res):
        if curr is not None:
            self.in_order(curr.left, res)
            res.append(curr.value)
            self.in_order(curr.right, res)
        return res
    
    def pre_order(self, curr, res):
        if curr is not None:
            res.append(curr.value)
            self.pre_order(curr.left, res)
            self.pre_order(curr.right, res)
        return res
   

    def create_graph(self):
        dot = gv.Digraph(format='png')
        nodes, edges = [], []
        stack = [(self.root, "")] if self.root is not None else []

        while stack:
            node, label = stack.pop()
            if node:
                dot.node(str(id(node)), str(node.value))
                nodes.append((id(node), node.value))

                if node.left:
                    dot.edge(str(id(node)), str(id(node.left)))
                    edges.append((node.value, node.left.value))
                    stack.append((node.left, "L"))

                if node.right:
                    dot.edge(str(id(node)), str(id(node.right)))
                    edges.append((node.value, node.right.value))
                    stack.append((node.right, "R"))

        return dot, nodes, edges

    def render_avl_tree(self):
        dot, _, _ = self.create_graph()
        dot.render('avl_tree.gv', view=True) 
         
                 
'''tree = AVLTree()
arr = [212, 69, 369, 390, 145,51, 350, 350 ]
for i in arr:
    tree.insert(i)
print(tree.pre_order(tree.root, []))
tree.render_avl_tree() '''
 
   

        





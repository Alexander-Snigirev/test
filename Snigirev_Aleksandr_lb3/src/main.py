from modules.avl_tree import AVLTree
from datetime import datetime


tree = AVLTree()


a = datetime.now()
tree.insert(150)
arr = [i for i in range(100)]
#tree.delete(150)
#tree.delete(120)
for item in arr:
    tree.insert(item)
b = datetime.now()
print(tree.pre_order(tree.root, []))
print(b-a)
tree.render_avl_tree()
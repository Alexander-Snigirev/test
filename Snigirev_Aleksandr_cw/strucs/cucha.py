class TreeNode:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None
        self.parent = None  # Adding the parent attribute to the TreeNode class

class BinaryHeapTree:
    def __init__(self, heap_type="min"):
        if heap_type not in {"min", "max"}:
            raise ValueError("heap_type must be either 'min' or 'max'")
        self.heap_type = heap_type
        self.root = None
        self.nodes = []

    def _compare(self, parent_value, child_value):
        """
        Compare function for the heap. Returns True if the parent satisfies the heap property with respect to the child.
        """
        if self.heap_type == "min":
            return parent_value <= child_value
        else:
            return parent_value >= child_value

    def _swap_values(self, node_a, node_b):
        """Helper method to swap values of two nodes."""
        node_a.value, node_b.value = node_b.value, node_a.value

    def _heapify_up(self, node):
        """Move a node up to maintain the heap property."""
        if not node or not node.parent:
            return
        parent = node.parent
        if not self._compare(parent.value, node.value):
            self._swap_values(parent, node)
            self._heapify_up(parent)

    def _heapify_down(self, node):
        """Move a node down to maintain the heap property."""
        if not node:
            return

        smallest_or_largest = node

        if node.left and not self._compare(node.value, node.left.value):
            smallest_or_largest = node.left

        if node.right and not self._compare(smallest_or_largest.value, node.right.value):
            smallest_or_largest = node.right

        if smallest_or_largest != node:
            self._swap_values(node, smallest_or_largest)
            self._heapify_down(smallest_or_largest)

    def insert(self, value):
        """Insert a value into the heap."""
        new_node = TreeNode(value)
        if not self.root:
            self.root = new_node
        else:
            parent = self.nodes[0]
            if not parent.left:
                parent.left = new_node
            elif not parent.right:
                parent.right = new_node
                self.nodes.pop(0)

            new_node.parent = parent  # Assign the parent

        self.nodes.append(new_node)
        self._heapify_up(new_node)

    def extract(self):
        """
        Remove and return the root element (min or max depending on the heap type).
        Raise an exception if the heap is empty.
        """
        if not self.root:
            raise IndexError("extract from an empty heap")

        root_value = self.root.value
        last_node = self.nodes.pop()

        if self.root == last_node:
            self.root = None
        else:
            self.root.value = last_node.value
            if last_node.parent.right == last_node:
                last_node.parent.right = None
            else:
                last_node.parent.left = None

            self._heapify_down(self.root)

        return root_value

    def peek(self):
        """Return the root element without removing it."""
        if not self.root:
            raise IndexError("peek from an empty heap")
        return self.root.value

    def is_empty(self):
        """Check if the heap is empty."""
        return self.root is None

# Function to check if a tree is a max-heap
def is_max_heap(root):
    """
    Check if a binary tree rooted at `root` satisfies the max-heap property.

    :param root: The root of the binary tree (node with `value`, `left`, and `right` attributes).
    :return: True if the tree is a max-heap, False otherwise.
    """
    def is_heap(node):
        if not node:
            return True

        left = node.left
        right = node.right

        if left and node.value < left.value:
            return False
        if right and node.value < right.value:
            return False

        return is_heap(left) and is_heap(right)

    return is_heap(root)

# Пример использования:
max_heap_tree = BinaryHeapTree("max")
max_heap_tree.insert(10)
max_heap_tree.insert(20)
max_heap_tree.insert(5)
print(max_heap_tree.extract())  # 20

min_heap_tree = BinaryHeapTree("min")
min_heap_tree.insert(10)
min_heap_tree.insert(20)
min_heap_tree.insert(5)
print(min_heap_tree.extract())  # 5

print(is_max_heap(max_heap_tree.root))
print(is_max_heap(min_heap_tree.root))

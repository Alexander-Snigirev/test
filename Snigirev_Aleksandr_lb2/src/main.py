class Node:
    def __init__(self, key):
        self.val = key
        self.left = None
        self.right = None
        self.height = 1

def insert(val, node: Node) -> Node:
    # Если дерево пустое, создаем новый узел
    if node is None:
        return Node(val)

    # Поиск позиции для вставки узла
    curr = node
    parent_stack = []  # Стек для хранения пути к новому узлу

    while curr:
        parent_stack.append(curr)  # Запоминаем путь к корню для балансировки
        if val < curr.val:
            if curr.left is None:
                curr.left = Node(val)
                parent_stack.append(curr.left)
                break
            curr = curr.left
        elif val > curr.val:
            if curr.right is None:
                curr.right = Node(val)
                parent_stack.append(curr.right)
                break
            curr = curr.right
        else:
            # Если значение уже существует, возвращаем узел
            return node

    # Обновление высот и балансировка узлов по пути к корню
    while parent_stack:
        curr = parent_stack.pop()
        curr.height = 1 + max(get_height(curr.left), get_height(curr.right))
        balance = get_balance(curr)

        # Проверяем и выполняем нужное вращение
        if balance > 1 and val < curr.left.val:
            if parent_stack:
                parent = parent_stack[-1]
                if parent.left == curr:
                    parent.left = rotate_right(curr)
                else:
                    parent.right = rotate_right(curr)
            else:
                node = rotate_right(curr)
        elif balance < -1 and val > curr.right.val:
            if parent_stack:
                parent = parent_stack[-1]
                if parent.right == curr:
                    parent.right = rotate_left(curr)
                else:
                    parent.left = rotate_left(curr)
            else:
                node = rotate_left(curr)
        elif balance > 1 and val > curr.left.val:
            curr.left = rotate_left(curr.left)
            if parent_stack:
                parent = parent_stack[-1]
                if parent.left == curr:
                    parent.left = rotate_right(curr)
                else:
                    parent.right = rotate_right(curr)
            else:
                node = rotate_right(curr)
        elif balance < -1 and val < curr.right.val:
            curr.right = rotate_right(curr.right)
            if parent_stack:
                parent = parent_stack[-1]
                if parent.right == curr:
                    parent.right = rotate_left(curr)
                else:
                    parent.left = rotate_left(curr)
            else:
                node = rotate_left(curr)

    return node

def get_height(node: Node) -> int:
    if node is None:
        return 0
    return node.height

def get_balance(node: Node) -> int:
    if node is None:
        return 0
    return get_height(node.left) - get_height(node.right)

def rotate_left(z: Node) -> Node:
    y = z.right
    T2 = y.left

    # Выполняем вращение
    y.left = z
    z.right = T2

    # Обновляем высоты
    z.height = 1 + max(get_height(z.left), get_height(z.right))
    y.height = 1 + max(get_height(y.left), get_height(y.right))

    # Возвращаем новый корень
    return y

def rotate_right(z: Node) -> Node:
    y = z.left
    T3 = y.right

    # Выполняем вращение
    y.right = z
    z.left = T3

    # Обновляем высоты
    z.height = 1 + max(get_height(z.left), get_height(z.right))
    y.height = 1 + max(get_height(y.left), get_height(y.right))

    # Возвращаем новый корень
    return y

import math
DEBUG = True
class Node:
    def __init__(self, matrix, infinum, way, edges, parent=None, height=0):
        self.matrix = matrix
        self.infinum = infinum
        self.way = way
        self.edges = edges
        self.parent = parent
        self.height = height

    @staticmethod
    def copy_matrix(matrix):
        return [row[:] for row in matrix]

    @staticmethod
    def row_mins(matrix):
        return [min(row) for row in matrix]

    @staticmethod
    def column_mins(matrix):
        return [min(matrix[i][j] for i in range(len(matrix))) for j in range(len(matrix[0]))]

    @staticmethod
    def reduce_rows(matrix, mins):
        for i in range(len(matrix)):
            if math.isfinite(mins[i]):
                matrix[i] = [cell - mins[i] for cell in matrix[i]]

    @staticmethod
    def reduce_columns(matrix, mins):
        for j in range(len(matrix[0])):
            if math.isfinite(mins[j]):
                for i in range(len(matrix)):
                    matrix[i][j] -= mins[j]

    @staticmethod
    def reduce(matrix):
        if DEBUG:
            print("Редукция матрицы...")
        row_mins = Node.row_mins(matrix)
        Node.reduce_rows(matrix, row_mins)

        col_mins = Node.column_mins(matrix)
        Node.reduce_columns(matrix, col_mins)
        result = sum(val for val in row_mins if math.isfinite(val)) + sum(val for val in col_mins if math.isfinite(val))
        if DEBUG:
            print(f"Нижняя граница, полученная при редукции: {result}")
        return result

    def find_edge_max_penalty(self):
        max_penalty = -math.inf
        edge_max_penalty = None

        if DEBUG:
            print("Поиск ребра с максимальным штрафом")

        for i in range(len(self.matrix)):
            for j in range(len(self.matrix[i])):
                if self.matrix[i][j] == 0:
                    row_min = min(
                        (self.matrix[i][k] for k in range(len(self.matrix[i])) if k != j),
                        default=math.inf
                    )
                    col_min = min(
                        (self.matrix[k][j] for k in range(len(self.matrix)) if k != i),
                        default=math.inf
                    )
                    penalty = row_min + col_min
                    if penalty > max_penalty:
                        max_penalty = penalty
                        edge_max_penalty = (i, j, max_penalty)

                    if DEBUG:
                        print(f"Ребро ({i}, {j}): Штраф {penalty}")

        if DEBUG:
            print(f"Ребро с максимальным штрафом: {edge_max_penalty}\n")
        return edge_max_penalty

    def get_infinums(self):
        if DEBUG:
            print("Вычисление нижней оценки с помощью Минимального Остовного Дерева:")

        mod_infinum = self.calculate_mod_weight(self.make_mod_graph(self.get_acceptable_edges()))

        if DEBUG:
            print(f"Нижняя оценка: {mod_infinum}\n")
        return mod_infinum

    def get_acceptable_edges(self):
        acceptable_edges = []
        for i in range(len(self.edges)):
            for j in range(i + 1, len(self.edges)):
                u, v = self.edges[i][-1], self.edges[j][0]
                if u != v and math.isfinite(self.matrix[u][v]):
                    acceptable_edges.append((u, v))
                u, v = self.edges[j][-1], self.edges[i][0]
                if u != v and math.isfinite(self.matrix[u][v]):
                    acceptable_edges.append((u, v))
        return acceptable_edges

    def make_mod_graph(self, edges):
        if DEBUG:
            print("Построение графа МОД:")
        mod_graph = {}
        for edge in edges:
            if edge[0] not in mod_graph:
                mod_graph[edge[0]] = []
            if edge[1] not in mod_graph:
                mod_graph[edge[1]] = []
            mod_graph[edge[0]].append((edge[1], self.matrix[edge[0]][edge[1]]))
            mod_graph[edge[1]].append((edge[0], self.matrix[edge[0]][edge[1]]))

        if DEBUG:
            print(f"Минимальное остовное дерево: {mod_graph}\n")
        return mod_graph

    def calculate_mod_weight(self, mod_graph):
        mod_weight = 0
        used_edges = set()
        edges = []
        for node in mod_graph:
            for edge in mod_graph[node]:
                if math.isfinite(edge[1]):  
                    edges.append((edge[1], node, edge[0]))
        edges.sort()
        for edge in edges:
            if (edge[1], edge[2]) not in used_edges and (edge[2], edge[1]) not in used_edges:
                mod_weight += edge[0]
                used_edges.add((edge[1], edge[2]))
                used_edges.add((edge[2], edge[1]))
        return mod_weight


def is_hamilton_cycle(way):
    if DEBUG:
        print("Проверка на гамильтонов цикл:")

    if len(way) != len(set(way)):
        if DEBUG:
            print("Не гамильтонов цикл: длина маршрута не соответствует количеству уникальных вершин.")
        return False
    graph = {}
    for u, v in way:
        graph[u] = v
    visited = set()
    current = way[0][0]
    while current not in visited:
        visited.add(current)
        if current not in graph:
            if DEBUG:
                print("Не гамильтонов цикл: не все вершины посещены.")
            return False
        current = graph[current]

    if len(visited) == len(graph):
        if DEBUG:
            print("Гамильтонов цикл найден.\n")
        return True
    else:
        if DEBUG:
            print("Не гамильтонов цикл: не все вершины посещены.\n")
        return False


def create_branches(min_node):
    if DEBUG:
        print("Создание потомков:")

    row, column, left_penalty = min_node.find_edge_max_penalty()
    if row is None or column is None:
        return []
    left_matrix = Node.copy_matrix(min_node.matrix)
    left_matrix[row][column] = math.inf
    left_way = min_node.way[:]

    left_edges = [edge[:] for edge in min_node.edges]

    cyclic_row, cyclic_col = None, None
    if min_node.height == 0:
        cyclic_row, cyclic_col = column, row
    else:
        for edge in left_edges:
            if edge[-1] == row:
                edge.append(column)
                break
        else:
            left_edges.append([row, column])

        for edge in left_edges:
            if len(edge) >= 3:
                if edge[-2] == row and edge[-1] == column:
                    for other_edge in left_edges:
                        if other_edge != edge and other_edge[0] != edge[-1]:
                            cyclic_row, cyclic_col = edge[-1], other_edge[0]
                            break
                    if cyclic_row is None:
                        cyclic_row, cyclic_col = edge[-1], edge[0]
                    break

        if cyclic_row is None or cyclic_col is None:
            cyclic_row, cyclic_col = column, row

    if cyclic_row is not None and cyclic_col is not None:
        left_matrix[cyclic_row][cyclic_col] = math.inf

    Node.reduce(left_matrix)
    left_infinum = min_node.infinum + left_penalty
    left_child = Node(left_matrix, left_infinum, left_way, left_edges, parent=min_node, height=min_node.height + 1)

    right_matrix = Node.copy_matrix(min_node.matrix)
    right_matrix[column][row] = math.inf
    for i in range(len(right_matrix)):
        right_matrix[row][i] = math.inf
        right_matrix[i][column] = math.inf

    right_way = min_node.way + [(row, column)]
    right_penalty = Node.reduce(right_matrix)
    right_infinum = min_node.infinum + right_penalty
    right_edges = [edge[:] for edge in min_node.edges]
    for edge in right_edges:
        if edge[-1] == row:
            edge.append(column)
            break
    else:
        right_edges.append([row, column])

    right_child = Node(right_matrix, right_infinum, right_way, right_edges, parent=min_node, height=min_node.height + 1)

    if DEBUG:
        print(f"Левый потомок: нижняя граница: {left_infinum}, путь: {left_way}, запрещенная дуга: ({cyclic_row}, {cyclic_col})")
        print(f"Правый потомок: нижняя граница {right_infinum}, путь {right_way}")

    return [left_child, right_child]


def LittleAlgorithm(matrix):
    root_matrix = Node.copy_matrix(matrix)
    min_infinum = Node.reduce(root_matrix)
    root = Node(root_matrix, min_infinum, [], [[0]])
    priority_queue = [root]
    best_way = None
    nodes_for_graph = []

    while priority_queue:
        if DEBUG:
            print("Выбираем из очереди узел с минимальной нижней границей")

        min_node = min(priority_queue, key=lambda node: node.infinum)
        # Удаляем этот узел из очереди
        priority_queue.remove(min_node)
        nodes_for_graph.append(min_node)
        
        if DEBUG:
            print(f"Граница: {min_node.infinum}, Путь: {min_node.way}\n")

        if best_way is not None and best_way['length'] <= min_node.infinum:
            if DEBUG:
                print("Больше нет конкурентоспособных путей. Алгоритм завершен")
            break

        if len(min_node.way) == len(matrix) - 1:
            if DEBUG:
                print("Построение полного маршрута:")
               
            for row in range(len(matrix)):
                for column in range(len(matrix)):
                    if math.isfinite(min_node.matrix[row][column]):
                        min_node.infinum += min_node.matrix[row][column]
                        min_node.way.append((row, column))
                        # Добавление замыкающего ребра

            if is_hamilton_cycle(min_node.way):
                if best_way is None or best_way['length'] > min_node.infinum:
                    best_way = {'length': min_node.infinum, 'way': min_node.way}
                    if DEBUG:
                        print(f"Найден новый маршрут: Длина {best_way['length']}, Путь {best_way['way']}\n")
        else:
            if DEBUG:
                print("Вычисление нижней оценки:")
            # Применение МОД для уточнения минимальной нижней границы
            lower_infinum = min_node.get_infinums()
            if lower_infinum > min_node.infinum:
                min_node.infinum = lower_infinum
                if DEBUG:
                    print(f"Обновлена граница узла: {min_node.infinum}\n")
            # Создание потомков    
            left_child, right_child = create_branches(min_node)
            # Добавление потомков в очередь
            priority_queue.append(left_child)
            priority_queue.append(right_child)

    return best_way, nodes_for_graph
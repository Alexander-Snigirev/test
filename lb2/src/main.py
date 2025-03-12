from little_algorithm import LittleAlgorithm
from matrix import generate_matrix, generate_symmetric_matrix,save_matrix_to_file,load_matrix_from_file
from nearest_neighbour import NearestNeighborAlgorithm

print("Считать матрицу из файла? (Y/n)")
answer = input()
if answer == 'Y':
    print("Введите имя файла")
    filename = input()
    matrix = load_matrix_from_file(filename)
else:
    print("Введите размер матрицы:")
    size = int(input())
    print("1 - Симметричная")
    print("2 - Обычная")
    type = int(input())

    if type == 1:
        matrix = generate_symmetric_matrix(size)
    elif type == 2:
        matrix = generate_matrix(size)

    print("Сгенерированная матрица:")
    for row in matrix:
        print(row)
    filename = 'matrix.txt'
    save_matrix_to_file(matrix, filename)
    print(f"Матрица сохранена в файл {filename}")

    print(" ")
    matrix = load_matrix_from_file('matrix.txt')
result, nodes = LittleAlgorithm(matrix)
print("Алгоритм Литтла:")
print(f"Минимальный путь: {result['way']}")
print(f"Длина пути: {result['length']}\n")


print("Алгоритм АБС:")
print("Введите стартовую вершину:")
start_vertex = int(input())
route, distance = NearestNeighborAlgorithm(matrix, start_vertex)
print(f"Маршрут: {route}")
print(f"Длина маршрута: {distance}")

import matplotlib.pyplot as plt
import numpy as np
from typing import List, Tuple

def is_valid(grid: List[List[int]], x: int, y: int, size: int, N: int) -> bool:
    """Проверяет, можно ли разместить квадрат размера size в позиции (x, y)."""
    if x + size > N or y + size > N:
        return False
    for i in range(x, x + size):
        for j in range(y, y + size):
            if grid[i][j] != 0:
                return False
    return True

def place_square(grid: List[List[int]], x: int, y: int, size: int, label: int):
    """Размещает квадрат размера size в позиции (x, y)."""
    for i in range(x, x + size):
        for j in range(y, y + size):
            grid[i][j] = label

def remove_square(grid: List[List[int]], x: int, y: int, size: int):
    """Удаляет квадрат размера size из позиции (x, y)."""
    for i in range(x, x + size):
        for j in range(y, y + size):
            grid[i][j] = 0

def backtrack(grid: List[List[int]], squares: List[Tuple[int, int, int]], N: int, best_solution: List[Tuple[int, int, int]]):
    """Рекурсивный поиск с возвратом с минимизацией количества квадратов."""
    if best_solution and len(squares) >= len(best_solution):
        return
    
    min_x, min_y = None, None
    for x in range(N):
        for y in range(N):
            if grid[x][y] == 0:
                min_x, min_y = x, y
                break
        if min_x is not None:
            break
    
    if min_x is None:
        if not best_solution or len(squares) < len(best_solution):
            best_solution.clear()
            best_solution.extend(squares[:])
        return
    
    max_size = min(N - min_x, N - min_y, N//2+1)
    for size in range(max_size, 0, -1):
        if is_valid(grid, min_x, min_y, size, N):
            place_square(grid, min_x, min_y, size, len(squares) + 1)
            squares.append((min_x, min_y, size))
            backtrack(grid, squares, N, best_solution)
            squares.pop()
            remove_square(grid, min_x, min_y, size)

def squaring_the_square(N: int) -> List[Tuple[int, int, int]]:
    """Запускает алгоритм и возвращает минимальный набор квадратов."""
    grid = [[0] * N for _ in range(N)]
    best_solution = []
    backtrack(grid, [], N, best_solution)
    return best_solution

def visualize_solution(N: int, solution: List[Tuple[int, int, int]]):
    """Визуализирует разбиение квадрата."""
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.set_xticks(np.arange(0, N+1, 1))
    ax.set_yticks(np.arange(0, N+1, 1))
    ax.set_xlim(0, N)
    ax.set_ylim(0, N)
    ax.grid(True, which='both', color='black', linestyle='-', linewidth=1)
    ax.set_xticklabels([])
    ax.set_yticklabels([])
    ax.set_frame_on(False)
    for x, y, size in solution:
        rect = plt.Rectangle((y, N - x - size), size, size, linewidth=2, edgecolor='black', facecolor=np.random.rand(3,))
        ax.add_patch(rect)
        ax.text(y + size / 2, N - x - size / 2, str(size), ha='center', va='center', fontsize=12, color='white', weight='bold')
    plt.title(f"Разбиение квадрата {N}x{N} на минимальные квадраты")
    plt.savefig('go6.png')

def print_solution_matrix(N: int, solution: List[Tuple[int, int, int]]):
    """Создает и выводит матрицу разбиения квадрата."""
    matrix = [[0] * N for _ in range(N)]
    for index, (x, y, size) in enumerate(solution, start=1):
        for i in range(x, x + size):
            for j in range(y, y + size):
                matrix[i][j] = index
    print("Итоговая матрица разбиения:")
    for row in matrix:
        print(" ".join(str(cell) if cell > 0 else "." for cell in row))

# Запуск алгоритма
N = int(input())
solution = squaring_the_square(N)
print("Минимальное разбиение квадрата 6x6:", solution)
print_solution_matrix(N, solution)
visualize_solution(N, solution)

from modules.graham import graham_algorithm, calculate_area
from modules.karp import rabin_karp
from draw import plot_points_and_lines

command = int(input())


if command == 1:
    n = int(input())
    points = []
    for i in range(n):
        x, y = map(int, input().split(","))
        points.append([x, y])
    
    go_round = graham_algorithm(points)
    area = calculate_area(go_round)
    print((go_round, area))
    plot_points_and_lines(points, go_round)


elif command == 2:
    pattern = input().strip()
    text = input().strip()

    result = rabin_karp(pattern, text)
    print(" ".join(map(str, result)))    
import pytest


from modules.graham import graham_algorithm, calculate_area, check_positions
from modules.karp import rabin_karp

def test_graham_algorithm():
    points = [[0,0],[2,2],[2,0],[0,2]]
    assert graham_algorithm(points) == [[0, 0], [2, 0], [2, 2], [0, 2]]
    points.append([1,1])
    assert graham_algorithm(points) == [[0, 0], [2, 0], [2, 2], [0, 2]]
    points.append([5,5])
    assert graham_algorithm(points) == [[0, 0], [2, 0], [2, 2], [1, 1], [5, 5], [0, 2]]
    points[2], points[3] = points[3], points[2]
    assert graham_algorithm(points) == [[0, 0], [2, 0], [2, 2], [1, 1], [5, 5], [0, 2]]
    
    assert graham_algorithm([[0,0], [1,1]])==[[0,0], [1,1]]

    points = [[3, 1], [6, 8], [1, 7], [9, 0], [9, 3], [9, 6]]
    assert graham_algorithm(points) == [[1, 7], [3, 1], [9, 0], [9, 3], [9, 6], [6, 8]]


def test_calculate_area():
    points = [[1, 7], [3, 1], [9, 0], [9, 3], [9, 6], [6, 8]]
    assert calculate_area(points) == 47.5
    points.pop(5)
    assert calculate_area(points) == 41.0
    null_points = [[0,0],[1,1]]
    assert calculate_area(null_points) == 0.0
    points = [[0, 0], [2, 0], [2, 2], [0, 2]]
    assert calculate_area(points) == 4.0

def test_check_positions():
    p1 = [0,0]
    p2 = [1,1]
    p3 = [2,2]
    assert check_positions(p1,p2,p3) == 0
    assert check_positions(p3,p1,p2) == 0
    p1 = [5,0]
    p2 = [11,6]
    p3 = [4,8]
    assert (check_positions(p1,p2,p3)>0)
    p2 = [0,6]
    assert (check_positions(p1,p2,p3)<0)


def test_rabin_karp():
    pattern = 'Ring'
    text = 'The One Ring to rule them all'
    assert rabin_karp(pattern, text) == [8]
    pattern = 'rule'
    assert rabin_karp(pattern, text) == [16]
    pattern = 'l'
    assert rabin_karp(pattern, text) == [18, 27, 28]
    pattern = 'Arthas Menethil'
    assert rabin_karp(pattern, text) == []












import pytest
from modules import timsort

@pytest.fixture
def test_bin_search():
    assert timsort.bin_search([6,5,2,1], 3) == 2
    assert timsort.bin_search([5,4,3,2,1], 10) == 0
    assert timsort.bin_search([-5, -4, 3, 3, -2, -1], -3) == 4
    assert timsort.bin_search([666, 42, -12], 15) == 2

def test_insertion_sort():
    array = [1, 7, 8, 12, 21, 3, 9, 0]
    assert timsort.insertion_sort(array, False, sorted_end=4) == sorted(array)[::-1]
    assert timsort.insertion_sort(array, False) == sorted(array)[::-1]
    assert timsort.insertion_sort([51, 23, 8, 4, 4], False, sorted_end=4) == [51, 23, 8, 4, 4]


def test_runs_merge():
    assert timsort.runs_merge([[11,5,3], [9,8,1]]) == [11,9,8,5,3,1]
    assert timsort.runs_merge([3,2,0],[9,6,1],[10,6,0]) == [10,9,6,6,3,2,1,0,0]
    assert timsort.runs_merge([3,2,0],[9,6,1],[10,6,0]) == [10,9,6,6,3,2,1,0,0]
    assert timsort.runs_merge([-3,3,2,0],[9,6,1],[10,6,0]) == [10,9,6,6,-3,3,2,1,0,0]


def test_calculate_optimal_minrun():
    assert timsort.calculate_optimal_minrun(100, 64)==50
    assert timsort.calculate_optimal_minrun(1000, 64)==63
    assert timsort.calculate_optimal_minrun(65, 64)==33

def test_timsort():
    assert timsort.TimSort([1,2,3,4,5])==[5,4,3,2,1]
    assert timsort.TimSort([1,2,3,4,5])==[5,4,3,2,1]
    assert timsort.TimSort([-100,1,28,3006,445,571])==[3006,571,445,-100,28,1]
    assert timsort.TimSort([100,91,28,30,4,1])==[100,91,28,30,4,1]



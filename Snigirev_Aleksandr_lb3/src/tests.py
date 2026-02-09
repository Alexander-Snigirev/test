import pytest
from main import AVLTree

@pytest.fixture
def avl_tree():
    return AVLTree()

def test_insert_and_balance(avl_tree):
    avl_tree.insert(56)
    avl_tree.insert(112)
    avl_tree.insert(-5)
    expected_list = [56, -5, 112]
    assert avl_tree.pre_order(avl_tree.root,[])==expected_list
    avl_tree.insert(10)
    expected_list = [56,-5,10,112]
    assert avl_tree.pre_order(avl_tree.root,[])==expected_list
    avl_tree.insert(-10)
    avl_tree.insert(-6)
    expected_list = [-5,-10,-6,56,10,112]
    assert avl_tree.pre_order(avl_tree.root,[])==expected_list
    avl_tree.insert(-7)
    expected_list = [-5,-7,-10,-6,56,10,112]
    assert avl_tree.pre_order(avl_tree.root,[])==expected_list


def test_delete_and_balance(avl_tree):
    arr = [212, 69, 369, 390, 145,51, 350, 350]
    for item in arr:
        avl_tree.insert(item)
    avl_tree.delete(1000)
    exp_lst = [212,69,51,145,369,350,390]
    assert avl_tree.pre_order(avl_tree.root,[])==exp_lst
    avl_tree.delete(212)
    exp_lst = [350,69,51,145,369,390]
    assert avl_tree.pre_order(avl_tree.root,[])==exp_lst
    avl_tree.delete(369)
    exp_lst = [350,69,51,145,390]
    assert avl_tree.pre_order(avl_tree.root,[])==exp_lst
    avl_tree.delete(390)
    exp_lst = [69,51,350,145]
    assert avl_tree.pre_order(avl_tree.root,[])==exp_lst

def test_tree_sorting(avl_tree):
    arr = [212, 69, 369, 390, 145,51, 350]
    for item in arr:
        avl_tree.insert(item)
    assert avl_tree.in_order(avl_tree.root,[])==sorted(arr)    











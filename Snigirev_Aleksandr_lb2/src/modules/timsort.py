from modules.constants import Constants


def find_runs(array):
    if len(array) <= 1:
        return [array]
    min_run = calculate_optimal_minrun(len(array))
    run_start = 0
    runs = []
    while run_start < len(array) - 1:
        end_idx = run_start
        increase_flag = abs(array[run_start + 1]) > abs(array[run_start])
        if increase_flag:
            while end_idx < len(array) - 1 and abs(array[end_idx + 1]) > abs(array[end_idx]):
                end_idx += 1
        else:
            while end_idx < len(array) - 1 and abs(array[end_idx + 1]) <= abs(array[end_idx]):
                end_idx += 1

        if (end_idx - run_start + 1) < min_run:
            sorted_idx = end_idx
            end_idx = min(run_start + min_run - 1,len(array) - 1)
            current_run = array[run_start:end_idx+1]
            current_run = insertion_sort(current_run, increase_flag, sorted_end=sorted_idx - run_start)
        else:
            current_run = array[run_start:end_idx + 1]
            if increase_flag:
                current_run = current_run[::-1]
        runs.append(current_run)
        run_start += len(current_run)

    return runs


def runs_merge(runs, debug = False):
    stack = []
    merges_number = 0
    yx_mode = (-2,-1)
    zy_mode = (-3,-2)
    if debug:
        for i in range(len(runs)):
            print(f"Part {i}:", *runs[i])

    for run in runs:
        stack.append(run)
        while len(stack) >= 2:
            X = len(stack[-1])
            Y = len(stack[-2])
            if len(stack) > 2:
                Z = len(stack[-3])
                if Z > X + Y or Y > X:
                    break
                if X < Z:
                    stack = stack_merge(stack, merges_number, debug, indexes=yx_mode)
                    merges_number += 1
                else:
                    stack = stack_merge(stack, merges_number, debug, indexes=zy_mode)
                    merges_number += 1
            if X < Y:
                break
            stack = stack_merge(stack, merges_number, debug, indexes=yx_mode)
            merges_number += 1

    if len(stack) >= 2:
        X = len(stack[-1])
        Z = None
        if len(stack) > 2:
            Z = len(stack[-3])
        if Z and X > Z:
            stack = stack_merge(stack, merges_number, debug, indexes=zy_mode)
            merges_number += 1
        else:
            stack = stack_merge(stack, merges_number, debug, indexes=yx_mode)
            merges_number += 1

    return stack[0]


def stack_merge(stack, merges_number, debug, indexes):
    second, first = indexes
    stack[-2] = gallop_merge(stack[second], stack[first], merges_number=merges_number, debug=debug)
    if debug:
        print(f"Merge {merges_number}:", *stack[second])
    stack.pop(first)
    return stack


def gallop_merge(left_array,right_array, gallop_start = Constants.GALLOP_N, merges_number = -1, debug = Constants.DEBUG):
    gallops_num = 0
    res_array = []
    gallop_buffer = 0
    left = 1
    right = 2
    gallop_array = left
    while len(left_array) > 0 and len(right_array) > 0:
        merge_idx = 1
        if abs(left_array[0]) >= abs(right_array[0]):
            if gallop_array == left:
                gallop_buffer += 1
                if gallop_buffer == gallop_start:
                    merge_idx = bin_search(left_array, right_array[0])
                    gallop_buffer = 0
                    gallops_num+=1
            else:
                gallop_array = left
                gallop_buffer = 1
            res_array.extend(left_array[:merge_idx])
            left_array = left_array[merge_idx:]
        else:
            if gallop_array == right:
                gallop_buffer += 1
                if gallop_buffer == gallop_start:
                    merge_idx = bin_search(right_array, left_array[0])
                    gallop_buffer = 0
                    gallops_num += 1
            else:
                gallop_array = right
                gallop_buffer = 1
            res_array.extend(right_array[:merge_idx])
            right_array = right_array[merge_idx:]
    if debug and merges_number >= 0:
        print(f"Gallops {merges_number}: {gallops_num}")
    return res_array + left_array + right_array


def bin_search(array,number):
    left = 0
    right = len(array)
    mid = (left + right)//2
    while right > left:
        mid = (left + right)//2
        if abs(array[mid]) < abs(number):
            right = mid
        else:
            left = mid+1
    return left


def insertion_sort(array, increase = False, sorted_end = 0):
    if increase == True:
        array = array[:sorted_end+1][::-1] + array[sorted_end+1:]
    for i in range(sorted_end+1,len(array)):
        array.insert(bin_search(array[:i],array[i]),array.pop(i))
    return array


def calculate_optimal_minrun(num, min_num = Constants.MIN_RUN):
    flag = 0
    while num >= min_num:
        flag = num%2
        num //= 2
    return num + flag


def TimSort(array, debug = False):
    runs = find_runs(array)
    return runs_merge(runs,debug=debug)
     
print(calculate_optimal_minrun(65, 64))     
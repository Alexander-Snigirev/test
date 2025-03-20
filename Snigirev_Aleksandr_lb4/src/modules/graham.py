def check_positions(p1: list[int], p2: list[int], p3: list[int]):
    return (p2[0]-p1[0])*(p3[1]-p2[1])-(p2[1]-p1[1])*(p3[0]-p2[0])

def calculate_area(points: list[list[int]]):
    area = 0
    for i in range(len(points)):
        j = (i + 1) % len(points)
        area += points[i][0] * points[j][1] - points[i][1] * points[j][0]
    return abs(area)/2.0

def graham_algorithm(points: list[list[int]]):
    length = len(points)
    indexes = [i for i in range(length)]
    
    for i in range(1,length):
        if points[indexes[i]][0]<points[indexes[0]][0]:
            indexes[i], indexes[0] = indexes[0], indexes[i]
    for i in range(2,length):
        j = i
        while j>1 and (check_positions(points[indexes[0]],points[indexes[j-1]],points[indexes[j]])<0):
            indexes[j], indexes[j-1] = indexes[j-1], indexes[j]
            j -= 1
    stack = [indexes[0],indexes[1]]
    for i in range(2,length):
        while check_positions(points[stack[-2]], points[stack[-1]], points[indexes[i]])<0:
            stack.pop()
        stack.append(indexes[i])
    ans = [points[i] for i in stack]
    return ans
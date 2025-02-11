#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;




bool is_valid(vector<vector<int>>& grid, int x, int y, int size, int N) {
    if (x + size > N || y + size > N) return false;
    //for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            if (grid[x][j] != 0) return false;
    return true;
}

void place_square(vector<vector<int>>& grid, int x, int y, int size, int label) {
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            grid[i][j] = label;
}

void remove_square(vector<vector<int>>& grid, int x, int y, int size) {
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            grid[i][j] = 0;
}

pair<int,int> find_place(vector<vector<int>>& matrix){
    pair<int, int> result = {-1,-1};
    for (int x = 0; x < matrix.size(); x++) {
        for (int y = 0; y < matrix.size(); y++) {
            if (matrix[x][y] == 0) {
                result.first = x;
                result.second = y;
                break;
            }
        }
        if (result.first != -1) break;
    }
    return result;
}

void backtrack(vector<vector<int>>& grid, vector<tuple<int, int, int>>& squares, int N, vector<tuple<int, int, int>>& best_solution, int remaining_area) {
    if (!best_solution.empty() && squares.size() >= best_solution.size()) return;

    
    pair<int,int> coords = find_place(grid);
    int min_x = coords.first;
    int min_y = coords.second;
    if (min_x == -1) {
        if (best_solution.empty() || squares.size() < best_solution.size()) {
            best_solution = squares;
        }
        return;
    }

    int max_size = min(N - min_x, N - min_y);
    if(max_size > N - (N+1)/2){
        max_size = N-(N+1)/2;
    }
    for (int size = max_size; size > 0; size--) {
        if (size*size<=remaining_area && is_valid(grid, min_x, min_y, size, N)) {
            place_square(grid, min_x, min_y, size, squares.size() + 1);
            squares.emplace_back(min_x, min_y, size);
            remaining_area-=(size*size);
            backtrack(grid, squares, N, best_solution, remaining_area);
            squares.pop_back();
            remove_square(grid, min_x, min_y, size);
            remaining_area+=(size*size);
        }
    }
}


vector<tuple<int, int, int>> squaring_the_square(int N) {
    vector<vector<int>> grid(N, vector<int>(N, 0));
    vector<tuple<int, int, int>> best_solution;
    if(N%2==0){
        best_solution.emplace_back(0, 0, N/2);
        best_solution.emplace_back(N/2, 0, N/2);
        best_solution.emplace_back(N/2, N/2, N/2);
        best_solution.emplace_back(0, N/2, N/2);
        return best_solution;
    }
    if(N%3==0){
        best_solution.emplace_back(0, 0, N*2/3);
        best_solution.emplace_back(0, N*2/3, N/3);
        best_solution.emplace_back(N/3, N*2/3, N/3);
        best_solution.emplace_back(N*2/3, 0, N/3);
        best_solution.emplace_back(N*2/3, N/3, N/3);
        best_solution.emplace_back(N*2/3, N*2/3, N/3);
        return best_solution;
    }
    if(N%5==0){
        best_solution.emplace_back(0, 0, N*3/5);
        best_solution.emplace_back(N*3/5, N*2/5, N*2/5);
        best_solution.emplace_back(N*3/5, 0, N*2/5);
        best_solution.emplace_back(0, N*3/5, N*2/5);
        best_solution.emplace_back(N*2/5, N*3/5, N/5);
        best_solution.emplace_back(N*2/5, N*4/5, N/5);
        best_solution.emplace_back(N*3/5, N*4/5, N/5);
        best_solution.emplace_back(N*4/5, N*4/5, N/5);
        return best_solution;
    }
    
    vector<tuple<int,int,int>> squares;
    int maxW = (N+1)/2;
    int bigW = N-maxW;
    place_square(grid, 0, 0, maxW, maxW);
    tuple<int,int,int> biggest_sqr = {0,0, maxW};
    squares.emplace_back(biggest_sqr);

    biggest_sqr = {0, maxW, bigW};
    squares.emplace_back(biggest_sqr);
    place_square(grid, 0, maxW, bigW, bigW);

    biggest_sqr = {maxW, 0, bigW};
    squares.emplace_back(biggest_sqr);
    place_square(grid, maxW, 0, bigW, bigW);


    backtrack(grid, squares, N, best_solution, N*N);
    return best_solution;
}



void print_solution_matrix(int N, const vector<tuple<int, int, int>>& solution) {
    vector<vector<int>> matrix(N, vector<int>(N, 0));
    int index = 1;
    for (const auto& [x, y, size] : solution) {
        for (int i = x; i < x + size; i++)
            for (int j = y; j < y + size; j++)
                matrix[i][j] = index;
        index++;
    }
    cout << "Итоговая матрица разбиения:\n";
    for (const auto& row : matrix) {
        for (int cell : row) cout << (cell > 0 ? to_string(cell) : ".") << " ";
        cout << endl;
    }
}



int main() {
    int N; 
    
    std::cin>>N;

    vector<tuple<int, int, int>> solution = squaring_the_square(N);
    std::cout<<solution.size()<<endl;
    for (const auto& [x, y, size] : solution)
        cout << x+1 << " " << y+1 << " " << size <<endl;
    return 0;
}

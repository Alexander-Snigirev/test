

#include <bits/stdc++.h> 
 
using namespace std; 
using ll = int64_t; 
 
struct Square { 
 int x, y, size; 
}; 
 
int n; 
int mn = INT_MAX; 
vector<Square> best_solution; 
 
pair<int, int> find(vector<vector<bool>>& a, int next_i) { 
 for (int i=next_i; i<n; ++i) { 
  for (int j=0; j<n; ++j) { 
   if (!a[i][j]) { 
    next_i = i; 
    return {i, j}; 
   } 
  } 
 } 
 return {-1, -1}; 
} 
 
int maxW(pair<int, int>& top_left, vector<vector<bool>>& a) { 
 int w = 0; 
 for (int j=top_left.second; j<n; ++j) { 
  if (a[top_left.first][j]) return min(min(w, (n+1)/2), n-top_left.first); 
  ++w; 
 } 
 return min(min(w, (n+1)/2), n-top_left.first); 
} 
 
void place_square(vector<vector<bool>>& a, Square& p, bool val) { 
 for (int i=p.x; i<p.x+p.size; ++i) { 
  for (int j=p.y; j<p.y+p.size; ++j) { 
   a[i][j] = val; 
  } 
 } 
} 
 
void backtracking(vector<vector<bool>>& matrix, vector<Square>& d, int next_i) { 
  
 if (d.size() >= mn) { 
  return; 
 } 
  
 auto top_left = find(matrix, next_i); 
  
 if (top_left.first == -1) { 
  best_solution = d; 
  mn = best_solution.size(); 
  return; 
 } 
  
 int start = maxW(top_left, matrix); 
 int end = 1; 
 for (int w=start; w>=end; --w) { 
  Square p = {top_left.first, top_left.second, w}; 
  place_square(matrix, p, 1); d.push_back(p); 
  backtracking(matrix, d, top_left.first); 
  place_square(matrix, p, 0); d.pop_back(); 
 } 
} 
 
int main() { 
 ios_base::sync_with_stdio(0); 
 cin.tie(nullptr); 
  
 int N; cin >> N; 
  
 n = N; 
 for (int i=2; i<=N; ++i) { 
  if (n % i == 0) { 
   n = i; 
   break; 
  } 
 } 
  
  
 int max_width = (n+1)/2; 
 vector<vector<bool>> matrix(n, vector<bool> (n, 0)); 
 vector<Square> d; 
 Square big_square = {0, 0, max_width}; 
 Square right_square = {0, max_width, n-max_width}; 
 Square low_square = {max_width, 0, n-max_width}; 
 place_square(matrix, big_square, 1); d.push_back(big_square); 
 place_square(matrix, right_square, 1); d.push_back(right_square); 
 place_square(matrix, low_square, 1); d.push_back(low_square); 
  
 backtracking(matrix, d, n-max_width); 
  
 cout << best_solution.size() << "\n"; 
 for (auto& s : best_solution) { 
  cout << s.x * N/n + 1 << " " << s.y * N/n + 1 << " " << s.size * N/n << "\n"; 
 } 
}

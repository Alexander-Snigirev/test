#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <iomanip>

using namespace std;


struct Operation {
    string type; 
    int pos;     
    char a, b;   
};


int editDistance(const string& A, const string& B, int cr, int ci, int cd,
                 char special_replace_char, int special_replace_cost,
                 char special_delete_char, int special_delete_cost) {
    int n = A.length();
    int m = B.length();

    
    cout << "=== Начало работы алгоритма ===" << endl;
    cout << "Строка A: " << A << " (длина = " << n << ")" << endl;
    cout << "Строка B: " << B << " (длина = " << m << ")" << endl;
    cout << "Стоимости: замена = " << cr << ", вставка = " << ci << ", удаление = " << cd << endl;
    cout << "Особый заменитель: '" << special_replace_char << "' (стоимость = " << special_replace_cost << ")" << endl;
    cout << "Особый удаляемый символ: '" << special_delete_char << "' (стоимость = " << special_delete_cost << ")" << endl;

    
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, LLONG_MAX));
    
    vector<vector<pair<int, int>>> prev(n + 1, vector<pair<int, int>>(m + 1, {-1, -1}));
    
    vector<vector<string>> op(n + 1, vector<string>(m + 1, ""));

    
    dp[0][0] = 0;
    cout << "\nБазовые случаи:" << endl;
    cout << "dp[0][0] = 0 (пустые строки)" << endl;
    for (int i = 1; i <= n; ++i) {
        long long delete_cost = (A[i-1] == special_delete_char) ? special_delete_cost : cd;
        string marker = (A[i-1] == special_delete_char) ? "special_delete" : "delete";
        dp[i][0] = dp[i-1][0] + delete_cost;
        prev[i][0] = {i - 1, 0};
        op[i][0] = marker;
        cout << "dp[" << i << "][0] = " << dp[i][0] << " (" << marker << " '" << A[i-1] << "', стоимость = " << delete_cost << ")" << endl;
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = dp[0][j-1] + ci;
        prev[0][j] = {0, j - 1};
        op[0][j] = "insert";
        cout << "dp[0][" << j << "] = " << dp[0][j] << " (insert '" << B[j-1] << "', стоимость = " << ci << ")" << endl;
    }

    
    cout << "\nЗаполнение"<<endl;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cout << "\nРассмотрение dp[" << i << "][" << j << "] (A[" << i-1 << "] = '" << A[i-1] << "', B[" << j-1 << "] = '" << B[j-1] << "')" << endl;
            long long min_cost = dp[i][j];
            
            if (A[i-1] == B[j-1]) {
                if (dp[i-1][j-1] < dp[i][j]) {
                    dp[i][j] = dp[i-1][j-1];
                    prev[i][j] = {i-1, j-1};
                    op[i][j] = "match";
                    cout << "  Совпадение: dp[" << i << "][" << j << "] = " << dp[i][j] << " (match)" << endl;
                }
            } else {
                
                long long replace_cost = (B[j-1] == special_replace_char) ? special_replace_cost : cr;
                string marker = (B[j-1] == special_replace_char) ? "special_replace" : "replace";
                if (dp[i-1][j-1] + replace_cost < dp[i][j]) {
                    dp[i][j] = dp[i-1][j-1] + replace_cost;
                    prev[i][j] = {i-1, j-1};
                    op[i][j] = marker;
                    cout << "  Замена: dp[" << i << "][" << j << "] = " << dp[i][j] << " (" << marker << ", стоимость = " << replace_cost << ")" << endl;
                }

                
                long long delete_cost = (A[i-1] == special_delete_char) ? special_delete_cost : cd;
                long long delete_insert_cost = delete_cost + ci;
                if (dp[i-1][j-1] + delete_insert_cost < dp[i][j]) {
                    dp[i][j] = dp[i-1][j-1] + delete_insert_cost;
                    prev[i][j] = {i-1, j-1};
                    op[i][j] = "delete_insert";
                    cout << "  Удаление+вставка: dp[" << i << "][" << j << "] = " << dp[i][j] << " (delete_insert, стоимость = " << delete_insert_cost << ")" << endl;
                }
            }
            
            if (dp[i][j-1] + ci < dp[i][j]) {
                dp[i][j] = dp[i][j-1] + ci;
                prev[i][j] = {i, j-1};
                op[i][j] = "insert";
                cout << "  Вставка: dp[" << i << "][" << j << "] = " << dp[i][j] << " (insert '" << B[j-1] << "', стоимость = " << ci << ")" << endl;
            }
            
            long long delete_cost = (A[i-1] == special_delete_char) ? special_delete_cost : cd;
            string marker = (A[i-1] == special_delete_char) ? "special_delete" : "delete";
            if (dp[i-1][j] + delete_cost < dp[i][j]) {
                dp[i][j] = dp[i-1][j] + delete_cost;
                prev[i][j] = {i-1, j};
                op[i][j] = marker;
                cout << "  Удаление: dp[" << i << "][" << j << "] = " << dp[i][j] << " (" << marker << " '" << A[i-1] << "', стоимость = " << delete_cost << ")" << endl;
            }
        }
    }

    
    cout << "\nТаблица DP:" << endl;
    cout << setw(8) << " ";
    for (int j = 0; j <= m; ++j) {
        cout << setw(8) << (j == 0 ? " " : string(1, B[j-1]));
    }
    cout << endl;
    for (int i = 0; i <= n; ++i) {
        cout << setw(8) << (i == 0 ? " " : string(1, A[i-1]));
        for (int j = 0; j <= m; ++j) {
            if (dp[i][j] == LLONG_MAX) {
                cout << setw(8) << "INF";
            } else {
                cout << setw(8) << dp[i][j];
            }
        }
        cout << endl;
    }

    
    long long minCost = dp[n][m];
    cout << "\nМинимальная стоимость: " << minCost << endl;

    
    cout << "\nВосстановление операций:" << endl;
    vector<Operation> operations;
    int i = n, j = m;
    int step = 0;
    while (i > 0 || j > 0) {
        int pi = prev[i][j].first;
        int pj = prev[i][j].second;
        string operation = op[i][j];
        cout << "Шаг " << step++ << ": (" << i << "," << j << ") -> (" << pi << "," << pj << "), операция = " << operation;
        
        if (operation == "match") {
            cout << " (символы '" << A[i-1] << "' совпадают)" << endl;
            operations.push_back({"match", i-1, 'n', 'n'});
        } else if (operation == "replace") {
            cout << " (замена '" << A[i-1] << "' на '" << B[j-1] << "', стоимость = " << cr << ")" << endl;
            operations.push_back({"replace", i-1, A[i-1], B[j-1]});
        } else if (operation == "special_replace") {
            cout << " (специальная замена '" << A[i-1] << "' на '" << B[j-1] << "', стоимость = " << special_replace_cost << ")" << endl;
            operations.push_back({"special_replace", i-1, A[i-1], B[j-1]});
        } else if (operation == "insert") {
            cout << " (вставка '" << B[j-1] << "', стоимость = " << ci << ")" << endl;
            operations.push_back({"insert", j-1, B[j-1], ' '});
        } else if (operation == "delete") {
            cout << " (удаление '" << A[i-1] << "', стоимость = " << cd << ")" << endl;
            operations.push_back({"delete", i-1, A[i-1], ' '});
        } else if (operation == "special_delete") {
            cout << " (специальное удаление '" << A[i-1] << "', стоимость = " << special_delete_cost << ")" << endl;
            operations.push_back({"special_delete", i-1, A[i-1], ' '});
        } else if (operation == "delete_insert") {
            cout << " (удаление '" << A[i-1] << "' + вставка '" << B[j-1] << "', стоимость = " 
                 << ((A[i-1] == special_delete_char) ? special_delete_cost : cd) << " + " << ci << ")" << endl;
            operations.push_back({"insert", j-1, B[j-1], ' '});
            operations.push_back({"delete", i-1, A[i-1], ' '});
        }

        i = pi;
        j = pj;
    }

    
    cout << "\nПоследовательность операций (в обратном порядке):" << endl;
    for (int k = operations.size() - 1; k >= 0; --k) {
        auto& op = operations[k];
        if (op.type == "replace") {
            cout << "R";
        } else if (op.type == "special_replace") {
            cout << "r";
        } else if (op.type == "insert") {
            cout << "I";
        } else if (op.type == "delete") {
            cout << "D";
        } else if (op.type == "special_delete") {
            cout << "d";
        } else if (op.type == "match") {
            cout << "M";
        }
    }
    cout << endl;
    
    return minCost;
}

int main() {
    string A, B;
    int cr, ci, cd, other_cr, other_cd;
    char special_ch_r, special_ch_d;
    cout<<"Цены стандартных операций: <замена> <вставка> <удаление>"<<endl;
    cin>>cr>>ci>>cd;
    cout<<"Особая замена: <цена> <cимвол>"<<endl;
    cin>>other_cr>>special_ch_r;
    cout<<"Особое удаление: <цена> <cимвол>"<<endl;
    cin>>other_cd>>special_ch_d;
    cout<<"Строка 1:"<<endl;
    cin >> A;
    cout<<"Строка 2:"<<endl;
    cin >> B;
    int minCost = editDistance(A, B, cr, ci, cd, special_ch_r, other_cr, special_ch_d, other_cd);
    cout<<minCost<<endl;

    return 0;
}
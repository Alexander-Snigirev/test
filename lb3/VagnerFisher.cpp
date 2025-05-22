#include <iostream>
#include <memory> 
#include <vector>
using namespace std;

int min_t(int fir, int sec, int thr){
    int cur = min(fir,sec);
    return min(cur, thr);
}


int levensteinInstruction(string s1, string s2, int InsertCost, int DeleteCost, int ReplaceCost){
    int N = s2.size();
    int M = s1.size();
    if (M == 0) return N * InsertCost;
    if (N == 0) return M * DeleteCost;
    vector<vector<int>> D(M+1, vector<int>(N+1, 0));
    D[0][0] = 0;
    for(int j = 1; j < N+1 ; j++)
        D[0][j] = D[0][j - 1] + InsertCost;                  
    for(int i = 1; i < M+1; i++){
        D[i][0] = D[i - 1][0] + DeleteCost;                  
        for(int j = 1; j < N+1 ; j++){
            if(s1[i-1] != s2[j-1])
                D[i][j] = min_t(D[i - 1][j] + DeleteCost,        
                            D[i][j - 1] + InsertCost,                      
                            D[i - 1][j - 1] + ReplaceCost);            
            else 
                D[i][j] = D[i - 1][j - 1];
        }
    }
    return D[M][N];
}

int main(){
    string s1,s2;
    int RepCost, InsCost, DelCost;
    cin>>RepCost>>InsCost>>DelCost;
    cin>>s1;
    cin>>s2;
    cout<<levensteinInstruction(s1,s2,InsCost,DelCost,RepCost)<<endl;
}
#include <iostream>
#include <vector>
#define DEBUG 1
using namespace std;

void print(vector<int> vec){
    cout<<"["<<vec[0];
    for(int i=1;i<vec.size();i++){
        cout<<", "<<vec[i];
    }
    cout<<"]"<<endl;
}
vector<int> prefixFunction(string str){
    vector<int> result;
    result.push_back(0);
    if(DEBUG) cout<<"     Первую позицию инициализируем 0 по определению"<<endl;
    if(DEBUG){ cout<<"     Вектор: ";
    print(result);}
    for(int i = 1; i<str.size(); i++){
        int k = result[i - 1];
        while(k > 0 && str[i] != str[k]){
            k = result[k - 1];
        }
        if(str[i] == str[k])
            k++;
        result.push_back(k);
        if(DEBUG){
            cout<<"     Текущая подстрока: "<<str.substr(0,i)<<endl;
            cout<<"     Префикс-функция: ";
            print(result);
        }
    }    
    return result;
}

vector<int> kmp(string P, string T)
{
    if(DEBUG) cout<<"Запуск алгоритма Кнутта-Мориса-Пратта..."<<endl;
    
    int pl = P.size();
    int tl = T.size();
    if(DEBUG) cout<<"Длина паттерна: "<<pl<<endl<<"Длина строки: "<<tl<<endl;
    vector<int> answer;
    if(DEBUG) cout<<"Вычисление префиксной функции строки "<<P+"#"+T<<endl;
    vector<int> p = prefixFunction(P + "#" + T);
    int count = 0;
    for (int i = 0;i<tl;i++){
        if(p[pl + i + 1] == pl){
            answer.push_back(i - pl + 1);
            if(DEBUG)
            {
                cout<<"Найден элемент с префикс-функцией == "<<pl<<endl;
                cout<<"Добавляем в ответ индекс нового вхождения == "<<pl<<endl;
                cout<<"Найденные индексы вхождений: "<<endl;
                print(answer);
            }
        }

    }
    if(answer.empty()){
        if(DEBUG) cout<<"Ни одного вхождения не найдено, результат инициализируем -1"<<endl;
        answer.push_back(-1);
    }
    return answer;
}

int main(){
    string str1;
    string str2;
    cout<<"Введите строку: ";
    cin>>str1;
    cout<<"Введите паттерн: ";
    cin>>str2;
    cout<<"Все вхождения: "<<endl;
    print(kmp(str2, str1));
}
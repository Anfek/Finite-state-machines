#include <fstream>
#include <iostream>
#include <vector>

#include "Machine_Table.h"

#include "Linear_machine.h"
#include "Shift_register.h"

using namespace std;

void readFile_LM(string filename, int &q, int &m, int &n, int &k, 
                vector<vector<int>> &A, vector<vector<int>> &B, 
                vector<vector<int>> &C, vector<vector<int>> &D) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    // Считываем q
    infile >> q;

    // Считываем m, n, k
    infile >> m >> n >> k;

    // Считываем матрицы
    for (int i = 0; i < n; i++) {
        A.push_back(vector<int>(n));
        for (int j = 0; j < n; j++) {
        infile >> A[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        B.push_back(vector<int>(n));
        for (int j = 0; j < n; j++) {
        infile >> B[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        C.push_back(vector<int>(k));
        for (int j = 0; j < k; j++) {
        infile >> C[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        D.push_back(vector<int>(k));
        for (int j = 0; j < k; j++) {
        infile >> D[i][j];
        }
    }

    // Закрытие файла
    infile.close();
}

void print_func(vector<vector<int>> A){
    for(int i=0; i<A.size(); i++){
        for(int j=0; j<A[i].size(); j++){
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//проверяем что число простое или степень простого 
bool is_prime_or_power_of_prime(int num) {
    if (num <= 1  || num > 0x10000) {
        return false;
    }

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            while (num % i == 0) {
                num /= i;
            }
            if (num > 1) {
                return false;
            }
        }
    }

    return true;
}

Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> *LM_table(string filename){
    //LM *lin = new LM(2, 3, 4, 3);
    int q, m, n, k;

    vector<vector<int>> A, B, C, D;
    readFile_LM(filename, q, m, n, k, A, B, C, D);
    if(!is_prime_or_power_of_prime(q)){
        cout << "q не является простым или степенью простого или больше 2^16" <<endl;
        return 0;
    }
    /*print_func(A);
    print_func(B);
    print_func(C);
    print_func(D);*/

    LM *lin = new LM(q, m, n, k, A, B, C, D);
    lin->print_test();


    Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> *TT;// = new Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>>(q, m, n, k);
    TT = lin->get_table();
    delete lin;
    return TT;
}

void readFile_RC(string filename, int &n, vector<int> &phi, vector<int> &psi){
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    // Считываем n
    infile >> n;
    int size = 2 * pow(2, n);
    phi.resize(size);
    psi.resize(size);

    for(int i=0; i<size; i++){
        infile >> phi[i];
    }
    for(int i=0; i<size; i++){
        infile >> psi[i];
    }
    infile.close();
}

Table<int, std::vector<uint32_t>, int> *PC_table(string filename){
    int n;
    vector<int> phi;
    vector<int> psi;
    readFile_RC(filename, n, phi, psi);

    PC *reg = new PC(n, phi, psi);
    reg->print_test();

    Table<int, std::vector<uint32_t>, int> *TT;// = new Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>>(q, m, n, k);
    TT = reg->get_table();
    delete reg;
    return TT;
}


int main(){
    system("chcp 65001 >nul 2>&1");
    
    //Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> *TT = LM_table("LM.txt");
    //Table<int, std::vector<uint32_t>, int> *TT = PC_table("PC.txt");
    
    //Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> *TT = LM_table("KKCO-03-19\\ForStudent\\11\\Lin\\11_q_7");
    Table<int, std::vector<uint32_t>, int> *TT = PC_table("KKCO-03-19\\ForStudent\\11\\Reg\\11_n_3");
    //TT->print_table();

    //TT->Lab1();
    //TT->Lab2();
    //TT->Lab3();
    TT->Lab4();

    return 0;
}
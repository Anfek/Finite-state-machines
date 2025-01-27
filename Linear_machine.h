//#include "Machine_Table.h"

class Polynomeal{
private:

    std::vector<uint32_t> poly;
    uint32_t p_prime_number, k_degree, p_k_module;
    std::vector<uint32_t> f_x_irreducible_polynomial;
    


    // раскладываем число в многочлен в n-ичной системе счисления
    std::vector<uint32_t> to_n_ary(int num, int n) {
        std::vector<uint32_t> digits;

        while (num > 0) {
            digits.push_back(num % n);
            num /= n;
        }

        reverse(digits.begin(), digits.end());
        return digits;
    }

    // Функция перевода из n-ичной системы счисления в десятичную
    uint32_t from_n_ary(std::vector<uint32_t>& digits, int n) {
        int num = 0;
        int pow = 1;

        for (int i = digits.size() - 1; i >= 0; i--) {
            num += digits[i] * pow;
            pow *= n;
        }

        return num;
    }

    void init_module(uint32_t num){
        k_degree=0;
        for (uint32_t i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                p_prime_number = i;
                while (num % i == 0) {
                    k_degree++;
                    num /= i;
                }
                break;
            }
        }
    }

    std::vector<uint32_t> polynomial_sum(std::vector<uint32_t>& p1, std::vector<uint32_t>& p2) {
        int n = std::max(p1.size(), p2.size());
        std::vector<uint32_t> sum(n, 0);

        if (p1.size() < n) {
            std::fill(sum.begin() + p1.size(), sum.end(), 0);
        } else if (p2.size() < n) {
            std::fill(sum.begin() + p2.size(), sum.end(), 0);
        }

        for (int i = 0; i < n; i++) {
            sum[i] = (p1[i] + p2[i])%p_prime_number;
        }

        return sum;
    }

    std::vector<uint32_t> polynomial_multiplication(std::vector<uint32_t>& a, std::vector<uint32_t>& b) {
        int n = a.size() + b.size() - 1;
        std::vector<uint32_t> c(n, 0);

        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < b.size(); j++) {
                c[i + j] += a[i] * b[j];
                c[i + j] %= p_prime_number;
            }
        }

        return c;
    }

    uint32_t modular_inverse(uint32_t a, uint32_t m) {
        uint32_t x = 0, y = 1, u = 1, v = 0, gcd = a, t;

        while (a) {
            t = a / m;
            a -= t * m;
            std::swap(x, u);
            std::swap(y, v);
            a ^= m;
            m ^= a;
            a ^= m;
        }

        return (u % m + m) % m;
    }

    std::vector<uint32_t> polynomial_division(std::vector<uint32_t>& dividend, std::vector<uint32_t>& divisor){
        int n = dividend.size() - divisor.size() + 1;
        std::vector<uint32_t> quotient(n, 0);
        std::vector<uint32_t> remainder;

        for (int i = n - 1; i >= 0; i--) {
            uint32_t q = (dividend[i + divisor.size() - 1] * modular_inverse(divisor[divisor.size() - 1], p_prime_number)) % p_prime_number;
            quotient[i] = q;

            for (int j = 0; j < divisor.size(); j++) {
                dividend[i + j] -= q * divisor[j];
                while(dividend[i + j] < 0){
                    dividend[i + j] += p_prime_number;
                }
            }
        }

        remainder = dividend;
        remainder.resize(divisor.size() - 1);

        return remainder;
    }


public:
    Polynomeal (uint32_t number, uint32_t module){
        p_k_module = module;
        init_module(module); 

        poly = to_n_ary(number, p_prime_number);
    }

    uint32_t operator+(uint32_t other_number){
        std::vector<uint32_t> other_poly;
        other_poly = to_n_ary(other_number, p_prime_number);
        other_poly = polynomial_sum(poly, other_poly);
        return from_n_ary(other_poly, p_prime_number);
    }

    uint32_t operator*(uint32_t other_number){
        std::vector<uint32_t> other_poly;
        other_poly = to_n_ary(other_number, p_prime_number);

        other_poly = polynomial_multiplication(poly, other_poly);
        other_poly = polynomial_division (other_poly, f_x_irreducible_polynomial);

        return from_n_ary(other_poly, p_prime_number);
    }
};


class LM{
private:
    Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>> *TT;
    uint32_t q, m, n, k; 
    uint32_t **A, **B, **C, **D; 
    /*
    q - размерность поля элементов Fq
    m = |X|     n = |S|     k = |Y|
    A(n*n)  B(m*n)  C(n*k)  D(m*k)
    */

    void malloc_func(uint32_t*** array, int a, int b, std::vector<std::vector<int>> vector){
        if(vector.size() == a && vector[0].size() == b){
            *array = (uint32_t**)malloc(sizeof(uint32_t*) * a);
            for(int i=0; i<a; i++){
                (*array)[i] = (uint32_t*)malloc(sizeof(uint32_t) * b);
                for(int j=0; j<b; j++){
                    (*array)[i][j] = vector[i][j];
                }
            }
        }
        else {
            printf("Некорректный размер массива. Ожидалось %d*%d, вместо %d*%d\n", a, b, vector.size(), vector[0].size());
        }
    }

    std::vector<uint32_t> func_H(std::vector<uint32_t> s, std::vector<uint32_t> x){
        std::vector<uint32_t> result;
        for(int i=0; i<n; i++){
            uint32_t buf1=0, buf2=0;
            for (int j=0; j<n; j++){
                buf1 += s[j]*A[j][i];
            }
            for (int j=0; j<m; j++){
                buf2 += x[j]*B[j][i];
            }
            result.push_back((buf1+buf2)%q);
        }
        return result;
    }

    std::vector<uint32_t> func_F(std::vector<uint32_t> s, std::vector<uint32_t> x){
        std::vector<uint32_t> result;
        for(int i=0; i<k; i++){
            uint32_t buf1=0, buf2=0;
            for (int j=0; j<n; j++){
                buf1 += s[j]*C[j][i];
            }
            for (int j=0; j<m; j++){
                buf2 += x[j]*D[j][i];
            }
            result.push_back((buf1+buf2)%q);
        }
        return result;
    }


    void generate_combinations(int size_vector, std::vector<uint32_t>& combination, std::vector<std::vector<uint32_t>>& result) {
        // Базовый случай:
        if (size_vector == 0) {
            result.push_back(combination);
            return;
        }

        // Перебор всех возможных значений для текущего элемента:
        for (int i = 0; i < q; ++i) {
            combination.push_back(i);
            generate_combinations(size_vector - 1, combination, result);
            combination.pop_back();
        }
    }

    std::vector<std::vector<uint32_t>> generate_all_combinations(int size_vector) {
        std::vector<uint32_t> combination;
        std::vector<std::vector<uint32_t>> result;
        generate_combinations(size_vector, combination, result);
        return result;
    }


    void print_array(uint32_t **array, int a, int b){
        for(int i=0; i<a; i++){
            for(int j=0; j<b; j++){
                std::cout << array[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Вспомогательная функция для расширенного алгоритма Евклида
    int64_t gcd_extended(int64_t a, int64_t b, int64_t &x, int64_t &y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        int64_t x1, y1;
        int64_t gcd = gcd_extended(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return gcd;
    }

    uint32_t modular_inverse(uint32_t a) {
        int64_t x, y;
        int64_t g = gcd_extended(a, q, x, y);
        if (g != 1) {
            throw std::invalid_argument("Inverse does not exist");
        }
        // Убедимся, что результат положительный
        return (x % q + q) % q;
    }

    int matrix_rank_mod_q(std::vector<std::vector<uint32_t>> matrix) {
        // Проверка размеров матрицы
        if (matrix.empty() || matrix[0].empty()) {
            return 0;
        }

        int n_rows = matrix.size();  // Количество строк
        int n_cols = matrix[0].size();  // Количество столбцов

        // Ранг матрицы
        int rank = 0;

        // Алгоритм Гаусса-Жордана в кольце по модулю q
        for (int i = 0; i < std::min(n_rows, n_cols); ++i) {
            // Выбираем ведущий элемент
            int pivot_row = i;
            uint32_t pivot = 0;
            for (int j = i; j < n_rows; ++j) {
            if (matrix[j][i] != 0) {
                pivot_row = j;
                pivot = matrix[j][i];
                break;
            }
            }

            // Если ведущий элемент равен 0, то ранг матрицы не больше i
            if (pivot == 0) {
            break;
            }

            // Приводим ведущий элемент к 1
            if (pivot != 1) {
            for (int j = 0; j < n_cols; ++j) {
                matrix[pivot_row][j] = (matrix[pivot_row][j] * modular_inverse(pivot)) % q;
            }
            }

            // Вычитаем кратные ведущей строки из остальных строк
            for (int j = 0; j < n_rows; ++j) {
            if (j != pivot_row) {
                uint32_t multiplier = matrix[j][i];
                for (int k = 0; k < n_cols; ++k) {
                matrix[j][k] = (matrix[j][k] - (multiplier * matrix[pivot_row][k])) % q;
                }
            }
            }

            // Увеличиваем ранг
            ++rank;
        }

        return rank;
    }

    uint32_t int_pow(uint32_t base, uint32_t exp) {
        uint32_t result = 1;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result *= base;
            }
            base *= base;
            exp /= 2;
        }
        return result;
    }

    void lab2_LM(){
        std::vector<std::vector<std::vector<uint32_t>>> K(n);
        std::vector<std::vector<uint32_t>> deg_A;
        int current_K = 0;
        K[0].resize(n);
        for(int i=0; i<n; i++){
            K[0][i].resize(k);
            for(int j=0; j<k; j++){
                K[0][i][j] = C[i][j];
            }
        }
        int current_rank = matrix_rank_mod_q(K[current_K]);

        deg_A.resize(n);
        for(int i=0; i<n; i++){
            deg_A[i].resize(n);
            for(int j=0; j<n; j++){
                deg_A[i][j] = A[i][j];
            }
        }
        std::cout << "\rПредвычисляем некоторые значения, пожалуйста, подождите...\n" << std::endl;
        do{
            K[current_K+1].resize(n);
            for(int i=0; i<n; i++){
                int old_size = k * (current_K+1);
                int new_size = k * (current_K+2);
                K[current_K+1][i].resize(new_size);
                for(int j=0; j<old_size; j++){
                    K[current_K+1][i][j] = K[current_K][i][j];
                }
                for(int j=0; j<k; j++){
                    K[current_K+1][i][j+old_size] = 0;
                    for(int a=0; a<n; a++){
                        K[current_K+1][i][j+old_size] += deg_A[i][j]*C[j][i];
                        K[current_K+1][i][j+old_size] %= q;
                    }
                }
            }
            int new_rank = matrix_rank_mod_q(K[current_K+1]);
            if(new_rank == current_rank){
                break;
            }
            current_rank = new_rank;
            current_K++;

            if(current_K < n){
                std::vector<std::vector<uint32_t>> new_deg_A(n);
                for(int i=0; i<n; i++){
                    new_deg_A[i].resize(n);
                    for(int j=0; j<n; j++){
                        new_deg_A[i][j] = 0;
                        for(int a=0; a<n; a++){
                            new_deg_A[i][j] += deg_A[i][j] * A[j][i];
                            new_deg_A[i][j] %= q;
                        }
                    }
                }
                deg_A = new_deg_A;
            }

        }while(current_K < n);

        int mu = int_pow(q, current_rank);
        TT->push_mu_delta(mu, current_K+1);
    }

    void create_table(){
        auto all_s_state = generate_all_combinations(n);
        auto all_x_input = generate_all_combinations(m);
        /*for(uint32_t i = 0; i < all_s_state.size(); i++){
            for(uint32_t j = 0; j < all_s_state[0].size(); j++){
                std::cout << all_s_state[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;
        for(uint32_t i = 0; i < all_x_input.size(); i++){
            for(uint32_t j = 0; j < all_x_input[0].size(); j++){
                std::cout << all_x_input[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;*/

        printf("Заполняем таблицу переходов-выходов...");
        for(auto s : all_s_state){
            for(auto x : all_x_input){
                auto h = func_H(s, x);
                auto f = func_F(s, x);
                /*
                for(uint32_t j = 0; j < s.size(); j++){
                    std::cout << s[j] << " ";
                }
                std::cout << "(";
                for(uint32_t j = 0; j < x.size(); j++){
                    std::cout << x[j] << " ";
                }
                std::cout << ",\t";
                for(uint32_t j = 0; j < h.size(); j++){
                    std::cout << h[j] << " ";
                }
                std::cout << ",\t";
                for(uint32_t j = 0; j < f.size(); j++){
                    std::cout << f[j] << " ";
                }
                std::cout << ") ";
                */
                /*auto x_i = x;
                std::cout << "(";
                for(uint32_t j = 0; j < x_i.size(); j++){
                    std::cout << x_i[j] << " ";
                }
                std::cout << ") ";*/
                TT->table[{s, x}] = {h, f};
                //TT->print(buf1.x_input);
                if(s == *all_s_state.begin()){
                    TT->push_x(x);
                }
            }
            TT->push_s(s);
        }
        printf("\rТаблица готова!                                     ");
        lab2_LM();
    }

public:

    LM (int q, int m, int n, int k, 
        std::vector<std::vector<int>> vector_A, std::vector<std::vector<int>> vector_B, 
        std::vector<std::vector<int>> vector_C, std::vector<std::vector<int>> vector_D){
        
        this->q = q;
        this->m = m;
        this->n = n;
        this->k = k;

        malloc_func(&A, this->n, this->n, vector_A);
        malloc_func(&B, this->m, this->n, vector_B);
        malloc_func(&C, this->n, this->k, vector_C);
        malloc_func(&D, this->m, this->k, vector_D);

        TT = new Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>>(q, m, n, k);
        create_table();
    }

    Table<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<uint32_t>>* get_table(){
        return TT;
    }

    void print_test(){
        printf("q = %d, m = %d, n = %d, k = %d\n", q, m, n, k);
        print_array(A, n, n);
        print_array(B, m, n);
        print_array(C, n, k);
        print_array(D, m, k);
    }


protected:








};
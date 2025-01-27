#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cstdint>

template <typename type_x, typename type_s, typename type_y>
class Table{
private:
    /*
    q - размерность поля элементов Fq
    m = |x|     n = |s|     k = |y|
    q^m = |X|   q^n = |S|   q^k = |Y|
    */
    uint32_t const_q, const_m, const_n, const_k;
    int const_mu = 0, const_delta = 0;

    std::vector<type_s> all_s;
    std::vector<type_x> all_x;

    typedef struct{
        type_s s_state;
        type_x x_input;
    } pair_SX;

    typedef struct{
        type_s h_func_transit;
        type_y f_func_output;
    } pair_HF;

public:
    //std::map<std::pair<type_s, type_x>, std::pair<type_s, type_y>> table;
    std::map<pair_SX, pair_HF> table; //table{{s, x}, {h, f}};
    
private:
    void print(int value){
        std::cout << value << " ";
    }

    void print(uint32_t value){
        std::cout << value << " ";
    }

    void print(std::vector<uint32_t> value) {
        std::cout << "(";
        for (size_t i = 0; i < value.size(); ++i) {
            std::cout << value[i];
            if (i != value.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ") ";
    }

public:

    //конструктор для инициализации
    Table(uint32_t val_q, uint32_t val_m, uint32_t val_n, uint32_t val_k){
        const_q = val_q;
        const_m = val_m;
        const_n = val_n;
        const_k = val_k;
    }

    //конструктор по умолчанию
    Table() : const_q(0), const_m(0), const_n(0), const_k(0){}

    void push_s(type_s s){
        all_s.push_back(s);
    }

    void push_x(type_x x){
        all_x.push_back(x);
    }

    void push_mu_delta(int mu, int delta){
        const_mu = mu;
        const_delta = delta;
    }

    //ХУИТА НА КОТОРУЮ Я ПОТРАТИЛА 2 ЧАСА БЛЯТЬ ПОТОМУ ЧТО 
    //ЕБУЧЕМУ STD::MAP НАХУЙ НУЖНО ЧТОБЫ МЫ МОГЛИ СРАВНИВАТЬ 
    //ЕГО ЕБУЧИЕ ЭЛЕМЕНТЫ, ДАЖЕ ЕСЛИ НАМ НАХЕР НЕ ВСРАЛОСЬ
    //ЭТО ЕГО БЛЯДСКОЕ СРАВНЕНИЕ ПИДАРАС ЕБАНЫЙ ПОШЁЛ ОН НАХУЙ
    //УДАЛИТЕ И ПОМОТРИТЕ КАКУЮ ХУИТУ - ПАПИРУС БЛЯТЬ ДЛИННОЮ В ЖИЗНЬ
    //ВЫСЕРАЕТ КОМПИЛЯТОР ЕСЛИ УДАЛИТЬ ЭТУ БЕСПОЛЕЗНУЮ ЕБАНИНУ
    //СУКА КАК ЖЕ Я НАХУЙ ЗЛА В РОТ Я ЕБАЛА ЭТОТ ВАШ c++ ЕБУЧИЙ
    //ПОШЁЛ ОН В ЖОПУ, БЛЯТЬ.
    friend bool operator<(const pair_SX& lhs, const pair_SX& rhs) { 
        auto lhs_buf = std::make_pair(lhs.s_state, lhs.x_input);
        auto rhs_buf = std::make_pair(rhs.s_state, rhs.x_input);
        return lhs_buf < rhs_buf;
    }

    //печатанье таблички s\x (h,f) на экран
    void print_table(){
        std::cout << "\t";
        uint32_t counter = 0;
        for(auto& it : table){
            //print(it.first.s_state);
            if (counter % (uint32_t)std::pow(const_q, const_m) == 0 && counter!=0){
                break;
            }
            print(it.first.x_input);
            counter++;
        }
        std::cout << std::endl;
        counter = 0;
        for(auto& it : table){
            if (counter % (uint32_t)std::pow(const_q, const_m) == 0){
                print(it.first.s_state); 
            }
            counter++;
            std::cout <<"{";
            print(it.second.h_func_transit); 
            print(it.second.f_func_output); 
            std::cout <<"}";
            if (counter % (uint32_t)std::pow(const_q, const_m) == 0){
                std::cout << std::endl;
            }
        }
    }

private:   
    /*
    void initial_machine (type_s val_initial_state){
        initial_state = val_initial_state;
    }

    type_y output_machine(type_x input){
        return table[{initial_state, input}].f_func_output;
    }
    */
    int scan(int value, int size){
        std::cin >> value;
        return value;
    }

    uint32_t scan(uint32_t value, int size){
        std::cin >> value;
        return value;
    }

    std::vector<uint32_t> scan(std::vector<uint32_t> value, int size) {
        value.resize(size);
        for (int i = 0; i < size; ++i) {
            std::cin >> value[i];
        }
        return value;
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


public:
    void Lab1(){
        std::cout << "\n\n\t\tЛабораторная работа №1\n\n" << std::endl;
        type_s initial_state;
        type_x current_x;
        std::cout << "Задайте начальное состояние" << std::endl;
        initial_state = scan(initial_state, const_n);

        for (int i = 0; i < 5; i++){
            std::cout << "\nПодайте вход: ";
            current_x = scan(current_x, const_m);
            std::cout << "Ваш выход: ";
            print(table[{initial_state, current_x}].f_func_output);
            initial_state = table[{initial_state, current_x}].h_func_transit;
            std::cout << "\tНовое текущее состояние: ";
            print(initial_state);
            std::cout << std::endl;
        }        
    }

    void Lab2(){
        std::cout << "\n\n\t\tЛабораторная работа №2\n\n" << std::endl;

        if(const_mu !=0 && const_delta != 0){
            std::cout << "Степень различимости (б) = " << const_delta << std::endl;
            std::cout << "Приведённый вес (mu) = " << const_mu;
            if(int_pow(const_q, const_n) == const_mu){
                std::cout << "\t=>\tАвтомат  минимальный";
            }
            std::cout << std::endl;
            return;
        }


        std::vector<std::vector<std::vector<type_s>>> equivalent_states(2); // [~][{}][state]
        equivalent_states[0].push_back(all_s);
        printf("\r\tprogress: ~(%d) m = %d", 0, equivalent_states[0].size());
        for (int i = 0; i < equivalent_states[0][0].size(); i++){
            bool check_exist = false;
            for(int j=0; j < equivalent_states[1].size() && !check_exist; j++){
                for(int k=0; k < equivalent_states[1][j].size() && !check_exist; k++){
                    if(equivalent_states[1][j][k] == equivalent_states[0][0][i]){
                        check_exist = true;
                    }
                }
            }
            if (check_exist){
                continue;
            }
            
            std::vector<type_s> buf;
            buf.push_back(equivalent_states[0][0][i]);
            for (int j = i+1; j < equivalent_states[0][0].size(); j++){
                bool check_exit = true;
                for (int a = 0; a < all_x.size() && check_exit; a++){
                    if(table[{equivalent_states[0][0][i], all_x[a]}].f_func_output 
                    != table[{equivalent_states[0][0][j], all_x[a]}].f_func_output){
                        check_exit = false;
                    }
                }

                if(check_exit){
                    buf.push_back(equivalent_states[0][0][j]);
                }
            }
            equivalent_states[1].push_back(buf);
        }
        printf("\r\tprogress: ~(%d) m = %d", 1, equivalent_states[1].size());

        int current_equiv = 1;
        bool check_stop = false;

        do{
            std::vector<std::vector<type_s>> current_equivalent;
            for (int i = 0; i < equivalent_states[current_equiv].size(); i++){  //i-ый класс эквивалентности
                std::vector<std::vector<int>> number_class_transition(equivalent_states[current_equiv][i].size());  //номера классов эквивалентности выходов на всех входах (для всех элементов класса)
                for(int j=0; j<equivalent_states[current_equiv][i].size(); j++){  //j-ый элемент
                    number_class_transition[j].resize(all_x.size());
                    for(int k=0; k<all_x.size(); k++){  //проходимся по всем входам
                        for(int a=0; a < equivalent_states[current_equiv].size(); a++){    //a-ый класс эквивалентности
                            for(int b=0; b < equivalent_states[current_equiv][a].size(); b++){   //b-ый элемент
                                if(equivalent_states[current_equiv][a][b] == table[{equivalent_states[current_equiv][i][j], all_x[k]}].h_func_transit){
                                    number_class_transition[j][k] = a;
                                }
                            }
                        }
                    }
                }
                //получили массив массивов номеров классов эквивалентностей элементов из таблицы переходов
                //теперь разбиваем на группы состояний с одинаковыми массивами и пушим в current_equivalent


                for(int j=0; j<equivalent_states[current_equiv][i].size(); j++){
                    bool check_exist = false;
                    for(int k=0; k < current_equivalent.size() && !check_exist; k++){
                        for(int a=0; a < current_equivalent[k].size() && !check_exist; a++){
                            if(current_equivalent[k][a] == equivalent_states[current_equiv][i][j]){
                                check_exist = true;
                            }
                        }
                    }
                    if (check_exist){
                        continue;
                    }
                    
                    
                    std::vector<type_s> current_class;
                    current_class.push_back(equivalent_states[current_equiv][i][j]);                
                    for(int k=j+1; k<equivalent_states[current_equiv][i].size(); k++){
                        if(number_class_transition[j] == number_class_transition[k]){
                            current_class.push_back(equivalent_states[current_equiv][i][k]);
                        }
                    }
                    current_equivalent.push_back(current_class);
                }

            }




            if(equivalent_states[current_equiv] == current_equivalent){
                check_stop = true;
            }
            else{
                if(current_equivalent.size() == (std::pow(const_q, const_n))){
                    check_stop = true;
                }
                equivalent_states.push_back(current_equivalent);
                current_equiv++;
                printf("\r\tprogress: ~(%d) m = %d", current_equiv, equivalent_states[current_equiv].size());
            }
        }while(!check_stop);

        std::cout << "\rСтепень различимости (б) = " << current_equiv << "          " << std::endl;
        std::cout << "Приведённый вес (mu) = " << equivalent_states[current_equiv].size();
        if(int_pow(const_q, const_n) == equivalent_states[current_equiv].size()){
            std::cout << "\t=>\tАвтомат  минимальный";
        }
        std::cout << std::endl;

        const_mu = equivalent_states[current_equiv].size();
        const_delta = current_equiv;
        /*
        for(int i=0; i<=current_equiv; i++){
            printf("\n~(%d)\t", i);
            for(int j=0; j<equivalent_states[i].size(); j++){
                printf("{");
                for(int k=0; k<equivalent_states[i][j].size(); k++){
                    if(k!=0) printf(", ");
                    print(equivalent_states[i][j][k]);                    
                }
                printf("}");
            }
        }
        */
    }

private:

int get_number_s(type_s buf_s){
    for(int i=0; i<all_s.size(); i++){
        if(all_s[i] == buf_s){
            return i;
        }
    }
    return all_s.size()+404;
}

void DFS(std::vector<std::vector<std::pair<int, type_s>>> *G_Adj, std::vector<bool> *visited, int numb_current_s){
    (*visited)[numb_current_s] = true;
    for(int i=0; i<(*G_Adj)[numb_current_s].size(); i++){
        if((*visited)[(*G_Adj)[numb_current_s][i].first] == false){
            DFS(G_Adj, visited, (*G_Adj)[numb_current_s][i].first);
        }
    }
    /*for (const auto& edge : G_Adj[numb_current_s]) {
        if (!visited[edge.first]) {
            DFS(G_Adj, visited, edge.first);
        }
    }*/
}

std::pair<bool,int> simple_connectivity_check(){
    std::vector<std::vector<std::pair<int, type_s>>> *G_Adj = new std::vector<std::vector<std::pair<int, type_s>>>(all_s.size()); //список смежных для всех вершин в формате <№ в списке вершин, значение>
    for(int i=0; i<all_s.size(); i++){
        type_s current_s = all_s[i];
        for(int j=0; j<all_x.size(); j++){
            type_s buf_s = table[{current_s, all_x[j]}].h_func_transit;
            int numb_buf_s = get_number_s(buf_s);
            if(numb_buf_s == i){
                continue;
            }
            
            bool can_push = true;
            for(int k=0; k<(*G_Adj)[i].size() && can_push; k++){
                if((*G_Adj)[i][k].first == numb_buf_s){
                    can_push = false;
                }
            }
            if(can_push){
                (*G_Adj)[i].push_back(make_pair(numb_buf_s, buf_s));
            }

            can_push = true;
            for(int k=0; k<(*G_Adj)[numb_buf_s].size() && can_push; k++){
                if((*G_Adj)[numb_buf_s][k].first == i){
                    can_push = false;
                }
            }
            if(can_push){
                (*G_Adj)[numb_buf_s].push_back(make_pair(i, current_s));
            }

        }
    }
    std::vector<bool> *visited = new std::vector<bool>(all_s.size(), false);
    DFS(G_Adj, visited, 0);

    bool graph_connectivity_check = true;
    for(int i=0; i<all_s.size(); i++){
        //print(all_s[i]);
        //std::cout << i << "\t" << (*visited)[i] << std::endl;
        graph_connectivity_check = (*visited)[i] ? graph_connectivity_check : false;
    }

    if(graph_connectivity_check){
        delete G_Adj;
        delete visited;
        return std::make_pair(graph_connectivity_check, 1);
    }
    else{
        int count = 1;
        do{
            for(int i=1; i<(*visited).size(); i++){
                if(!(*visited)[i]){
                    DFS(G_Adj, visited, i);
                    break;
                }
            }
            graph_connectivity_check = true;
            for(int i=0; i<all_s.size(); i++){
                graph_connectivity_check = (*visited)[i] ? graph_connectivity_check : false;
            }
            count++;
        }while(!graph_connectivity_check);
        delete G_Adj;
        delete visited;
        return std::make_pair(false, count);
    }
}

bool strong_connectivity_check(){
    std::vector<std::vector<std::pair<int, type_s>>> *G_Adj = new std::vector<std::vector<std::pair<int, type_s>>>(all_s.size()); //список смежных для всех вершин в формате <№ в списке вершин, значение>
    for(int i=0; i<all_s.size(); i++){
        type_s current_s = all_s[i];
        for(int j=0; j<all_x.size(); j++){
            type_s buf_s = table[{current_s, all_x[j]}].h_func_transit;
            int numb_buf_s = get_number_s(buf_s);
            if(numb_buf_s == i){
                continue;
            }
            
            bool can_push = true;
            for(int k=0; k<(*G_Adj)[i].size() && can_push; k++){
                if((*G_Adj)[i][k].first == numb_buf_s){
                    can_push = false;
                }
            }
            if(can_push){
                (*G_Adj)[i].push_back(make_pair(numb_buf_s, buf_s));
            }
        }
    }
    std::vector<bool> *visited = new std::vector<bool>(all_s.size(), true);
    for(int i=0; i<all_s.size(); i++){
        std::vector<bool> *buf_visited = new std::vector<bool>(all_s.size(), false);
        DFS(G_Adj, buf_visited, i);
        for(int j=0; j<all_s.size(); j++){
            (*visited)[i] = (*visited)[i] && (*buf_visited)[i];
        }
    }

    bool checking_strong_connectivity_graph = true;
    for(int i=0; i<all_s.size(); i++){
        //print(all_s[i]);
        //std::cout << i << "\t" << (*visited)[i] << std::endl;
        checking_strong_connectivity_graph = (*visited)[i] ? checking_strong_connectivity_graph : false;
    }

    delete G_Adj;
    delete visited;
    return checking_strong_connectivity_graph;
}

public:
    void Lab3(){
        std::cout << "\n\n\t\tЛабораторная работа №3\n\n" << std::endl;
        std::pair<bool, int> buf_simple = simple_connectivity_check();
        bool graph_connectivity_check = buf_simple.first;
        bool checking_strong_connectivity_graph = false;
        std::cout << "Граф автомата ";
        if(graph_connectivity_check){
            checking_strong_connectivity_graph = strong_connectivity_check();
        }
        else{
            printf("имеет %d компонент связности - ", buf_simple.second);
            std::cout << "НЕ ";
        }
        std::cout << "связный и ";
        if(!checking_strong_connectivity_graph){
            std::cout << "НЕ ";
        }
        std::cout << "сильносвязный." << std::endl;
    }

private:

    bool check_null(){
        bool result = true;
        for(int i=0; i<all_x.size() && result; i++){
            for(int j=1; j<all_s.size() && result; j++){
                result *= table[{all_s[j], all_x[i]}].f_func_output == table[{all_s[j-1], all_x[i]}].f_func_output;
            }
        }
        return result;
    }

    std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>> L4_find_s(type_s find_s, int count, std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>> result, std::pair<std::vector<type_s>, std::vector<type_x>> path){
        if(count == 0){
            if(table[{path.first[path.first.size()-1], path.second[path.second.size()-1]}].h_func_transit == find_s){
                std::pair<std::vector<type_x>, std::vector<type_y>> buf_res;
                printf("[");
                for(int i=0; i<path.first.size(); i++){
                    buf_res.first.push_back(path.second[i]);
                    buf_res.second.push_back(table[{path.first[i], path.second[i]}].f_func_output);
                    printf("[");
                    print(path.first[i]);
                    printf("; ");
                    print(path.second[i]);
                    printf("]");
                }
                printf("]");
                result.push_back(buf_res);
            }
            return result;
        }

        if(path.first.size()==0 && path.second.size()==0){
            for(int i=0; i<all_s.size(); i++){
                for(int j=0; j<all_x.size(); j++){
                    std::pair<std::vector<type_s>, std::vector<type_x>> buf_path = path;
                    buf_path.first.push_back(all_s[i]);
                    buf_path.second.push_back(all_x[j]);
                    result = L4_find_s(find_s, count-1, result, buf_path);
                }
            }
        }
        else{
            
        }
        return result;
    }

    bool L4_find_intersections(std::vector<std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>>> current_Q){
        for(int i=0; i<all_s.size(); i++){
            for(int j=i+1; j<all_s.size(); j++){
                for(int a=0; a < current_Q[i].size(); a++){
                    for(int b=0; b < current_Q[j].size(); b++){
                        if(current_Q[i][a] == current_Q[j][b]){
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    
public:
    void Lab4(){
        const_n = 2;
        all_s = {{0,1}, {1,0}, {1,1}};
        all_x = {0, 1};
        table[{{0,1}, 0}].h_func_transit = {0,1};
        table[{{0,1}, 1}].h_func_transit = {1,0};
        table[{{1,0}, 0}].h_func_transit = {1,0};
        table[{{1,0}, 1}].h_func_transit = {1,1};
        table[{{1,1}, 0}].h_func_transit = {0,1};
        table[{{1,1}, 1}].h_func_transit = {0,1};

        table[{{0,1}, 0}].f_func_output = 1;
        table[{{0,1}, 1}].f_func_output = 1;
        table[{{1,0}, 0}].f_func_output = 0;
        table[{{1,0}, 1}].f_func_output = 0;
        table[{{1,1}, 0}].f_func_output = 0;
        table[{{1,1}, 1}].f_func_output = 1;



        std::cout << "\n\n\t\tЛабораторная работа №4\n\n" << std::endl;
        std::vector<std::vector<std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>>>> Q;
        bool check_stop = false;
        int m = 0;
        if(check_null()){
            printf("Память автомата: %d\n", m);
            return;
        }
        if(const_delta==0 && const_mu==0){
            Lab2();
        }
        int min_M = const_delta;
        int max_M = (all_s.size()*(all_s.size()-1))/2;

        do{
            m++;
            std::vector<std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>>> current_Q(all_s.size());
            printf("\nСчитаем Q%d...\n", m);
            for(int i=0; i<all_s.size(); i++){
                std::vector<std::pair<std::vector<type_x>, std::vector<type_y>>> buf_Qm_i;
                printf("\n\tСчитаем Q%d(%d)...", m, i);
                buf_Qm_i = L4_find_s(all_s[i], m, buf_Qm_i, make_pair(std::vector<type_s>(), std::vector<type_x>()));
                current_Q[i] = buf_Qm_i;

                for(int j=0; j<buf_Qm_i.size(); j++){
                    printf ("{");
                    for(int k=0; k<buf_Qm_i[j].first.size(); k++){
                        print(buf_Qm_i[j].first[k]);
                    }
                    printf (";");
                    for(int k=0; k<buf_Qm_i[j].second.size(); k++){
                        print(buf_Qm_i[j].second[k]);
                    }
                    printf ("} ");
                }
            }
            printf("\nИщем пересечения ... ");
            check_stop = !L4_find_intersections(current_Q);
            Q.push_back(current_Q);

            if(!check_stop) printf("пересечения множеств найдены\n");
            else printf("пересечения множеств НЕ найдены\n");

        }while(!check_stop && m <= max_M);

        if(m < min_M || m > max_M){
            printf("Память автомата: *бесконечность*\n");
            return;
        }
        else{
            printf("Память автомата: %d\n", m);
            return;
        }
    }
};


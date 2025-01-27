//#include "Machine_Table.h"

class PC{
private:
    Table<int, std::vector<uint32_t>, int> *TT;
    uint32_t N; 
    std::vector<int> phi;
    std::vector<int> psi;

    std::vector<uint32_t> int_to_vec_bit(int number, int bit_len){
        std::vector<uint32_t> bits(bit_len);
        
        for(int i=0; i<bit_len; ++i){
            bool bit = (number & (1 << i)) > 0;

            bits[bit_len-i-1] = bit ? 1 : 0;
        }
        return bits;
    }

    void create_table(){
        int size = std::pow(2, N);
        for(int i=0; i<size; i++){
            std::vector<uint32_t> s = int_to_vec_bit(i, N);
            
            std::vector<uint32_t> h0 = int_to_vec_bit((i<<1) & (size-1), N);
            h0[N-1] = phi[2*i];
            std::vector<uint32_t> h1 = int_to_vec_bit((i<<1) & (size-1), N);
            h1[N-1] = phi[2*i+1];

            int f0 = psi[2*i + phi[2*i + 0]];
            int f1 = psi[2*i + phi[2*i + 1]];

            TT->table[{s, 0}] = {h0, f0};
            TT->table[{s, 1}] = {h1, f1};

            TT->push_s(s);
        }
        TT->push_x(0);
        TT->push_x(1);
    }

    void print_vec(std::string str, std::vector<int> vec){
        std::cout << str <<": (";
        for(int i=0; i<vec.size(); i++){
            if(i!=0){
                std::cout << ", ";
            }
            std::cout << vec[i];
        }
        std::cout << ")" << std::endl;
    }

public:
    PC(int new_n, std::vector<int> new_phi, std::vector<int> new_psi){
        N = new_n;
        phi = new_phi;
        psi = new_psi;

        TT = new Table<int, std::vector<uint32_t>, int>(2, 1, N, 1);
        create_table();
    }

    void print_test(){
        printf("N = %d\n", N);
        print_vec("phi: ", phi);
        print_vec("psi: ", psi);
    }

    Table<int, std::vector<uint32_t>, int>* get_table(){
        return TT;
    }
};
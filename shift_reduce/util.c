// lsy 和 cys 提供的函数的定义
#include <cfg.h>

bool check_feasible(struct array a, struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]){
    int current_state = 0;
    for(int i = 0; i < a.len; i++){
        int symbol = a.elements[i];
        if (trans[current_state][symbol].t == 0){
            current_state = trans[current_state][symbol].id;
        } else {
            return false;
        }
    }
    return true;
}

void pre_trans(int number_of_symbol, int number_of_prod, struct prod *grammar, struct array *follow, struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]){
    // todo
}
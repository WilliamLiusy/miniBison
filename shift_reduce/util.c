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

// 从小到大排序
bool cmp_handler(struct handler a, struct handler b){
    return a.prod_id < b.prod_id || (a.prod_id == b.prod_id && a.dot_pos < b.dot_pos);
}

bool eq_handler(struct handler a, struct handler b){
    return a.prod_id == b.prod_id && a.dot_pos == b.dot_pos;
}

// expand the state
// used in pre_trans
void expand_state(int number_of_symbol, int number_of_prod, struct prod *grammar, struct state *state_info){
    bool used[number_of_symbol];
    memset(used, 0, sizeof(used));
    int current_length = state_info -> size;

    for(int i=0;i<current_length;++i){
        int prod_id = state_info->s[i].prod_id;
        int prod_len = grammar[prod_id].len;
        int dot_pos = state_info->s[i].dot_pos;
        if(dot_pos == prod_len){
            continue;
        }
        int next_symbol = grammar[prod_id].r[dot_pos];
        if(used[next_symbol]){
            continue;
        }
        used[next_symbol] = true;
        for(int j=0;j<number_of_prod;++j){
            if(grammar[j].l == next_symbol){
                struct handler tmp;
                tmp.prod_id = j;
                tmp.dot_pos = 0;
                state_info->s[current_length++] = tmp;
            }
        }
    }
    state_info->size = current_length;

    // sort the handlers in the state
    for(int i = 0;i < current_length;++i){
        for(int j = i + 1;j < state_info -> size;++j){
            if(eq_handler(state_info->s[i], state_info->s[j])){
                fprintf(stderr, "Error: two handlers have the same production id\n");
                exit(1);
            }
            if(cmp_handler(state_info->s[j], state_info->s[i])){
                struct handler tmp = state_info->s[i];
                state_info->s[i] = state_info->s[j];
                state_info->s[j] = tmp;
            }
        }
    }
}

// 每一个 state 在分配空间的时候都分配了 MAX_NUMBER_OF_PROD 个 handler 的空间
void pre_trans(int number_of_symbol, int number_of_prod, struct prod *grammar, struct array *follow, struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]){
    // todo
}
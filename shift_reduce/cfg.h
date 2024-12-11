#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_SYMB (100)
#define MAX_NUMBER_OF_PROD (1000)
#define MAX_NUMBER_OF_STATE (10000)

/* id of symbols are 0, 1, ... */
/* id of productions are 0, 1, ... */

struct prod {
	int l;   /* the symbol on the left */
	int *r;  /* the array of symbols on the right */
	int len; /* the number of symbols on the right */
};

/* Given a production like E -> E + E, a handler can be E -> E . + E */
struct handler {
	int prod_id;
	int dot_pos; /* dot is the number of symbols before the dot */
};

struct state {
	int size;          /* the number of handlers in a state */
	struct handler *s; /* array of handlers in every state */
};

struct trans_result {
	int t;  /* 0 for shift and 1 for reduce */
	int id; /* the id of destination state (in the case of shift), or the id of
	           production (in the case of reduction) */
};

struct array {
	int *elements;
	int len;
};

// 全局变量
/* inputs: */
// int number_of_symb; /* number of symbols */
// int number_of_prod; /* number of productions */
// struct prod grammar[MAX_NUMBER_OF_PROD];
// struct array terminal;
// struct array non_terminal;

/* outputs: */
// struct state state_info[MAX_NUMBER_OF_STATE];
// struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB];

/* other */
// int state_num = 0;
// bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];
// bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];

// 读入语法规则 并判断了终结符和非终结符
void read_in_grammar(char *filename, int *number_of_symb, int *number_of_prod,
                     struct prod grammar[MAX_NUMBER_OF_PROD]);

// 调用后全局变量即为 first和follow 集合
void getFirst(int number_of_symb, int number_of_prod,
              struct prod grammar[MAX_NUMBER_OF_PROD],
              bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]);
void getFollow(int number_of_symb, int number_of_prod,
               struct prod grammar[MAX_NUMBER_OF_PROD],
               bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB],
               bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]);

// 预处理出状态转移表以及所有结点上的状态信息
// 在预处理的时候会顺便检查是否有移入规约冲突，如果有，会输出冲突信息
// 该函数会使用follow集合，因此在调用这个函数之前必须先预处理出follow集合
// state_num 会被赋值为最终的状态数
// state_info 用于存储所有状态的信息
// trans 用于存储状态转移表
void pre_trans(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num);

// 用于检查给定的字符序列 a 是否是可行结构
// trans 是处理好的状态转移表
bool check_feasible(
    struct array a,
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]);

// 返回语法是否会使得移入规约算法冲突，有冲突返回 true
// 暂时有点问题，请不要调用这个函数，pre_trans已经检查语法是否有歧义
bool check_ShiftReduce_ambiguity(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int state_num);

// 以下是一些辅助函数

// return trur if a < b
bool cmp_handler(struct handler a, struct handler b);

// return true if a == b
bool eq_handler(struct handler a, struct handler b);

// 移入symbol，将符合移入条件的产生式存储在新状态中（此时
// 新的状态并不完整）
// new_state 的空间需要在外部分配
void shift_in(struct state current_state, int symbol,
              struct state *new_state_ptr,
              struct prod grammar[MAX_NUMBER_OF_PROD]);

// 对于移入后得到的新状态，将其扩展为完整的状态
// 并按照产生式的顺序进行排序
void expand_state(struct state *current_state_ptr, int number_of_symb,
                  int number_of_prod, struct prod grammar[MAX_NUMBER_OF_PROD]);

// 找到在某个状态下可以规约并且规约后仍然是可行结构的所有产生式
// state_history 是记录状态的栈，用于在规约后回溯状态
struct array get_available_prod(int current_state, struct array *state_history,
                                struct prod grammar[MAX_NUMBER_OF_PROD],
                                struct state state_info[MAX_NUMBER_OF_STATE]);

// 通过dfs遍历所有结点并生成状态转移表
// state_history 是记录状态的栈，用于查找可用的规约
// dfs_trans 会调用 get_available_prod
void dfs_trans(
    struct array *state_history, struct array *symb_history, int number_of_symb,
    int number_of_prod, struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num);

// 打印编号为 prod_id 的产生式
void print_prod(int prod_id, struct prod grammar[MAX_NUMBER_OF_PROD]);

// 打印状态
void print_state(struct state s);

// 打印数组
void print_array(struct array);

// 打印 first和follow 集合
void printFirst(int number_of_symb,
                bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]);
void printFollow(int number_of_symb,
                 bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]);

#endif // CFG_H_INCLUDED

#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_SYMB (100)
#define MAX_NUMBER_OF_PROD (1000)
#define MAX_NUMBER_OF_STATE (10000)

// 全局变量
/* inputs: */
int number_of_symb; /* number of symbols */
int number_of_prod; /* number of productions */
struct prod grammar[MAX_NUMBER_OF_PROD];

/* outputs: */
struct state state_info[MAX_NUMBER_OF_STATE];
struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB];

/* other */
int state_num = 0;
bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];
bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];

/* id of symbols are 0, 1, ... */
/* id of productions are 0, 1, ... */

struct prod
{
    int l;   /* the symbol on the left */
    int* r;  /* the array of symbols on the right */
    int len; /* the number of symbols on the right */
};

/* Given a production like E -> E + E, a handler can be E -> E . + E */
struct handler
{
    int prod_id;
    int dot_pos; /* dot is the number of symbols before the dot */
};

struct state
{
    int size;          /* the number of handlers in a state */
    struct handler* s; /* array of handlers in every state */
};

struct trans_result
{
    int t;  /* 0 for shift and 1 for reduce */
    int id; /* the id of destination state (in the case of shift), or the id of
               production (in the case of reduction) */
};

struct array
{
    int* elements;
    int len;
};

bool check_feasible(struct array a);

void pre_trans();

// 以下是一些辅助函数

bool cmp_handler(struct handler a, struct handler b);

bool eq_handler(struct handler a, struct handler b);

void expand_state(struct state* current_state);

void shift_in(struct state current_state, int symbol, struct state* new_state);

struct array get_available_prod(int current_state, struct array* state_history);

void dfs_trans(struct array* state_history);

#endif  // CFG_H_INCLUDED

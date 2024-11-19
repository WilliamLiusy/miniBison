#include "cfg.h"

/* inputs: */
int number_of_symb; /* number of symbols */
int number_of_prod; /* number of productions */
struct prod grammar[MAX_NUMBER_OF_PROD];

/* outputs: */
struct state state_info[MAX_NUMBER_OF_STATE];
struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB];


#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBER_OF_SYMB (100)
#define MAX_NUMBER_OF_PROD (1000)
#define MAX_NUMBER_OF_STATE (10000)

/* id of symbols are 0, 1, ... */
/* id of productions are 0, 1, ... */

struct prod {
  int l; /* the symbol on the left */
  int * r; /* the array of symbols on the right */
  int len; /* the number of symbols on the right */
};

/* Given a production like E -> E + E, a handler can be E -> E . + E */
struct handler {
  int prod_id;
  int dot_pos; /* dot is the number of symbols before the dot */
};

struct state {
  int size; /* the number of handlers in a state */
  struct handler * s; /* array of handlers in every state */
};

struct trans_result {
  int t; /* 0 for shift and 1 for reduce */
  int id; /* the id of destination state (in the case of shift), or the id of production (in the case of reduction) */
};

#endif // CFG_H_INCLUDED

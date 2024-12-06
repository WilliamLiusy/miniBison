// lsy 和 cys 提供的函数的定义
#include "cfg.h"

void read_in_grammar(char *filename, int *number_of_symb, int *number_of_prod,
                     struct prod grammar[MAX_NUMBER_OF_PROD]) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
	fprintf(stderr, "Error: cannot open file %s\n", filename);
	exit(1);
  }

  fscanf(fp, "%d %d", number_of_symb, number_of_prod);
  for (int i = 0; i < number_of_prod; i++) {
	fscanf(fp, "%d", &grammar[i].len);
	fscanf(fp, "%d", &grammar[i].l);
	grammar[i].r = (int *)malloc(grammar[i].len * sizeof(int));
	for (int j = 0; j < grammar[i].len; j++) {
	  fscanf(fp, "%d", &grammar[i].r[j]);
	}
  }

  // get Terminal and NonTerminal
  // int flag[number_of_symb];
  // memset(flag, 0, sizeof(flag));
  // terminal.elements = (int*)malloc(number_of_symb * sizeof(int));
  // non_terminal.elements = (int*)malloc(number_of_symb * sizeof(int));
  // // I should ignore 0th production, which is START -> x
  // for (int i = 1; i < number_of_prod; i++)
  // {
  //     if(flag[grammar[i].l] == 0)
  //     {
  //         printf("%s","!");
  //         flag[grammar[i].l] = 1;
  //         non_terminal.len++;
  //         non_terminal.elements[non_terminal.len - 1] = grammar[i].l;
  //     }
  // }
  // for (int i = 0; i < number_of_symb; i++) {
  //     if(flag[i] == 0) {
  //         printf("%s","#");
  //         terminal.len++;
  //         terminal.elements[terminal.len - 1] = i;
  //     }
  // }

  fclose(fp);
}

void print_prod(int prod_id, struct prod grammar[MAX_NUMBER_OF_PROD]) {
  printf("prod_id = %d: ", prod_id);
  printf("%d -> ", grammar[prod_id].l);
  for (int i = 0; i < grammar[prod_id].len; i++) {
	printf("%d ", grammar[prod_id].r[i]);
  }
  printf("\n");
}

// void print_terminal_and_nonterminal() {
//     printf("Terminal: ");
//     for(int i = 0; i < terminal.len; i++) {
//         printf("%d ", terminal.elements[i]);
//     }
//     printf("\n");
//     printf("NonTerminal: ");
//     for(int i = 0; i < non_terminal.len; i++) {
//         printf("%d ", non_terminal.elements[i]);
//     }
//     printf("\n");
// }

void print_state(struct state s) {
  printf("---------print_state-------------\n");
  printf("size of the state = %d\n", s.size);
  for (int i = 0; i < s.size; i++) {
	printf("prod_id = %d, dot_pos = %d\n", s.s[i].prod_id, s.s[i].dot_pos);
  }
  printf("--------------------\n");
}

void print_array(struct array a) {
  printf("---------print_array-------------\n");
  printf("len = %d\n", a.len);
  for (int i = 0; i < a.len; i++) {
	printf("%d ", a.elements[i]);
  }
  printf("\n");
  printf("--------------------\n");
}

bool check_feasible(
    struct array a,
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]) {
  int current_state = 0;
  for (int i = 0; i < a.len; i++) {
	// printf("current_state = %d\n", current_state);
	int symbol = a.elements[i];
	if (trans[current_state][symbol].t == 0) {
	  current_state = trans[current_state][symbol].id;
	} else {
	  return false;
	}
  }
  return true;
}

// 从小到大排序
bool cmp_handler(struct handler a, struct handler b) {
  return a.prod_id < b.prod_id ||
         (a.prod_id == b.prod_id && a.dot_pos < b.dot_pos);
}

// 判断两个 handler 是否相等
bool eq_handler(struct handler a, struct handler b) {
  return a.prod_id == b.prod_id && a.dot_pos == b.dot_pos;
}

// expand the state
// used in pre_trans
// current_state 是指向待扩展的状态的指针
void expand_state(struct state *current_state_ptr, int number_of_symb,
                  int number_of_prod, struct prod grammar[MAX_NUMBER_OF_PROD]) {
  bool used[number_of_symb];
  memset(used, 0, sizeof(used));
  int current_length = current_state_ptr->size;

  for (int i = 0; i < current_length; ++i) {
	int prod_id = current_state_ptr->s[i].prod_id;
	int prod_len = grammar[prod_id].len;
	int dot_pos = current_state_ptr->s[i].dot_pos;
	if (dot_pos == prod_len) {
	  continue;
	}
	int next_symbol = grammar[prod_id].r[dot_pos];
	if (used[next_symbol]) {
	  continue;
	}
	used[next_symbol] = true;
	for (int j = 0; j < number_of_prod; ++j) {
	  if (grammar[j].l == next_symbol) {
		struct handler tmp;
		tmp.prod_id = j;
		tmp.dot_pos = 0;
		current_state_ptr->s[current_length++] = tmp;
	  }
	}
  }
  current_state_ptr->size = current_length;

  // sort the handlers in the state
  for (int i = 0; i < current_length; ++i) {
	for (int j = i + 1; j < current_state_ptr->size; ++j) {
	  if (eq_handler(current_state_ptr->s[i], current_state_ptr->s[j])) {
		fprintf(stderr, "Error: two handlers have the same production id\n");
		exit(1);
	  }
	  if (cmp_handler(current_state_ptr->s[j], current_state_ptr->s[i])) {
		struct handler tmp = current_state_ptr->s[i];
		current_state_ptr->s[i] = current_state_ptr->s[j];
		current_state_ptr->s[j] = tmp;
	  }
	}
  }
}

// 移入符号
// new_state 的空间需要在外部分配
void shift_in(struct state current_state, int symbol,
              struct state *new_state_ptr,
              struct prod grammar[MAX_NUMBER_OF_PROD]) {
  new_state_ptr->size = 0;
  for (int i = 0; i < current_state.size; ++i) {
	int prod_id = current_state.s[i].prod_id;
	int dot_pos = current_state.s[i].dot_pos;
	if (dot_pos < grammar[prod_id].len &&
	    grammar[prod_id].r[dot_pos] == symbol) {
	  struct handler tmp;
	  tmp.prod_id = prod_id;
	  tmp.dot_pos = dot_pos + 1;
	  new_state_ptr->s[new_state_ptr->size++] = tmp;
	}
  }
}

bool eq_state(struct state a, struct state b) {
  if (a.size != b.size) {
	return false;
  }
  for (int i = 0; i < a.size; ++i) {
	if (!eq_handler(a.s[i], b.s[i])) {
	  return false;
	}
  }
  return true;
}

struct array get_available_prod(int current_state, struct array *state_history,
                                struct prod grammar[MAX_NUMBER_OF_PROD],
                                struct state state_info[MAX_NUMBER_OF_STATE]) {
  // 存储可行的产生式编号
  struct array available_prod;
  available_prod.elements = (int *)malloc(MAX_NUMBER_OF_PROD * sizeof(int));
  available_prod.len = 0;

  for (int i = 0; i < state_info[current_state].size; ++i) {
	int prod_id = state_info[current_state].s[i].prod_id;
	int dot_pos = state_info[current_state].s[i].dot_pos;

	// 先判断产生式是否可以规约
	if (dot_pos != grammar[prod_id].len) {
	  continue;
	}

	// 对于可以规约的产生式子，先通过历史状态栈找到
	// 去掉待规约产生式右半部分后 的状态
	int past_state =
	    state_history->elements[state_history->len - 1 - grammar[prod_id].len];
	bool available_flag = false;

	// 加入规约后产生的新字符（产生式的左边），检查是否是可行状态
	for (int j = 0; j < state_info[past_state].size; ++j) {
	  int past_prod_id = state_info[past_state].s[j].prod_id;
	  int past_dot_pos = state_info[past_state].s[j].dot_pos;

	  if (past_dot_pos < grammar[past_prod_id].len &&
	      grammar[past_prod_id].r[past_dot_pos] == grammar[prod_id].l) {
		available_flag = true;
		break;
	  }
	}
	if (available_flag) {
	  available_prod.elements[available_prod.len++] = prod_id;
	}
  }
  return available_prod;
}

void dfs_trans(
    struct array *state_history, int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num) {
  int current_state = state_history->elements[state_history->len - 1];

  // printf("current_state = %d\n", current_state);
  // printf("shift from state %d\n",
  //        state_history->len > 1
  //            ? state_history->elements[state_history->len - 2]
  //            : -1);
  // print_state(state_info[current_state]);

  // 查找目前状态可用的规约产生式
  struct array available_prod =
      get_available_prod(current_state, state_history, grammar, state_info);

  // print_array(available_prod);

  // 遍历所有可能的下一个符号，递归寻找新状态并生成trans数组
  for (int i = 0; i < number_of_symb; ++i) {
	struct state new_state;
	new_state.size = 0;

	new_state.s =
	    (struct handler *)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));

	shift_in(state_info[current_state], i, &new_state, grammar);
	expand_state(&new_state, number_of_symb, number_of_prod, grammar);

	if (new_state.size > 0) {
	  bool flag = false;

	  // 检查新状态是否已经存在
	  for (int j = 0; j < *state_num; ++j) {
		if (eq_state(new_state, state_info[j])) {
		  trans[current_state][i].t = 0;
		  trans[current_state][i].id = j;
		  flag = true;
		  free(new_state.s);
		  break;
		}
	  }

	  // 如果新状态不存在，递归到新状态
	  if (!flag) {
		state_history->elements[state_history->len++] = *state_num;
		++(*state_num);
		state_info[*state_num] = new_state;
		trans[current_state][i].t = 0;
		trans[current_state][i].id = *state_num - 1;
		// printf("shift in symbol %d\n", i);
		dfs_trans(state_history, number_of_symb, number_of_prod, grammar,
		          state_info, trans, follow, state_num);
		state_history->len--;
	  }
	} else {
	  // 无法移入，检查是否可以规约
	  // 先释放不需要的内存
	  free(new_state.s);

	  bool flag = false;
	  // 对于所有可行的产生式检查 follow 集合
	  for (int j = 0; j < available_prod.len; ++j) {
		int prod_id = available_prod.elements[j];
		int l_symbol = grammar[prod_id].l;
		if (follow[l_symbol][i] == 1) {
		  trans[current_state][i].t = 1;
		  trans[current_state][i].id = prod_id;
		  flag = true;
		  break;
		}
	  }

	  if (!flag) {
		trans[current_state][i].t = -1;
		trans[current_state][i].id = -1;
	  }
	}
  }

  free(available_prod.elements);
}

// 每一个 state 在分配空间的时候都分配了 MAX_NUMBER_OF_PROD 个 handler
// 的空间
void pre_trans(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num) {
  struct state initial_state;
  initial_state.size = 1;
  initial_state.s =
      (struct handler *)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
  initial_state.s[0].prod_id = 0;
  initial_state.s[0].dot_pos = 0;
  expand_state(&initial_state, number_of_symb, number_of_prod, grammar);

  state_info[0] = initial_state;

  *state_num = 1;

  struct array state_history;
  state_history.elements = (int *)malloc(MAX_NUMBER_OF_STATE * sizeof(int));
  state_history.len = 1;
  state_history.elements[0] = 0;

  dfs_trans(&state_history, number_of_symb, number_of_prod, grammar, state_info,
            trans, follow, state_num);
}

void getFirst(int number_of_symb, int number_of_prod,
              struct prod grammar[MAX_NUMBER_OF_PROD],
              bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]) {
  for (int i = 0; i < number_of_symb; i++) {
	first[i][i] = 1;
  }
  for (int i = 0; i < number_of_prod; i++) {
	first[grammar[i].l][grammar[i].l] = 0;
  }
  bool changed = false;
  do {
	changed = false;
	// I should ignore 0th production, which is START -> x
	for (int i = 1; i < number_of_prod; i++) {
	  int Y = grammar[i].l;
	  int Z = grammar[i].r[0];
	  for (int u = 0; u < number_of_symb; u++) {
		if (first[Z][u]) {
		  if (first[Y][u] == 0) {
			first[Y][u] = 1;
			changed = true;
		  }
		}
	  }
	}
  } while (changed);
}

void getFollow(int number_of_symb, int number_of_prod,
               struct prod grammar[MAX_NUMBER_OF_PROD],
               bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB],
               bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]) {
  // I should ignore 0th production, which is START -> x
  for (int i = 1; i < number_of_prod; i++) {
	for (int j = 0; j + 1 < grammar[i].len; j++) {
	  for (int u = 0; u < number_of_symb; u++) {
		if (first[grammar[i].r[j + 1]][u]) {
		  if (follow[grammar[i].r[j]][u] == 0) {
			follow[grammar[i].r[j]][u] = 1;
		  }
		}
	  }
	}
  }
  bool changed = false;
  do {
	changed = false;
	// I should ignore 0th production, which is START -> x
	for (int r = 1; r < number_of_prod; r++) {
	  int Z = grammar[r].l;
	  int Y = grammar[r].r[grammar[r].len - 1];
	  for (int u = 0; u < number_of_symb; u++) {
		if (follow[Z][u]) {
		  if (follow[Y][u] == 0) {
			follow[Y][u] = 1;
			changed = true;
		  }
		}
	  }
	}
  } while (changed);
}

void printFirst(int number_of_symb,
                bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]) {
  for (int i = 0; i < number_of_symb; i++) {
	printf("First[%d]: ", i);
	for (int j = 0; j < number_of_symb; j++) {
	  if (first[i][j]) {
		printf("%d ", j);
	  }
	}
	printf("\n");
  }
}

void printFollow(int number_of_symb,
                 bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB]) {
  for (int i = 0; i < number_of_symb; i++) {
	printf("Follow[%d]: ", i);
	for (int j = 0; j < number_of_symb; j++) {
	  if (follow[i][j]) {
		printf("%d ", j);
	  }
	}
	printf("\n");
  }
}

bool check_ShiftReduce_ambiguity(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num) {
  // shift-reduce conflict
  // 找一个是另一个前缀
  for (int i = 1; i < number_of_prod; i++) {
	for (int j = 1; j < number_of_prod; j++) {
	  if (i == j) {
		continue;
	  }
	  if (grammar[i].len >= grammar[j].len) {
		continue;
	  }
	  bool flag = true;
	  for (int k = 0; k < grammar[i].len; k++) {
		if (grammar[i].r[k] != grammar[j].r[k]) {
		  flag = false;
		  break;
		}
	  }
	  if (flag) {
		// i 是 j 的前缀
		// i : V -> X.
		// j : U -> X.aY
		int V = grammar[i].l;
		int U = grammar[j].l;
		int a = grammar[j].r[grammar[i].len];
		// 若 a \in Follow(V) 则冲突
		int flag2 = follow[V][a];

		// 若两个产生式在dfa一个状态集里，则冲突
		int flag1 = false;
		for (int s = 0; s < *state_num; s++) {
		  bool i_in = false;
		  bool j_in = false;
		  for (int l = 0; l < state_info[s].size; l++) {
			if (state_info[s].s[l].prod_id == i &&
			    state_info[s].s[l].dot_pos == grammar[i].len) {
			  i_in = true;
			  break;
			}
		  }
		  for (int l = 0; l < state_info[s].size; l++) {
			if (state_info[s].s[l].prod_id == j &&
			    state_info[s].s[l].dot_pos == grammar[i].len) {
			  j_in = true;
			  break;
			}
		  }
		  if (i_in && j_in) {
			flag1 = true;
			break;
		  }
		}
		if (flag1 && flag2) {
		  // printf("%s\n","!!!");
		  return true;
		}
	  }
	}
  }

  // reduce-reduce conflict
  // 找一个是另一个后缀
  for (int i = 1; i < number_of_prod; i++) {
	for (int j = 1; j < number_of_prod; j++) {
	  if (i == j) {
		continue;
	  }
	  if (grammar[i].len > grammar[j].len) {
		continue;
	  }
	  bool flag = true;
	  for (int k = 0; k < grammar[i].len; k++) {
		if (grammar[i].r[grammar[i].len - 1 - k] !=
		    grammar[j].r[grammar[j].len - 1 - k]) {
		  flag = false;
		  break;
		}
	  }
	  if (flag) {
		// i 是 j 的后缀
		// i : V -> Y.
		// j : U -> XY.
		int V = grammar[i].l;
		int U = grammar[j].l;
		// 若 Follow(U) 交 Follow(V) 不为空 则冲突
		int flag2 = false;
		for (int a = 0; a < number_of_symb; a++) {
		  if (follow[U][a] && follow[V][a]) {
			flag2 = true;
			break;
		  }
		}

		// 若两个产生式在dfa一个状态集里，则冲突
		int flag1 = false;
		for (int s = 0; s < state_num; s++) {
		  bool i_in = false;
		  bool j_in = false;
		  for (int l = 0; l < state_info[s].size; l++) {
			if (state_info[s].s[l].prod_id == i &&
			    state_info[s].s[l].dot_pos == grammar[i].len) {
			  i_in = true;
			  break;
			}
		  }
		  for (int l = 0; l < state_info[s].size; l++) {
			if (state_info[s].s[l].prod_id == j &&
			    state_info[s].s[l].dot_pos == grammar[j].len) {
			  j_in = true;
			  break;
			}
		  }
		  if (i_in && j_in) {
			flag1 = true;
			break;
		  }
		}
		if (flag1 && flag2) {
		  // printf("%s\n","$$$");
		  return true;
		}
	  }
	}
  }
  return false;
}
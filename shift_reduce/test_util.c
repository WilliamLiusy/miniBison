
#include "util.c"

struct state state_info_glob[MAX_NUMBER_OF_STATE];
struct trans_result trans_glob[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB];

bool first_glob[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB],
    follow_glob[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];

// Test functions
void test_check_feasible() {
	struct array a;
	a.elements = (int *)malloc(MAX_NUMBER_OF_SYMB * sizeof(int));
	a.len = 3;
	a.elements[0] = 5;
	a.elements[1] = 6;
	a.elements[2] = 6;

	bool result = check_feasible(a, trans_glob);
	printf("check_feasible: %s\n", result ? "true" : "false");
}

void test_trans(
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]) {
	struct array a;
	a.elements = (int *)malloc(MAX_NUMBER_OF_SYMB * sizeof(int));
	a.len = 4;
	a.elements[0] = 0;
	a.elements[1] = 4;
	a.elements[2] = 0;
	a.elements[3] = 2;
	a.elements[4] = 4;
	a.elements[5] = 1;
	a.elements[6] = 6;

	int current_state = 0;
	for (int i = 0; i < a.len; i++) {
		int symbol = a.elements[i];
		if (trans[current_state][symbol].t == 0) {
			current_state = trans[current_state][symbol].id;
			printf("shift\n");
		} else {
			printf("reduce\n");
			printf("prod_id = %d\n", trans[current_state][symbol].id);
			break;
		}
	}
}

// void test_shift_in_expand() {
//   struct state current_state;
//   current_state.size = 7;
//   current_state.s =
//       (struct handler *)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
//   for (int i = 0; i < 7; ++i) {
// 	current_state.s[i].prod_id = i;
// 	current_state.s[i].dot_pos = 0;
//   }

//   struct state new_state;
//   new_state.s =
//       (struct handler *)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));

//   shift_in(current_state, 0, &new_state);
//   expand_state(&new_state);

//   struct state new_state2;
//   new_state2.s =
//       (struct handler *)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
//   shift_in(new_state, 3, &new_state2);
//   expand_state(&new_state2);

//   print_state(new_state2);

//   free(current_state.s);
//   free(new_state.s);
//   free(new_state2.s);
// }

int main() {
	// printf("OK");
	char *filename = "./shift_reduce/data/grammar_simple_languang.txt";
	int number_of_symb = 0, number_of_prod = 0;
	struct prod grammar[MAX_NUMBER_OF_PROD];
	read_in_grammar(filename, &number_of_symb, &number_of_prod, grammar);

	getFirst(number_of_symb, number_of_prod, grammar, first_glob);
	getFollow(number_of_symb, number_of_prod, grammar, follow_glob, first_glob);

	// printFirst(number_of_symb, first_glob);
	// printFollow(number_of_symb, follow_glob);

	int state_num;
	state_num = 1;

	pre_trans(number_of_symb, number_of_prod, grammar, state_info_glob,
	          trans_glob, follow_glob, &state_num);

	// printf("number_of_symb: %d\n", number_of_symb);
	// print_prod(2, grammar);
	// test_shift_in_expand();

	// test_check_feasible();
	// test_trans(trans_glob);

	// printf("check_ShiftReduce_ambiguity: %s\n",
	//        check_ShiftReduce_ambiguity(number_of_symb, number_of_prod,
	//        grammar,
	//                                    state_info_glob, follow_glob,
	//                                    state_num)
	//            ? "true"
	//            : "false");
}
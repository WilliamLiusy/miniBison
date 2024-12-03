
#include "util.c"

// Test functions
void test_check_feasible()
{
    struct array a;
    a.elements = (int*)malloc(MAX_NUMBER_OF_SYMB * sizeof(int));
    a.len = 1;
    a.elements[0] = 7;
    a.elements[1] = 3;
    a.elements[2] = 1;

    bool result = check_feasible(a);
    printf("check_feasible: %s\n", result ? "true" : "false");
}

void test_shift_in_expand()
{
    struct state current_state;
    current_state.size = 7;
    current_state.s =
        (struct handler*)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
    for (int i = 0; i < 7; ++i)
    {
        current_state.s[i].prod_id = i;
        current_state.s[i].dot_pos = 0;
    }

    struct state new_state;
    new_state.s =
        (struct handler*)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));

    shift_in(current_state, 0, &new_state);
    expand_state(&new_state);

    struct state new_state2;
    new_state2.s =
        (struct handler*)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
    shift_in(new_state, 3, &new_state2);
    expand_state(&new_state2);

    print_state(new_state2);

    free(current_state.s);
    free(new_state.s);
    free(new_state2.s);
}

int main()
{
    char* filename = "./data/grammar_add_multiply.txt";
    read_in_grammar(filename);

    // printf("number_of_symb: %d\n", number_of_symb);
    // print_prod(2);

    // test_shift_in_expand();

    pre_trans();

    test_check_feasible();
}
#include "cfg.h"


int number_of_symb; /* number of symbols */
int number_of_prod; /* number of productions */
struct prod grammar[MAX_NUMBER_OF_PROD];
struct array terminal;
struct array non_terminal;

/* outputs: */
struct state state_info[MAX_NUMBER_OF_STATE];
struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB];

/* other */
int state_num = 0;

int input[MAX_NUMBER_OF_INPUT];
OutputSequence output;

int main(int argc, char** argv)
{
	// argv[1]表示输入的语法
	// argv[2]表示输入的终结符序列
	// argv[3]表示输出的文件位置
	init_output_sequence(&output);
    char* input_grammer = argv[1];
    char* input_file = argv[2];
	char* output_file = argv[3];
    read_in_grammar(input_grammer, &number_of_symb, &number_of_prod, grammar);
	FILE *fp = fopen(input_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open file %s\n", input_file);
		exit(1);
	}
	int num;
	fscanf(fp, "%d", &num);
	for (int i = 0; i < num; ++i) {
		fscanf(fp, "%d", &input[i]);
	}

	fclose(fp);

	GenerateStates(number_of_symb, number_of_prod, grammar, state_info, trans, &state_num);
	shift_reduce(number_of_symb, grammar, trans, input, num, &output);

	FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        perror("Error opening file");
		exit(1);
    }

	for (int i = 0; i < output.size; ++i) {
		if (get_from_output_sequence(&output, i) == -1)
			fprintf(out, "%d\n", get_from_output_sequence(&output, i));
		else
			fprintf(out, "%d\n", get_from_output_sequence(&output, i) - 1);
	}

	fclose(out);

	// 清理空间
	free_output_sequence(&output);
	for (int i = 0; i < number_of_prod; ++i) {
		free(grammar[i].r);
	}
	for(int i = 0; i < state_num; ++i) {
		free(state_info[i].s);
	}

    return 0;
}
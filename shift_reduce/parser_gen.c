#include <stdio.h>

#include "cfg.h"

bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];
bool first[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB];

// 初始化栈
void init_stack(Stack* stack)
{
    stack->state = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    if (stack->state == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    stack->len = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    if (stack->len == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    stack->content = (int**)malloc(INITIAL_CAPACITY * sizeof(int*));
    if (stack->content == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    stack->top = -1;  // 初始栈为空
    stack->capacity = INITIAL_CAPACITY;
}

// 检查栈是否为空
int is_empty(Stack* stack) { return stack->top == -1; }

// 检查栈是否已满
int is_full(Stack* stack) { return stack->top == stack->capacity - 1; }

// 动态扩展栈的容量
void resize_stack(Stack* stack)
{
    stack->capacity *= 2;  // 容量翻倍
    stack->state = (int*)realloc(stack->state, stack->capacity * sizeof(int));
    if (stack->state == NULL)
    {
        printf("内存重新分配失败\n");
        exit(1);
    }
    stack->len = (int*)realloc(stack->len, stack->capacity * sizeof(int));
    if (stack->state == NULL)
    {
        printf("内存重新分配失败\n");
        exit(1);
    }
    stack->content =
        (int**)realloc(stack->content, stack->capacity * sizeof(int*));
    if (stack->state == NULL)
    {
        printf("内存重新分配失败\n");
        exit(1);
    }
}

// 将元素压入栈
void push(Stack* stack, int state_value, int len_value, int* content_value)
{
    if (is_full(stack))
    {
        resize_stack(stack);  // 如果栈满，扩展容量
    }
    ++stack->top;
    stack->state[stack->top] = state_value;      // 将元素压入栈顶
    stack->len[stack->top] = len_value;          // 将元素压入栈顶
    stack->content[stack->top] = content_value;  // 将元素压入栈顶
}

// 从栈顶弹出元素
void pop(Stack* stack)
{
    if (is_empty(stack))
    {
        printf("栈为空，无法弹出元素\n");
        exit(1);
    }
    free(stack->content[stack->top]);
    --stack->top;
}

// 查看栈顶元素
int peek_state(Stack* stack)
{
    if (is_empty(stack))
    {
        printf("输入不合法\n");
        exit(1);
    }
    return stack->state[stack->top];  // 返回栈顶元素
}
int peek_len(Stack* stack)
{
    if (is_empty(stack))
    {
        printf("输入不合法\n");
        exit(1);
    }
    return stack->len[stack->top];  // 返回栈顶元素
}
int* peek_content(Stack* stack)
{
    if (is_empty(stack))
    {
        printf("输入不合法\n");
        exit(1);
    }
    return stack->content[stack->top];  // 返回栈顶元素
}

// 释放栈的内存
void free_stack(Stack* stack)
{
    for (int i = 0; i < stack->top; ++i)
    {
        free(stack->content[i]);
    }
    free(stack->state);
    free(stack->len);
    free(stack->content);
    stack->top = -1;
    stack->capacity = 0;
}

void init_output_sequence(OutputSequence* seq)
{
    seq->data = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
    seq->size = 0;
    seq->capacity = INITIAL_CAPACITY;
}

// 扩展动态数组的容量
void resize_output_sequence(OutputSequence* seq)
{
    // 分配新的内存
    int* new_data = (int*)realloc(seq->data, 2 * seq->capacity * sizeof(int));
    if (new_data == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    seq->data = new_data;
    seq->capacity = 2 * seq->capacity;
}

// 向动态数组中添加元素
void append_to_output_sequence(OutputSequence* seq, int value)
{
    if (seq->size >= seq->capacity)
    {
        resize_output_sequence(seq);
    }
    seq->data[seq->size] = value;
    ++seq->size;
}

// 获取动态数组中的元素
int get_from_output_sequence(OutputSequence* seq, int index)
{
    if (index < 0 || index >= seq->size)
    {
        printf("索引越界\n");
        exit(1);
    }
    return seq->data[index];
}

// 释放动态数组的内存
void free_output_sequence(OutputSequence* seq)
{
    free(seq->data);
    seq->data = NULL;
    seq->size = 0;
    seq->capacity = 0;
}

// parser_gen的两个函数

void GenerateStates(
    int number_of_symb, int number_of_prod,
    struct prod grammer[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    int* state_num)
{
    if (number_of_prod >= MAX_NUMBER_OF_PROD)
    {
        printf("Too many productions!\n");
        exit(-1);
    }
    if (number_of_symb >= MAX_NUMBER_OF_SYMB)
    {
        printf("Too many symbols!\n");
        exit(-1);
    }
    // 额外处理第一个 START -> E

    grammer[0].len += 1;
    grammer[0].r = (int*)realloc(grammer[0].r, grammer[0].len * sizeof(int));
    grammer[0].r[grammer[0].len - 1] = number_of_symb;
    ++number_of_symb;

    // 先计算First集合
    getFirst(number_of_symb, number_of_prod, grammer, first);
    // 再根据First集合计算Follow集合
    getFollow(number_of_symb, number_of_prod, grammer, follow, first);
    // 利用Follow集合和语法生成状态转移表
    pre_trans(number_of_symb, number_of_prod, grammer, state_info, trans,
              follow, state_num);

    grammer[0].len -= 1;
    --number_of_symb;

    return;
}

void shift_reduce(
    int number_of_symb, struct prod grammer[MAX_NUMBER_OF_PROD],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    int input[MAX_NUMBER_OF_INPUT], int length_of_input, OutputSequence* output)
{
    if (length_of_input >= MAX_NUMBER_OF_INPUT)
    {
        printf("Input too long!\n");
    }
    // trick

    grammer[0].len += 1;
    grammer[0].r = (int*)realloc(grammer[0].r, grammer[0].len * sizeof(int));
    grammer[0].r[grammer[0].len - 1] = number_of_symb;
    ++number_of_symb;
    ++length_of_input;
    input[length_of_input - 1] = number_of_symb - 1;

    Stack state_history;
    init_stack(&state_history);
    push(&state_history, 0, 0, NULL);  // 将初始状态压入栈中
    init_output_sequence(output);
    // 着重看移入)的时候
    for (int next_symbol = 0; next_symbol < length_of_input; ++next_symbol)
    {
        bool move_next = false;
        while (!move_next)
        {
            struct trans_result next_step =
                trans[peek_state(&state_history)][input[next_symbol]];
            if (next_step.t == 0)
            {
                // 移入
                move_next = true;
                // 根据栈顶的元素生成新的栈元素，状态是next_step.id
                int* new_content =
                    (int*)malloc((peek_len(&state_history) + 1) * sizeof(int));
                for (int i = 0; i < peek_len(&state_history); ++i)
                {
                    new_content[i] = (peek_content(&state_history))[i];
                }
                new_content[peek_len(&state_history)] = input[next_symbol];
                push(&state_history, next_step.id, peek_len(&state_history) + 1,
                     new_content);
                //
                append_to_output_sequence(output, -1);
            }
            else
            {
                // 规约
                struct prod production = grammer[next_step.id];
                append_to_output_sequence(output, next_step.id);
                // 接下来更新栈
                // production = {l ,(int*) r, len}
                while (true)
                {
                    // 栈底部
                    if (peek_len(&state_history) == 0)
                    {
                        next_step =
                            trans[peek_state(&state_history)][production.l];
                        int* new_content = (int*)malloc(
                            (peek_len(&state_history) + 1) & sizeof(int));
                        for (int i = 0; i < peek_len(&state_history); ++i)
                        {
                            new_content[i] = (peek_content(&state_history))[i];
                        }
                        new_content[peek_len(&state_history)] = production.l;
                        push(&state_history, next_step.id,
                             peek_len(&state_history) + 1, new_content);
                        break;
                    }
                    int* stack_content = peek_content(&state_history);
                    // for (int i = 0; i < peek_len(&state_history); ++i)
                    //    printf("%d ", stack_content[i]);
                    // printf("\n");
                    bool flag = true;
                    for (int i = 0; i < production.len; ++i)
                    {
                        if (stack_content[peek_len(&state_history) - i - 1] !=
                            production.r[production.len - i - 1])
                        {
                            flag = false;
                            pop(&state_history);
                            break;
                        }
                    }
                    if (flag == false)
                    {
                        continue;
                    }
                    else
                    {
                        // 还要pop掉许多其他东西
                        int length = peek_len(&state_history);
                        int* what_we_want = (int*)malloc(
                            (peek_len(&state_history) + 1) * sizeof(int));
                        for (int i = 0; i < length - production.len; ++i)
                            what_we_want[i] = (peek_content(&state_history))[i];
                        length = length - production.len;
                        // what_we_want, length就是我们要匹配的
                        pop(&state_history);
                        while (true)
                        {
                            if (peek_len(&state_history) != length)
                            {
                                pop(&state_history);
                                continue;
                            }
                            bool flag = true;
                            for (int i = 0; i < length; ++i)
                            {
                                if (what_we_want[i] !=
                                    peek_content(&state_history)[i])
                                {
                                    flag = false;
                                    pop(&state_history);
                                    break;
                                }
                            }
                            if (flag)
                            {
                                break;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        next_step =
                            trans[peek_state(&state_history)][production.l];
                        int* new_content = (int*)malloc(
                            (peek_len(&state_history) + 1) * sizeof(int));
                        for (int i = 0; i < peek_len(&state_history); ++i)
                        {
                            new_content[i] = (peek_content(&state_history))[i];
                        }
                        new_content[peek_len(&state_history)] = production.l;
                        push(&state_history, next_step.id,
                             peek_len(&state_history) + 1, new_content);
                        break;
                    }
                }
            }
        }
    }

    free_stack(&state_history);
    grammer[0].len -= 1;
    --number_of_symb;
    output->size -= 1;
}
// lsy 和 cys 提供的函数的定义
#include <cfg.h>

bool check_feasible(struct array a)
{
    int current_state = 0;
    for (int i = 0; i < a.len; i++)
    {
        int symbol = a.elements[i];
        if (trans[current_state][symbol].t == 0)
        {
            current_state = trans[current_state][symbol].id;
        }
        else
        {
            return false;
        }
    }
    return true;
}

// 从小到大排序
bool cmp_handler(struct handler a, struct handler b)
{
    return a.prod_id < b.prod_id ||
           (a.prod_id == b.prod_id && a.dot_pos < b.dot_pos);
}

// 判断两个 handler 是否相等
bool eq_handler(struct handler a, struct handler b)
{
    return a.prod_id == b.prod_id && a.dot_pos == b.dot_pos;
}

// expand the state
// used in pre_trans
// current_state 是指向待扩展的状态的指针
void expand_state(struct state* current_state)
{
    bool used[number_of_symb];
    memset(used, 0, sizeof(used));
    int current_length = current_state->size;

    for (int i = 0; i < current_length; ++i)
    {
        int prod_id = current_state->s[i].prod_id;
        int prod_len = grammar[prod_id].len;
        int dot_pos = current_state->s[i].dot_pos;
        if (dot_pos == prod_len)
        {
            continue;
        }
        int next_symbol = grammar[prod_id].r[dot_pos];
        if (used[next_symbol])
        {
            continue;
        }
        used[next_symbol] = true;
        for (int j = 0; j < number_of_prod; ++j)
        {
            if (grammar[j].l == next_symbol)
            {
                struct handler tmp;
                tmp.prod_id = j;
                tmp.dot_pos = 0;
                current_state->s[current_length++] = tmp;
            }
        }
    }
    current_state->size = current_length;

    // sort the handlers in the state
    for (int i = 0; i < current_length; ++i)
    {
        for (int j = i + 1; j < current_state->size; ++j)
        {
            if (eq_handler(current_state->s[i], current_state->s[j]))
            {
                fprintf(stderr,
                        "Error: two handlers have the same production id\n");
                exit(1);
            }
            if (cmp_handler(current_state->s[j], current_state->s[i]))
            {
                struct handler tmp = current_state->s[i];
                current_state->s[i] = current_state->s[j];
                current_state->s[j] = tmp;
            }
        }
    }
}

// 移入符号
// new_state 的空间需要在外部分配
void shift_in(struct state current_state, int symbol, struct state* new_state)
{
    new_state->size = 0;
    for (int i = 0; i < current_state.size; ++i)
    {
        int prod_id = current_state.s[i].prod_id;
        int dot_pos = current_state.s[i].dot_pos;
        if (dot_pos < grammar[prod_id].len &&
            grammar[prod_id].r[dot_pos] == symbol)
        {
            struct handler tmp;
            tmp.prod_id = prod_id;
            tmp.dot_pos = dot_pos + 1;
            new_state->s[new_state->size++] = tmp;
        }
    }
}

bool eq_state(struct state a, struct state b)
{
    if (a.size != b.size)
    {
        return false;
    }
    for (int i = 0; i < a.size; ++i)
    {
        if (!eq_handler(a.s[i], b.s[i]))
        {
            return false;
        }
    }
    return true;
}

struct array get_available_prod(int current_state, struct array* state_history)
{
    // 存储可行的产生式编号
    struct array available_prod;
    available_prod.elements = (int*)malloc(MAX_NUMBER_OF_PROD * sizeof(int));
    available_prod.len = 0;

    for (int i = 0; i < state_info[current_state].size; ++i)
    {
        int prod_id = state_info[current_state].s[i].prod_id;
        int dot_pos = state_info[current_state].s[i].dot_pos;

        // 先判断产生式是否可以规约
        if (dot_pos != grammar[prod_id].len)
        {
            continue;
        }

        // 对于可以规约的产生式子，先通过历史状态栈找到
        // 去掉待规约产生式右半部分后 的状态
        int past_state =
            state_history
                ->elements[state_history->len - 1 - grammar[prod_id].len];
        bool available_flag = false;

        // 加入规约后产生的新字符（产生式的左边），检查是否是可行状态
        for (int j = 0; j < state_info[past_state].size; ++j)
        {
            int past_prod_id = state_info[past_state].s[j].prod_id;
            int past_dot_pos = state_info[past_state].s[j].dot_pos;

            if (past_dot_pos < grammar[past_prod_id].len &&
                grammar[past_prod_id].r[past_dot_pos] == grammar[prod_id].l)
            {
                available_flag = true;
                break;
            }
        }
        if (available_flag)
        {
            available_prod.elements[available_prod.len++] = prod_id;
        }
    }
    return available_prod;
}

void dfs_trans(struct array* state_history)
{
    int current_state = state_history->elements[state_history->len - 1];

    // 查找目前状态可用的规约产生式
    struct array available_prod =
        get_available_prod(current_state, state_history);

    // 遍历所有可能的下一个符号，递归寻找新状态并生成trans数组
    for (int i = 0; i < number_of_symb; ++i)
    {
        struct state new_state;
        new_state.size = 0;
        new_state.s = (struct handler*)malloc(MAX_NUMBER_OF_PROD *
                                              sizeof(struct handler));
        shift_in(state_info[current_state], i, &new_state);
        expand_state(&new_state);

        if (new_state.size > 0)
        {
            bool flag = false;

            // 检查新状态是否已经存在
            for (int i = 0; i < state_num; ++i)
            {
                if (eq_state(new_state, state_info[i]))
                {
                    trans[current_state][i].t = 0;
                    trans[current_state][i].id = i;
                    flag = true;
                    free(new_state.s);
                    break;
                }
            }

            // 如果新状态不存在，递归到新状态
            if (!flag)
            {
                state_info[state_num++] = new_state;
                state_history->elements[state_history->len++] = state_num - 1;
                dfs_trans(state_history);
                state_history->len--;
            }
        }
        else
        {
            // 无法移入，检查是否可以规约
            // 先释放不需要的内存
            free(new_state.s);

            // 对于所有可行的产生式检查 follow 集合
            for (int j = 0; j < available_prod.len; ++j)
            {
                int prod_id = available_prod.elements[j];
                int l_symbol = grammar[prod_id].l;
                if (follow[l_symbol][i] == 1)
                {
                    trans[current_state][i].t = 1;
                    trans[current_state][i].id = prod_id;
                    break;
                }
            }
        }
    }

    free(available_prod.elements);
}

// 每一个 state 在分配空间的时候都分配了 MAX_NUMBER_OF_PROD 个 handler
// 的空间
void pre_trans()
{
    struct state initial_state;
    initial_state.size = 1;
    initial_state.s =
        (struct handler*)malloc(MAX_NUMBER_OF_PROD * sizeof(struct handler));
    initial_state.s[0].prod_id = 0;
    initial_state.s[0].dot_pos = 0;
    expand_state(&initial_state);

    state_info[0] = initial_state;
    ++state_num;

    int state_num = 1;

    struct array state_history;
    state_history.elements = (int*)malloc(MAX_NUMBER_OF_STATE * sizeof(int));
    state_history.len = 1;
    state_history.elements[0] = 0;

    dfs_trans(&state_history);
}
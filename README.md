# miniBison

为了保持一致，请使用 linux 平台。

使用如下数据结构：

## 注意事项

1、额外定义了如下数据结构。

```
struct array{
    int l;  // 长度
    int *a;  // 元素
};
```

2、目前已经实现的函数的声明都在`cfg.h`中，可自行查阅，以下是几个可能会用到的函数：

```
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
// 该函数会使用follow集合，因此在调用这个函数之前必须先预处理出follow集合
void pre_trans(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num);

// 用于检查给定的字符序列 a 是否是可行结构
// 该函数会用到状态转移表，因此在调用这个函数之前必须先预处理出状态转移表
bool check_feasible(
    struct array a,
    struct trans_result trans[MAX_NUMBER_OF_STATE][MAX_NUMBER_OF_SYMB]);

// 返回语法是否会使得移入规约算法冲突，有冲突返回 true
bool check_ShiftReduce_ambiguity(
    int number_of_symb, int number_of_prod,
    struct prod grammar[MAX_NUMBER_OF_PROD],
    struct state state_info[MAX_NUMBER_OF_STATE],
    bool follow[MAX_NUMBER_OF_SYMB][MAX_NUMBER_OF_SYMB], int *state_num);
```

3、生成的可执行文件请放在 `./shift_reduce/bin` 路径中

## 分工

1. NFA DFA
2. First Follow (almost done), 移入规约基于前两者，main 函数
3. 转成语法树，检验每一步的程序
4. 检验 conflict

每个人都要捏数据

zyy：2
cys：1
xmt: 3
lsy: 4

## 输入

语法输入：

每个语法规则应当对应一个符号到整数的映射，这个映射也应当被记录在文件中并上传到 github 上。（新建这个语法规则的人负责上传这个映射文件，命名就随意了）

映射约定: **_约定起始符对应的整数为 0_**

```
# N 表示符号数量（0-based） M 表示产生式数量（0-based）
# 要加入 start -> E 这个产生式, 且应该放在第一条（E只是举例子），start 记作 -1

N M
prod 1
prod 2
...
prod M

# 样例
# 产生式的第一个数表示产生式的右边的符号数量，第二个数表示产生式左边的符号，之后的数表示右边的符号序列

4 3
1 -1 1
3 1 2 3 0
1 1 2
```

终结符序列输入：

```
# N 表示终结符数量

N
Array of terminal characters

# 样例

5
1 1 2 4 1
```

### 输入文件命名

语法文件：`grammar_{name}.txt`

示例：`grammar_add.txt`

终结符序列文件：`{name}_instance{index}_input.txt`

示例：`add_instance1_input.txt`

zyy 给 xmt 的文件的命名：`{name}_instance{index}_output.txt`

示例：如果输入的终结符序列的名称是`add_instance1_input.txt`,那么输出应当是`add_instance1_output.txt`。

## 接口

cys：

check_feasible: 检查给定序列是否是一个合法状态（需要用到状态转移表）
pre_trans: 预处理出状态转移表（给 zyy 用的）
约定：编号为 0 的状态是初始状态。

```
bool check_feasible(struct array a);

void pre_trans();
```

---

lsy：实现如下函数

```
struct array get_first();

struct array get_follow();
```

---

zyy：

输出给 xmt 每一步的选择到文件（命名是捏的数据编号），

输出格式

每行一个整数 x，若为 -1 表示移入， 否则表示使用第 x 个产生式进行规约 (无需输出步骤次数)

cys 和 lsy 提供的都是函数，zyy 需要使用这些函数来搭建最终的两个 **接口** 函数。

由于 C 可能需要频繁使用 `malloc` 分配内存，因此在 `main` 函数中要注意内存的回收。

TODO：
- 函数1：接收语法参数（number_of_symb,number_of_prod,grammar）、用于存储生成的状态转移表的参数（state_num,state_info,trans）。在函数内调用getfirst、getfollow、pretrans来生成状态和状态转移表。
- 该函数内需要声明first和follow数组。由于这两个数组较大，可能会爆栈，所以要用malloc分配空间（注意回收）。类似的，在测试中如果需要声明trans、grammar等数组，建议使用全局变量以防止爆栈。
- 函数2：接收状态转移表，以及一个符号序列，进行移入规约。
- 最好提供相关的函数用于回收grammar、state_info这两个数组中用malloc分配的空间。
- 测试。

---

xmt:
拿到 zyy 给你的
生成语法树

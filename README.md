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

2、目前的实现中，输入的语法、生成的Follow集合、状态转移表以及所有结点的状态信息都是全局变量。其定义都在`cfg.h`中。
```
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
```

3、目前已经实现的函数的声明都在`cfg.h`中，可自行查阅，以下是几个可能会用到的函数：
```
// 用于检查给定的字符序列 a 是否是可行结构
// 该函数会用到状态转移表，因此在调用这个函数之前必须先预处理出状态转移表
bool check_feasible(struct array a);

// 预处理出状态转移表以及所有结点上的状态信息
// 该函数会使用follow集合，因此在调用这个函数之前必须先预处理出follow集合
void pre_trans();

// 读入语法规则
void read_in_grammar(char* filename);

// 打印编号为 prod_id 的产生式
void print_prod(int prod_id);

// 打印状态
void print_state(struct state s);

// 打印数组
void print_array(struct array);
```

4、生成的可执行文件请放在 `./shift_reduce/bin` 路径中

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

映射约定: ***约定起始符对应的整数为0***

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

为了避免重名，要加入新的样例的时候请在 `README.md` 里同步更新。

样例列表：

```
# 示例
default_grammar
3
# 3 表示该语法已经有三个终结符序列样例

add_multiply
4


```

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
**Note: first 和 follow 都是全局变量，都被定义在了 cfg.h 中**

```
struct array get_first();

struct array get_follow();
```

---

zyy：

输出给 xmt 每一步的选择到文件（命名是捏的数据编号），

输出格式

每行一个整数 x，若为 -1 表示移入， 否则表示使用第 x 个产生式进行规约 (无需输出步骤次数)




cys 和 lsy 提供的都是函数，zyy 需要使用这些函数来搭建最终的 `main` 函数。

由于 C 可能需要频繁使用 `malloc` 分配内存，因此在 `main` 函数中要注意内存的回收。

---

xmt:
拿到 zyy 给你的
生成语法树

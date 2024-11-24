# miniBison

为了保持一致，请使用 linux 平台。

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

```
# N 表示符号数量（0-based） M 表示产生式数量（0-based）

N M
prod 1
prod 2
...
prod M

# 样例
# 产生式的第一个数表示产生式的右边的符号数量，第二个数表示产生式左边的符号，之后的数表示右边的符号序列

4 2
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
1


```

终结符序列文件：`{name}_instance{index}_input.txt`

示例：`add_instance1_input.txt`

zyy 给 xmt 的文件的命名：`{name}_instance{index}_output.txt`

示例：如果输入的终结符序列的名称是`add_instance1_input.txt`,那么输出应当是`add_instance1_output.txt`。

## 接口

cys：
给你整数列（字符串），告诉我 T/F，表示可行/不可行

```
bool check(int number_of_symbol, int number_of_prod, prod *grammar, array *follow, int *a, int l)
```

---

lsy：

```
struct array{
    int l;  // 长度
    int *a;  // 元素
} follow[MAX_NUMBER_OF_SYMBOLS], first[MAX_NUMBER_OF_SYMBOLS];
```

---

zyy：

输出给 xmt 每一步的选择到文件（命名是捏的数据编号），首先是一个数字表示操作数量，然后是一行 整数 的序列，之后是若干行操作记录，每一行里是两个整数（用一个空格分割）；

输出移入规约过程到命令行。

```
struct{
int a; // 0 表示移入，1 表示规约
int b; // 你用的是第几个产生式，如果没用写-1
} xxx[MAX_NUMBER_OF_SYMBOLS];
```

cys 和 lsy 提供的都是函数，zyy 需要使用这些函数来搭建最终的 `main` 函数。

由于 C 可能需要频繁使用 `malloc` 分配内存，因此在 `main` 函数中要注意内存的回收。

---

xmt:
拿到 zyy 给你的
生成语法树

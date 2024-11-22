# miniBison

为了保持一致，请使用 linux 平台。

## 分工
1、NFA DFA
2、First Follow (almost done), 移入规约基于前两者，main函数
3、转成语法树，检验每一步的程序
4、检验conflict

每个人都要捏数据
zyy: 1~10
cys: 11~20
lsy: 21~30
xmt: 31~40

zyy：2
cys：1
xmt: 3
lsy: 4

## 接口
输入：symbol 整数， 字符要转成整数，后面都用整数做输入

接口：
cys：
    给你整数列（字符串），告诉我T/F，表示可行/不可行
    副产品：
    bool check(int *a, int l)

lsy：
    First Follow 
    struct xxx {
        int l;  // 长度
        int *a;  // 元素
    } xxx[MAX_NUMBER_OF_SYMBOLS];

zyy：
    输出给xmt每一步的选择，到文件（命名是捏的数据编号）
    一行里是（一个空格分割）{
        int a; // 0表示移入，1表示规约
        int b; // 你用的是第几个产生式，如果没用写-1
    } xxx[MAX_NUMBER_OF_SYMBOLS];

xmt:
    拿到zyy给你的
    生成语法树

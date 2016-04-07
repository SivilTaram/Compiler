# SymbolSet Design

在我的设计中，符号表中有着如下信息：
```c++
class SymbolSet{
public:
	// the pointer to the father of a procedure or a function.
	SymbolSet* father_table;
private:
	SymbolItem* proc;
	vector<SymbolItem*> local_list;
	map<string, SymbolItem*> symbol_map;
	//store the names of current level function or procedure.
	map<string, SymbolSet*> table_map;
	int level;
	int stack_size;
	int args_size;
	int display_size;
};
```

`father_table`是指向父符号表的指针，`proc`是该符号表对应的符号表项。

在我的设计中，编译某个分程序的声明部分时，会在其父符号表中插入一个该分程序对应的符号表项，并且生成一个新的符号表。

- *local_list* 是局部变量集合
- *symbol_map* 和 *table_map* 的设计会在后面详述。
- *Level* 是当前符号表的层次，0为起始值。
- *Stack_size* 是符号表在生成四元式时计算偏移时所用的，记录了该分程序所使用的所有栈空间大小。
- *args_size* 是该分程序所有的参数所占空间的大小
- *display_size* 是display区的大小。

在我的设计中，符号表设计了很久，符号表设计很久主要是考虑了几点问题：

1. 函数与过程互相的调用关系
2. 同名变量的不同作用域
3. 同名函数、同名过程的不同作用域
4. 函数与过程互相之间的可见与封装
5. 
最终采用的方案是经过跟同学探讨后得到的一种方案，这种方案设计上很像一颗B树，但是其中加入了一个很重要的东西：父指针，这种方案的设计如下图可以很清楚地可以看出。

![SymbolSetDesign.png](http://static.zybuluo.com/lq372899855/pcbpxktusurznusvgbdxh3cn/SymbolDesign.png)

在此图中可以看出，我设置了一个root_table来作为根符号表，是SymbolSet类的一个实例化对象。实际上在RootSymbolSet类的设计中，除了需要设置一个根符号表外，还需要一个符号表指针指向当前的符号表，即current_table。它永远指向当前的符号表，只有四元式BEGIN和END能够改变current_table指针，其中BEGIN是将current_table指向要编译的某个子函数/过程，而END则是将current_table设置为当前符号表的father_table，回退到上一级符号表。

插入符号表时需要判断是否重复定义，为了方便地实现这一点，我在SymbolSet符号表类中加入了
```c++
map<string, SymbolItem*> symbol_map
```
HashMap可以方便地寻找是否在当前符号表中已经有过定义。

在符号表中查找时，主要面临的问题在于跨域使用，并且最重要的问题在于要从本层符号表开始向上寻找，即一个变量或常量等如果不在本层中定义过，则要到其父符号表中去搜索，如果还没有就继续向上搜索，直到搜索到root_table，root_table的父符号表为NULL。

在函数和过程调用的作用域的查找上，和变量/常量的查找思路是一致的，首先在本层的符号表中进行检索，再向上检索。这样的做法和pascal中对不同函数的作用域和互相隐藏的原则是一致的：即一个函数/过程只能调用的有：

1. 在其前出现过的同层函数/过程（在本层符号表的父符号表中）
2. 本层符号表中记录的直接子函数/过程（在本层的符号表中）
3. 包括父亲在内的祖先函数/过程（祖先符号表中）

因为调用函数/过程的语句从四元式生成到汇编时需要计算每个变量是否分配空间，偏移量是多少，数组如何分配空间等问题，在查找到某个函数/过程时，光能查找到其所对应的符号表项是不够的，因为在我的符号表项中没有记录多余的信息。所以这时候必须借助函数/过程对应的符号表，所以有必要在符号表中设置其直接子符号表的集合，我设置的子符号表的集合即
```c++
map<string, SymbolSet*> table_map
```

还有一点需要额外提到的是，从四元式到目标代码的过程中，需要符号表中登记的各个变量的（包括分配空间的临时变量）偏移信息。而这一步计算我没有放在插入符号表的时候进行，而是放在了从四元式生成到目标代码前所做的预操作——计算偏移量。

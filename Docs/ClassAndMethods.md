# Class and Methods

## Overview

- **public class:** Error、Lexer、MiddleCode、MipsInstr、Parser、QuaterInstr、RootSymbolSet、SymbolSet、Token
- **private class:**Dataseg、Instruction、SymbolItem
- **Enum class:**MipsCode、TokenKind、TokenType、Symbol、Opcode

目录:
- [Error](#error)
- [Lexer](#lexer)
- [MiddleCode](#middlecode)
- [MipsInstr](#mipsInstr)
- [Parser](#parser)
- [QuaterInstr](#quaterInstr)
- [RootSymbolSet](#rootsymbolset)
- [SymbolSet](#symbolset)
- [Token](#token)


## Error

专门用于错误处理的类

```c++
int getErrorCount()
```

该函数用于获得编译错误的总数

```c++
bool IsSuccess()
```
在Error类中设置了类属性success，只要有错误信息被加入到错误信息向量中，就会自动将success属性置为false。该函数返回success的值，即表示编译是否成功，是否要打印错误信息等行为。

```c++
void print()
```
将错误信息向量中插入的错误信息全部打印出来

```c++
void errorMessage(int errortype, int line);
void errorMessage(int errortype, int line, string message1);
void errorMessage(int errortype, int line, string message1, string message2);
```
这三个是分别应用于不同场景的报错函数，其中无法明确指代错误发生或者错误类别本身定义比较模糊的使用第一个报错函数；只能准确指出错在哪个地方使用第二个错误函数；既能指出错在哪个地方，还能得知期望符号串的，使用第三个错误函数。

## Lexer

用于词法分析的类

```c++
void getch()
```
读取单个字符，字符存放于peek中
```c++
Token getsym()
```
根据语法将单个或多个字符拼成单词，单词存放于current_token中。如果遇到非法字符或一些其他的错误情况会报错。

```c++
void initialReserved()
```
初始化保留字数组

## MiddleCode

用于产生并存放所有四元式的中间代码类

```c++
QuaterInstr* gen(Opcode _op, SymbolItem* _des  , SymbolItem* _src1  , SymbolItem* _src2 )
```
这个方法用于产生一个四元式，并且自动将其插入到middle_codes(四元式向量)中去。

```c++
void printMiddleCode()
```
该方法用于打印所有的四元式，主要是用于前期中间代码生成时的对照调试

```c++
void addDataSeg(string _stringconst)
```
该方法用于产生一个aciiz的数据段字符串，并将其加入到data段中，最后统一放置于text段前。
```c++
vector<QuaterInstr*>* getMiddleCode()
```
该方法用于从middle_codes中获取生成的所有四元式。

## MipsInstr

将四元式转换为mips汇编指令的类

```c++
void add(MipsCode _op);
void add(MipsCode _op, string _des);
void add(MipsCode _op, string _des, string _src1);
void add(MipsCode _op, string _des, string _src1, string _src2);
```
以上这四个add的功能一致，都是生成一个正确的符合格式要求的mips汇编指令。由于各种指令需要的寄存器、label数等不同，故重载了四个函数。

```c++
void addData(string _name, string _conststring);
```
_name是生成的编号不重复的字符串标识符，_conststring是字符串本身的内容，这个方法的作用是将字符串_conststring以_name的名字存入dataseg中。

```c++
void Handle(QuaterInstr* middle);
```
这个方法的作用是根据不同的Opcode进行挑选，寻找对应的Handle进行处理。

```c++
void translate()
```
将四元式翻译为mips汇编指令

```c++
void HandleWrite(SymbolItem* des);
void HandleBegin(SymbolItem* des);
void HandleEnd(SymbolItem* des);
void HandleCalc(Opcode choose, SymbolItem* des, SymbolItem* src1, SymbolItem* src2);
void HandleNeg(SymbolItem* des,SymbolItem* src);
void HandleAssignAddr(SymbolItem* addr, SymbolItem* value);
void HandleArrayAddr(SymbolItem* addr, SymbolItem* base, SymbolItem* offset);
void HandleArrayAssign(SymbolItem* value,SymbolItem* addr);
void HandleAssign(SymbolItem* des,SymbolItem* src);
void HandleBranch(Opcode branch,SymbolItem* des,SymbolItem* src1,SymbolItem* src2);
void HandleCall(SymbolItem* caller,SymbolItem* callee);
void HandleInc(SymbolItem* src);
void HandleDec(SymbolItem* src);
void HandleSetLabel(SymbolItem* des);
void HandleJump(SymbolItem* des);
void HandlePush(SymbolItem* des);
void HandlePushVar(SymbolItem* des);
void HandleRead(SymbolItem* des);
```
以上这些所有的Handle函数都是针对不同的四元式进行各式各样的处理，将他们翻译成mips指令。

```c++
void loadReg(SymbolItem* item,const string _$i,bool _loadaddrvalue=false);
```
loadReg这个函数的主要作用是将item在运行栈中的值取到_$i中。最后的_loadaddrvalue主要用于控制是否为两次var嵌套使用的情况。

```c++
void storeMemory(const string _$i, SymbolItem* item);
```
storeMemory的作用和loadReg的作用相反，它将_$i的值存入item在运行栈中的实际地址中。
```c++
string genString();
```
产生编号不重复的字符串标识符用来标记要输出的字符串
```c++
void getRef(SymbolItem* item);
```
这个函数的作用是将item在运行栈中的绝对地址通过display区或者本层符号表算出后，存入$t0(已限制死)中，让之后在想load或者sw时可以直接使用$t0作为地址。

## Parser
Parser是递归下降子程序进行语法语义分析的类。

```c++
bool match(Symbol sym);
```
match是用于检测当前current_token的类型与sym是否匹配，匹配则返回true，否则返回false;
```c++
void next();
```
next()函数主要是调用了getsym()进行读取单词，同时进行了文件末尾的判断。
```c++
void parser();
```
parser是所有递归下降子程序的开头程序。在我的设计中，递归下降子程序开始只需要直接读即可，所有的递归下降子程序都会在末尾提前读一个准备好。
```c++
void translate();
```
translate首先要判断当前是否有错误产生，之后要调用计算偏移量的函数，将四元式翻译为目标代码。
```c++
void expect(Symbol sym,string message);
void expect(Symbol sym, string message1, string message2);
```
这两个函数的作用是判断sym是否和当前current_token的Symbol相同，不同的话就按照给出的参数报错，错误号是固定的。
```c++
SymbolItem* get(string _ident_name);
```
get函数用于根据当前标识符的名字逐层检索，最终得到其对应的符号表项或者是一个未定义的临时生成的符号表项指针。
```c++
void skip(symset fsys, int error_code);
void skip(symset fsys);
void test(symset s1);
```
以上的skip和test均是为跳读进行处理的。其中skip的意思是略过所有符号，直到遇到fsys中的某个符号为止，一般在入口处用。而test的意思是如果遇到了s1的某个符号为止会停下来，然后再读一个，一般用于出口控制。

```c++
string getErrorString(Token token);
```
这个函数用于更好地报错，可以将token根据不同的类型返回不同的string，更详细地展示了错误的信息。
```c++
void block(symset blockbegin);
```
分程序分析的递归下降子程序，对应文法中的
<分程序>        ::=   [<常量说明部分>][<变量说明部分>]{[<过程说明部分>]| [<函数说明部分>]}<复合语句>
<过程说明部分>  ::=  <过程首部><分程序>{; <过程首部><分程序>};
<函数说明部分>  ::=  <函数首部><分程序>{; <函数首部><分程序>};
```c++
void constDec();
```
<常量说明部分> :: = const<常量定义>{,<常量定义>};
```c++
void constDef();
```
<常量定义> ::= <标识符>＝ <常量>
<常量>     ::= [+| -] <无符号整数>|<字符>
```c++
void variableDec();
```
<变量说明部分>  :: = var <变量说明>; {<变量说明>; }
```c++
void variableDef();
```
<变量说明> ::= <标识符>{, <标识符>} : <类型>
```c++
void varType(queue<string>* var_name);
```
<类型> :: = <基本类型> | <数组类型>
```c++
void procDec();
```
<过程首部>   ::=   procedure<标识符>[<形式参数表>];
<形式参数表> ::= '('<形式参数段>{; <形式参数段>}')'
```c++
void parameterList();
```
<形式参数段> ::= [var]<标识符>{, <标识符>}: <基本类型>
```c++
void basicType(queue<string>* args_name,TokenKind kind);
```
<基本类型>  ::=  integer | char
```c++
void funcDec();
```
<函数首部>	 ::= function <标识符>[<形式参数表>]: <基本类型>;
<形式参数表>  :: = '('<形式参数段>{; <形式参数段>}')'
```c++
void statement();
```
<语句> ::= <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
```c++
void selector(queue<string>* var_name);
```
<数组类型> ::= array'['<无符号整数>']' of <基本类型>
```c++
void forStatement();
```
<for循环语句>:: = for <标识符>  : = <表达式> （downto | to） <表达式> do <语句> //步长为1
```c++
void whileStatement();
```
<当循环语句> :: = do<语句> while<条件>
```c++
void ifStatement();
```
<条件语句> :: = if<条件>then<语句> | if<条件>then<语句>else<语句>
```c++
void condition(SymbolItem* label,bool _ifsuccess);
```
<条件>       ::= <表达式><关系运算符><表达式>
<关系运算符> ::= <|<=|>|>= |=|<>
```c++
void compoundStatement();
```
<复合语句>       ::=  begin<语句>{; <语句>}end
```c++
void assignment(SymbolItem* ident,string func_name = "");
```
<赋值语句>      ::=  <标识符> := <表达式>| <函数标识符> := <表达式> | <标识符>'['<表达式>']':= <表达式>
```c++
void callPro(SymbolItem* proc,string proc_name);
```
<过程调用语句>  :: = <标识符>[<实在参数表>]
```c++
SymbolItem* callFunc(SymbolItem* func,string func_name);
```
<函数调用语句>  :: = <标识符>[<实在参数表>]
```c++
SymbolItem* realParameter(SymbolItem* func,string func_name);
```
<实在参数表>    :: = '(' <实在参数> {, <实在参数>}')'
<实在参数>      :: = <表达式>
```c++
SymbolItem* item();
```
<项>::= <因子>{<乘法运算符><因子>}
```c++
SymbolItem* expression();
```
<表达式> ::= [+|-]<项>{<加法运算符><项>}
```c++
SymbolItem* factor();
```
<因子> ::= <标识符>|<标识符>'['<表达式>']'|<无符号整数>| '('<表达式>')' | <函数调用语句>
```c++
void readStatement();
```
<读语句> ::=  read'('<标识符>{,<标识符>}')'
```c++
void writeStatement();
```
<写语句> :: = write'('<字符串>, <表达式>')' | write'('<字符串>')' | write'('<表达式>')'

## QuaterInstr
QauterInstr是单个四元式的类，主要是一些get操作。
```c++
SymbolItem* getDes();
```
获取四元式中des对应的符号表项指针
```c++
SymbolItem* getSrc1();
```
获取四元式中src1对应的符号表项指针
```c++
SymbolItem* getSrc2();
```
获取四元式中src2对应的符号表项指针
```c++
Opcode getOpType();
```
获取四元式的Opcode
```c++
void printQuater();
```
打印单个四元式
```c++
string printOpcode();
```
获取四元式Opcode对应的字符串

## RootSymbolSet
RootSymbolSet封装了符号表的一系列操作，并且包含了指向根符号表和当前符号表的两个指针。

```c++
SymbolItem* search(string _name); 
```
根据名字在符号表中查询是否寻在，如果存在返回对应的符号表项的指针，如果不存在返回NULL
```c++
SymbolItem* insert(string _name, TokenKind _kind, TokenType _type,int _value=0);
```
将新的信息登入到符号表中，如果符号表中已经存在，则返回NULL，否则返回新建的符号表项。
```c++
SymbolItem* genTemp(TokenKind _kind,TokenType _type,string _tag_string="");
```
产生一个临时变量，临时变量使用static int控制统一编号。
```c++
SymbolItem* genLabel();
```
产生一个不会重复的Label，并且返回构造的符号表项指针
```c++
vector<SymbolItem*> getArgList(string _name);
```
根据_name获得其对应的参数列表，_name一般是funciton或procedure.
```c++
void printSymbolSet();
```
打印符号表信息
```c++
SymbolSet* getRootSet();
```
获取根符号表，即名为”root_”的根符号表，类似于C里的main函数。
```c++
SymbolSet* getCurrentSet();
```
获取当前符号表，获取current_table指向的符号表
```c++
SymbolSet* serachTable(string _name);
```
根据_name在table_map中进行查询，并返回相应的直接子符号表
```c++
bool RootSymbolSet::goback();
```
函数的符号表退出，将当前符号表指针置为本符号表的父符号表指针值
```c++
void calcOffset();
```
计算每个变量在运行栈中的偏移量
```c++
string getCurrentName();
```
获取当前符号表对应的函数/过程的名字

## SymbolSet
SymbolSet是符号表类，统一管理符号表有关操作。

```c++
SymbolItem* getItem(string _name);
```
根据名字获取当前符号表中的符号表项指针，如果没找到返回NULL
```c++
bool enterItem(SymbolItem* _item);
```
将符号表项item插入到符号表中
```c++
SymbolItem* getProcItem();
```
获取当前符号表对应的函数/过程的符号表项
```c++
SymbolSet* getProcTable(string _procname);
```
根据_procname获取当前符号表的子符号表
```c++
string		getProcName();
```
获取当前符号表对应的函数/过程的名称
```c++
string getProcLabelName();
```
获取当前符号表对应的函数/过程的label名称，即重命名过的名称
```c++
SymbolSet* enterProc(SymbolItem* _proc);
```
根据_proc构造一个与之匹配的新符号表，并返回该符号表的指针
```c++
vector<SymbolItem*> getArgList();
```
获取当前符号表对应的所有参数的集合
```c++
void printData();
```
打印当前符号表
```c++
void calcOffset();
```
计算偏移量
    
## Token
Token是单词类，current_token是Token类，永远保存读到的最新的单词。

```c++
Symbol getType();
```
获取单词对应的Symbol
```c++
int getLineNo();
```
获取单词所在的行数，用于报错
```c++
string getName();
```
获取单词的名字
```c++
int getValue();
```
获取单词的值，如果不是有值的单词，默认值为0
```c++
string getTypeName();
```
获取单词的种类的名字

# QuaterInstr Design

在四元式的设计中，为了能够在四元式中获取更多的信息，进行更多的判断，我使用了符号表项指针作为四元式的基本单位。四元式类的属性定义如下：
```c++
class QuaterInstr {
	//op des src1 src2
private:
	Opcode op;
	SymbolItem* des;
	SymbolItem* src1;
	SymbolItem* src2;
};
```
其中`Opcode`是枚举类，其中包含了如下表的26种四元式操作码。

下表中的属性分为三种：`Label`、`SymbolSet*`、`SymbolItem*`。

`SymbolItem*`是指向符号表项的指针，而`SymbolSet*`是指向某个符号表的指针，`Label`是标明跳转点的标签


| 四元式操作码 | 语义 |公式表示 | 属性 |
| ---- | :----: | :---: | :---: |
|ADD	|两个数相加，将结果存放在目标操作数中	| des = src1 + src2	|des: SymbolItem\*、src1:SymbolItem\*、src2:SymbolItem\* |
|SUB	|两个数相减，将结果存放在目标操作数中	|des = src1 - src2	|des:SymbolItem\*、src1:SymbolItem\*、src2:SymbolItem\*|
|MUL	|两个数相乘，将结果存放在目标操作数中	|des = src1 * src2	|des:SymbolItem\*、src1:SymbolItem\*、src2:SymbolItem\*|
|DIV	|两个数相除，将结果的低32位放在目标操作数中	|des = src1 / src2	| des:SymbolItem\*、src1:SymbolItem\*、src2:SymbolItem\*|
|NEG	|源操作数值取反，赋给目标操作数|	des = -src1	|des:SymbolItem\*、src1:SymbolItem\*|
|BGR	|如果源操作数1大于源操作数2，则跳转向目标label预定位置	|if src1 > src2 then goto des |des: Label、src1:SymbolItem\*、src2:SymbolItem\*|
|BGE	|如果源操作数1大于等于源操作数2，则跳转向目标label预定位置	|if src1>= src2 then goto des	|	des: Label、src1:SymbolItem\*、src2:SymbolItem\*|
|BLS	|如果源操作数1小于等于源操作数2，则跳转向目标label预定位置	|if src1> src2 then goto des	|	des: Label、src1:SymbolItem\*、src2:SymbolItem\*|
|BLE	|如果源操作数1小于源操作数2，则跳转向目标label预定位置	|if src1<=src2 then goto des|	des: Label、src1:SymbolItem\*	、src2:SymbolItem\*|
|BEQ	|如果源操作数1等于源操作数2，则跳转向目标label预定位置	|if src1 == src2 then goto des	|	des: Label、src1:SymbolItem\*、src2:SymbolItem\*|
|BNE	|如果源操作数1不等于源操作数2，则跳转向目标label预定位置|	if src1 <> 0 then goto des	|	des:Label、src1:SymbolItem\*、src2:SymbolItem\*|
|ASS	|将源操作数1的值赋给目标操作数|des = src1	|	des:SymbolItem\*、src1:SymbolItem\*|
|ASSADD	|将源操作数1的值赋给目标操作数作为地址所指向的变量	|[des] = src1|	des:SymbolItem\*、src1:SymbolItem\*|
|ARRADD	|将源操作数1的地址取出，加上偏移量源操作数2的值，结果赋给目标操作数	|des = &src1 + src2	|des:SymbolItem\*、src1:SymbolItem\*、src2:SymbolItem\*|
|ARRASS	|将源操作数1作为地址所指向变量的值取出，赋给目标操作数|	des = [src1]|des:SymbolItem\*、src1:SymbolItem\*|
|JUMP	|无条件跳转到目标Label	|Goto des|des: Label|
|SETL	|打一个标签Label用于在汇编中的跳转	|set a lable:des	| des: Label|
|READ	|通过mips中的系统调用实现输入	|Read using syscall	| des:SymbolItem\*|
|WRITE	|通过mips中的系统调用和asciiz实现包括常量、变量、数组元素、字符串等的输出	|Print using syscall	| des:SymbolItem\* |
|BEGIN	|函数的运行部分开始	| Get the symbolset	| des:SymbolSet\* |
|END	|函数的运行部分编译结束，退栈操作	|Symbolset goback| des:SymbolSet\*|
|CALL	|调用des，需要调用者和被调用者对应的符号表的信息	|call src1 in des|	des:SymbolSet\*、src1:SymbolSet\*|
|PUSH	|普通参数压栈	|push des	| des:SymbolItem\*|
|PUSHVAR	|将带var的参数压入栈	|push des	|	des:SymbolItem\*|
|INC	|在for循环中循环变量每次自增	|des = des +1	|	des:SymbolItem\*|
|DEC	|在for循环中循环变量每次自减	|des = des - 1	|	des:SymbolItem\*|


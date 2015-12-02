是个四元式的生成思路大致如下：
1. 针对类似于函数调用的高级语法，在生成四元式的时候可以将参数压栈，也可以将参数按照vector<>的形式传给
函数调用所用到的四元式指令，这里为了保持统一性，我认为还是将参数压栈比较合适，压栈之后生成调用的四元式。
而关于POP之类的指令，因为这些指令本身也无法再进行优化，所以我认为生成的四元式是
CALL NULL,NULL,FUNC 比较合适，因为FUNC本身是指向符号表项的，
2. ADD TEMP,a,b 目前我觉得所有的加法都生成临时变量的做法好像好一些...
3. SUB TEMP,a,b
4. MULT TEMP,a,b
5. DIV TEMP,a,b
6. NEG dst,src1,0 对b取反得到a
对应的汇编指令可以是：
7. ASS dst,src1,0

这个赋值函数的意思是将dst的值变为src1。
在函数中，不能重定义函数名。这样就意味着，我们的函数名本身所代表的变量其实在一开始的时候就最好指定好。

实际上在对待数组的时候，应该就把它当作一个普通变量来对待即可，
对于类似如下语句
a[2] := 2;
实际上我们只需要把a[2]存到一个临时变量里去即可？是可以的吗？

8. ADDR dst,src1,0
这个赋值函数的意思是将dst这个地址所在的值变为src1
变换成汇编指令实际上是这样的构造：
//读取dst的值
lw $s0,offset($sp)
//读取src1的值
lw $s1,offset($sp)
//存回内存
sw $s1,0($s0)

9. 数组应该有一个专门算偏移到的地址的一个函数，如果是类似于如下的形式：
a[2] := 3
则应该先算出a[2]对应的地址，然后将使用ADDR指令将3赋给该地址
如果是类似于如下的形式：
x := a[2]
则应该先算出a[2]对应的变量的值，然后将该值赋给该变量。
对于var类型传参应该有一样的约定，当var变量在左侧时，我们要生成的应该是ADDR四元式。
如果var变量在右侧时，我们要生成的依然只是Assign四元式。

| 指令opcode | 目标变量  | 操作数1 | 操作数2 | 指令语义 |
| ---- | :----: | :---: | :---: | :---: |
| ADD | des | src1 | src2 | des = src1 + src2 | 
| SUB | des | src1 | src2 | des = src1 - src2 |
| MUL | des | src1 | src2 | des = src1 * src2(这里不考虑其会溢出的情况了就...)|
| DIV | des | src1 | src2 | des = src1 / src2 |
| NEG | des | src1 | - | des = - src1 |
| BGR | des | src1 | src2 | if src1 > src2 then goto des |
| BGE | des | src1 | src2 | if src1 >= src2 then goto des |
| BLS | des | src1 | src2 | if src1 < src2 then goto des|
| BLE | des | src1 | src2 | if src1 <= src2 then goto des |
| BEQ | des | src1 | src2 | if src1 == src2 then goto des |
| BNE | des | src1 | src2 | if src1 <> src2 then goto des |
| JUMP | des | - | - | goto des |
| LABEL | des | - | - | 打一个名字为des的label |
| ASS | des | src1 | - | des = src1 |
| ASSADD | des | src1 | - | [des] = src1 将src1的值赋给des所在的地址|
| READ | des | - | - | 读取东西到des，调用系统服务即可 |
| WRITE | des | - | - | 打印des，同样调用系统服务即可，但是注意string和char所使用的系统服务号是不同的 |
| CALL | des | src1 | - | 调用函数，返回值存到一个临时变量里。如果后面有赋值语句的话，将这个语句和调用函数的语句拆分成两个函数。|
| BEGIN | des | - | - | 函数开头 |
| END | des | - | - | 函数结尾 |
| PUSH | des | - | - | 将des变量压入栈 |
| RETURN | - | - | - | 无返回值返回 |
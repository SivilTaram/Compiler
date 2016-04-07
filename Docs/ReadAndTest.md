## 正确测试程序

也可以在这里获取测试程序的txt版本:[链接](https://github.com/SivilTaram/Compiler/tree/master/Tests/ComplexSamples)

### 1、全覆盖正确测试程序
```pascal
const test1j = ' ', test5c5='s',const77=233;
var i,index,test22,test33,test44,test55,test77,test88,test99,test1010:integer;
hehe:char;
test66:array[10] of integer;
test5c:array[10] of char;
procedure Test1(j:char);
begin
	write("Test1 begin");
	write(j);
	write("success");
	write("\n");
end;
procedure Test2(var j:integer);
	procedure Test2T2(var j:integer);
	begin
		j := 5;
	end;
begin
	write("Test2 begin");
	j := 3;
	Test2T2(j);
end;
function Test3(i:integer):integer;
begin
	write("Test3 begin");
	Test3 := i;
end;
function Test4(i:integer):integer;
var j:integer;
begin
	if i=0 then
		begin
			write("Test4 begin");
			Test4 := 0
		end
	else
	begin
		j := i + Test4(i-1);
		Test4 := j;
	end
end;
function Test5:char;
const s = 's';
begin
	write("Test5 begin");
	Test5 := s;
end;
procedure Test6(var i:integer);
begin
	write("Test6 begin");
	do
		i:=i+2
	while i<5;
end;
function Test7(i,j,k,h:integer;m:char):integer;
var temp:integer;
begin
	write("Test7 begin");
	temp := (m * h + k)/j - i +27;
	j := +temp;
	j := -j;
	Test7 := j;
end;
function Test8(j:integer):integer;
var i,ret:integer;
begin
	write("Test8 begin");
	ret := 0;
	for j:=(23-20) downto 2 do
		for i:= 1 to index do
			ret := ret+i*j;
	Test8 := -ret;
end;
function Test9(para:integer):integer;
begin
	if para = 3 then
		write("Test9 begin");
	Test9 := para*3;
end;
procedure Test10;
	procedure Test1010;
	var test1010:integer;
	begin
		test1010:=1010;
	end;
begin
write("Test10 begin");
test1010:=test1010 * 6;
Test1010;
end;
procedure Test11;
	procedure Test11;
	begin
		write("Test11 begin");
	end;
begin
Test11;
end;
begin
	test22 := 3;
	for i:= 1 to 110/10 do
	begin
		read(index);
		if index = 1 then
			Test1(test1j)
		else if index = 2 then
		begin
			Test2(test22);
			if test22 = 5 then
				begin
				write(" success");
				write("\n")
				end
			else
				write("fail");
		end
		else if index = 3 then
		begin
			test33 := Test3(55);
			if test33 = 55 then
				begin
				write(" success");
				write("\n")
				end
		end
		else if index = 4 then
		begin
			test44 := Test4(5);
			if test44 = 15 then
				begin
				write(" success");
				write("\n")
				end
		end
		else if index = 5 then
		begin
			test5c[5] := Test5;
			if test5c[5] = test5c5 then
					write(" success\n");
		end
		else if index = 6 then
		begin
			Test6(test66[3]);
			test22 := test66[3];
			if test22 = 6 then
					write(" success\n");
		end
		else if index = 7 then
		begin
			test77 := Test7(27,5,20,3,test5c5);
			if test77 = -73 then
				write(" success\n");
		end
		else if index = 8 then
		begin
			test88 := Test8(233);
			if test88 = -180 then
				write(" success\n");
		end
		else if index = 9 then
		begin
			test99 := Test9(Test9(Test9(3)));
			if test99 = 81 then
				write(" success\n");
		end
		else if index = 10 then
		begin
			test1010 := 5;
			Test10;
			if test1010 = 30 then
				write(" success\n");
		end
		else if index = 11 then
		begin
			Test11;
		end
	end;
	read(hehe);
	test5c[6] := hehe;
	write("You input",test5c[6]);
	write("Finished!\n");
end.
```
### 2、全覆盖正确测试程序2
```pascal
const constchar='c';
var vara : integer;
arr:array[100] of integer;
procedure testmulti(var c :integer);
	begin
		c := c+6;
	end;
procedure testvar(var b :integer); 
	begin
		b := b+1;
		write("|",b);
		testmulti(b);
	end;
procedure testPara(c,j:integer;a:char);
var b:integer;
	begin
		b := c;
		c := -c;
		if b = -c then
			write("|success neg");
		write("|a:",a);
		j :=12 / c;
		write("|j:",j);
		c := a + (vara * j);
		write("|c:",c);
	end;
function testFuncReturn:integer;
	begin
		testFuncReturn := 999;
	end;
procedure f(b:integer);
	begin
		if b = 1 then
			write("|",b)
		else
		begin
			write("|",b);
			f(b-1);
		end
	end;
procedure testFor;
	begin
		for vara:=1 to 10 do
			write("|",vara)
	end;
procedure testDo;
	begin
	do
	begin
		write("|",vara);
		vara := vara -1 ;
	end
	while vara >= 1;
	end;		
begin
	vara := 0;
	testvar(vara);
	write("|vara:",vara);
	vara := testFuncReturn;
	if vara = 999 then
		write("|success fun return")
	else
		write("|fail fun return");
	testPara(4,6,constchar);
	f(8);
	arr[1] := 6;
	arr[2] := arr[1] * arr[1];
	write("|arr[1]:",arr[1]);
	write("|arr[2]:",arr[2]);
	testFor;
	testDo;
	read(vara);
	write("|readint vara:",vara);
end.
```

### 3、条件判断
```pascal
procedure testCondition(x,y:integer);
	begin
		write(" testCondition begin:");
		if x=y then write(" x=y:true") else write(" x=y:false");
		if x>y then write(" x>y:true") else write(" x>y:false");
		if x<y then write(" x<y:true") else write(" x<y:false");
		if x>=y then write(" x>=y:true") else write(" x>=y:false");
		if x<=y then write(" x<=y:true") else write(" x<=y:false");
		if x<>y then write(" x<>y:true") else write(" x<>y:false");
		write(" test end                \r\n")
	end;
begin
testCondition(2,2);
testCondition(1,3);
testCondition(3,1);
end.
```
### 4、类型转换
```pascal
const sc='c';
var c,a,j,vara:integer;
i:char;
cc:array[100] of char;
cc2:array[100] of integer;
function A:char;
begin
A := sc;
end;
begin
	i := A;
	write(i);
	c := i;
	write(c);
	cc[2] := i;
	write(cc[2]);
	cc2[2] := cc[2];
	write(cc2[2]);
	a := i+j-c;
	write(a);
end.
```

### 5、正确程序之测试函数与变量的作用域
```pascal
var B:integer;
function A:integer;
	var B:char;
	function C:integer;
		begin
		write("this is the second C\n");
		C := 2;
		end;
	begin
		write("this is the second A\n");
		A := 97;
	end;
function C:integer;
	var B:integer;
	function A:integer;
		begin
			write("this is the second A\n");
			A := 99;
		end;
	begin
	if A = 99 then
		write("correct,call inner A:")
	else
		write("not correct,call the wrong A");
	B := 5;
	write("the inner B:",B);
	write("\n");
	C := -1;
	end;
procedure D;
	var D:integer;
	begin
	write("this is the procedure D\n");
	if C = -1 then
		begin
		D := C;
		write("correct,this is the var D:",D);
		end
	else
		write("not correct,call the wrong C");
	write("\n");
	D := A;
	end;
begin
write("the init of outter B:",B);
write("\n");
B := C;
write("the last of outter B:",B);
write("\n");
D;
end.
```

正确程序的输出结果依次是：

1、顺次输入1到11，再输入一个a，有如下结果
```
**** user input : 1
Test1 begin success
**** user input : 2
Test2 begin success
**** user input : 3
Test3 begin success
**** user input : 4
Test4 begin success
**** user input : 5
Test5 begin success
**** user input : 6
Test6 begin success
**** user input : 7
Test7 begin success
**** user input : 8
Test8 begin success
**** user input : 9
Test9 begin success
**** user input : 10
Test10 begin success
**** user input : 11
Test11 begin**** user input : a
You inputaFinished!
```
2、输入1个数字12，直接运行即可
```
|1|vara:7|success fun return|success neg|a:c|j:-3|c:-2898|8|7|6|5|4|3|2|1|arr[1]:6|arr[2]:36|1|2|3|4|5|6|7|8|9|10|11|10|9|8|7|6|5|4|3|2|1**** user input : 12
|readint vara:12
```

3、无输入，直接运行即可
```
testCondition begin: x=y:true x>y:false x<y:false x>=y:true x<=y:true x<>y:false test end                
 testCondition begin: x=y:false x>y:false x<y:true x>=y:false x<=y:true x<>y:true test end                
 testCondition begin: x=y:false x>y:true x<y:false x>=y:true x<=y:false x<>y:true test end  
```

4、无输入，直接运行即可
```
c99c990
```
5、无输入，直接运行即可
```
the init of outter B:0
this is the second A
correct,call inner A:the inner B:5
the last of outter B:-1
this is the procedure D
this is the second A
correct,call inner A:the inner B:5
this is the second A
correct,call inner A:the inner B:5
correct,this is the var D:-1
this is the second A
```
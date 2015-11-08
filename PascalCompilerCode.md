```pascal
program pl0(input,output,fin) ;  { version 1.0 oct.1989 }
{ PL/0 compiler with code generation }
const norw = 13;          { no. of reserved words }{保留字的数量}
      txmax = 100;        { length of identifier table }{标识符表的长度}
      nmax = 14;          { max. no. of digits in numbers }{数字最大长度}
      al = 10;            { length of identifiers }{*标识符的数量*}
      amax = 2047;        { maximum address }{* 地址最大范围*}
      levmax = 3;         { maximum depth of block nesting }[FIXME]{* 最大嵌套层次？ *}
      cxmax = 200;        { size of code array }
{* 查询资料得知，()是子界类型，类似于java和C#里的枚举类，以下为查找的一个示例：
type
months = (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec);  *}

type symbol =
     ( nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,lss,
       leq,gtr,geq,lparen,rparen,comma,semicolon,period,becomes,
       beginsym,endsym,ifsym,thensym,whilesym,dosym,callsym,constsym,
       varsym,procsym,readsym,writesym );{* 单词集，即定义的单词*}
     alfa = packed array[1..al] of char;{*查阅资料得知，packed是为了压缩数组，但是很奇怪，没有给相应的标识符集合，怎么判断是否是标识符呢？*}
     objecttyp = (constant,variable,prosedure);
     symset = set of symbol;
     fct = ( lit,opr,lod,sto,cal,int,jmp,jpc,red,wrt ); { functions }{* 抽象机定义的10条汇编指令*}
     instruction = packed record {*record是记录类型，类似于C里结构体的感觉*}
                     f : fct;            { function code }{* 这个记录类型记录了目标代码集，层次域以及地址空间范围*}
                     l : 0..levmax;      { level }
                     a : 0..amax;        { displacement address }
                   end;
                  {   lit 0, a : load constant a
                      opr 0, a : execute operation a
                      lod l, a : load variable l,a
                      sto l, a : store variable l,a
                      cal l, a : call procedure a at level l
                      int 0, a : increment t-register by a
                      jmp 0, a : jump to a
                      jpc 0, a : jump conditional to a
                      red l, a : read variable l,a
                      wrt 0, 0 : write stack-top
                  }

var   ch : char;      { last character read }{*读的最后一个字符*}
      sym: symbol;    { last symbol read }{*读到的最后一个单词*}
      id : alfa;      { last identifier read }{*读到的最后一个标识符*}
      num: integer;   { last number read }{*读到的最后一个数字，应该是用来参与计算的*}
      cc : integer;   { character count }{*字符的数量*}
      ll : integer;   { line length }{*读入的一行的长度*}
      kk,err: integer;{* kk 是什么？根据下面的推测，err应该是错误的个数*}
      cx : integer;   { code allocation index }{*代码分配序号，代码地址？*}
      line: array[1..81] of char;
      a : alfa;
      code : array[0..cxmax] of instruction;
      word : array[1..norw] of alfa;
      wsym : array[1..norw] of symbol;
      ssym : array[char] of symbol;
      mnemonic : array[fct] of
                   packed array[1..5] of char;
      declbegsys, statbegsys, facbegsys : symset;
      table : array[0..txmax] of{* 符号表的设计，其中包含了符号的名字：alfa，符号的类型 *}
                record
                  name : alfa;{*单词的名字*}
                  case kind: objecttyp of{*kind 是个 objecttype类型，有constant常量，variable变量，prosedure过程*}
                    constant : (val:integer );{*如果是常量，则在table里记录一个名为val的变量*}
                    variable,prosedure: (level,adr: integer ){*如果是变量或者过程的话，符号表中需要记录其层次与地址*}
                end;
      fin : text;     { source program file }{* 源代码 *}
      sfile: string;  { source program file name }{* 源文件的名字*}

procedure error( n : integer );  { P384 }
  begin
    writeln( '****', ' ':cc-1, '^', n:2 );{* 在第几个符号处犯了错误码为n的错误？*}
    err := err+1{*记录error被调用的次数，即记录错误的个数*}
  end; { error }
 
{*该过程为词法分析，读取一个单词*}
procedure getsym; 
  var i,j,k : integer;
  {* 取字符过程，实际上先读入缓冲区，然后再一个一个字符地读取*}
  procedure getch; 
    begin
      if cc = ll  { get character to end of line }{*如果当前已经读到最后一个字符结束，就开始读下一行*}
      then begin { read next line }
             if eof(fin){*如果读到文件末尾，则打印结束信息，关闭文件并退出*}
             then begin
                    writeln('program incomplete');
                    close(fin);
                    exit;
                  end;
             ll := 0;{*开始读取本行内容*}
             cc := 0;
             write(cx:4,' ');  { print code address }{*以4个字符宽度来输出cx，cx是代码地址*}
             while not eoln(fin) do{*一直到读到换行符为止*}
               begin
                 ll := ll+1; {*行的长度+1*}
                 read(fin,ch);{*把字符读入*}
                 write(ch);{*字符串打印*}
                 line[ll] := ch{*将本行的缓冲区填满*}
               end;{*至此将一行已经读入到内存中，存储在line数组中*}
             writeln;{*输出源程序*}
             readln(fin);{*跳过一个换行符*}
             ll := ll+1;{*最后一个字符，填入字符串终结符*}
             line[ll] := ' ' { process end-line }
           end;
      {*如果不是到了行末尾，就正常读字符*}
      cc := cc+1;{*开始读缓冲区中的字符*}
      ch := line[cc]{*ch中保存最后一个读到的字符，这个程序居然是以1为下标开始序号*}
    end; { getch }
  begin { procedure getsym;  P384 }{*getsym函数开始*}
    while ch = ' ' do{*如果读到的字符为空白符就一直向后读*}
      getch;
    if ch in ['a'..'z']{*如果读到的字符是a-z之间，要么是标识符，要么是保留字(起始处字符必须是小写字母)*}
    then begin  { identifier of reserved word }
           k := 0;{*开始读字符*}
           repeat
             if k < al{*如果k小于标识符的最大长度*}
             then begin
                    k := k+1;
                    a[k] := ch{*a是个临时数组，用来存放读到的字符串*}
                  end;
             getch
           until not( ch in ['a'..'z','0'..'9']);{*直到不是a-z且不是0-9*}
           if k >= kk        { kk : last identifier length }{*kk表示所有单词中的最长字符长度，长度不足的用空格补足*}
           then kk := k
           else repeat {*否则要进行对齐处理，但是不太明白，为什么是kk-而不是k+，这样kk下一次的时候不就不一样了吗？*}
                  a[kk] := ' ';
                  kk := kk-1
                until kk = k;
           id := a;{*id存放字符串*}
           i := 1;
           j := norw;   { binary search reserved word table }{*norw是保留字的个数，使用二分法来查找保留字*}
           repeat{二分法确认标识符是否为保留字}
             k := (i+j) div 2;
             if id <= word[k]
             then j := k-1;
             if id >= word[k]
             then i := k+1
           until i > j;
           if i-1 > j{*为什么这里i-1>j时就是保留字符？*}
           then sym := wsym[k]{}
           else sym := ident{*没有找到，单词记录为标识符*}
         end
    else if ch in ['0'..'9']{*数字*}
         then begin  { number }
                k := 0;{*单词长度重置为0*}
                num := 0;
                sym := number;{*类型记录为number*}
                repeat{*循环将字符串的值转变为数字*}
                  num := 10*num+(ord(ch)-ord('0'));
                  k := k+1;
                  getch
                until not( ch in ['0'..'9']);
                if k > nmax{*如果超过数字单词规定长度，则报错*}
                then error(30)
              end
         else if ch = ':'{*如果是冒号*}
              then begin
                     getch;{*再读入一个单词*}
                     if ch = '='{*赋值符号读入*}
                     then begin
                            sym := becomes;{*赋值符号语义单词*}
                            getch
                          end
                     else sym := nul{*否则冒号无效*}
                   end
              else if ch = '<'{*如果是小于号*}
                   then begin
                          getch;
                          if ch = '='{*如果是<=号*}
                          then begin
                                 sym := leq;{*leq表示小于等于比较，less equal*}
                                 getch
                               end
                          else if ch = '>'{*如果是<>号*}
                               then begin
                                      sym := neq;{*neq表示不等于，not equal*}
                                      getch
                                    end
                               else sym := lss{*否则的话就是一个简单的小于号，less*}
                        end
                   else if ch = '>'{*如果是大于号*}
                        then begin
                               getch;
                               if ch = '='{*>=符号标识*}
                               then begin
                                      sym := geq;{*geq 即 greater equal*}
                                      getch
                                    end
                               else sym := gtr{*否则就是普通的大于号，即greater*}
                             end
                        else begin
                               sym := ssym[ch];{*操作符集合，字符作为下标进行索引*}
                               getch
                             end
  end; { getsym }
  
{*目标代码生成*}
procedure gen( x: fct; y,z : integer ); {*fct是目标代码指令集*}
  begin
    if cx > cxmax{*如果目标代码生成位置超出规定的地址边界*}
    then begin
           writeln('program too long');{*如果程序过长...才200行就过长了...*}
           close(fin);
           exit
         end;
    with code[cx] do{*将符合条件的代码存入code*}
      begin
        f := x;{*指令*}
        l := y;{*层级*}
        a := z{*操作编码*}
      end;
    cx := cx+1{*存储下一句符合条件的代码*}
  end; { gen }

{*用于分析错误并处理*}
procedure test( s1,s2 :symset; n: integer );  { P386 }{*S1为合法头符号集合，S2为停止符号集合，n为错误编码*}
  begin
    if not ( sym in s1 ){*如果sym不在s1里，就报错*}
    then begin
           error(n);{*错误类型为n*}
           s1 := s1+s2;{*报错后s2加入头符号集合中，检测后面是否出现错误*}
           while not( sym in s1) do{*如果后面再遇到非法符号全部略过*}
             getsym
           end
  end; { test }
  
{*分程序分析处理程序*}
procedure block( lev,tx : integer; fsys : symset ); { P386 }
  var  dx : integer;  { data allocation index }{}
       tx0: integer;  { initial table index }
       cx0: integer;  { initial code index }

  procedure enter( k : objecttyp ); { P386 }
    begin  { enter object into table }
      tx := tx+1;
      with table[tx] do
        begin
          name := id;
          kind := k;
          case k of
            constant : begin
                         if num > amax
                         then begin
                                error(30);
                                num := 0
                              end;
                         val := num
                       end;
            variable : begin
                         level := lev;
                         adr := dx;
                         dx := dx+1
                       end;
            prosedure: level := lev;
          end
        end
    end; { enter }

  function position ( id : alfa ): integer; { P386 }
    var i : integer;
    begin
      table[0].name := id;
      i := tx;
      while table[i].name <> id do
        i := i-1;
      position := i
    end;  { position }
    
  procedure constdeclaration; { P386 }
    begin
      if sym = ident
      then begin
             getsym;
             if sym in [eql,becomes]
             then begin
                    if sym = becomes
                    then error(1);
                    getsym;
                    if sym = number
                    then begin
                           enter(constant);
                           getsym
                         end
                    else error(2)
                  end
             else error(3)
           end
      else error(4)
    end; { constdeclaration }

{*变量声明处理程序*} 
  procedure vardeclaration; { P387 }
    begin
      if sym = ident
      then begin
             enter(variable);
             getsym
           end
      else error(4)
    end; { vardeclaration }
    
  procedure listcode;  { P387 }
    var i : integer;
    begin

      for i := cx0 to cx-1 do
        with code[i] do
          writeln( i:4, mnemonic[f]:7,l:3, a:5)
    end; { listcode }
    
  procedure statement( fsys : symset ); { P387 }
    var i,cx1,cx2: integer;
    procedure expression( fsys: symset); {P387 }
      var addop : symbol;
      procedure term( fsys : symset);  { P387 }
        var mulop: symbol ;
        procedure factor( fsys : symset ); { P387 }
          var i : integer;
          begin
            test( facbegsys, fsys, 24 );
            while sym in facbegsys do
              begin
                if sym = ident
                then begin
                       i := position(id);
                       if i= 0
                       then error(11)
                       else
                         with table[i] do
                           case kind of
                             constant : gen(lit,0,val);
                             variable : gen(lod,lev-level,adr);
                             prosedure: error(21)
                           end;
                       getsym
                     end
                else if sym = number
                     then begin
                            if num > amax
                            then begin
                                   error(30);
                                   num := 0
                                 end;
                            gen(lit,0,num);
                            getsym
                          end
                     else if sym = lparen
                          then begin
                                 getsym;
                                 expression([rparen]+fsys);
                                 if sym = rparen
                                 then getsym
                                 else error(22)
                               end;
                test(fsys,[lparen],23)
              end
          end; { factor }
        begin { procedure term( fsys : symset);   P388
                var mulop: symbol ;    }
          factor( fsys+[times,slash]);
          while sym in [times,slash] do
            begin
              mulop := sym;
              getsym;
              factor( fsys+[times,slash] );
              if mulop = times
              then gen( opr,0,4 )
              else gen( opr,0,5)
            end
        end; { term }
      begin { procedure expression( fsys: symset);  P388
              var addop : symbol; }
        if sym in [plus, minus]
        then begin
               addop := sym;
               getsym;
               term( fsys+[plus,minus]);
               if addop = minus
               then gen(opr,0,1)
             end
        else term( fsys+[plus,minus]);
        while sym in [plus,minus] do
          begin
            addop := sym;
            getsym;
            term( fsys+[plus,minus] );
            if addop = plus
            then gen( opr,0,2)
            else gen( opr,0,3)
          end
      end; { expression }
      
    procedure condition( fsys : symset ); { P388 }
      var relop : symbol;
      begin
        if sym = oddsym
        then begin
               getsym;
               expression(fsys);
               gen(opr,0,6)
             end
        else begin
               expression( [eql,neq,lss,gtr,leq,geq]+fsys);
               if not( sym in [eql,neq,lss,leq,gtr,geq])
               then error(20)
               else begin
                      relop := sym;
                      getsym;
                      expression(fsys);
                      case relop of
                        eql : gen(opr,0,8);
                        neq : gen(opr,0,9);
                        lss : gen(opr,0,10);
                        geq : gen(opr,0,11);
                        gtr : gen(opr,0,12);
                        leq : gen(opr,0,13);
                      end
                    end
             end
      end; { condition }
    begin { procedure statement( fsys : symset );  P389
            var i,cx1,cx2: integer; }
      if sym = ident
      then begin
             i := position(id);
             if i= 0
             then error(11)
             else if table[i].kind <> variable
                  then begin { giving value to non-variation }
                         error(12);
                         i := 0
                       end;
             getsym;
             if sym = becomes
             then getsym
             else error(13);
             expression(fsys);
             if i <> 0
             then
               with table[i] do
                 gen(sto,lev-level,adr)
           end
      else if sym = callsym
      then begin
             getsym;
             if sym <> ident
             then error(14)
             else begin
                    i := position(id);
                    if i = 0
                    then error(11)
                    else
                      with table[i] do
                        if kind = prosedure
                        then gen(cal,lev-level,adr)
                        else error(15);
                    getsym
                  end
           end
      else if sym = ifsym
           then begin
                  getsym;
                  condition([thensym,dosym]+fsys);
                  if sym = thensym
                  then getsym
                  else error(16);
                  cx1 := cx;
                  gen(jpc,0,0);
                  statement(fsys);
                  code[cx1].a := cx
                end
           else if sym = beginsym
                then begin
                       getsym;
                       statement([semicolon,endsym]+fsys);
                       while sym in ([semicolon]+statbegsys) do
                         begin
                           if sym = semicolon
                           then getsym
                           else error(10);
                           statement([semicolon,endsym]+fsys)
                         end;
                       if sym = endsym
                       then getsym
                       else error(17)
                     end
                else if sym = whilesym
                     then begin
                            cx1 := cx;
                            getsym;
                            condition([dosym]+fsys);
                            cx2 := cx;
                            gen(jpc,0,0);
                            if sym = dosym
                            then getsym
                            else error(18);
                            statement(fsys);
                            gen(jmp,0,cx1);
                            code[cx2].a := cx
                          end
                     else if sym = readsym
                          then begin
                                 getsym;
                                 if sym = lparen
                                 then
                                   repeat
                                     getsym;
                                     if sym = ident
                                     then begin
                                            i := position(id);
                                            if i = 0
                                            then error(11)
                                            else if table[i].kind <> variable
                                                 then begin
                                                        error(12);
                                                        i := 0
                                                      end
                                                 else with table[i] do
                                                        gen(red,lev-level,adr)
                                          end
                                     else error(4);
                                     getsym;
                                   until sym <> comma
                                 else error(40);
                                 if sym <> rparen
                                 then error(22);
                                 getsym
                               end
                          else if sym = writesym
                               then begin
                                      getsym;
                                      if sym = lparen
                                      then begin
                                             repeat
                                               getsym;
                                               expression([rparen,comma]+fsys);
                                               gen(wrt,0,0);
                                             until sym <> comma;
                                             if sym <> rparen
                                             then error(22);
                                             getsym
                                           end
                                      else error(40)
                                    end;
      test(fsys,[],19)
    end; { statement }
  begin  {   procedure block( lev,tx : integer; fsys : symset );    P390
                var  dx : integer;  /* data allocation index */
                     tx0: integer;  /*initial table index */
                     cx0: integer;  /* initial code index */              }
    dx := 3;
    tx0 := tx;
    table[tx].adr := cx;
    gen(jmp,0,0); { jump from declaration part to statement part }
    if lev > levmax
    then error(32);

    repeat
      if sym = constsym
      then begin
             getsym;
             repeat
               constdeclaration;
               while sym = comma do
                 begin
                   getsym;
                   constdeclaration
                 end;
               if sym = semicolon
               then getsym
               else error(5)
             until sym <> ident
           end;
      if sym = varsym
      then begin
             getsym;
             repeat
               vardeclaration;
               while sym = comma do
                 begin
                   getsym;
                   vardeclaration
                 end;
               if sym = semicolon
               then getsym
               else error(5)
             until sym <> ident;
           end;
      while sym = procsym do
        begin
          getsym;
          if sym = ident
          then begin
                 enter(prosedure);
                 getsym
               end
          else error(4);
          if sym = semicolon
          then getsym
          else error(5);
          block(lev+1,tx,[semicolon]+fsys);
          if sym = semicolon
          then begin
                 getsym;
                 test( statbegsys+[ident,procsym],fsys,6)
               end
          else error(5)
        end;
      test( statbegsys+[ident],declbegsys,7)
    until not ( sym in declbegsys );
    code[table[tx0].adr].a := cx;  { back enter statement code's start adr. }
    with table[tx0] do
      begin
        adr := cx; { code's start address }
      end;
    cx0 := cx;
    gen(int,0,dx); { topstack point to operation area }
    statement( [semicolon,endsym]+fsys);
    gen(opr,0,0); { return }
    test( fsys, [],8 );
    listcode;
  end { block };
  
procedure interpret;  { P391 }
  const stacksize = 500;
  var p,b,t: integer; { program-,base-,topstack-register }
      i : instruction;{ instruction register }
      s : array[1..stacksize] of integer; { data store }
  function base( l : integer ): integer;
    var b1 : integer;
    begin { find base l levels down }
      b1 := b;
      while l > 0 do
        begin
          b1 := s[b1];
          l := l-1
        end;
      base := b1
    end; { base }
  begin  { P392 }
    writeln( 'START PL/0' );
    t := 0;
    b := 1;
    p := 0;
    s[1] := 0;
    s[2] := 0;
    s[3] := 0;
    repeat
      i := code[p];
      p := p+1;
      with i do
        case f of
          lit : begin
                  t := t+1;
                  s[t]:= a;
                end;
          opr : case a of { operator }
                  0 : begin { return }
                        t := b-1;
                        p := s[t+3];
                        b := s[t+2];
                      end;
                  1 : s[t] := -s[t];
                  2 : begin
                        t := t-1;
                        s[t] := s[t]+s[t+1]
                      end;
                  3 : begin
                        t := t-1;
                        s[t] := s[t]-s[t+1]
                      end;
                  4 : begin
                        t := t-1;
                        s[t] := s[t]*s[t+1]
                      end;
                  5 : begin
                        t := t-1;
                        s[t] := s[t]div s[t+1]
                      end;
                  6 : s[t] := ord(odd(s[t]));
                  8 : begin
                        t := t-1;
                        s[t] := ord(s[t]=s[t+1])
                      end;
                  9 : begin
                        t := t-1;
                        s[t] := ord(s[t]<>s[t+1])
                      end;
                  10: begin
                        t := t-1;
                        s[t] := ord(s[t]< s[t+1])
                      end;
                  11: begin
                        t := t-1;
                        s[t] := ord(s[t] >= s[t+1])
                      end;
                  12: begin
                        t := t-1;
                        s[t] := ord(s[t] > s[t+1])
                      end;
                  13: begin
                        t := t-1;
                        s[t] := ord(s[t] <= s[t+1])
                      end;
                end;
          lod : begin
                  t := t+1;
                  s[t] := s[base(l)+a]
                end;
          sto : begin
                  s[base(l)+a] := s[t];  { writeln(s[t]); }
                  t := t-1
                end;
          cal : begin  { generate new block mark }
                  s[t+1] := base(l);
                  s[t+2] := b;
                  s[t+3] := p;
                  b := t+1;
                  p := a;
                end;
          int : t := t+a;
          jmp : p := a;
          jpc : begin
                  if s[t] = 0
                  then p := a;
                  t := t-1;
                end;
          red : begin
                  writeln('??:');
                  readln(s[base(l)+a]);
                end;
          wrt : begin
                  writeln(s[t]);
                  t := t+1
                end
        end { with,case }
    until p = 0;
    writeln('END PL/0');
  end; { interpret }

{* main函数 *}
begin { main }
  writeln('please input source program file name : ');
  {*通过源文件的名字将要编译的文件读入*}
  readln(sfile);
  {*fin是源代码文件，assign的作用是将文件名字符串sfile赋给文件变量fin，程序对文件变量fin的操作代替对文件sfile的操作。*}
  assign(fin,sfile);
  {*以只读的方式打开文件fin*}
  reset(fin);
  for ch := 'A' to ';' do
    ssym[ch] := nul;
  word[1] := 'begin        '; word[2] := 'call         ';
  word[3] := 'const        '; word[4] := 'do           ';
  word[5] := 'end          '; word[6] := 'if           ';
  word[7] := 'odd          '; word[8] := 'procedure    ';
  word[9] := 'read         '; word[10]:= 'then         ';
  word[11]:= 'var          '; word[12]:= 'while        ';
  word[13]:= 'write        ';
  
  wsym[1] := beginsym;      wsym[2] := callsym;
  wsym[3] := constsym;      wsym[4] := dosym;
  wsym[5] := endsym;        wsym[6] := ifsym;
  wsym[7] := oddsym;        wsym[8] := procsym;
  wsym[9] := readsym;       wsym[10]:= thensym;
  wsym[11]:= varsym;        wsym[12]:= whilesym;
  wsym[13]:= writesym;
  
  ssym['+'] := plus;        ssym['-'] := minus;
  ssym['*'] := times;       ssym['/'] := slash;
  ssym['('] := lparen;      ssym[')'] := rparen;
  ssym['='] := eql;         ssym[','] := comma;
  ssym['.'] := period;
  ssym['<'] := lss;         ssym['>'] := gtr;
  ssym[';'] := semicolon;
  
  mnemonic[lit] := 'LIT  '; mnemonic[opr] := 'OPR  ';
  mnemonic[lod] := 'LOD  '; mnemonic[sto] := 'STO  ';
  mnemonic[cal] := 'CAL  '; mnemonic[int] := 'INT  ';
  mnemonic[jmp] := 'JMP  '; mnemonic[jpc] := 'JPC  ';
  mnemonic[red] := 'RED  '; mnemonic[wrt] := 'WRT  ';
  
  declbegsys := [ constsym, varsym, procsym ];
  statbegsys := [ beginsym, callsym, ifsym, whilesym];
  facbegsys := [ ident, number, lparen ];
  err := 0;
  cc := 0;
  cx := 0;
  ll := 0;
  ch := ' ';
  kk := al;
  getsym;
  block( 0,0,[period]+declbegsys+statbegsys );
  if sym <> period
  then error(9);
  if err = 0
  then interpret
  else write('ERRORS IN PL/0 PROGRAM');
  writeln;
  close(fin)
end.  
```
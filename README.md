# CPP-expression-evaluator
表达式计算分析库
-----------------------------------------------

关于如何使用：
我提供了包含头文件"eval.hpp"
其中具有命名空间eval
主要内容就在其中了
关于其中类的作用与具体使用我放在README结尾

-----------------------------------------------

关于实现：
基于我自己拓展过的逆波兰序

---------------------------------------------

当前支持:
  （1）自定义变量，常量，函数，一二元运算符
  （2）还可以通过自定义用来计算一些除了实数以外的东西
  （比如字符串，自定义高精度，复数之类的）
  
------------------------------------------------

具体使用：
以下其实程序里会用到的只有epre和eval
======
eval::func是基础函数单元
其中三个成员分别为函数参数数量，优先级大小，具体实现
size_t size;
size_t flag;
std::function<type(const type*)> f;
具体实现为一个可调用对象type(const type*)
可调用对象使用的话看个例子
加法：[](const double*arr)->double {return arr[0]+arr[1]; }}
正弦：[](const double*arr)->double {return sin(arr[0]); }
自己理解一下就好了
======
var为常变量基本单元
有两个成员分别表示属性和值
vartype vtype;//可以取值eval::vartype::CONST表示常量，eval::vartype::FREE表示变量
type value;
======
epre为表达式存储单元
其中有
std::vector<func<type>*> funcs;//函数
std::vector<type*> vars;//变量
std::vector<type> consts;//常量
std::string index;//索引
void clear()//重置表达式
epre不需要初始化，它将由eval生成
======
eval为表达式分析求值单元
初始化需要
std::function<bool(const char&)> constbegin_,//表示非用户定义常量开头判断
std::function<bool(const char&)> constin_,//表示非用户定义常量中间判断
std::function<type(const std::string&)> stot_,//提供string -> type函数
sstree<var<type>>* vars_,（可省）//提供变量表指针，SSTREEE就请自己看看实现了（码不动了，也欢迎大家补充）
sstree<func<type>>* funcs_,（可省）//提供普通函数表表指针
sstree<func<type>>* oper1_,（可省）//提供1元函数表指针
sstree<func<type>>* oper2_（可省）//提供2元函数表指针
成员函数
size_t cpre(epre<type>& epre_,const std::string &str);
可以通过一个string表达式初始化epre
type result(const epre<type>& epre_);
可以计算一个epre的当前值

----------------------------------------------

具体使用例子：
比如需要double实数求值
初始化分析器
eval::eval<double> eval_(isdigit_,isdigitin,stod_);
其中
bool isdigit_(const char&ch)
{
    return ch>='0'&&ch<='9';
}
bool isdigitin(const char&ch)
{
    return (ch>='0'&&ch<='9')||ch=='.';
}
double stod_(const std::string&s)
{
    return std::stod(s);
}
（可以稍微理解一下我说的常量判断是指的什么）
如何为eval提供运算？
我用一些常见的一二元和函数运算举些例子：
    eval_.oper1.first->insert("-",{1,-1u,[](const double*arr)->double {return -arr[0]; }});
    eval_.oper1.first->insert("+",{1,-1u,[](const double*arr)->double {return arr[0]; }});
    eval_.oper2.first->insert("+",{2,0,[](const double*arr)->double {return arr[0]+arr[1]; }});
    eval_.oper2.first->insert("-",{2,0,[](const double*arr)->double {return arr[0]-arr[1]; }});
    eval_.oper2.first->insert("*",{2,1,[](const double*arr)->double {return arr[0]*arr[1]; }});
    eval_.oper2.first->insert("/",{2,1,[](const double*arr)->double {return arr[0]/arr[1]; }});
    eval_.funcs.first->insert("sin",{1,-1u,[](const double*arr)->double {return sin(arr[0]); }});
    eval_.funcs.first->insert("cos",{1,-1u,[](const double*arr)->double {return cos(arr[0]); }});
    eval_.funcs.first->insert("tan",{1,-1u,[](const double*arr)->double {return tan(arr[0]); }});
    eval_.funcs.first->insert("asin",{1,-1u,[](const double*arr)->double {return asin(arr[0]); }});
    eval_.funcs.first->insert("acos",{1,-1u,[](const double*arr)->double {return acos(arr[0]); }});
    eval_.funcs.first->insert("atan",{1,-1u,[](const double*arr)->double {return atan(arr[0]); }});
    eval_.funcs.first->insert("sqrt",{1,-1u,[](const double*arr)->double {return sqrt(arr[0]); }});
    eval_.funcs.first->insert("log",{1,-1u,[](const double*arr)->double {return log(arr[0]); }});
    eval_.funcs.first->insert("log10",{1,-1u,[](const double*arr)->double {return log10(arr[0]); }});
    eval_.funcs.first->insert("exp",{1,-1u,[](const double*arr)->double {return exp(arr[0]); }});
    eval_.funcs.first->insert("abs",{1,-1u,[](const double*arr)->double {return abs(arr[0]); }});
    eval_.funcs.first->insert("floor",{1,-1u,[](const double*arr)->double {return floor(arr[0]); }});
    eval_.funcs.first->insert("ceil",{1,-1u,[](const double*arr)->double {return ceil(arr[0]); }});
添加常变量：
    eval_.vars.first->insert("PI",{eval::vartype::CONST,M_PI});
    eval_.vars.first->insert("a",{eval::vartype::FREE,0.0});
修改常变量的值：
    eval_.vars.first->search("变量名")->data->value=修改值;
    （要注意常量标志的作用仅为标志，具体改不改值取决于你）
    另外其实有很多东西是可以获取的，但是这边讲不下去了（太多了，建议自己翻阅一下）
分析计算表达式：
    eval::epre<double> epre;（保存前确定epre是否为空，为空的话clear一下）
    eval_.cpre(epre,“1*(sin(PI/a))”);(分析表达式，成功返回-1u，失败返回失败索引值)
    eval_.result(epre);（计算表达式的值，返回结果，失败抛出一个std::runtime_error）
    
-----------------------------------------------
更多内容我以后会更新的，敬请期待~
（话说我其实不知道README怎么写来着。。。）
-----------------------------------------------

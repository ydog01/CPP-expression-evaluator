# CPP-expression-evaluator
表达式计算分析库
-----------------------------------------------
关于如何使用：
----------------------------------------------
我提供了包含头文件"eval.hpp"\n
其中具有命名空间eval\n
主要内容就在其中了\n
关于其中类的作用与具体使用我放在README结尾

-----------------------------------------------
关于实现：
----------------------------------------------
基于我自己拓展过的逆波兰序

---------------------------------------------
当前支持:
----------------------------------------------
  （1）自定义变量，常量，函数，一二元运算符\n
  （2）还可以通过自定义用来计算一些除了实数以外的东西\n
  （比如字符串，自定义高精度，复数之类的）
  
------------------------------------------------
具体使用：
----------------------------------------------

eval::func是基础函数单元\n
其中三个成员分别为函数参数数量，优先级大小，具体实现\n
size_t size;\n
size_t flag;\n
std::function<type(const type*)> f;\n
具体实现为一个可调用对象type(const type*)\n
可调用对象使用的话看个例子\n
加法：[](const double*arr)->double {return arr[0]+arr[1]; }}\n
正弦：[](const double*arr)->double {return sin(arr[0]); }\n
自己理解一下就好了

======

var为常变量基本单元\n
有两个成员分别表示属性和值\n
vartype vtype;//可以取值eval::vartype::CONST表示常量，eval::vartype::FREE表示变量\n
type value;

======

epre为表达式存储单元\n
其中有\n
std::vector<func<type>*> funcs;//函数\n
std::vector<type*> vars;//变量\n
std::vector<type> consts;//常量\n
std::string index;//索引\n
void clear()//重置表达式\n
epre不需要初始化，它将由eval生成

======

eval为表达式分析求值单元\n
初始化需要\n
std::function<bool(const char&)> constbegin_,//表示非用户定义常量开头判断\n
std::function<bool(const char&)> constin_,//表示非用户定义常量中间判断\n
std::function<type(const std::string&)> stot_,//提供string -> type函数\n
sstree<var<type>>* vars_,（可省）//提供变量表指针，SSTREEE就请自己看看实现了（码不动了，也欢迎大家补充）\n
sstree<func<type>>* funcs_,（可省）//提供普通函数表表指针\n
sstree<func<type>>* oper1_,（可省）//提供1元函数表指针\n
sstree<func<type>>* oper2_（可省）//提供2元函数表指针\n
成员函数\n
size_t cpre(epre<type>& epre_,const std::string &str);\n
可以通过一个string表达式初始化epre\n
type result(const epre<type>& epre_);\n
可以计算一个epre的当前值

----------------------------------------------

具体使用例子：\n
比如需要double实数求值\n
初始化分析器\n
eval::eval<double> eval_(isdigit_,isdigitin,stod_);\n
其中\n
bool isdigit_(const char&ch)\n
{\n
    return ch>='0'&&ch<='9';\n
}\n
bool isdigitin(const char&ch)\n
{\n
    return (ch>='0'&&ch<='9')||ch=='.';\n
}\n
double stod_(const std::string&s)\n
{\n
    return std::stod(s);\n
}\n
（可以稍微理解一下我说的常量判断是指的什么）\n
如何为eval提供运算？\n
我用一些常见的一二元和函数运算举些例子：\n
    eval_.oper1.first->insert("-",{1,-1u,[](const double*arr)->double {return -arr[0]; }});\n
    eval_.oper1.first->insert("+",{1,-1u,[](const double*arr)->double {return arr[0]; }});\n
    eval_.oper2.first->insert("+",{2,0,[](const double*arr)->double {return arr[0]+arr[1]; }});\n
    eval_.oper2.first->insert("-",{2,0,[](const double*arr)->double {return arr[0]-arr[1]; }});\n
    eval_.oper2.first->insert("*",{2,1,[](const double*arr)->double {return arr[0]*arr[1]; }});\n
    eval_.oper2.first->insert("/",{2,1,[](const double*arr)->double {return arr[0]/arr[1]; }});\n
    eval_.funcs.first->insert("sin",{1,-1u,[](const double*arr)->double {return sin(arr[0]); }});\n
    eval_.funcs.first->insert("cos",{1,-1u,[](const double*arr)->double {return cos(arr[0]); }});\n
    eval_.funcs.first->insert("tan",{1,-1u,[](const double*arr)->double {return tan(arr[0]); }});\n
    eval_.funcs.first->insert("asin",{1,-1u,[](const double*arr)->double {return asin(arr[0]); }});\n
    eval_.funcs.first->insert("acos",{1,-1u,[](const double*arr)->double {return acos(arr[0]); }});\n
    eval_.funcs.first->insert("atan",{1,-1u,[](const double*arr)->double {return atan(arr[0]); }});\n
    eval_.funcs.first->insert("sqrt",{1,-1u,[](const double*arr)->double {return sqrt(arr[0]); }});\n
    eval_.funcs.first->insert("log",{1,-1u,[](const double*arr)->double {return log(arr[0]); }});\n
    eval_.funcs.first->insert("log10",{1,-1u,[](const double*arr)->double {return log10(arr[0]); }});\n
    eval_.funcs.first->insert("exp",{1,-1u,[](const double*arr)->double {return exp(arr[0]); }});\n
    eval_.funcs.first->insert("abs",{1,-1u,[](const double*arr)->double {return abs(arr[0]); }});\n
    eval_.funcs.first->insert("floor",{1,-1u,[](const double*arr)->double {return floor(arr[0]); }});\n
    eval_.funcs.first->insert("ceil",{1,-1u,[](const double*arr)->double {return ceil(arr[0]); }});\n
添加常变量：\n
    eval_.vars.first->insert("PI",{eval::vartype::CONST,M_PI});\n
    eval_.vars.first->insert("a",{eval::vartype::FREE,0.0});\n
修改常变量的值：\n
    eval_.vars.first->search("变量名")->data->value=修改值;\n
    （要注意常量标志的作用仅为标志，具体改不改值取决于你）\n
    另外其实有很多东西是可以获取的，但是这边讲不下去了（太多了，建议自己翻阅一下）\n
分析计算表达式：\n
    eval::epre<double> epre;（保存前确定epre是否为空，为空的话clear一下）\n
    eval_.cpre(epre,“1*(sin(PI/a))”);(分析表达式，成功返回-1u，失败返回失败索引值)\n
    eval_.result(epre);（计算表达式的值，返回结果，失败抛出一个std::runtime_error）\n
    
-----------------------------------------------
更多内容我以后会更新的，敬请期待~\n
（话说我其实不知道README怎么写来着。。。）
-----------------------------------------------

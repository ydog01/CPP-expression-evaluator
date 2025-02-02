# CPP-expression-evaluator
表达式计算分析库
-----------------------------------------------
关于如何使用：
----------------------------------------------
我提供了包含头文件"eval.hpp"<br>
其中具有命名空间eval<br>
主要内容就在其中了<br>
关于其中类的作用与具体使用我放在README结尾

-----------------------------------------------
关于实现：
----------------------------------------------
基于我自己拓展过的逆波兰序

---------------------------------------------
当前支持:
----------------------------------------------
  （1）自定义变量，常量，函数，一二元运算符<br>
  （2）还可以通过自定义用来计算一些除了实数以外的东西<br>
  （比如字符串，自定义高精度，复数之类的）
  
------------------------------------------------
具体使用：
----------------------------------------------
eval::func是基础函数单元<br>
其中三个成员分别为函数参数数量，优先级大小，具体实现<br>
size_t size;<br>
size_t flag;<br>
std::function<type(const type*)> f;<br>
具体实现为一个可调用对象type(const type*)<br>
可调用对象使用的话看个例子<br>
加法：[](const double*arr)->double {return arr[0]+arr[1]; }<br>
正弦：[](const double*arr)->double {return sin(arr[0]); }<br>
自己理解一下就好了

----------------------------------------------
var为常变量基本单元<br>
有两个成员分别表示属性和值<br>
vartype vtype;//可以取值eval::vartype::CONSTVAR表示常量，eval::vartype::FREEVAR表示变量<br>
type value;

----------------------------------------------
epre为表达式存储单元<br>
其中有<br>
std::vector<func<type>*> funcs;//函数<br>
std::vector<type*> vars;//变量<br>
std::vector<type> consts;//常量<br>
std::string index;//索引<br>
void clear()//重置表达式<br>
epre不需要初始化，它将由eval生成

----------------------------------------------
eval为表达式分析求值单元<br>
初始化需要<br>
std::function<bool(const char&)> constbegin_,//表示非用户定义常量开头判断<br>
std::function<bool(const char&)> constin_,//表示非用户定义常量中间判断<br>
std::function<type(const std::string&)> stot_,//提供string -> type函数<br>
sstree<var<type>>* vars_,（可省）//提供变量表指针，SSTREEE就请自己看看实现了（码不动了，也欢迎大家补充）<br>
sstree<func<type>>* funcs_,（可省）//提供普通函数表表指针<br>
sstree<func<type>>* oper1_,（可省）//提供1元函数表指针<br>
sstree<func<type>>* oper2_（可省）//提供2元函数表指针<br>
成员函数<br>
size_t cpre(epre<type>& epre_,const std::string &str);<br>
可以通过一个string表达式初始化epre<br>
type result(const epre<type>& epre_);<br>
可以计算一个epre的当前值

----------------------------------------------

具体使用例子：
-------------
比如需要double实数求值

------------
初始化分析器：<br>
实现放在了eval_DOUBLE.help里面<br>
（另外可以稍微理解一下我说的常量判断是指的什么）<br>

----------
添加常变量：<br>
    eval_.vars.first->insert("PI",{eval::vartype::CONSTVAR,M_PI});<br>
    eval_.vars.first->insert("a",{eval::vartype::FREEVAR,0.0});<br>

-----------
修改常变量的值：<br>
    eval_.vars.first->search("变量名")->data->value=修改值;<br>
    （要注意常量标志的作用仅为标志，具体改不改值取决于你）<br>
    另外其实有很多东西是可以获取的，但是这边讲不下去了（太多了，建议自己翻阅一下）<br>

-------------
分析计算表达式：<br>
    eval::epre<double> epre;（保存前确定epre是否为空，为空的话clear一下）<br>
    eval_.cpre(epre,“1*(sin(PI/a))”);(分析表达式，成功返回-1u，失败返回失败索引值)<br>
    eval_.result(epre);（计算表达式的值，返回结果，失败抛出一个std::runtime_error）<br>
    
-----------------------------------------------
我以后会更新更多内容的，敬请期待~<br>
我的QQ:1917927623<br>
（话说我其实不知道README怎么写来着。。。）
-----------------------------------------------

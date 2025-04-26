目录概述
----------------------------
<table class="api-table">
<tr><th>1. 功能特性</th></tr>

<tr><th>2. 快速开始</th></tr>

<tr><th>3. 核心类说明</th></tr>

<tr><th>4. 表达式语法</th></tr>

<tr><th>5. 自定义扩展</th></tr>

<tr><th>6. 示例代码</th></tr>

<tr><th>7. 注意事项</th></tr>

<tr><th>8. 性能考虑</th></tr>

<tr><th>9. 许可证</th></tr>
</table>

----------------------------

<a id="概述"></a>概述
----------------------------
本库是一个***C++模板化的表达式求值引擎***，提供了> ~~强大的~~ **数学表达式解析和计算功能**。
<br>**它支持：**

<table class="api-table">
<tr><th>1. 基本算术运算</th></tr>

<tr><th>2. 自定义函数</th></tr>

<tr><th>3. 变量系统</th></tr>

<tr><th>4. 运算符优先级</th></tr>

<tr><th>5. 前缀/中缀/后缀运算符</th></tr>

<tr><th>6. 常量定义</th></tr>
</table>

**库采用头文件方式实现，无需额外编译**，只需包含**eval.hpp**和**eval_init.hpp** (如果需要默认实数初始化的话)即可使用。

----------------------------

<a id="功能特性"></a>功能特性
----------------------------
**多种数据类型**：支持**float、double、long double等类型**，甚至是 ***自定义类型*** 比如**complex**这些

**变量支持**：支持动态定义**变量**和**常量**

**默认运算符与函数**: 如果你需要的是 ***实数类型*** ，则可以直接 ***通过模板创建支持大量函数/运算符的解析器***

**自定义运算符**：可扩展前缀、中缀和后缀运算符

**错误处理**：提供基本的语法错误检测

使用 ***前缀树（Trie）*** 高效存储和查找符号

支持**运算符优先级**

***轻量级*** 实现，仅依赖 ***STL***

----------------------------

<a id="快速开始"></a>快速开始
----------------------------
**基本使用**
```
#include "eval_init.hpp"

int main() 
{
    // 创建double类型的计算器实例
    auto calc = eval_init::create_real_eval<double>();
    
    // 准备表达式结构
    eval::epre<double> expr;
    
    // 解析表达式
    calc.parse_expression(expr, "2 + 3 * sin(pi/2)");
    
    // 计算表达式
    double result = calc.evaluate(expr);
    
    return 0;
}
```

----------------------------

<a id="核心类说明"></a>核心类说明
----------------------------

evaluator<CharType, DataType>:

表达式求值器核心类，负责解析和计算表达式。

----------------------------

主要方法
----------------------------
<table class="api-table"> <tr> <th>方法</th> <th>描述</th> </tr> <tr> <td><code>parse_expression</code></td> <td>解析表达式字符串</td> </tr> <tr> <td><code>evaluate</code></td> <td>计算已解析的表达式</td> </tr> </table>
epre<DataType>
表达式中间表示结构，存储解析后的表达式信息。

成员变量
funcs: 函数/运算符列表

vars: 变量引用列表

consts: 常量值列表

index: 表达式元素索引序列

----------------------------
sstree<CharType, DataType>:

前缀树（Trie）实现，用于高效存储和查找符号（变量、函数、运算符）。

主要方法
<table class="api-table"> <tr> <th>方法</th> <th>描述</th> </tr> <tr> <td><code>insert</code></td> <td>插入键值对</td> </tr> <tr> <td><code>find</code></td> <td>查找单个字符对应的节点</td> </tr> <tr> <td><code>search</code></td> <td>查找字符串对应的节点</td> </tr> <tr> <td><code>erase</code></td> <td>删除键值对</td> </tr> </table>

----------------------------

<a id="表达式语法"></a>表达式语法
----------------------------
实数类型可默认支持的运算符
<table class="api-table">
<tr><th>运算符</th><th>描述</th><th>优先级</th></tr>
<tr><th>+</th>	<th>加法</th>	<th>1</th></tr>
<tr><th>-</th>	<th>减法</th>	<th>1</th></tr>
<tr><th>*</th>	<th>乘法</th>	<th>2</th></tr>
<tr><th>/</th>	<th>除法</th>	<th>2</th></tr>
<tr><th>%</th>	<th>取模</th>	<th>2</th></tr>
<tr><th>^</th>	<th>幂运算</th>	<th>3</th></tr>
<tr><th>-</th>	<th>负号(前缀)</th>	<th>2</th></tr>
</table>
可默认内置函数/常量

```
三角函数: sin, cos, tan, asin, acos, atan

双曲函数: sinh, cosh, tanh

对数函数: log, lg, ln, log2

内置常量
pi: π值

e: 自然对数的底
```

表达式示例
```
"2 + 3 * 4"           // 基本算术
"sin(pi/2) + cos(0)"  // 函数调用
"-3^2"                // 幂运算与负号
"log(2, 8)"           // 对数函数
"e^(ln(5))"           // 自然对数和指数
```

----------------------------

<a id="自定义扩展"></a>自定义扩展
----------------------------
**先创建解析器**

```
auto calc = eval_init::create_real_eval<double>();
```
**添加自定义函数**
```

// 定义自定义函数
eval::func<double> sqrt_op
{
    1, 
    size_t(-1),  // 最高优先级
    [](const double *args) { return sqrt(args[0]); }
};

// 注册函数
calc.funcs->insert("sqrt", sqrt_op);
```

**添加自定义变量**
```
// 定义变量
eval::var<double> my_var
{
    eval::vartype::FREEVAR,  // 可变量
    42.0                     // 初始值
};

// 注册变量
calc.vars->insert("my_var", my_var);
```

**添加自定义运算符**
```
// 定义阶乘后缀运算符
eval::func<double> fact_op
{
    1, 
    size_t(-1),  // 最高优先级
    [](const double *args) 
    { 
        double result = 1;
        for (int i = 2; i <= args[0]; ++i)
            result *= i;
        return result;
    }
};

// 注册为后缀运算符
calc.suffix_ops->insert("!", fact_op);
```

----------------------------

<a id="示例代码"></a>示例代码
----------------------------
**基本计算示例**
```
#include <iostream>
#include "eval_init.hpp"

int main() 
{
    auto calc = eval_init::create_real_eval<double>();
    eval::epre<double> expr;
    
    // 解析并计算简单表达式
    calc.parse_expression(expr, "3 + 4 * 2 / (1 - 5)^2");
    std::cout << "Result: " << calc.evaluate(expr) << std::endl;
    
    return 0;
}
使用变量示例
cpp
auto calc = eval_init::create_real_eval<double>();

// 添加变量
eval::var<double> x{eval::vartype::FREEVAR, 5.0};
calc.vars->insert("x", x);

eval::epre<double> expr;
calc.parse_expression(expr, "2*x + 3");
std::cout << "Result: " << calc.evaluate(expr) << std::endl;  // 输出13
自定义函数示例
cpp
auto calc = eval_init::create_real_eval<double>();

// 添加自定义max函数
eval::func<double> max_op
{
    2, 
    size_t(-1),
    [](const double *args) { return std::max(args[0], args[1]); }
};
calc.funcs->insert("max", max_op);

eval::epre<double> expr;
calc.parse_expression(expr, "max(3, 7)");
std::cout << "Result: " << calc.evaluate(expr) << std::endl;  // 输出7
```

----------------------------

<a id="注意事项"></a>注意事项
----------------------------
<div class="warning"> <strong>重要提示：</strong> <ul> <li>表达式字符串编码可以根据需要自行调整模板</li> <li>变量和函数名区分大小写</li> <li>函数调用必须使用括号，即使没有参数</li> <li>负号和减号使用相同符号，根据上下文自动识别</li> </ul> </div>

----------------------------

错误处理
----------------------------
语法错误时，parse_expression会返回错误位置

计算错误（如除以零）会抛出std::runtime_error

未定义的变量或函数会导致解析失败

----------------------------

<a id="性能考虑"></a>性能考虑
----------------------------
解析性能：解析阶段使用前缀树查找符号，效率较高

计算性能：计算阶段使用栈式求值，性能接近手写代码

内存使用：epre结构会存储所有中间结果，复杂表达式可能占用较多内存

<div class="note"> <strong>优化建议：</strong> 对于需要重复计算的表达式，可以解析一次后保存<code>epre</code>结构，然后多次调用<code>evaluate</code>。 </div>

----------------------------

<a id="许可证"></a>许可证
----------------------------
本项目采用MIT许可证发布，可以自由使用于个人和商业项目。

----------------------------
作者QQ number:1917927623

欢迎加个好友哇！
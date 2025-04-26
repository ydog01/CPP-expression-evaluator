目录
概述

功能特性

快速开始

核心类说明

表达式语法

自定义扩展

示例代码

注意事项

性能考虑

许可证

<a id="概述"></a>概述
本库是一个C++模板化的表达式求值引擎，提供了强大的数学表达式解析和计算功能。它支持：

基本算术运算

自定义函数

变量系统

运算符优先级

前缀/中缀/后缀运算符

常量定义

库采用头文件方式实现，无需额外编译，只需包含eval.hpp和eval_init.hpp即可使用。

<a id="功能特性"></a>功能特性
主要功能
数学表达式解析：支持复杂数学表达式的解析

多种数据类型：支持float、double、long double等实数类型

内置函数：包含常见数学函数（sin, cos, log等）

变量支持：支持预定义变量和常量

自定义运算符：可扩展前缀、中缀和后缀运算符

错误处理：提供基本的语法错误检测

技术特点
基于模板设计，类型安全

使用前缀树（Trie）高效存储和查找符号

支持运算符优先级

轻量级实现，仅依赖STL

<a id="快速开始"></a>快速开始
基本使用
cpp
#include "eval_init.hpp"

int main() {
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
<a id="核心类说明"></a>核心类说明
sstree<CharType, DataType>
前缀树（Trie）实现，用于高效存储和查找符号（变量、函数、运算符）。

主要方法
<table class="api-table"> <tr> <th>方法</th> <th>描述</th> </tr> <tr> <td><code>insert</code></td> <td>插入键值对</td> </tr> <tr> <td><code>find</code></td> <td>查找单个字符对应的节点</td> </tr> <tr> <td><code>search</code></td> <td>查找字符串对应的节点</td> </tr> <tr> <td><code>erase</code></td> <td>删除键值对</td> </tr> </table>
evaluator<CharType, DataType>
表达式求值器核心类，负责解析和计算表达式。

主要方法
<table class="api-table"> <tr> <th>方法</th> <th>描述</th> </tr> <tr> <td><code>parse_expression</code></td> <td>解析表达式字符串</td> </tr> <tr> <td><code>evaluate</code></td> <td>计算已解析的表达式</td> </tr> </table>
epre<DataType>
表达式中间表示结构，存储解析后的表达式信息。

成员变量
funcs: 函数/运算符列表

vars: 变量引用列表

consts: 常量值列表

index: 表达式元素索引序列

<a id="表达式语法"></a>表达式语法
支持的运算符
运算符	描述	优先级	结合性
+	加法	1	左
-	减法	1	左
*	乘法	2	左
/	除法	2	左
%	取模	2	左
^	幂运算	3	右
-	负号(前缀)	2	右
+	正号(前缀)	2	右
内置函数
三角函数: sin, cos, tan, asin, acos, atan

双曲函数: sinh, cosh, tanh

对数函数: log, lg, ln, log2

内置常量
pi: π值

e: 自然对数的底

表达式示例
cpp
"2 + 3 * 4"           // 基本算术
"sin(pi/2) + cos(0)"  // 函数调用
"-3^2"                // 幂运算与负号
"log(2, 8)"           // 对数函数
"e^(ln(5))"           // 自然对数和指数
<a id="自定义扩展"></a>自定义扩展
添加自定义函数
cpp
auto calc = eval_init::create_real_eval<double>();

// 定义自定义函数
eval::func<double> sqrt_op{
    1, 
    size_t(-1),  // 最高优先级
    [](const double *args) { return sqrt(args[0]); }
};

// 注册函数
calc.funcs->insert("sqrt", sqrt_op);
添加自定义变量
cpp
// 定义变量
eval::var<double> my_var{
    eval::vartype::FREEVAR,  // 可变量
    42.0                     // 初始值
};

// 注册变量
calc.vars->insert("my_var", my_var);
添加自定义运算符
cpp
// 定义阶乘后缀运算符
eval::func<double> fact_op{
    1, 
    size_t(-1),  // 最高优先级
    [](const double *args) { 
        double result = 1;
        for (int i = 2; i <= args[0]; ++i)
            result *= i;
        return result;
    }
};

// 注册为后缀运算符
calc.suffix_ops->insert("!", fact_op);
<a id="示例代码"></a>示例代码
基本计算示例
cpp
#include <iostream>
#include "eval_init.hpp"

int main() {
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
eval::func<double> max_op{
    2, 
    size_t(-1),
    [](const double *args) { return std::max(args[0], args[1]); }
};
calc.funcs->insert("max", max_op);

eval::epre<double> expr;
calc.parse_expression(expr, "max(3, 7)");
std::cout << "Result: " << calc.evaluate(expr) << std::endl;  // 输出7
<a id="注意事项"></a>注意事项
<div class="warning"> <strong>重要提示：</strong> <ul> <li>表达式字符串必须是有效的UTF-8编码</li> <li>变量和函数名区分大小写</li> <li>函数调用必须使用括号，即使没有参数</li> <li>负号和减号使用相同符号，根据上下文自动识别</li> </ul> </div>
错误处理
语法错误时，parse_expression会返回错误位置

计算错误（如除以零）会抛出std::runtime_error

未定义的变量或函数会导致解析失败

<a id="性能考虑"></a>性能考虑
解析性能：解析阶段使用前缀树查找符号，效率较高

计算性能：计算阶段使用栈式求值，性能接近手写代码

内存使用：epre结构会存储所有中间结果，复杂表达式可能占用较多内存

<div class="note"> <strong>优化建议：</strong> 对于需要重复计算的表达式，可以解析一次后保存<code>epre</code>结构，然后多次调用<code>evaluate</code>。 </div>
<a id="许可证"></a>许可证
本项目采用MIT许可证发布，可以自由使用于个人和商业项目。
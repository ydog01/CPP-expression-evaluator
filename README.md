表达式求值库（eval）使用指南
简介
该库提供了一种灵活的方式来解析和计算数学表达式，支持变量、函数、运算符以及自定义扩展。通过模板化设计，用户可以选择不同的数据类型（如 double、float），并动态注册函数和变量。

功能特性
基本运算符：支持加减乘除、幂运算、取模等。

数学函数：内置 sin, cos, log 等常见函数。

常量与变量：支持预定义常量（如 pi, e）和自定义变量。

优先级与括号：自动处理运算符优先级和括号嵌套。

扩展性：可自定义函数、运算符及变量。

快速开始
包含头文件
cpp
#include "eval_init.hpp"
示例代码
cpp
#include <iostream>
#include "eval_init.hpp"

int main() 
{
    // 创建 double 类型的求值器
    auto calc = eval_init::create_real_eval<double>();
    
    // 解析并计算表达式
    eval::epre<double> expr;
    size_t error_pos = calc.parse_expression(expr, "3 + 5 * sin(pi/2)");
    
    if (error_pos != static_cast<size_t>(-1)) 
    {
        std::cerr << "解析错误，位置: " << error_pos << std::endl;
        return 1;
    }
    
    try 
    {
        double result = calc.evaluate(expr);
        std::cout << "结果: " << result << std::endl; // 输出 8.0
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "计算错误: " << e.what() << std::endl;
    }
    
    return 0;
}
详细用法
1. 初始化求值器
使用 eval_init::create_real_eval<T>() 创建指定类型的求值器：

cpp
auto calc = eval_init::create_real_eval<float>();  // 使用 float 类型
2. 解析表达式
调用 parse_expression 解析表达式字符串：

cpp
eval::epre<double> expr;
size_t error_pos = calc.parse_expression(expr, "2^3 + log(100)");
若返回 error_pos 不等于 -1，表示解析出错的位置。

解析后的中间结果存储在 expr 对象中。

3. 计算结果
使用 evaluate 方法计算结果：

cpp
double result = calc.evaluate(expr);
扩展功能
添加自定义函数
cpp
eval::func<double> custom_op
{
    2,                                // 参数数量
    size_t(-1),                       // 最高优先级
    [](const double* args) { return args[0] * args[1] + 1; }
};
calc.funcs->insert("myfunc", custom_op);
添加自定义变量
cpp
eval::var<double> my_var
{
    eval::vartype::FREEVAR,           // 变量类型（可修改）
    42.0                              // 初始值
};
calc.vars->insert("myvar", my_var);
运算符优先级
优先级数值越大，运算顺序越优先。

内置优先级：

加减：1

乘除：2

幂、取模：3

函数：size_t(-1)（最高）

错误处理
解析错误：parse_expression 返回错误位置。

计算错误：evaluate 抛出 std::runtime_error，需捕获处理。

API 参考
主要类
evaluator<CharType, DataType>：求值器核心类。

sstree<CharType, DataType>：前缀树结构，用于存储符号。

epre<DataType>：存储解析后的表达式信息。

关键方法
parse_expression(expr, str)：解析表达式字符串。

evaluate(expr)：计算表达式结果。

insert(key, data)：向符号树插入函数或变量。

许可证
自由使用，遵循 MIT 许可证。
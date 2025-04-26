#ifndef EVAL_INIT_HPP
#define EVAL_INIT_HPP
#include "eval.hpp"

namespace eval_init
{
    using namespace eval;

    template<typename T>
    T convert(const std::string&);
    template<>
    inline double convert<double>(const std::string&str)
    {
        return std::stod(str);
    }
    template<>
    inline float convert<float>(const std::string&str)
    {
        return std::stof(str);
    }
    template<>
    inline long double convert<long double>(const std::string&str)
    {
        return std::stold(str);
    }

    template<typename T>
    eval::evaluator<char, double> create_real_eval()
    {
        evaluator<char, T> calc(
            [](char c)
            { return isdigit(c); }, // 数字开始
            [](char c)
            { return isdigit(c) || c == '.'; }, // 数字部分
            convert<T>
        );

        // 注册基本运算符
        func<T> add_op{2, 1, [](const T *args)
                            { return args[0] + args[1]; }};
        func<T> sub_op{2, 1, [](const T *args)
                            { return args[0] - args[1]; }};
        func<T> mul_op{2, 2, [](const T *args)
                            { return args[0] * args[1]; }};
        func<T> div_op{2, 2, [](const T *args)
                            { return args[0] / args[1]; }};
        func<T> pow_op{2, 3, [](const T *args)
                            { return pow(args[0], args[1]); }};
        func<T> mod_op{2, 2, [](const T *args)
                            { return fmod(args[0], args[1]); }};
        func<T> neg_op{1, 2, [](const T *args)
                            { return -args[0]; }};
        func<T> aff_op{1, 2, [](const T *args)
                       { return args[0]; }};

        calc.infix_ops->insert("+", add_op);
        calc.infix_ops->insert("-", sub_op);
        calc.infix_ops->insert("*", mul_op);
        calc.infix_ops->insert("/", div_op);
        calc.infix_ops->insert("^", pow_op);
        calc.infix_ops->insert("%", mod_op);
        calc.prefix_ops->insert("-", neg_op);
        calc.prefix_ops->insert("+", aff_op);

        // 注册数学函数
        func<T> sin_op{1, size_t(-1), [](const T *args)
                            { return sin(args[0]); }};
        func<T> cos_op{1, size_t(-1), [](const T *args)
                            { return cos(args[0]); }};
        func<T> tan_op{1, size_t(-1), [](const T *args)
                            { return tan(args[0]); }};
        func<T> asin_op{1, size_t(-1), [](const T *args)
                             { return asin(args[0]); }};
        func<T> acos_op{1, size_t(-1), [](const T *args)
                             { return acos(args[0]); }};
        func<T> atan_op{1, size_t(-1), [](const T *args)
                             { return atan(args[0]); }};
        func<T> sinh_op{1, size_t(-1), [](const T *args)
                             { return sinh(args[0]); }};
        func<T> cosh_op{1, size_t(-1), [](const T *args)
                             { return cosh(args[0]); }};
        func<T> tanh_op{1, size_t(-1), [](const T *args)
                             { return tanh(args[0]); }};
        func<T> log_op{2, size_t(-1), [](const T *args)
                            { return log(args[1]) / log(args[0]); }};
        func<T> lg_op{1, size_t(-1), [](const T *args)
                           { return log10(args[0]); }};
        func<T> ln_op{1, size_t(-1), [](const T *args)
                           { return log(args[0]); }};
        func<T> log2_op{1, size_t(-1), [](const T *args)
                             { return log2(args[0]); }};

        calc.funcs->insert("sin", sin_op);
        calc.funcs->insert("cos", cos_op);
        calc.funcs->insert("tan", tan_op);
        calc.funcs->insert("asin", asin_op);
        calc.funcs->insert("acos", acos_op);
        calc.funcs->insert("atan", atan_op);
        calc.funcs->insert("sinh", sinh_op);
        calc.funcs->insert("cosh", cosh_op);
        calc.funcs->insert("tanh", tanh_op);
        calc.funcs->insert("log", log_op);
        calc.funcs->insert("lg", lg_op);
        calc.funcs->insert("ln", ln_op);
        calc.funcs->insert("log2", log2_op);

        // 注册数学常量
        var<T> pi{vartype::CONSTVAR,asin(T(1))*T(2)};
        var<T> e{vartype::CONSTVAR,exp(T(1))};

        calc.vars->insert("pi",pi);
        calc.vars->insert("e",e);

        return calc;
    }
}
#endif
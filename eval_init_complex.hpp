#ifndef EVAL_INIT_COMPLEX_HPP
#define EVAL_INIT_COMPLEX_HPP
#include "eval_init.hpp"
#include <complex>

namespace eval_init
{

    template <typename CT>
    eval::evaluator<char, std::complex<CT>> create_complex_eval()
    {
        using namespace eval;
        using T = std::complex<CT>;

        evaluator<char, T> calc(
            [](const std::string &str, size_t &pos, epre<T> &expr) -> bool
            {
                if (str[pos] < '0' || str[pos] > '9')
                    return false;
                size_t start = pos;
                while (pos < str.size() && str[pos] >= '0' && str[pos] <= '9')
                    pos++;
                if (str[pos] == '.' && str[pos + 1] >= '0' && str[pos + 1] <= '9')
                    do pos++;
                    while (pos < str.size() && str[pos] >= '0' && str[pos] <= '9');
                expr.consts.push_back(T(convert<CT>(str.substr(start, pos - start)), CT(0)));
                expr.index += 'c';
                return true;
            });

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
                       { return std::pow(args[0], args[1]); }};
        func<T> neg_op{1, 2, [](const T *args)
                       { return -args[0]; }};
        func<T> aff_op{1, 2, [](const T *args)
                       { return args[0]; }};
        func<T> i_op{1, 2, [](const T *args)
                       { return T(-args[0].imag(),args[0].real()); }};

        calc.infix_ops->insert("+", add_op);
        calc.infix_ops->insert("-", sub_op);
        calc.infix_ops->insert("*", mul_op);
        calc.infix_ops->insert("/", div_op);
        calc.infix_ops->insert("^", pow_op);
        calc.prefix_ops->insert("-", neg_op);
        calc.prefix_ops->insert("+", aff_op);
        calc.suffix_ops->insert("i", i_op);

        // 注册数学函数
        func<T> sin_op{1, size_max, [](const T *args)
                       { return std::sin(args[0]); }};
        func<T> cos_op{1, size_max, [](const T *args)
                       { return std::cos(args[0]); }};
        func<T> tan_op{1, size_max, [](const T *args)
                       { return std::tan(args[0]); }};
        func<T> asin_op{1, size_max, [](const T *args)
                        { return std::asin(args[0]); }};
        func<T> acos_op{1, size_max, [](const T *args)
                        { return std::acos(args[0]); }};
        func<T> atan_op{1, size_max, [](const T *args)
                        { return std::atan(args[0]); }};
        func<T> sinh_op{1, size_max, [](const T *args)
                        { return std::sinh(args[0]); }};
        func<T> cosh_op{1, size_max, [](const T *args)
                        { return std::cosh(args[0]); }};
        func<T> tanh_op{1, size_max, [](const T *args)
                        { return std::tanh(args[0]); }};
        func<T> sqrt_op{1, size_max, [](const T *args)
                        { return std::sqrt(args[0]); }};
        func<T> real_op{1, size_max, [](const T *args)
                              { return std::real(args[0]); }};
        func<T> imag_op{1, size_max, [](const T *args)
                              { return std::imag(args[0]); }};
        func<T> arg_op{1, size_max, [](const T *args)
                             { return std::arg(args[0]); }};
        func<T> norm_op{1, size_max, [](const T *args)
                              { return std::norm(args[0]); }};
        func<T> conj_op{1, size_max, [](const T *args)
                              { return std::conj(args[0]); }};
        func<T> proj_op{1, size_max, [](const T *args)
                              { return std::proj(args[0]); }};

        calc.funcs->insert("sin", sin_op);
        calc.funcs->insert("cos", cos_op);
        calc.funcs->insert("tan", tan_op);
        calc.funcs->insert("asin", asin_op);
        calc.funcs->insert("acos", acos_op);
        calc.funcs->insert("atan", atan_op);
        calc.funcs->insert("sinh", sinh_op);
        calc.funcs->insert("cosh", cosh_op);
        calc.funcs->insert("tanh", tanh_op);
        calc.funcs->insert("sqrt", sqrt_op);
        calc.funcs->insert("real", real_op);
        calc.funcs->insert("imag", imag_op);
        calc.funcs->insert("arg", arg_op);
        calc.funcs->insert("norm", norm_op);
        calc.funcs->insert("conj", conj_op);
        calc.funcs->insert("proj", proj_op);
        // 注册数学常量
        var<T> pi{vartype::CONSTVAR, T(std::acos(CT(-1)),CT(0))};
        var<T> e{vartype::CONSTVAR, T(std::exp(CT(1)),CT(0))};
        var<T> i{vartype::CONSTVAR, T(CT(0),CT(1))};

        calc.vars->insert("pi", pi);
        calc.vars->insert("e", e);
        calc.vars->insert("i", i);

        return calc;
    }
}
#endif
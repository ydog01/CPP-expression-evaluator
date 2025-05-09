/*

经过我的实测

这个还不能正常使用

容易出BUG

建议先别用

*/


#ifndef EVAL_LATEX_HPP
#define EVAL_LATEX_HPP

#include <string>
#include <stdexcept>
#include <cctype>
#include <unordered_set>
#include <stack>
#include <vector>
#include <algorithm>

namespace eval
{
    template<typename cType=char>
    class latex_parser
    {
    private:
        std::unordered_set<std::basic_string<cType>> registered_vars;

        enum CharType
        {
            DIGIT,
            LETTER,
            SYMBOL,
            OPERATOR,
            PAREN_CLOSE,
            PAREN_OPEN,
            NONE
        };

        CharType get_char_type(char c)
        {
            if (std::isdigit(c))
                return DIGIT;
            if (std::isalpha(c) || c == '_')
                return LETTER;
            if (c == ')')
                return PAREN_CLOSE;
            if (c == '(')
                return PAREN_OPEN;
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%')
                return OPERATOR;
            return SYMBOL;
        }

        void add_part(std::basic_string<cType> &result, CharType &last_type, const std::basic_string<cType> &part)
        {
            if (part.empty())
                return;
            CharType current_first = get_char_type(part[0]);
            if (last_type != NONE &&
                (last_type == DIGIT || last_type == LETTER || last_type == PAREN_CLOSE) &&
                (current_first == DIGIT || current_first == LETTER || current_first == PAREN_OPEN))
            {
                result += '*';
            }
            result += part;
            last_type = get_char_type(part.back());
        }

        std::basic_string<cType> parse_number(const std::basic_string<cType> &latex, size_t &pos)
        {
            size_t start = pos;
            while (pos < latex.size() && std::isdigit(latex[pos]))
                pos++;
            return latex.substr(start, pos - start);
        }

        std::basic_string<cType> parse_identifier(const std::basic_string<cType> &latex, size_t &pos)
        {
            size_t start = pos;
            size_t max_len = 0;
            size_t remaining = latex.size() - start;

            for (size_t len = std::min(remaining, size_t(20)); len >= 1; --len)
            {
                std::basic_string<cType> candidate = latex.substr(start, len);
                if (registered_vars.count(candidate))
                {
                    max_len = len;
                    break;
                }
            }

            if (max_len > 0)
            {
                pos = start + max_len;
                return latex.substr(start, max_len);
            }

            while (pos < latex.size() && (std::isalnum(latex[pos]) || latex[pos] == '_'))
                pos++;
            std::basic_string<cType> id = latex.substr(start, pos - start);

            if (id.size() > 1 && !registered_vars.count(id))
            {
                std::basic_string<cType> new_id;
                for (size_t i = 0; i < id.size(); ++i)
                {
                    if (i != 0)
                        new_id += '*';
                    new_id += id[i];
                }
                return new_id;
            }
            return id;
        }

        std::basic_string<cType> parse_braces(const std::basic_string<cType> &latex, size_t &pos)
        {
            if (pos >= latex.size() || latex[pos] != '{')
                throw std::runtime_error("Expected '{'");

            pos++;
            std::basic_string<cType> content;
            int depth = 1;
            while (pos < latex.size() && depth > 0)
            {
                if (latex[pos] == '{')
                    depth++;
                else if (latex[pos] == '}')
                    depth--;
                if (depth > 0)
                {
                    if (std::isdigit(latex[pos]))
                        content += parse_number(latex, pos);
                    else
                        content += latex[pos++];
                }
                else
                    pos++;
            }
            if (depth != 0)
                throw std::runtime_error("Unbalanced braces");
            return parse(content);
        }

        std::basic_string<cType> parse_brackets(const std::basic_string<cType> &latex, size_t &pos)
        {
            if (pos >= latex.size() || latex[pos] != '[')
                throw std::runtime_error("Expected '['");

            pos++;
            std::basic_string<cType> content;
            int depth = 1;
            while (pos < latex.size() && depth > 0)
            {
                if (latex[pos] == '[')
                    depth++;
                else if (latex[pos] == ']')
                    depth--;
                if (depth > 0)
                    content += latex[pos++];
                else
                    pos++;
            }
            if (depth != 0)
                throw std::runtime_error("Unbalanced brackets");
            return parse(content);
        }

        std::basic_string<cType> parse_sqrt(const std::basic_string<cType> &latex, size_t &pos)
        {
            std::basic_string<cType> degree = "2";
            if (pos < latex.size() && latex[pos] == '[')
            {
                degree = parse_brackets(latex, pos);
            }
            std::basic_string<cType> radicand = parse_braces(latex, pos);
            return (degree == "2") ? "sqrt(" + radicand + ")" : "root(" + degree + "," + radicand + ")";
        }

        std::basic_string<cType> parse_frac(const std::basic_string<cType> &latex, size_t &pos)
        {
            std::basic_string<cType> numerator = parse_braces(latex, pos);
            std::basic_string<cType> denominator = parse_braces(latex, pos);
            return "(" + numerator + ")/(" + denominator + ")";
        }

        std::basic_string<cType> parse_log(const std::basic_string<cType> &latex, size_t &pos)
        {
            if (pos < latex.size() && latex[pos] == '_')
            {
                pos++;
                std::basic_string<cType> base;
                if (pos < latex.size() && latex[pos] == '{')
                {
                    base = parse_braces(latex, pos);
                }
                else
                {
                    if (std::isdigit(latex[pos]))
                        base = parse_number(latex, pos);
                    else
                        base = parse_identifier(latex, pos);
                }
                std::basic_string<cType> arg = parse_braces(latex, pos);
                return "log(" + base + "," + arg + ")";
            }
            else if (pos < latex.size() && latex[pos] == '{')
            {
                std::basic_string<cType> arg = parse_braces(latex, pos);
                return "log(" + arg + ")";
            }
            return "log";
        }

        std::basic_string<cType> parse_command(const std::basic_string<cType> &latex, size_t &pos)
        {
            pos++;
            std::basic_string<cType> cmd;
            while (pos < latex.size() && std::isalpha(latex[pos]))
            {
                cmd += latex[pos];
                pos++;
            }

            if (cmd == "frac")
                return parse_frac(latex, pos);
            if (cmd == "sqrt")
                return parse_sqrt(latex, pos);
            if (cmd == "log")
                return parse_log(latex, pos);
            if (cmd == "cdot" || cmd == "times")
                return "*";
            if (cmd == "div")
                return "/";
                
            else if (pos < latex.size() && latex[pos] == '{')
            {
                std::basic_string<cType> arg = parse_braces(latex, pos);
                return cmd + "(" + arg + ")";
            }
            return cmd;
        }

        int get_precedence(const std::basic_string<cType>& op)
        {
            if (op == "^") return 4;
            if (op == "*" || op == "/" || op == "%") return 3;
            if (op == "+" || op == "-") return 2;
            return 0;
        }

        bool is_operator(const std::basic_string<cType>& s)
        {
            return s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || s == "%";
        }

        bool is_function(const std::basic_string<cType>& s)
        {
            return s.find('(') != std::basic_string<cType>::npos && 
                   s.find(')') != std::basic_string<cType>::npos &&
                   s.find(',') == std::basic_string<cType>::npos;
        }

        bool is_multi_arg_function(const std::basic_string<cType>& s)
        {
            return s.find('(') != std::basic_string<cType>::npos && 
                   s.find(')') != std::basic_string<cType>::npos &&
                   s.find(',') != std::basic_string<cType>::npos;
        }

        std::vector<std::basic_string<cType>> tokenize(const std::basic_string<cType>& expr)
        {
            std::vector<std::basic_string<cType>> tokens;
            size_t pos = 0;
            
            while (pos < expr.size())
            {
                if (std::isspace(expr[pos]))
                {
                    pos++;
                    continue;
                }
                
                if (std::isalpha(expr[pos]))
                {
                    size_t start = pos;
                    while (pos < expr.size() && (std::isalnum(expr[pos]) || expr[pos] == '_'))
                    {
                        pos++;
                    }
                    tokens.push_back(expr.substr(start, pos - start));
                }
                else if (std::isdigit(expr[pos]))
                {
                    size_t start = pos;
                    while (pos < expr.size() && std::isdigit(expr[pos]))
                    {
                        pos++;
                    }
                    tokens.push_back(expr.substr(start, pos - start));
                }
                else if (expr[pos] == '(' || expr[pos] == ')')
                {
                    tokens.push_back(std::basic_string<cType>(1, expr[pos]));
                    pos++;
                }
                else if (is_operator(std::basic_string<cType>(1, expr[pos])))
                {
                    tokens.push_back(std::basic_string<cType>(1, expr[pos]));
                    pos++;
                }
                else if (expr[pos] == ',')
                {
                    tokens.push_back(",");
                    pos++;
                }
            }
            
            return tokens;
        }


    public:
        void register_variable(const std::basic_string<cType> &var)
        {
            registered_vars.insert(var);
        }

        std::basic_string<cType> parse(const std::basic_string<cType> &latex)
        {
            size_t pos = 0;
            std::basic_string<cType> result;
            CharType last_type = NONE;

            while (pos < latex.size())
            {
                char c = latex[pos];
                if (c == '\\')
                {
                    std::basic_string<cType> cmd_part = parse_command(latex, pos);
                    add_part(result, last_type, cmd_part);
                }
                else if (c == '^')
                {
                    pos++;
                    std::basic_string<cType> content;
                    if (pos < latex.size() && latex[pos] == '{')
                    {
                        content = parse_braces(latex, pos);
                    }
                    else if (std::isdigit(latex[pos]))
                    {
                        content = parse_number(latex, pos);
                    }
                    else
                    {
                        content = parse_identifier(latex, pos);
                    }
                    add_part(result, last_type, "^(" + content + ")");
                }
                else if (std::isdigit(latex[pos]))
                {
                    std::basic_string<cType> num = parse_number(latex, pos);
                    add_part(result, last_type, num);
                }
                else if (std::isalpha(c) || c == '_')
                {
                    std::basic_string<cType> id = parse_identifier(latex, pos);
                    add_part(result, last_type, id);
                }
                else if (std::isspace(c))
                {
                    pos++;
                }
                else
                {
                    add_part(result, last_type, std::basic_string<cType>(1, c));
                    pos++;
                }
            }
            return result;
        }

    };
}

#endif
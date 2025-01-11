#ifndef eval_CLASS
#define eval_CLASS
#include"sstree.hpp"
#include<functional>
#include<stdexcept>
#include<vector>
namespace eval
{
    template <typename type>
    struct func
    {
        size_t size;
        size_t flag;
        std::function<type(const type*)> f;
    };
    enum class vartype
    {
        CONST,
        FREE
    };
    template <typename type>
    struct var
    {
        vartype vtype;
        type value;       
    };
    template<typename type>
    struct epre
    {
        std::vector<func<type>*> funcs;
        std::vector<type*> vars;
        std::vector<type> consts;
        std::string index;
        void clear()
        {
            funcs.clear();
            vars.clear();
            consts.clear();
            index.clear();
        }
    };
    template<typename type>
    struct eval
    {
        std::function<bool(const char&)> constbegin;
        std::function<bool(const char&)> constin;
        std::function<type(const std::string&)> stot;
        std::pair<sstree<var<type>>*,bool> vars;
        std::pair<sstree<func<type>>*,bool> funcs;
        std::pair<sstree<func<type>>*,bool> oper1;
        std::pair<sstree<func<type>>*,bool> oper2;

        eval(std::function<bool(const char&)> constbegin_,
        std::function<bool(const char&)> constin_,
        std::function<type(const std::string&)> stot_):

        constbegin(constbegin_),
        constin(constin_),
        stot(stot_),
        vars{new sstree<var<type>>,true},
        funcs{new sstree<func<type>>,true},
        oper1{new sstree<func<type>>,true}, 
        oper2{new sstree<func<type>>,true} {}

        eval(std::function<bool(const char&)> constbegin_,
            std::function<bool(const char&)> constin_,
            std::function<type(const std::string&)> stot_,
            sstree<var<type>>* vars_,
            sstree<func<type>>* funcs_,
            sstree<func<type>>* oper1_,
            sstree<func<type>>* oper2_):
        
        constbegin(constbegin_),
        constin(constin_),
        stot(stot_),    
        vars(vars?std::make_pair(vars_,false):std::make_pair(new sstree<var<type>>,true)),
        funcs(funcs?std::make_pair(funcs_,false):std::make_pair(new sstree<func<type>>,true)),
        oper1(oper1?std::make_pair(oper1_,false):std::make_pair(new sstree<func<type>>,true)), 
        oper2(oper2_?std::make_pair(oper2_,false):std::make_pair(new sstree<func<type>>,true)) {}

        ~eval()
        {
            if(vars.second)
                delete vars.first;
            if(funcs.second)
                delete funcs.first;
            if(oper1.second)
                delete oper1.first;
            if(oper2.second)
                delete oper2.first;
        }

        size_t cpre(epre<type>& epre_,const std::string &str);
        type result(const epre<type>& epre_);
    };
};
#endif

#ifndef eval_CLASS
#define eval_CLASS
#include"sstree.hpp"
#include<functional>
#include<vector>
#include<memory>
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
        CONSTVAR,
        FREEVAR
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
        std::shared_ptr<sstree<var<type>>> vars;
        std::shared_ptr<sstree<func<type>>> funcs;
        std::shared_ptr<sstree<func<type>>> oper1;
        std::shared_ptr<sstree<func<type>>> oper2;

        eval(std::function<bool(const char&)> constbegin_,
        std::function<bool(const char&)> constin_,
        std::function<type(const std::string&)> stot_):
        constbegin(constbegin_),
        constin(constin_),
        stot(stot_),
        vars(new sstree<var<type>>),
        funcs(new sstree<func<type>>),
        oper1(new sstree<func<type>>),
        oper2(new sstree<func<type>>){}

        eval(std::function<bool(const char&)> constbegin_,
            std::function<bool(const char&)> constin_,
            std::function<type(const std::string&)> stot_,
            std::shared_ptr<sstree<var<type>>> vars_,
            std::shared_ptr<sstree<func<type>>> funcs_,
            std::shared_ptr<sstree<func<type>>> oper1_,
            std::shared_ptr<sstree<func<type>>> oper2_):
        constbegin(constbegin_),
        constin(constin_),
        stot(stot_),    
        vars(vars_?vars_:new sstree<var<type>>),
        funcs(funcs_?funcs_:new sstree<func<type>>),
        oper1(oper1_? oper1_:new sstree<func<type>>), 
        oper2(oper2_? oper2_:new sstree<func<type>>) {}
        
        ~eval()=default;

        size_t cpre(epre<type>& epre_,const std::string &str);
        type result(const epre<type>& epre_);
    };
};
#endif

#ifndef eval_HPP
#define eval_HPP
#include"eval_C.hpp"
namespace eval
{
    template<typename type>
    size_t eval<type>::cpre(epre<type>& epre_,const std::string &str)
    {
        size_t flag = 0,Lpos;
        typename sstree<func<type>>::iterator fptr;
        typename sstree<var<type>>::iterator vptr;
        std::vector<func<type> *> Lfuncs;
        for (size_t pos = 0; pos < str.size(); pos++)
        {
            if (pos == flag)
            {
                if (str[pos] == '(')
                {
                    Lfuncs.push_back(nullptr);
                    flag++;
                }
                else if(constbegin(str[pos]))
                {
                    Lpos=pos;
                    while (++pos<str.size()&&constin(str[pos]));
                    epre_.consts.push_back(stot(str.substr(Lpos, pos - Lpos)));
                    epre_.index.push_back('c');
                    pos--;
                }
                else
                {
                    Lpos = pos;
                    if (fptr = funcs.first->find(str[pos]))
                    {
                        do
                            funcs.first->setptr(fptr);
                        while (++pos < str.size()&&str[pos]!='('&&(fptr = funcs.first->find(str[pos])));
                        if((str[pos]=='(')&&(fptr->data))
                        {
                            Lfuncs.push_back(fptr->data);
                            Lfuncs.push_back(nullptr);
                            flag=++pos;
                        }
                        else
                            pos=Lpos;
                        funcs.first->rebegin();
                    }
                    if((pos==Lpos)&&(vptr=vars.first->find(str[pos])))
                    {
                        do
                            vars.first->setptr(vptr);
                        while(++pos<str.size()&&(vptr=vars.first->find(str[pos])));
                        if(vars.first->data())
                        {
                            if(vars.first->data()->vtype==vartype::CONST)
                            {
                                epre_.consts.push_back(vars.first->data()->value);
                                epre_.index.push_back('c');
                            }
                            else
                            {
                                epre_.vars.push_back(&(vars.first->data()->value));
                                epre_.index.push_back('v');
                            }
                        }
                        else
                            pos=Lpos;
                        vars.first->rebegin();
                    }
                    if((pos==Lpos)&&(fptr=oper1.first->find(str[pos])))
                    {
                        do
                            oper1.first->setptr(fptr);
                        while (++pos<str.size()&&(fptr=oper1.first->find(str[pos])));
                        if(oper1.first->data())
                        {
                            Lfuncs.push_back(oper1.first->data());
                            flag=pos;
                        }
                        else
                            pos=Lpos;
                        oper1.first->rebegin();
                    }
                    if(pos==Lpos)
                        return pos;
                    else
                        pos--;
                }
            }
            else
            {
                if(str[pos]==')')
                {
                    while(Lfuncs.size()&&Lfuncs.back())
                    {
                        epre_.funcs.push_back(Lfuncs.back());
                        epre_.index.push_back('f');
                        Lfuncs.pop_back();
                    }
                    if(Lfuncs.empty())
                        return pos;
                    Lfuncs.pop_back();
                }
                else if (str[pos] == ',')
                    flag = pos + 1;
                else if(fptr = oper2.first->find(str[pos]))
                {
                    do
                        oper2.first->setptr(fptr);
                    while(++pos<str.size()&&(fptr = oper2.first->find(str[pos])));
                    if(oper2.first->data())
                    {
                        while(!Lfuncs.empty()&&Lfuncs.back()&&(Lfuncs.back()->flag>=oper2.first->data()->flag))
                        {
                            epre_.funcs.push_back(Lfuncs.back());
                            epre_.index.push_back('f');
                            Lfuncs.pop_back();
                        }
                        Lfuncs.push_back(oper2.first->data());
                    }
                    else
                        return pos; 
                    flag=pos--;
                    oper2.first->rebegin();
                }
            }        
        }
        while(Lfuncs.size())
        {
            if(!Lfuncs.back())
                return str.size();
            epre_.funcs.push_back(Lfuncs.back());
            epre_.index.push_back('f');
            Lfuncs.pop_back();
        }
        return -1;
    } 
    template<typename type>
    type eval<type>::result(const epre<type>& epre_)
    {
        size_t fpos=0,vpos=0,cpos=0,LSE;
        type LRE;
        std::vector<type> stack;
        for(const char&ch:epre_.index)
        {
            switch(ch)
            {
            case 'f':
                LSE = epre_.funcs[fpos]->size;
                if (stack.size() < LSE)
                    throw std::runtime_error("eval::result:stack size error");
                LRE = epre_.funcs[fpos]->f(&(stack.back()) - LSE + 1);
                while (LSE--)
                    stack.pop_back();
                stack.push_back(LRE);
                fpos++;
                break;
            case 'v':
                stack.push_back(*(epre_.vars[vpos++]));
                break;
            case 'c':
                stack.push_back(epre_.consts[cpos++]);
                break;
            default:
                throw std::runtime_error(std::string("eval::result:invalid index")+std::string(ch,1));
            }
        }
        if(stack.size()!=1)
            throw std::runtime_error("eval::result:stack size error");
        return stack.back();
    }
}
#endif

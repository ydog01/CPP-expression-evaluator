#ifndef sstree_HPP
#define sstree_HPP
#include"sstree_C.hpp"
template<typename type>
inline type* sstree<type>::data()
{
    return ptr->data;
}
template<typename type>
inline typename sstree<type>::iterator sstree<type>::begin()
{
    return &tree;
}
template<typename type>
inline sstree<type>& sstree<type>::rebegin()
{
    ptr=&tree;
    return*this;
}
template<typename type>
inline sstree<type>& sstree<type>::operator()(iterator it)
{
    ptr=it;
    return*this;
}
template<typename type>
inline sstree<type>& sstree<type>::setptr(iterator it)
{
    return operator()(it);
}
template<typename type>
inline std::map<char,typename sstree<type>::tree_in>& sstree<type>::map()
{
    return ptr->child;
}
template<typename type>
inline typename sstree<type>::iterator sstree<type>::getptr()
{
    return ptr;
}
template<typename type>
bool sstree<type>::insert(const std::string& str, const type& data)
{
    iterator ptr_l=&tree;
    typename std::map<char,tree_in>::iterator ptr_r;
    for(size_t pos=0;pos<str.size();pos++)
        if((ptr_r=ptr_l->child.find(str[pos]))!=ptr_l->child.end())
            ptr_l=&(ptr_r->second);
        else do
            ptr_l=&(ptr_l->child[str[pos]]);
        while (++pos<str.size());
    if(ptr_l->data)
        return false;
    ptr_l->data = new type(data);
    return true;
}
template<typename type>
inline typename sstree<type>::iterator sstree<type>::find(const char& ch)
{
    typename std::map<char,tree_in>::iterator ptr_r=ptr->child.find(ch);
    return ptr_r==ptr->child.end()?nullptr:&(ptr_r->second);
}
template<typename type>
typename sstree<type>::iterator sstree<type>::search(const std::string& str)
{
    typename std::map<char,tree_in>::iterator ptr_r;
    iterator ptr_l=ptr;
    for(const char&ch:str)
    {
        ptr_r=ptr_l->child.find(ch);
        if(ptr_r==ptr_l->child.end())
            return nullptr;
        ptr_l=&(ptr_r->second);
    }
    return ptr_l;
}
#endif
#ifndef sstree_CLASS
#define sstree_CLASS
#include<map>
#include<string>
template<typename type>
class sstree
{
public:
    struct tree_in
    {
        std::map<char,tree_in> child;
        type* data;
        tree_in():data(nullptr){}
        ~tree_in()
        {
            delete data;
        }
    };
    using iterator = tree_in*;
private:
    tree_in tree;
    iterator ptr;
public:
    sstree():ptr(&tree){}
    inline type* data();
    inline iterator begin();
    inline sstree& rebegin();
    inline sstree& operator()(iterator it);
    inline sstree& setptr(iterator it);
    inline iterator getptr();
    inline std::map<char,tree_in>& map();
    bool insert(const std::string&str,const type&data);
    inline iterator find(const char&ch);
    iterator search(const std::string&str);
    inline bool erase(const std::string&str);
};
#endif

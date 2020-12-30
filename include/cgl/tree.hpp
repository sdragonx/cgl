/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 tree.hpp

 2018-10-22 14:35:35

*/
#ifndef TREE_HPP_20181022143535
#define TREE_HPP_20181022143535

#include <cgl/public.h>

namespace cgl{

template<typename T, template<typename> class Ax = std::allocator >
class basic_tree;

template<typename T>
class basic_node
{
public:
    typedef basic_node this_type;
    typedef basic_node* pointer;
    typedef T value_type;

    template<typename Tx, template<typename > class Ax>
    friend class basic_tree;

private:
    //pointer m_tree;
    pointer m_parent;
    pointer m_prev;
    pointer m_next;
    pointer m_first;
    pointer m_last;
public:
    value_type value;

    basic_node() :
        m_parent(null),
        m_prev(null),
        m_next(null),
        m_first(null),
        m_last(null)
    {
    }

    ~basic_node()
    {

    }

    pointer root()
    {
        pointer node = this;
        while(node->parent()){
            node = node->parent();
        }
        return node;
    }

    pointer parent()const{ return m_parent;}
    pointer first()const { return m_first; }
    pointer last()const  { return m_last;  }
    pointer previous()const { return m_prev; }
    pointer next()const { return m_next; }

    pointer add()
    {
        pointer root = this->root();
        pointer node = root->allocate();
        if(node){
            this->insert(node);
        }
        return node;
    }

    int erase(pointer node)
    {
        if(!node){
            return -1;
        }
        if(!node->parent()){//is root
            return -2;
        }
        if(node->parent() != this){
            return -3;
        }

        pointer root = this->root();
        node->remove();
        root->deallocate(node);
        return 0;
    }

    void clear()
    {
        pointer root = this->root();
        pointer subnode = m_first;

        while(subnode){
            m_first = subnode->m_next;
            subnode->clear();
            root->deallocate(subnode);
            subnode = m_first;
        }
        m_first = m_last = null;
    }

    int size()const
    {
        int n = 0;
        pointer node = first();
        while(node){
            ++n;
            node = node->next();
        }
        return n;
    }

    int size_all()const
    {
        int n = this->size();
        pointer node = this->first();
        while(node){
            n += node->size_all();
            node = node->next();
        }
        return n;
    }

    //bubble algorithm
    void sort()
    {
        pointer node = this->first();
        if(!node){// || !node->next()){
            return ;
        }

        pointer p;
        pointer prev_node, next_node;

        std::less<T> compare;

        //compare this and next
        node = node->next();
        while(node){
            p = node;
            node = node->next();
            prev_node = p->previous();
            while(prev_node && compare(p->value, prev_node->value)){
                next_node = p->next();
                p->m_next = prev_node;
                p->m_prev = prev_node->previous();

                if(p->m_prev){
                    p->m_prev->m_next = p;
                }
                else{//fixed first node
                    m_first = p;
                }

                prev_node->m_prev = p;
                prev_node->m_next = next_node;
                if(next_node){
                    next_node->m_prev = prev_node;
                }
                else{//fixed last node
                    m_last = prev_node;
                }

                prev_node = p->previous();
            }
        }
    }

    void move(this_type* parent)
    {
        if(tree_is_branch(this, parent)){
            return ;
        }
        this->remove();
        parent->insert(this);
    }

private:

    void insert(pointer node)
    {
        node->m_parent = this;
        if(m_last){
            node->m_prev = m_last;
            m_last->m_next = node;
            m_last = node;
        }
        else{//last is empty
            m_first = m_last = node;//set first = last, one element
        }
    }

    void remove()
    {
        if(m_prev){
            m_prev->m_next = m_next;
        }
        else{
            if(m_parent){//fixed parent first node
                m_parent->m_first = m_next;
                m_parent = null;
            }
        }

        if(m_next){
            m_next->m_prev = m_prev;
        }
        else{
            if(m_parent){
                m_parent->m_last = m_prev;
                m_parent = null;
            }
        }
    }

protected:
    virtual pointer allocate()
    {
        return new this_type;
    }

    virtual void deallocate(pointer node)
    {
        delete node;
    }
};

template<typename T, template<typename > class Ax>
class basic_tree : public basic_node<T>
{
public:
    typedef basic_tree this_type;
    typedef T value_type;
    typedef basic_node<T> node_type;
    typedef node_type* pointer;
    typedef Ax<node_type> allocator_type;

    allocator_type allocator;

public:
    basic_tree() : basic_node<T>()
    {

    }

    ~basic_tree()
    {
        this->clear();
    }

    /*
    pointer first()
    {
        return nodes.first();
    }

    pointer last()
    {
        return nodes.last();
    }

    int size()const
    {
        return nodes.size();
    }

    int size_all()const
    {
        return nodes.size_all();
    }

    pointer add(node_type* parent = null)
    {
        if(!parent)parent = this;
        pointer node = this->allocate();
        parent->insert(node);
        return node;
    }

    int erase(pointer node)
    {
        if(!node){
            return -1;
        }
        if(!node->parent()){//root
            return -2;
        }

        node->remove();
        this->clear(node);//delete all child
        this->deallocate(node);

        return 0;
    }

    void clear(pointer node = null)
    {
        if(!node)node = this;

        pointer subnode = node->m_first;

        while(subnode){
            node->m_first = subnode->m_next;
            this->clear(subnode);
            this->deallocate(subnode);
            subnode = node->m_first;
        }
        node->m_first = node->m_last = null;
    }

    //bubble algorithm
    void sort(pointer node = null)
    {
        if(!node)node = &nodes;
        node->sort();
    }
    */

protected:
    pointer allocate()
    {
        pointer node = allocator.allocate(sizeof(node_type));
        allocator.construct(node, node_type());
        return node;
    }

    void deallocate(node_type* node)
    {
        allocator.destroy(node);
        allocator.deallocate(node, CGL_ONE);
    }
};

/*
template<typename T, template<typename > class Ax>
class basic_tree
{
public:
    typedef basic_tree this_type;
    typedef T value_type;
    typedef basic_node<T> node_type;
    typedef node_type* pointer;
    typedef Ax<node_type> allocator_type;

    static allocator_type allocator;
private:
    node_type nodes;
public:
    basic_tree() : nodes()
    {

    }

    ~basic_tree()
    {
        this->clear(&nodes);
    }

    pointer first()
    {
        return nodes.first();
    }

    pointer last()
    {
        return nodes.last();
    }

    int size()const
    {
        return nodes.size();
    }

    int size_all()const
    {
        return nodes.size_all();
    }

    pointer add(node_type* parent = null)
    {
        if(!parent)parent = &nodes;
        pointer node = this->allocate();
        parent->insert(node);
        return node;
    }

    int erase(pointer node)
    {
        if(!node){
            return -1;
        }
        if(!node->parent()){//root
            return -2;
        }

        node->remove();
        this->clear(node);//delete all child
        this->deallocate(node);

        return 0;
    }

    void clear(pointer node = null)
    {
        if(!node)node = &nodes;

        pointer subnode = node->m_first;

        while(subnode){
            node->m_first = subnode->m_next;
            this->clear(subnode);
            this->deallocate(subnode);
            subnode = node->m_first;
        }
        node->m_first = node->m_last = null;
    }

    //bubble algorithm
    void sort(pointer node = null)
    {
        if(!node)node = &nodes;
        node->sort();
    }
private:
    pointer allocate()
    {
        pointer node = allocator.allocate(sizeof(node_type));
        allocator.construct(node, node_type());
        return node;
    }

    void deallocate(node_type* node)
    {
        allocator.destroy(node);
        allocator.deallocate(node, CGL_ONE);
    }
};

template<typename T, template<typename > class Ax>
TYPENAME basic_tree<T, Ax>::allocator_type basic_tree<T, Ax>::allocator = basic_tree<T, Ax>::allocator_type();
*/

//
// itree
//
// ×Ô¶¯Ìí¼Ó¡¢¸²¸ÇÊ÷
//

class inode
{
public:
    typedef std::map<string_t, inode> itemlist;

    string_t value;
    itemlist* nodes;

public:
    inode() : nodes(null)
    {
    }

    ~inode()
    {
        this->dispose();
    }

    void dispose()
    {
        if(nodes){
            delete nodes;
            nodes = null;
        }
    }

    inode& subnode(const string_t& name)
    {
        if(!nodes){
            nodes = new itemlist;
        }
        return (*nodes)[name];
    }

    size_t size()const
    {
        return nodes ? nodes->size() : 0;
    }

    size_t total_size()const
    {
        size_t n = 0;
        if(nodes){
            itemlist::iterator itr = nodes->begin();
            while(itr != nodes->end()){
                ++n;
                n += itr->second.total_size();
                ++itr;
            }
        }
        return n;
    }
};

typedef inode itree;

}//end namespace cgl

#endif //TREE_HPP_20181022143535

/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 quadtree.hpp

 2018-12-20 17:53:13

 四叉树分为点四叉树、线四叉树、矩形四叉树，这里实现的是矩形四叉树

//64字节
//std::list 40....

四叉树内存占用(full tree)
level    分割块数  内存
1024     1         1        -
512      4         4
256      4x4       16
128      16*4      64
64       64*4      256
32       256*4     1024     64k
16       1024*4    4096     256k
8        4096*4    16384    1024k
4        16k*4     64k      4M
2        256k      256K     16M
1        1024k     1024k    64M
计算内存要每层都加

不使用四叉树的二维表，四叉树意义不大
1048576*64  std::list 40M   最小体积列表12M
8个方块为一个区域
16384*64    std::list 1M    最小体积列表192K

*/
#ifndef QUADTREE_HPP_20181220175313
#define QUADTREE_HPP_20181220175313

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <vector>
#include <list>

namespace cgl{

//矩形四叉树

//四个象限
//quadrant
// 2 | 1
//-------
// 3 | 4

template<typename T, typename OBJ>
class quadtree
{
public:
    const static int QTnull = -1;
    const static int QT1 = 0;
    const static int QT2 = 1;
    const static int QT3 = 2;
    const static int QT4 = 3;

    //保存对象的矩形
    struct value_type : public vec4<T>
    {
        OBJ value;
        value_type(T ax, T ay, T aw, T ah) : vec4<T>(ax, ay, aw, ah), value()
        {
        }
    };

    typedef std::list<value_type>::iterator iterator;
    typedef std::vector<value_type*> itemlist;

    //分支分割矩形
    struct region : public vec4<T>
    {
        itemlist items;
        region* qt[4];

        region() : vec4<T>(), items()
        {
            memset(qt, 0, sizeof(qt));
        }
        region(T ax, T ay, T aw, T ah) : vec4<T>(ax, ay, aw, ah)
        {
            memset(qt, 0, sizeof(qt));
        }
        region(const vec4<T>& rc) : vec4<T>(rc)
        {
            memset(qt, 0, sizeof(qt));
        }
        ~region()
        {
            this->dispose();
        }

        void dispose()
        {
            items.clear();
            for(size_t i=0; i<4; ++i){
                if(qt[i]){ delete qt[i]; qt[i] = null; }
            }
        }
    };
    typedef region* pointer;

public:
    //根
    region root;

private:
    //保存树中所以对象
    std::list<value_type> m_items;
    //最小分割大小
    int m_min_size;

public:
    quadtree() : root()
    {

    }
    ~quadtree()
    {
        this->dispose();
    }

    //树初始化
    void init(
        T x,        //四叉树大小，
        T y,
        T width,
        T height,
        T size        //最小分割尺寸
        )
    {
        this->dispose();
        root.x = x;
        root.y = y;
        root.width = width;
        root.height = height;
        m_min_size = size;
    }

    void dispose()
    {
        root.dispose();
        m_items.clear();
    }

    iterator begin()
    {
        return m_items.begin();
    }

    iterator end()
    {
        return m_items.end();
    }

    int insert(T x, T y, T width, T height, const OBJ& value)
    {
        //剔除边界外部的矩形
        if(x < root.x || y < root.y || x + width >= root.get_right() || y + height >= root.get_bottom()){
            return -1;
        }

        pointer node = &root;
        int midx = 0;
        int midy = 0;
        tag_begin:
        int qt = QTnull;
        vec4<T> rc;
        while(true){
            //计算中心点坐标
            midx = node->x + node->width / 2;
            midy = node->y + node->height / 2;

            //判断对象位于那个象限内
            if(x + width < midx){//2, 3
                if(y + height < midy){//2
                    qt = QT2;
                }
                else if(midy < y){//3
                    qt = QT3;
                }
                else break;
            }
            else if(midx < x){//1, 4
                if(y + height < midy){//1
                    qt = QT1;
                }
                else if(midy < y){//4
                    qt = QT4;
                }
                else break;
            }
            else break;

            if(node->qt[qt]){//检查子分支
                node = node->qt[qt];
            }
            else if(qt != QTnull && (m_min_size < midx || m_min_size < midy)){
                //split node
                rc = vec4<T>(node->x, node->y, node->width /2, node->height /2);
                switch(qt){
                case QT1:
                    rc.x = midx;
                    break;
                case QT2:
                    break;
                case QT3:
                    rc.y = midy;
                    break;
                case QT4:
                    rc.x = midx;
                    rc.y = midy;
                    break;
                }
                node->qt[qt] = new region(rc);
                node = node->qt[qt];
                qt = QTnull;
            }
            else{
                break;//分割结束
            }
        }//end while
        //添加对象
        m_items.push_back(value_type(x, y, width, height));
        node->items.push_back(&m_items.back());
        node->items.back()->value = value;

        return 0;
    }

    void erase()
    {

    }

    //把对象重新分配到树中
    void reset()
    {
    }

    void clear()
    {

    }

    //边界上的矩形先被选中，因为存在于高级别的分支内
    value_type* hit_test(int x, int y)
    {
        pointer node = &root;
        pointer pitem = null;
        int midx = 0;
        int midy = 0;
        int q = QTnull;
        //先检测子项
        while(node){
            for(itemlist::iterator itr = node->items.begin(); itr != node->items.end(); ++itr){
                if((*itr)->contains(x, y)){
                    return *itr;
                }
            }
            midx = node->x + node->width / 2;
            midy = node->y + node->height / 2;
            //遍历象限
            if(x < midx){//2, 3
                q = (y < midy) ? QT2 : QT3;
            }
            else{//1, 4
                q = (y < midy) ? QT1 : QT4;
            }
            if(node->qt[q]){
                node = node->qt[q];
            }
            else{
                return 0;//last node
            }
        }
    }

    //列取范围内的所以对象
    int ls(std::vector<OBJ>& list, const vec4<T>& rect)
    {
        list.clear();
        return this->ls(list, &root, rect);
    }

    int ls(std::vector<OBJ>& list, T x, T y, T width, T height)
    {
        return this->ls(list, &root, vec4<T>(x, y, width, height));
    }
private:
    int ls(std::vector<OBJ>& list, pointer node, const vec4<T>& rect)
    {
        for(itemlist::iterator itr = node->items.begin(); itr != node->items.end(); ++itr){
            if((*itr)->intersect(rect)){
                list.push_back((*itr)->value);
            }
        }

        for(int i=0; i<4; ++i){//检测4个分支
            if(node->qt[i] && node->qt[i]->intersect(rect)){
                ls(list, node->qt[i], rect);
            }
        }
    }
};

}//end namespace cgl

#endif //QUADTREE_HPP_20181220175313

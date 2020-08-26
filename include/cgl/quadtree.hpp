/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 quadtree.hpp

 2018-12-20 17:53:13

 �Ĳ�����Ϊ���Ĳ��������Ĳ����������Ĳ���������ʵ�ֵ��Ǿ����Ĳ���

//64�ֽ�
//std::list 40....

�Ĳ����ڴ�ռ��(full tree)
level    �ָ����  �ڴ�
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
�����ڴ�Ҫÿ�㶼��

��ʹ���Ĳ����Ķ�ά���Ĳ������岻��
1048576*64  std::list 40M   ��С����б�12M
8������Ϊһ������
16384*64    std::list 1M    ��С����б�192K

*/
#ifndef QUADTREE_HPP_20181220175313
#define QUADTREE_HPP_20181220175313

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <vector>
#include <list>

namespace cgl{

//�����Ĳ���

//�ĸ�����
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

    //�������ľ���
    struct value_type : public vec4<T>
    {
        OBJ value;
        value_type(T ax, T ay, T aw, T ah) : vec4<T>(ax, ay, aw, ah), value()
        {
        }
    };

    typedef std::list<value_type>::iterator iterator;
    typedef std::vector<value_type*> itemlist;

    //��֧�ָ����
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
    //��
    region root;

private:
    //�����������Զ���
    std::list<value_type> m_items;
    //��С�ָ��С
    int m_min_size;

public:
    quadtree() : root()
    {

    }
    ~quadtree()
    {
        this->dispose();
    }

    //����ʼ��
    void init(
        T x,        //�Ĳ�����С��
        T y,
        T width,
        T height,
        T size        //��С�ָ�ߴ�
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
        //�޳��߽��ⲿ�ľ���
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
            //�������ĵ�����
            midx = node->x + node->width / 2;
            midy = node->y + node->height / 2;

            //�ж϶���λ���Ǹ�������
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

            if(node->qt[qt]){//����ӷ�֧
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
                break;//�ָ����
            }
        }//end while
        //��Ӷ���
        m_items.push_back(value_type(x, y, width, height));
        node->items.push_back(&m_items.back());
        node->items.back()->value = value;

        return 0;
    }

    void erase()
    {

    }

    //�Ѷ������·��䵽����
    void reset()
    {
    }

    void clear()
    {

    }

    //�߽��ϵľ����ȱ�ѡ�У���Ϊ�����ڸ߼���ķ�֧��
    value_type* hit_test(int x, int y)
    {
        pointer node = &root;
        pointer pitem = null;
        int midx = 0;
        int midy = 0;
        int q = QTnull;
        //�ȼ������
        while(node){
            for(itemlist::iterator itr = node->items.begin(); itr != node->items.end(); ++itr){
                if((*itr)->contains(x, y)){
                    return *itr;
                }
            }
            midx = node->x + node->width / 2;
            midy = node->y + node->height / 2;
            //��������
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

    //��ȡ��Χ�ڵ����Զ���
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

        for(int i=0; i<4; ++i){//���4����֧
            if(node->qt[i] && node->qt[i]->intersect(rect)){
                ls(list, node->qt[i], rect);
            }
        }
    }
};

}//end namespace cgl

#endif //QUADTREE_HPP_20181220175313

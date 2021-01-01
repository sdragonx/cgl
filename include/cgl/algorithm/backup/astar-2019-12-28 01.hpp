/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ASTAR.hpp

 2018-12-19 03:43:33

*/
#ifndef ASTAR_HPP_20181219034333
#define ASTAR_HPP_20181219034333

#include <cgl/public.h>
#include <cgl/math.hpp>
#include <cgl/vec.hpp>

#include <cmath>
#include <vector>

namespace cgl {

//allocate only pool
//2019/5/31 21:14
template<typename T>
class astar_pool
{
public:
    typedef T value_type;
    typedef value_type* pointer;

    enum{
        PAGE_SIZE = 1024,
        PAGE_MASK = 1023
    };

private:
    std::vector<pointer> poolbuf;
    size_t m_size;
public:
    astar_pool() : poolbuf(), m_size()
    {
    }

    ~astar_pool()
    {
        this->dispose();
    }

    void dispose()
    {
        for(size_t i = 0; i < poolbuf.size(); ++i){
            deallocate_buffer(poolbuf[i]);
        }
        poolbuf.clear();
    }

    void clear()
    {
        m_size = 0;
    }

    pointer allocate()
    {
        int page = m_size >> 10;
        if(poolbuf.size() <= page){
            allocate_buffer();
        }
        pointer p = &poolbuf[page][m_size & PAGE_MASK];
        memset(p, 0, sizeof(value_type));
        ++m_size;
        return p;
    }

    void deallocate(pointer p)
    {
        memset(p, 0, sizeof(value_type));
    }

    size_t size()const
    {
        return m_size;
    }

    size_t buffer_size()const
    {
        return poolbuf.size() * PAGE_SIZE;
    }

private:
    pointer allocate_buffer()
    {
        pointer buf = new value_type[PAGE_SIZE];
        poolbuf.push_back(buf);
        return buf;
    }

    void deallocate_buffer(pointer &buf)
    {
        delete []buf;
        buf = null;
    }
};


/*
 7 0 1
 6   2
 5 4 3

 ��ǰ����-Ŀ�����꣬���ڷ���0-7
 �õ�б�Ƿ���1 3 5 7��(��ǰ���� + [ 1 | -1 ] ) & 7���õ��ٽ�����
*/

struct ASTAR_DIR
{
    int x;
    int y;
    int direct;
};

//North�� Northeast���� East�� Southeast���� South�� Southwest���� West�� Northwest����
const ASTAR_DIR ASTAR_DIRECTION[8] = {
    {  0, -1,  0 }, {  1, -1,  1 }, {  1,  0,  2 }, {  1,  1,  3 },
    {  0,  1,  4 }, { -1,  1,  5 }, { -1,  0,  6 }, { -1, -1,  7 }
};

//const vec2i direction[8] = {
//        vec2i(  0, -1 ), vec2i( 0,  1 ), vec2i( -1, 0 ), vec2i( 1, 0 ),
//        vec2i( -1, -1 ), vec2i( 1, -1 ), vec2i( -1, 1 ), vec2i( 1, 1 )
//    };

//���ͨ��Ŀ��ש��ķ���
int astar_direction(const vec2i& current, const vec2i& target)
{
    vec2i p = target - current;
    if(p.x == 0){
        return p.y < 0 ? CGL_N : CGL_S;
    }
    else if(p.x < 0){
        return p.y == 0 ? CGL_W : ( p.y < 0 ? CGL_NW : CGL_SW );
    }
    else{
        return p.y == 0 ? CGL_E : ( p.y < 0 ? CGL_NE : CGL_SE );
    }
}

vec2i astar_vector(int direct)
{
    direct &= 7;
    return vec2i(ASTAR_DIRECTION[direct].x, ASTAR_DIRECTION[direct].y);
}

//10->14 32->45 64->90
//ֱ�߹�ֵ    10
//б�߹�ֵ    14
//б�߹�ֵ = ֱ�߹�ֵ x sqprt(2) = 1.414

//�������꣬���ص�ͼ��ֵ��Ϣ����ֵԽС����ֵԽС
//Ĭ����ֹ8��С��0�޷�ͨ�������ֵ����С��128
//��ֵ��С���ᵼ�������������ۻ���������
//��ֵ��0��ʱ���ɱ�ɹ����������ٶȷǳ���
//��ֵ������16����ֵh��ֵ��󲻻ᳬ��65536(16*16)
//object����һ���Զ������
//typedef int (*astar_callback)( void* map, int x, int y, void* object );

#pragma pack(push, 1)

//·���ڵ�
//short���� ȫͼ����
//
//sizeof()=18   64λ = 22
//1024  18M     22M
//512   4.7M    5.6M
//256   1.2M    1.4M
//128   288K    352K
//64    72K     88K

template<typename T>
struct astar_node
{
    vec2<T>    pos;     // �ڵ�λ��
    int    g;           // ������ֵ
    int h;           // ���յ��ֵ
    short    state;   // �ڵ�״̬
    astar_node * parent; // ���ڵ�

    astar_node() : pos(), g( 0 ), h( 0 ), state( 0 ), parent( null )
    {
    }

    // ����fֵ
    int f() const // f = gone + heuristic
    {
        return g + h;
    }
};

#pragma pack(pop)

template<typename T>
class astar_pathfinder
{
public:
    typedef astar_node<T> node;
    typedef vec2<T>       point;
    //���ص�·������
    typedef std::vector<point> path;

    // ·���ڵ�״̬
    enum NODE_STATE
    {
        NOTEXIST,       // ������
        IN_OPENLIST,    // �ڿ����б�̽����Ե��
        IN_CLOSELIST    // ���ڲ��б�̽����������
    };

    enum {
        HEstimate = 2
    };

    enum METHOD
    {
        MANHATTAN,    //������
        EUCLIDEAN    //ŷ�����
    };

private:
    //std::vector<node*> m_map;
    array2d<node*> m_map;
    std::vector<node*> m_openlist;
    vec4<int> m_range;//������Χ 2019/5/31 21:14
    int m_step;        //��������
    astar_pool<node> pool;

public:
    void *instance;    //���浱ǰ��ͼ
    void *unit;    //���浱ǰ��ɫ

    //��ֵ���㺯�� default = MANHATTAN
    int method;

    //�Ƿ�֧��б������ default = true
    bool corner;

    //���·���޷�����Ƿ񷵻������ default = true
    bool find_nearest;

    //��ͼ��ֵ����
    int (*on_block_test)( void* map, int x, int y, void* object );

    //����ÿһ���������¼�������false��������
    bool (*on_step)( const node* currentNode, const node* nearestNode, int step );

public:
    astar_pathfinder() :
        m_map(),
        m_openlist(),
        m_step(),
        m_range(),
        instance(null),
        unit(null),
        method(MANHATTAN),
        corner(true),
        find_nearest(true),
        on_block_test(null),
        on_step(null)
    {
    }

    ~astar_pathfinder()
    {
        this->dispose();
    }

    int find( path & path, int width, int height,
        const point & start, point end,
        int range = 0, int max_step = INT_MAX );

    void dispose();

    int step()const { return m_step; }

    // һ�������Ƿ�������һ�������ĸ�����
    static inline bool is_corner( const point & pos1, const point & pos2 )
    {
        return (pos1.x != pos2.x) && (pos1.y != pos2.y);
    }

    //���һ��������
    void add_teleport(int x, int y)
    {
    }

private:
    // �������λ�õ�ͼ��ѯ��Ϣ
    node* get_node( const point & pos )
    {
        int x = pos.x - m_range.x;
        int y = pos.y - m_range.y;
        //y *= m_range.width;
        x = pos.x;
        y = pos.y;
        return m_map[y][x];
        //return m_map[ pos.y * m_range.width + pos.x - m_range.x ];
    }

    void set_node( const point & pos , node * n )
    {
        int y = pos.y - m_range.y;
        //y *= m_range.width;
        //m_map[ y * + pos.x - m_range.x ] = n;
        m_map[y][pos.x - m_range.x] = n;
        //m_map[ pos.y * m_range.width + pos.x - m_range.x ] = n;
    }

    //��������Ƿ���Ч
    bool is_invalid_position( const point & pos )
    {
        return pos.x < m_range.x || pos.y < m_range.y || pos.x >= m_range.get_right() || pos.y >= m_range.get_bottom();
    }

    // ��������Ƿ���Ч
    bool is_valid_position( const point & pos )
    {
        return !is_invalid_position( pos );
    }

    // ��õ�ͼλ�õ�������Ϣ
    int block_test( int x, int y, bool corner )
    {
        int n = on_block_test( instance, x, y, unit );
        return corner ? (n * 90) >> 6 : n;//����б����ֵ
    }


    // �ڵ��Ƿ��ڿ����б���
    inline bool is_in_open_list( const point & pos, node* &out )
    {
        out = get_node( pos );
        return out && out->state == IN_OPENLIST;
    }


    // �ڵ��Ƿ��ڹر��б���
    inline bool is_in_close_list( const point & pos )
    {
        node* n = get_node( pos );
        return n && n->state == IN_CLOSELIST;
    }

    bool can_reach( const point & current_pos, const point & target_pos, bool corner );

    void handle_found_node( node* current_node, node* target_node );

    node* handle_not_found_node( node* current_node, const point & target_pos, const point & end_pos );

    // ����Gֵ����С��Gֵ���Ͽɡ�
    inline int calc_g_value( node* parent_node, const point & current_pos )
    {
        int value = block_test( current_pos.x, current_pos.y, is_corner(parent_node->pos, current_pos) );
        return value += parent_node->g;
    }

    // ����Hֵ�������յ�Ĺ�ֵ��������Ӱ�������㷨
    inline int calc_h_value( const point & pos, const point & end );

    //������
    static bool compare_node( const astar_node<T>* a, const astar_node<T>* b )
    {
        return a->f() > b->f();
    }

    void final(path & path, astar_node<T>* node);
    void node_test(astar_node<T>* node, const vec2<T>& end);
};

template<typename T>
int astar_pathfinder<T>::calc_h_value( const point & pos, const point & end )
{
    //�����پ���
    //H(n) = D * (abs ( n.x �C goal.x ) + abs ( n.y �C goal.y ) )
    //�Խ��߾��룬ò��·�����������෴�������ٶ�Ҫ��һЩ
    //h(n) = D * max(abs(n.x - goal.x), abs(n.y - goal.y))
    //ŷ����þ��룬Ч�����
    //h(n) = D * sqrt((n.x-goal.x)^2 + (n.y-goal.y)^2)

    int w = std::abs( pos.x - end.x );
    int h = std::abs( pos.y - end.y );
    int value;
    switch(method){
    case MANHATTAN:
        value = ( w + h ) * HEstimate;
        value *= block_test( pos.x, pos.y, false );
        return value;
    case EUCLIDEAN:
        value = w * w;
        value += h * h;
        //value = std::sqrt(float(value));
        value = math::sqrti32(value);
        value *= HEstimate;
        value *= block_test( pos.x, pos.y, false );
        return value;
    default://MaxDXDY
        return HEstimate * std::max( w, h ) * block_test( pos.x, pos.y, false );
    }
}

// �ж�����λ���Ƿ����ͨ��
template<typename T>
bool astar_pathfinder<T>::can_reach(
    const vec2<T> & current_pos,
    const vec2<T> & target_pos,
    bool corner )    //�Ƿ�б��
{
    if ( !is_valid_position( target_pos ) ){//��ΧԼ��֮�����ò��û����
        return false;
    }

    if ( is_in_close_list( target_pos ) ){
        return false;
    }

    //bool corner = is_corner( current_pos, target_pos );

    if( on_block_test( instance, target_pos.x, target_pos.y, unit ) < 0 ){//Ŀ�겻��ͨ��
        return false;
    }

    //б�������жϡ�
    //���б�Ƿ������߶���ש�飬��ͨ����ֻ��һ��ש��ͨ��
    //2019/5/26 23:26:12
    if(corner){
        int direct = astar_direction(current_pos, target_pos);
        vec2i p1 = current_pos + astar_vector(direct - 1);
        vec2i p2 = current_pos + astar_vector(direct + 1);
//        if( ( on_block_test( p1.x, p1.y, object ) < 0 ) && ( on_block_test( p2.x, p2.y, object ) < 0 ) ){
//            return false;
//        }
        //б�ߣ���һ��ש���򶼲�ͨ��
        if( ( on_block_test( instance, p1.x, p1.y, unit ) < 0 ) || ( on_block_test( instance, p2.x, p2.y, unit ) < 0 ) ){
            return false;
        }
    }

    return true;
}

// ����openlist����Ĳ��ҵ��Ľڵ�
template<typename T>
void astar_pathfinder<T>::handle_found_node( astar_node<T>* current_node, astar_node<T>* target_node )
{
    int g = calc_g_value( current_node, target_node->pos );
    // ���g���ã�Ŀ������ĸ��������õ���ǰ����
    if ( g < target_node->g ) {
        target_node->g = g;
        target_node->parent    = current_node;
    }
}

// ���ҵ���Ŀ�����겻��openlist���棬�µĽڵ��ʼ��
template<typename T>
astar_node<T>* astar_pathfinder<T>::handle_not_found_node(
    astar_node<T>* current_node, //���ҵĵ�ǰ�ڵ�
    const vec2<T> & target_pos,  //��Χ�ڵ�λ��
    const vec2<T> & end_pos )    //Ŀ���
{
    node* target_node = pool.allocate();// new node;

    target_node->parent = current_node;
    target_node->pos    = target_pos;
    target_node->h      = calc_h_value( target_pos, end_pos );
    target_node->g      = calc_g_value( current_node, target_pos );
    target_node->state  = IN_OPENLIST;

    set_node( target_pos , target_node );

    m_openlist.push_back( target_node );
    std::push_heap( m_openlist.begin(), m_openlist.end(), compare_node);
    return target_node;
}

template<typename T>
void astar_pathfinder<T>::node_test(astar_node<T>* current_node, const vec2<T>& end)
{
    // ����ж��ķ���
    const static point direction[8] = {
        point(  0, -1 ), point( 0,  1 ), point( -1, 0 ), point( 1, 0 ),//ֱ��
        point( -1, -1 ), point( 1, -1 ), point( -1, 1 ), point( 1, 1 ) //б��
    };

    point  target_pos;
    size_t size = corner ? 8 : 4;
    astar_node<T>*  next_node;

    for ( size_t i = 0; i < size; ++i ){
        target_pos = point( current_node->pos.x + direction[i].x, current_node->pos.y + direction[i].y );
        if ( !can_reach( current_node->pos, target_pos, i & 4 ) ) { // ����Ƿ����ͨ�� i >= 4
            continue;
        }

        if ( is_in_open_list( target_pos, next_node ) ){
            handle_found_node( current_node, next_node );
        }
        else{
            next_node = handle_not_found_node( current_node, target_pos, end );
        }
    }
}

template<typename T>
void astar_pathfinder<T>::final(astar_pathfinder<T>::path & path, astar_node<T>* node)
{
    while ( node->parent ){//ѹ�����㣬��ʹ��ɫ�߻�ͷ·
        path.push_back( node->pos );
        node = node->parent;
    }
    //std::reverse( path.begin(), path.end() ); // ��ת���겢����
}

template<typename T>
int astar_pathfinder<T>::find(
    astar_pathfinder<T>::path & path, //���ص�·���ڵ�����
    int width,            //��ͼ��С
    int height,
    const point & start,//���
    //const point & end,  //�յ�
    point end,  //�յ�
    int range,            //������Χ
    int max_step)        //���������
{
    this->dispose();
    path.clear();

    if( !on_block_test ){
        return -2;
    }

    if( !find_nearest ){
        //����Ŀ���Ƿ񲻿ɵ���������Ҫ�������㣬���̷���
        if( on_block_test( instance, end.x, end.y, unit ) < 0 ){
            return -3;
        }
    }

    //��Χ����
    m_range = vec4<T>(0, 0, width, height);

    if( range ){
        //���ҷ�Χ���
        /*
        int w = range + range + 1;
        if(w > width){
            w = width / 2;
        }
        int h = range + range + 1;
        if(h > height){
            h = height / 2;
        }

        m_range = vec4<T>(start.x - w, start.y - h, w + w + 1, h + h + 1);
        if(m_range.x < 0){
            m_range.width += m_range.x;
            m_range.x = 0;
        }
        if(m_range.y < 0){
            m_range.height += m_range.y;
            m_range.y = 0;
        }
        */

        m_range = m_range & vec4<T>(start.x - range, start.y - range, range + range + 1, range + range + 1);

        //��Χ����֮�����Ŀ�겻�ɴ������ȫ������
        //�յ㷶ΧԼ�����и�С���⣬���Լ��֮�����յ㲻�ɴͬ����ȫ����������
        if( end.x < m_range.x )end.x = m_range.x;
        if( end.y < m_range.y )end.y = m_range.y;
        if( end.x >= m_range.get_right() )end.x = m_range.get_right() - 1;
        if( end.y >= m_range.get_bottom() )end.y = m_range.get_bottom() - 1;
    }

    if( !m_range.area() ){//��Ч����
        return -4;
    }

    m_step = 0;
    //m_map.resize( width, height ); //����һ����ǰ��ͼ��С�Ļ�����
    m_map.resize( m_range.width, m_range.height );

    // ��������openlist
    //node* current_node  = get_node( start );
    node* current_node  = pool.allocate();//new node;// get_node( start );
    set_node(start, current_node);

    current_node->state = IN_OPENLIST;
    current_node->h = calc_h_value( start, end ); //UINT16_MAX
    //current_node->g  = calc_g_value( current_node, start );
    current_node->pos   = start;

    node* nearest = current_node;//��¼�����

    m_openlist.push_back( current_node );

    //ÿһ��ѭ��������һ������
    while ( !m_openlist.empty() ){
        // ��openlist�е�����ӽ��յ�����꣬���ó�closelist
        current_node = *m_openlist.begin();
        std::pop_heap( m_openlist.begin(), m_openlist.end(), compare_node);
        m_openlist.pop_back();
        current_node->state = IN_CLOSELIST;

        // ���Ҿ����յ�����ĵ�
        if( current_node->h < nearest->h ){
            nearest = current_node;
        }

        // find and return
        if ( current_node->pos == end ){
            this->final(path, current_node);

            if( on_step ){
                on_step( current_node, nearest, m_step );
            }
            return 0;
        }

        node_test(current_node, end);

        if( on_step ){
            if( !on_step( current_node, nearest, m_step ) ){
                break;
            }
        }

        ++m_step;
        if(m_step >= max_step){//�������ֹ
            break;
        }
    }

    //·��δ�ҵ������ؾ����յ�����ĵ�
    while ( nearest->parent ){
        path.push_back( nearest->pos );
        nearest = nearest->parent;
    }
    //std::reverse( path.begin(), path.end() ); // ��ת���겢����

    return -1;
}

template<typename T>
void astar_pathfinder<T>::dispose()
{
    /*
    TYPENAME std::vector<node*>::iterator itr = m_map.begin();
    for( ; itr != m_map.end(); ++itr){
        if( *itr ){
            //delete (*itr);
            pool.deallocate(*itr);
            (*itr) = null;
        }
    }
    */
    pool.clear();
    m_map.clear();
    m_openlist.clear();
    m_range = vec4<T>();
    //instance = unit = null;
}

} // end namespace cgl

#endif // ASTAR_HPP_20181219034333

/*

Fֵ����

switch(mFormula)
{
    default:
    case HeuristicFormula.Manhattan:
        mH = mHEstimate * (Math.Abs(mNewLocationX - end.X) + Math.Abs(mNewLocationY - end.Y));
        break;
    case HeuristicFormula.MaxDXDY:
        mH = mHEstimate * (Math.Max(Math.Abs(mNewLocationX - end.X), Math.Abs(mNewLocationY - end.Y)));
        break;
    case HeuristicFormula.DiagonalShortCut:
        int h_diagonal  = Math.Min(Math.Abs(mNewLocationX - end.X), Math.Abs(mNewLocationY - end.Y));
        int h_straight  = (Math.Abs(mNewLocationX - end.X) + Math.Abs(mNewLocationY - end.Y));
        mH = (mHEstimate * 2) * h_diagonal + mHEstimate * (h_straight - 2 * h_diagonal);
        break;
    case HeuristicFormula.Euclidean:
        mH = (int) (mHEstimate * Math.Sqrt(Math.Pow((mNewLocationY - end.X) , 2) + Math.Pow((mNewLocationY - end.Y), 2)));
        break;
    case HeuristicFormula.EuclideanNoSQR:
        mH = (int) (mHEstimate * (Math.Pow((mNewLocationX - end.X) , 2) + Math.Pow((mNewLocationY - end.Y), 2)));
        break;
    case HeuristicFormula.Custom1:
        Point dxy       = new Point(Math.Abs(end.X - mNewLocationX), Math.Abs(end.Y - mNewLocationY));
        int Orthogonal  = Math.Abs(dxy.X - dxy.Y);
        int Diagonal    = Math.Abs(((dxy.X + dxy.Y) - Orthogonal) / 2);
        mH = mHEstimate * (Diagonal + Orthogonal + dxy.X + dxy.Y);
        break;
}

*/

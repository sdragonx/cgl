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

#include <cgl/boxworld/tilemath.hpp>

#include <set>
#include <vector>
#include <queue>

namespace cgl {

template<typename T, class Comp = std::less<T> >
class binstack2
{
public:
    std::multiset<T, Comp> m_data;

    void push(const T& value)
    {
        m_data.insert(value);
    }

    void pop()
    {
        m_data.erase(m_data.begin());
    }

    const T& top()const
    {
        return *m_data.begin();
    }

    bool empty()const
    {
        return m_data.empty();
    }

    void clear()
    {
        m_data.clear();
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
//typedef int (*astar_callback)( void* map, int x, int y, int mode );

//·���ڵ�
//short���� ȫͼ����
//
//sizeof()=16   64λ = 20
//1024  18M     22M
//512   4.7M    5.6M
//256   1.2M    1.4M
//128   288K    352K
//64    72K     88K

#pragma pack(push, 1)

template<typename T>
struct astar_node
{
    vec2<T> pos;     // �ڵ�λ��
    int     g;       // ������ֵ
    int     h;       // ���յ��ֵ
    //short state;   // �ڵ�״̬
    astar_node * parent; // ���ڵ�
    //tree.parent
    //tree.left
    //tree.right

    astar_node() : pos(), g( 0 ), h( 0 ), parent( null )
    {
    }

    // ����fֵ
    int f() const // f = gone + heuristic
    {
        return g + h;
    }
};

#pragma pack(pop)

//������
template<typename T>
bool compare_node( const astar_node<T>* a, const astar_node<T>* b )
{
    return a->f() > b->f();
}

template<typename T>
struct node_compare
{
    bool operator()(const T* a, const T* b){
        return a->f() > b->f();
    }
};

struct node_comparei
{
    bool operator()(const void* a, const void* b){
        return ((astar_node<short>*)a)->f() < ((astar_node<short>*)b)->f();
    }
};

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
        IN_OPENLIST,    // �ڿ����б���̽����Ե��
        IN_CLOSELIST    // ���ڲ��б���̽����������
    };

    enum {
        HEstimate = 4
    };

    enum METHOD
    {
        MANHATTAN,    //������
        EUCLIDEAN    //ŷ�����
    };

    struct nodemap
    {
        node data[1024][1024];
//        node* operator[](size_t n)
//        {
//            return data[n];
//        }
    };

private:
    //static node m_map[1024][1024];
    //static node (*m_map)[1024];
    static nodemap *m_map;
    static char m_state[1024][1024];

    typedef std::priority_queue<node*, std::vector<node*>, node_compare<node> > stack_type;
//    typedef binstack<void*, node_comparei> stack_type;

    stack_type m_openlist;
//    std::set<void*, node_comparei > m_openlist;

    //std::multiset<node*> m_openlist;
    vec4<int> m_range;//������Χ 2019/5/31 21:14
    int m_step;        //��������

public:
    void *instance;    //���浱ǰ��ͼ
    //void *unit;    //���浱ǰ��ɫ
    int mode;

    //��ֵ���㺯�� default = MANHATTAN
    int method;

    //�Ƿ�֧��б������ default = true
    bool corner;

    //���·���޷�����Ƿ񷵻������ default = true
    bool find_nearest;

    //��ͼ��ֵ����
    int (*on_block_test)( void* map, int x, int y, int mode);

    //����ÿһ���������¼�������false��������
    bool (*on_step)( const node* currentNode, const node* nearestNode, int step );

public:
    astar_pathfinder() :
        //m_map(),
        m_openlist(),
        m_step(),
        m_range(),
        instance(null),
        //unit(null),
        mode(COLLISION_BOOT),
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

    //����һ��������
    void add_teleport(int x, int y)
    {
    }

private:
    // �������λ�õ�ͼ��ѯ��Ϣ
    node* allocate(int x, int y)
    {
        memset(&m_map->data[y][x], 0, sizeof(node));
        return &m_map->data[y][x];
    }

    node* get_node( const point & pos )
    {
        return &m_map->data[pos.y][pos.x];
    }

    int get_state( const point & pos )
    {
        return m_state[pos.y][pos.x];
    }

    void set_state( const point & pos, int value )
    {
        m_state[pos.y][pos.x] = value;
    }

    void set_state( int x, int y, int value )
    {
        m_state[y][x] = value;
    }

    //��������Ƿ���Ч
    bool is_invalid_position1( const point & pos )
    {
        return pos.x < m_range.x || pos.y < m_range.y || pos.x >= m_range.get_right() || pos.y >= m_range.get_bottom();
        //return pos.x < 0 || pos.y < 0 || pos.x >= 1024 || pos.y >= 1024;
    }

    // ��������Ƿ���Ч
    bool is_valid_position( const point & pos )
    {
        //assert(pos.x >= 0 && pos.x < 1024 && pos.y >= 0 && pos.y < 1024);
//        if((pos.x >= 0 && pos.x < 1024 && pos.y >= 0 && pos.y < 1024)){
//            return true;
//        }
//        else return false;
        //return !is_invalid_position( pos );
        return pos.x >= m_range.x && pos.x < m_range.width && pos.y >= m_range.y && pos.y < m_range.height;
    }

    // ��õ�ͼλ�õ�������Ϣ
    int block_test( int x, int y, bool corner )
    {
        int n = on_block_test( instance, x, y, mode );
        return corner ? (n * 90) >> 6 : n;//����б����ֵ
    }


    // �ڵ��Ƿ��ڿ����б���
    inline bool is_in_open_list( const point & pos, node* &out )
    {
        out = get_node( pos );
        return out && get_state( pos ) == IN_OPENLIST;
    }


    // �ڵ��Ƿ��ڹر��б���
    inline bool is_in_close_list( const point & pos )
    {
        node* n = get_node( pos );
        return n && get_state( pos ) == IN_CLOSELIST;
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

    void final(path & path, astar_node<T>* node);
    void node_test(astar_node<T>* node, const vec2<T>& end);

};

template<typename T>
astar_pathfinder<T>::nodemap* astar_pathfinder<T>::m_map = new astar_pathfinder<T>::nodemap;

template<typename T>
char astar_pathfinder<T>::m_state[1024][1024] = {0};

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

    if( on_block_test( instance, target_pos.x, target_pos.y, mode ) < 0 ){//Ŀ�겻��ͨ��
        return false;
    }

    //б�������жϡ�
    //���б�Ƿ������߶���ש�飬��ͨ����ֻ��һ��ש��ͨ��
    //�������ͨ����ͼ�Ż�
    //2019/5/26 23:26:12
    if(corner){
        int direct = astar_direction(current_pos, target_pos);
        vec2i p1 = current_pos + astar_vector(direct - 1);
        vec2i p2 = current_pos + astar_vector(direct + 1);
//        if( ( on_block_test( instance, p1.x, p1.y, mode ) < 0 ) && ( on_block_test( instance, p2.x, p2.y, mode ) < 0 ) ){
//            return false;
//        }
        //б�ߣ���һ��ש���򶼲�ͨ��
        if( ( on_block_test( instance, p1.x, p1.y, mode ) < 0 ) || ( on_block_test( instance, p2.x, p2.y, mode ) < 0 ) ){
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
    node* target_node = allocate(target_pos.x, target_pos.y);

    target_node->parent = current_node;
    target_node->pos    = target_pos;
    target_node->h      = calc_h_value( target_pos, end_pos );
    target_node->g      = calc_g_value( current_node, target_pos );
    //target_node->state  = IN_OPENLIST;
    set_state( target_pos, IN_OPENLIST );

    m_openlist.push( target_node );
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
        if( on_block_test( instance, end.x, end.y, mode ) < 0 ){
            return -3;
        }
    }

    m_range = vec4<T>(0, 0, width, height);

    #if 0
    //��Χ����
    if( range ){
        //���ҷ�Χ����
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
    #endif

//    if( !m_range.area() ){//��Ч����
//        return -4;
//    }

    m_step = 0;

    // ��������openlist
    node* current_node  = allocate( start.x, start.y );
    set_state( start, IN_OPENLIST );
    current_node->h = calc_h_value( start, end ); //UINT16_MAX
    //current_node->g  = calc_g_value( current_node, start );
    current_node->pos   = start;

    node* nearest = current_node;//��¼�����

    m_openlist.push( current_node );

    //ÿһ��ѭ��������һ������
    while ( !m_openlist.empty() ){
        // ��openlist�е�����ӽ��յ�����꣬���ó�closelist
        current_node = (node*)m_openlist.top();
        m_openlist.pop();
        set_state( current_node->pos, IN_CLOSELIST );

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
    this->final( path, nearest );

    return -1;
}

template<typename T>
void astar_pathfinder<T>::dispose()
{
    memset(m_state, 0, sizeof(m_state));
    //m_openlist.clear();
    m_openlist = stack_type();
    //m_range = vec4<T>();
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
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

 当前坐标-目标坐标，等于方向0-7
 得到斜角方向1 3 5 7，(当前方向 + [ 1 | -1 ] ) & 7，得到临近方向
*/

struct ASTAR_DIR
{
    int x;
    int y;
    int direct;
};

//North北 Northeast东北 East东 Southeast东南 South南 Southwest西南 West西 Northwest西北
const ASTAR_DIR ASTAR_DIRECTION[8] = {
    {  0, -1,  0 }, {  1, -1,  1 }, {  1,  0,  2 }, {  1,  1,  3 },
    {  0,  1,  4 }, { -1,  1,  5 }, { -1,  0,  6 }, { -1, -1,  7 }
};

//const vec2i direction[8] = {
//        vec2i(  0, -1 ), vec2i( 0,  1 ), vec2i( -1, 0 ), vec2i( 1, 0 ),
//        vec2i( -1, -1 ), vec2i( 1, -1 ), vec2i( -1, 1 ), vec2i( 1, 1 )
//    };

//获得通向目标砖块的方向
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
//直走估值    10
//斜走估值    14
//斜走估值 = 直走估值 x sqprt(2) = 1.414

//根据坐标，返回地图阻值信息，数值越小，阻值越小
//默认阻止8，小于0无法通过，最大值建议小于128
//阻值过小，会导致搜索减缓，折回现象严重
//阻值是0的时候，蜕变成广域搜索，速度非常慢
//阻值不超过16，估值h的值最大不会超过65536(16*16)
//object传入一个自定义对象
//typedef int (*astar_callback)( void* map, int x, int y, void* object );

#pragma pack(push, 1)

//路径节点
//short类型 全图缓存
//
//sizeof()=18   64位 = 22
//1024  18M     22M
//512   4.7M    5.6M
//256   1.2M    1.4M
//128   288K    352K
//64    72K     88K

template<typename T>
struct astar_node
{
    vec2<T>    pos;     // 节点位置
    int    g;           // 与起点估值
    int h;           // 与终点估值
    short    state;   // 节点状态
    astar_node * parent; // 父节点

    astar_node() : pos(), g( 0 ), h( 0 ), state( 0 ), parent( null )
    {
    }

    // 计算f值
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
    //返回的路径数组
    typedef std::vector<point> path;

    // 路径节点状态
    enum NODE_STATE
    {
        NOTEXIST,       // 不存在
        IN_OPENLIST,    // 在开放列表（探索边缘）
        IN_CLOSELIST    // 在内部列表（探索过的区域）
    };

    enum {
        HEstimate = 2
    };

    enum METHOD
    {
        MANHATTAN,    //曼哈顿
        EUCLIDEAN    //欧几里德
    };

private:
    //std::vector<node*> m_map;
    array2d<node*> m_map;
    std::vector<node*> m_openlist;
    vec4<int> m_range;//搜索范围 2019/5/31 21:14
    int m_step;        //搜索步数
    astar_pool<node> pool;

public:
    void *instance;    //保存当前地图
    void *unit;    //保存当前角色

    //估值计算函数 default = MANHATTAN
    int method;

    //是否支持斜角搜索 default = true
    bool corner;

    //如果路径无法到达，是否返回最近点 default = true
    bool find_nearest;

    //地图阻值测试
    int (*on_block_test)( void* map, int x, int y, void* object );

    //搜索每一步产生的事件。返回false结束搜索
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

    // 一个坐标是否在另外一个坐标四个角上
    static inline bool is_corner( const point & pos1, const point & pos2 )
    {
        return (pos1.x != pos2.x) && (pos1.y != pos2.y);
    }

    //添加一个传送门
    void add_teleport(int x, int y)
    {
    }

private:
    // 获得坐标位置地图查询信息
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

    //检测坐标是否无效
    bool is_invalid_position( const point & pos )
    {
        return pos.x < m_range.x || pos.y < m_range.y || pos.x >= m_range.get_right() || pos.y >= m_range.get_bottom();
    }

    // 检测坐标是否有效
    bool is_valid_position( const point & pos )
    {
        return !is_invalid_position( pos );
    }

    // 获得地图位置的修正信息
    int block_test( int x, int y, bool corner )
    {
        int n = on_block_test( instance, x, y, unit );
        return corner ? (n * 90) >> 6 : n;//修正斜走阻值
    }


    // 节点是否在开放列表中
    inline bool is_in_open_list( const point & pos, node* &out )
    {
        out = get_node( pos );
        return out && out->state == IN_OPENLIST;
    }


    // 节点是否在关闭列表中
    inline bool is_in_close_list( const point & pos )
    {
        node* n = get_node( pos );
        return n && n->state == IN_CLOSELIST;
    }

    bool can_reach( const point & current_pos, const point & target_pos, bool corner );

    void handle_found_node( node* current_node, node* target_node );

    node* handle_not_found_node( node* current_node, const point & target_pos, const point & end_pos );

    // 计算G值，最小的G值被认可。
    inline int calc_g_value( node* parent_node, const point & current_pos )
    {
        int value = block_test( current_pos.x, current_pos.y, is_corner(parent_node->pos, current_pos) );
        return value += parent_node->g;
    }

    // 计算H值，距离终点的估值。这个结果影响搜索算法
    inline int calc_h_value( const point & pos, const point & end );

    //排序函数
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
    //曼哈顿距离
    //H(n) = D * (abs ( n.x – goal.x ) + abs ( n.y – goal.y ) )
    //对角线距离，貌似路径和曼哈顿相反，而且速度要慢一些
    //h(n) = D * max(abs(n.x - goal.x), abs(n.y - goal.y))
    //欧几里得距离，效果最好
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

// 判断坐标位置是否可以通过
template<typename T>
bool astar_pathfinder<T>::can_reach(
    const vec2<T> & current_pos,
    const vec2<T> & target_pos,
    bool corner )    //是否斜走
{
    if ( !is_valid_position( target_pos ) ){//范围约束之后，这句貌似没用了
        return false;
    }

    if ( is_in_close_list( target_pos ) ){
        return false;
    }

    //bool corner = is_corner( current_pos, target_pos );

    if( on_block_test( instance, target_pos.x, target_pos.y, unit ) < 0 ){//目标不可通过
        return false;
    }

    //斜角行走判断。
    //如果斜角方向两边都有砖块，则不通过；只有一个砖块通过
    //2019/5/26 23:26:12
    if(corner){
        int direct = astar_direction(current_pos, target_pos);
        vec2i p1 = current_pos + astar_vector(direct - 1);
        vec2i p2 = current_pos + astar_vector(direct + 1);
//        if( ( on_block_test( p1.x, p1.y, object ) < 0 ) && ( on_block_test( p2.x, p2.y, object ) < 0 ) ){
//            return false;
//        }
        //斜走，有一个砖块则都不通过
        if( ( on_block_test( instance, p1.x, p1.y, unit ) < 0 ) || ( on_block_test( instance, p2.x, p2.y, unit ) < 0 ) ){
            return false;
        }
    }

    return true;
}

// 处理openlist里面的查找到的节点
template<typename T>
void astar_pathfinder<T>::handle_found_node( astar_node<T>* current_node, astar_node<T>* target_node )
{
    int g = calc_g_value( current_node, target_node->pos );
    // 如果g可用，目标坐标的父坐标设置到当前坐标
    if ( g < target_node->g ) {
        target_node->g = g;
        target_node->parent    = current_node;
    }
}

// 查找到的目标坐标不在openlist里面，新的节点初始化
template<typename T>
astar_node<T>* astar_pathfinder<T>::handle_not_found_node(
    astar_node<T>* current_node, //查找的当前节点
    const vec2<T> & target_pos,  //周围节点位置
    const vec2<T> & end_pos )    //目标点
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
    // 检测行动的方向
    const static point direction[8] = {
        point(  0, -1 ), point( 0,  1 ), point( -1, 0 ), point( 1, 0 ),//直走
        point( -1, -1 ), point( 1, -1 ), point( -1, 1 ), point( 1, 1 ) //斜走
    };

    point  target_pos;
    size_t size = corner ? 8 : 4;
    astar_node<T>*  next_node;

    for ( size_t i = 0; i < size; ++i ){
        target_pos = point( current_node->pos.x + direction[i].x, current_node->pos.y + direction[i].y );
        if ( !can_reach( current_node->pos, target_pos, i & 4 ) ) { // 检测是否可以通过 i >= 4
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
    while ( node->parent ){//压入的起点，会使角色走回头路
        path.push_back( node->pos );
        node = node->parent;
    }
    //std::reverse( path.begin(), path.end() ); // 反转坐标并返回
}

template<typename T>
int astar_pathfinder<T>::find(
    astar_pathfinder<T>::path & path, //返回的路径节点坐标
    int width,            //地图大小
    int height,
    const point & start,//起点
    //const point & end,  //终点
    point end,  //终点
    int range,            //搜索范围
    int max_step)        //搜索最大步数
{
    this->dispose();
    path.clear();

    if( !on_block_test ){
        return -2;
    }

    if( !find_nearest ){
        //测试目标是否不可到达，如果不许要获得最近点，立刻返回
        if( on_block_test( instance, end.x, end.y, unit ) < 0 ){
            return -3;
        }
    }

    //范围测试
    m_range = vec4<T>(0, 0, width, height);

    if( range ){
        //查找范围宽度
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

        //范围测试之后，如果目标不可达，会搜索全部区域
        //终点范围约束。有个小问题，如果约束之后，新终点不可达，同样会全部搜索区域
        if( end.x < m_range.x )end.x = m_range.x;
        if( end.y < m_range.y )end.y = m_range.y;
        if( end.x >= m_range.get_right() )end.x = m_range.get_right() - 1;
        if( end.y >= m_range.get_bottom() )end.y = m_range.get_bottom() - 1;
    }

    if( !m_range.area() ){//无效矩形
        return -4;
    }

    m_step = 0;
    //m_map.resize( width, height ); //建立一个当前地图大小的缓冲区
    m_map.resize( m_range.width, m_range.height );

    // 将起点放入openlist
    //node* current_node  = get_node( start );
    node* current_node  = pool.allocate();//new node;// get_node( start );
    set_node(start, current_node);

    current_node->state = IN_OPENLIST;
    current_node->h = calc_h_value( start, end ); //UINT16_MAX
    //current_node->g  = calc_g_value( current_node, start );
    current_node->pos   = start;

    node* nearest = current_node;//记录最近点

    m_openlist.push_back( current_node );

    //每一次循环，查找一个坐标
    while ( !m_openlist.empty() ){
        // 从openlist中弹出最接近终点的坐标，设置成closelist
        current_node = *m_openlist.begin();
        std::pop_heap( m_openlist.begin(), m_openlist.end(), compare_node);
        m_openlist.pop_back();
        current_node->state = IN_CLOSELIST;

        // 查找距离终点最近的点
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
        if(m_step >= max_step){//最大步数终止
            break;
        }
    }

    //路径未找到，返回距离终点最近的点
    while ( nearest->parent ){
        path.push_back( nearest->pos );
        nearest = nearest->parent;
    }
    //std::reverse( path.begin(), path.end() ); // 反转坐标并返回

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

F值计算

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

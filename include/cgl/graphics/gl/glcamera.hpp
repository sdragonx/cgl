/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glcamera.hpp

 2019-05-20 16:04:30

 2006年10月8日，15:58:52

 45度斜视视觉
 x = 80
 y = 315
 z = 0

 俯视2D
 x = 90
 y = 0
 z = 0

*/
#ifndef GLCAMERA_HPP_20061008155852
#define GLCAMERA_HPP_20061008155852

#include <cgl/math.hpp>
#include <cgl/graphics/gl/glcontext.hpp>
#include <cgl/system/keyboard.hpp>
#include <cgl/vec.hpp>

namespace cgl{
namespace graphics{

//
// tilemap_height(map, width, height, x, z) 计算3维地图插值高度
// 
// map      地图
// width    地图大小
// height
// x        位置
// z
// 返回值：xz位置y高度
//
template<typename T>
float tilemap_height(const T& map, int width, int height, GLfloat x, GLfloat z)
{
    float h;

    if(x < 0)x = 0;
    if(z < 0)z = 0;
    if(x >= width - 1)x = width - 1;
    if(z >= height - 1)z = height - 1;

    int Xa = (int)x;
    int Za = (int)z;
    float Xb = x - Xa;
    float Zb = z - Za;

    float a = map[Za  ][Xa  ].y;
    float b = map[Za  ][Xa+1].y;
    float c = map[Za+1][Xa  ].y;
    float d = map[Za+1][Xa+1].y;
    h= (a * (1.0f - Xb) + b * Xb) * (1.0f - Zb) + (c * (1.0f - Xb) + d * Xb) * Zb;
    return h;
}

/*
void draw_billboard1(glcontext& gl, const graphics::teximage* image,
    float x, float y, float z,
    float size_x, float size_y)
{
    gl.push_matrix();
    gl.translate(x, y, z);
    gl.rotate(-camera.angleY, 0, 1, 0);
    gl.rotate(-camera.angleX, 1, 0, 0);

    float u1=image->u1, u2=image->u2, v1=image->v1,v2=image->v2;
    vec4ub color = gl.color;
    gl.draw_image(image->image, -size_x * 0.5f, -size_y * 0.5f, size_x, size_y,
        image->u1, image->v2, image->u2, image->v1);
    gl.pop_matrix();
}


//x轴不旋转
void draw_billboardY(graphics::glcontext& gl,
    const graphics::teximage* image,
    float x, float y, float z, float size_x, float size_y)
{
    float u1 = image->u1;
    float u2 = image->u2;
    float v1 = image->v2;
    float v2 = image->v1;
    vec4ub color = gl.color;

    size_x *= 0.5f;
    size_y *= 0.5f;

    float sinval = math::fast_sin(-camera.angleY);
    float cosval = math::fast_cos(-camera.angleY);
    float cosx = size_x * cosval;
    float sinx = size_x * sinval;

    vtx3f vtx[] = {
        vtx3f(x - cosx, y - size_y, z + sinx, u1, v1, color),
        vtx3f(x + cosx, y - size_y, z - sinx, u2, v1, color),
        vtx3f(x + cosx, y + size_y, z - sinx, u2, v2, color),
        vtx3f(x - cosx, y + size_y, z + sinx, u1, v2, color)
    };

    gl.bind_texture(image->image);
    gl.draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);
}
*/

//---------------------------------------------------------------------------
//观察模式
enum CAMERA_MODE
{//观察模式，默认为自由视觉 
    CAMERA_MODE_FREE     = 0,    //自由视觉
    CAMERA_MODE_FIRST    = 1,    //第一人称 first person
    CAMERA_MODE_THIRD    = 2,    //第三人称追尾视觉（锁定一个单位）
    CAMERA_MODE_RTS      = 3,    //斜45度视觉 像魔兽的俯视视觉
    CAMERA_MODE_OVERLOOK = 4     //俯视视觉overlook
};

//键值映射
struct CAMERA_KEYMAP
{//摄影机的按键结构，可以自定义这些按键
    char lookUp;
    char lookDown;
    char lookLeft;
    char lookRight;
    char forward;
    char forward2;
    char back;
    char back2;
    char left;          //turnLeft
    char right;         //turnRight
    char translateLeft; //左平移
    char translateRight;//右平移
    char jump;          //跳
    char squat;         //蹲
    //grovel 趴,匍匐,卑躬屈膝
};

//动作
enum CAMERA_ACTION
{
    CAMERA_LOOK_UP = 0,
    CAMERA_LOOK_DOWN,
    CAMERA_LOOK_LEFT,
    CAMERA_LOOK_RIGHT,
    CAMERA_FORWARD,
    CAMERA_BACK,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_TRANSLATE_LEFT,
    CAMERA_TRANSLATE_RIGHT,
    CAMERA_JUMP,
    CAMERA_SQUAT,
};

/*
void LookAt(float atx, float aty, float atz,
            float anglex, float angley, float anglez)
{
    glRotatef(anglex, 1.0f, 0.0f, 0.0f);
    glRotatef(angley, 0.0f, 1.0f, 0.0f);
    glRotatef(anglez, 0.0f, 0.0f, 1.0f);
    glTranslatef(-atx, -aty, -atz);//这里移动的是负值，坐标系才正确
}
*/

template<typename Context>
void LookAt(Context& dc, float atx, float aty, float atz,
            float anglex, float angley, float anglez)
{
    dc.rotate(anglex, 1.0f, 0.0f, 0.0f);
    dc.rotate(angley, 0.0f, 1.0f, 0.0f);
    dc.rotate(anglez, 0.0f, 0.0f, 1.0f);
    dc.translate(-atx, -aty, -atz);//这里移动的是负值，坐标系才正确
}

class glcamera
{
private:
    vec3f vec;              //运动向量
    CAMERA_KEYMAP keymap;

public:
    CAMERA_MODE viewmode;

    vec3f position;         //位置

    //angleX=180为平视，<180 && >90为仰视，>180 && <270为俯视
    //float angleX, angleY, angleZ;
    vec3f view;

    vec4f const viewport;    //视口

    float move_speed; //移动速度
    float turn_speed; //转动速度

    //视距
    //float Stadia;
    //float lock_rts_view;//锁定rts俯视视觉
    //-1为不锁
//    vec3f lock_view;
//    vec3f lock_position;
public:
    glcamera();
    ~glcamera();

//    void Create(const CAMERA_KEYMAP &Key);
//    void SetKeyBoard(const CAMERA_KEYMAP& Key);

    int bind_key(CAMERA_ACTION action, int key);

    //执行，应用到OpenGL世界转换
    template<typename Context>
    void execute(Context& dc);

    void move(float x, float y, float z);
    void look(float x, float y, float z);
    void lookat(float x, float y, float z,
        float dx, float dy, float dz);

    void look_left(float angle);     //左转
    void look_right(float angle);    //右转
    void look_up(float angle);       //仰视
    void look_down(float angle);     //俯视

    void aslant(float angle)   //斜视
    {
        view.z = angle;
    }

    //前进
    void forward()
    {
        position.x -= vec.x;
        position.z -= vec.z;
        position.y += vec.y;
    }

    //后退
    void back()
    {
        position.x += vec.x;
        position.z += vec.z;
        position.y -= vec.y;
    }

    //左移
    void translateLeft()
    {
        position.x -= vec.z;
        position.z += vec.x;
    }

    //右移
    void translateRight()
    {
        position.x += vec.z;
        position.z -= vec.x;
    }

    //上移
    void move_up()
    {
        position.y += move_speed;
    }

    //下移
    void move_down()
    {
        position.y -= move_speed;
    }

    //锁定视觉
    //    void lock_view(float x, float y, float z);
    //    void lock_position(float x, float y, float z);

//消息
    void update();
    void touch(float x, float y, float scale = 0.05f);

    template<typename Context>
    void print_info(Context& gl, int x, int y)
    {
        char buf[64];
        snprintf(buf, 64, "angle = %0.2f, %0.2f, %0.2f", view.x, view.y, view.z);
        gl.print(x, y, buf, cslen(buf));

        snprintf(buf, 64, "at    = %0.2f, %0.2f, %0.2f", position.x, position.y, position.z);
        gl.print(x, y + 20, buf, cslen(buf));
    }

private:
    void onLookup();
    void onLookupY();

    void calc_vector();    //计算移动向量
};
//---------------------------------------------------------------------------

glcamera::glcamera()
{
    viewmode = CAMERA_MODE_FREE;

    move_speed = 0.05f;
    turn_speed = 1.0f;

    this->lookat(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    keymap.forward = 'W';
    keymap.forward2 = key_up;
    keymap.back = 'S';
    keymap.back2 = key_down;
    keymap.left = key_left;//'q';
    keymap.right = key_right;//'e';
    keymap.jump = key_space;
    keymap.translateLeft = 'A';
    keymap.translateRight = 'D';
    keymap.squat = key_shift;
}

glcamera::~glcamera()
{
}

int glcamera::bind_key(CAMERA_ACTION action, int key)
{
    //switch(action){
    //}
    return 0;
}

template<typename Context>
void glcamera::execute(Context& dc)
{
    dc.identity();
    LookAt(dc, position.x, position.y, position.z, view.x, view.y, view.z);
}

void glcamera::update()
{
    //speed*mouseview/10.0f;   //键盘转身速度暂时定为viewspeed的二分之一
    if(keyboard[keymap.left]){
        this->look_left(turn_speed);
    }else if(keyboard[keymap.right]){
        this->look_right(turn_speed);
    }

    if(keyboard[keymap.forward] || keyboard[keymap.forward2]){
        this->forward();
    }else if(keyboard[keymap.back] || keyboard[keymap.back2]){
        this->back();
    }

    if(keyboard[keymap.translateLeft]){
        this->translateLeft();
    }else if(keyboard[keymap.translateRight]){
        this->translateRight();
    }
    
    if(keyboard[keymap.jump]){
        this->move_up();
    }
    else if(keyboard[keymap.squat]){
        this->move_down();
    }

    /*
    if(keys[keymap.squat] && !dun){//蹲下动作
        dun=true;
        position.y-=15;
    }else if(!key.squat && dun){
        position.y+=15;
        dun=false;
    }//*/

}

void glcamera::calc_vector()
{
    //角度xy变化的时候，对移动量有变化
    float ax = view.x * M_RD;
    float ay = view.y * M_RD;
    //float az = view.z * M_RD;
    float sineX = sin(ax);
    float sineY = sin(ay);
    //float sineZ = sin(az);
    float cosineX = cos(ax);
    float cosineY = cos(ay);
    //float cosineZ = cos(az);

    float vxsin, vxcos;
    switch(viewmode)
    {
    case CAMERA_MODE_FREE:
        vxcos = move_speed * cosineX;
        vxsin = move_speed * -sineX;
        vec.x = vxcos * -sineY;
        vec.y = vxsin;
        vec.z = vxcos * cosineY;
        break;
    case CAMERA_MODE_FIRST:
        vec.x = move_speed * -sineY;
        vec.y = 0.0f;
        vec.z = move_speed * cosineY;
        break;
    case CAMERA_MODE_THIRD:
        vec.x = -move_speed;
        vec.y = 0.0f;
        vec.z = -move_speed;
        //view.x=45;
        //position.y=7;
        break;
    case CAMERA_MODE_RTS:
        vec.x = move_speed * -sineY;
        vec.y = 0.0f;
        vec.z = move_speed * cosineY;
        break;
    case CAMERA_MODE_OVERLOOK:
        vec.x = move_speed * -sineY;
        vec.y = 0.0f;
        vec.z = move_speed * cosineY;
        break;
    default:
        break;
    }
}

void glcamera::onLookup()
{
    if(viewmode == CAMERA_MODE_RTS){
        view.x = 45.0f;
    }
    else if(viewmode == CAMERA_MODE_OVERLOOK){
        view.x = 90.0f;
    }

    else if(360.0f < view.x){
        view.x -= 360.0f;
    }
    else if(view.x<0.0f){
        view.x += 360.0f;
    }

    //如果这里等于270，下面等于90，使视觉垂直，就会黑屏
    if(180.0f < view.x && view.x < 270.0f){
        view.x = 270.0f;
    }
    else if(view.x < 180.0f && 90.0f < view.x){
        view.x = 90.0f;
    }

    onLookupY();
}

//只有Y轴旋转对移动向量有影响
void glcamera::onLookupY()
{
    if(360.0f < view.y){
        view.y -= 360.0f;
    }
    else if(view.y < 0.0f){
        view.y += 360.0f;
    }
    //从新计算移动向量
    calc_vector();
}

void glcamera::move(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void glcamera::look(float x, float y, float z)
{
    view.x = x;
    view.y = y;
    view.z = z;
    onLookup();
}

void glcamera::lookat(float x, float y, float z, float dx, float dy, float dz)
{
    this->move(x, y, z);
    this->look(dx, dy, dz);
}

void glcamera::touch(float x, float y, float scale)
{
    view.x -= y * scale;
    view.y -= x * scale;

    onLookup();
}

void glcamera::look_left(float angle)
{
    view.y -= angle;
    onLookupY();
}

void glcamera::look_right(float angle)
{
    view.y += angle;
    onLookupY();
}

void glcamera::look_up(float angle)
{
    view.x -= angle;
}

void glcamera::look_down(float angle)
{
    view.x += angle;
}

//---------------------------------------------------------------------------


//绘制图像的时候，先要把坐标移动到摄像头的起点
//dc.translate(-currentWorld->camera.viewport.x, -currentWorld->camera.viewport.y);

//摄像头
class camera2d
{
private:
    vec2i m_screen;
    vec2i m_world;
    int m_tilesize;
public:
    vec3f const position;    //位置
    vec4f const viewport;    //视口

    float zoom_min;
    float zoom_max;

    //screen_edge screen_center
    int lock_mode;//视口约束方式   boundary_centered screen_boundary

    void* instance;
    void (*on_lookat)(void* instance, float x, float y, float z);
public:
    camera2d() :
        m_screen(1024, 768),
        m_world(),
        m_tilesize(128),
        position(0.0f, 0.0f, 1.0f),    //默认z位置为1.0
        viewport(0.0f, 0.0f, m_screen.width * position.z, m_screen.height * position.z),

        zoom_min(0.5f),
        zoom_max(32.0f),
        on_lookat()
    {

    }

    void init(int screen_width, int screen_height, int world_width, int world_height, int tilesize)
    {
        m_screen = vec2i(screen_width, screen_height);
        m_world = vec2i(world_width, world_height);
        m_tilesize = tilesize;
    }

    template<typename Context>
    void push_view(Context& dc)
    {
        dc.push_matrix();
        dc.push_view2D(0, 0, viewport.width, viewport.height);
        dc.translate(-viewport.x, -viewport.y, 10.0f);
    }

    template<typename Context>
    void pop_view(Context& dc)
    {
        dc.pop_view();
        dc.pop_matrix();
    }

    void lookat(float x, float y, float z = -1.0f)
    {
        if(z < 0.0f)z = position.z;
        //缩放约束
        if(z < zoom_min)z = zoom_min;
        if(z > zoom_max)z = zoom_max;

        //视口位置
        const_write(viewport).width = m_screen.width * z;
        const_write(viewport).height = m_screen.height * z;
        const_write(viewport).x = x - viewport.width * 0.5f;
        const_write(viewport).y = y - viewport.height * 0.5f;

        //世界尺寸
//        float world_width = boxworld_width(world);
//        float world_height = boxworld_height(world);

        //视口范围约束
        if(lock_mode == 0){
        }
        else if(lock_mode == 1){//screen_boundary
            //世界边界对齐到屏幕边界，如果世界大小小于屏幕，则居中
            if(viewport.width < m_world.width){
                if(viewport.x < 0){
                    const_write(viewport).x = 0;
                }
                else if(viewport.x + viewport.width >= m_world.width){
                    const_write(viewport).x = m_world.width - viewport.width;
                }
            }
            else{
                const_write(viewport).x = (m_world.width - viewport.width) * 0.5f;
            }

            if(viewport.height < m_world.height){
                if(viewport.y < 0){
                    const_write(viewport).y = 0;
                }
                else if(viewport.y + viewport.height >= m_world.height){
                    const_write(viewport).y = m_world.height - viewport.height;
                }
            }
            else{
                const_write(viewport).y = (m_world.height - viewport.height) * 0.5f;
            }
        }
        else if(lock_mode == 2){//boundary_centered
            //世界边界可以对齐到屏幕中心，砖块可以居中聚焦
            vec2f half_vp(viewport.width / 2.0f, viewport.height / 2.0f);
            float half_tile = m_tilesize / 2.0f;

            if(viewport.x < -half_vp.width + half_tile){
                const_write(viewport).x = -half_vp.width + half_tile;
            }
            if(viewport.x >= m_world.width - half_vp.width - half_tile){
                const_write(viewport).x = m_world.width - half_vp.width - half_tile;
            }

            if(viewport.y < -half_vp.height + half_tile){
                const_write(viewport).y = -half_vp.height + half_tile;
            }
            if(viewport.y >= m_world.height - half_vp.height - half_tile){
                const_write(viewport).y = m_world.height - half_vp.height - half_tile;
            }
        }

        //更新视口位置
        const_write(position) = vec3f(viewport.x + viewport.width * 0.5f, viewport.y + viewport.height * 0.5f, z);
        if(on_lookat)on_lookat(instance, position.x, position.y, position.z);
    }

    void lookat(const vec3f& pos)
    {
        return this->lookat(pos.x, pos.y, pos.z);
    }

    void move(float x, float y, float z = 0.0f)
    {
        this->lookat(position.x + x, position.y + y, position.z + z);
    }

    void zoom_in()//放大，拉近镜头
    {
        this->lookat(position.x, position.y, position.z * 1.05f);// + WORLD_Z_ZOOM_SETP);
    }

    void zoom_out()//缩小
    {
        this->lookat(position.x, position.y, position.z * 0.95f);//- WORLD_Z_ZOOM_SETP);
    }

    float zoom()const//缩放
    {
        return position.z;
    }

    //拖动窗口时视口移动，根据z值缩放速度
    void touch(float x, float y)
    {
        vec2f offset = vec2f(x, y);
        offset *= position.z;
        this->move(offset.x, offset.y);
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //GLCAMERA_HPP_20061008155852


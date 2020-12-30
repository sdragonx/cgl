/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 box2d_world.hpp

 2017-01-02 113014

*/
#ifndef BOX2D_WORLD_HPP_20170102113014
#define BOX2D_WORLD_HPP_20170102113014

#include <vector>
#include <cgl/graphics.hpp>
#include <cgl/math.hpp>

//#define CGL_BOX2D_STATIC_LIB

#include <cgl/external/box2d.hpp>

extern GLuint tex;

namespace cgl{

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
const float PTM_RATIO = 10.0f;    //一米10个单位


//const b2Filter FILTER_TITLE = {-1, 1, 0xFF};
//const b2Filter FILTER_ANIMATION = {1, 1, 0xFF};

int draw_polygon(const b2Vec2* vec, int count, b2Color color)
{
    glColor3f(color.r*1.0, color.g, color.b);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    for(int i=0; i<count; ++i){
        glVertex2f(vec[i].x, vec[i].y);
    }
    glEnd();
    return 0;
}

int draw_line(int x1, int y1, int x2, int y2, b2Color color)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
    return 0;
}

int draw_circle(float x, float y, float radius, b2Color color)
{
    const float points[] = {
        1.000000f, 0.000000f, 0.923880f, -0.382683f, 0.707107f, -0.707107f, 0.382683f, -0.923880f,
        0.000000f, -1.000000f, -0.382683f, -0.923880f, -0.707107f, -0.707107f, -0.923880f, -0.382683f,
        -1.000000f, 0.000000f, -0.923880f, 0.382683f, -0.707107f, 0.707107f, -0.382683f, 0.923880f,
        0.000000f, 1.000000f, 0.382683f, 0.923880f, 0.707107f, 0.707107f, 0.923880f, 0.382683f
        };//1.000000f, 0.000000f,
    /*
    //glColor4f(color.r, color.g, color.b, 0.75);
    //glColor4f(0, 0.5, 0.7, 0.5);
    glBindTexture(GL_TEXTURE_2D, tex);
    radius*=0.5;
    gl.glRectf(x-radius, y-radius, x+radius, y+radius, 0, 0, 1, 1);

    /*/
    #if 1
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4f(0, 0, 1, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    radius /= 2.0f;
    glBegin(GL_QUADS);
        glVertex2f(x-radius, y-radius);
        glVertex2f(x+radius, y-radius);
        glVertex2f(x+radius, y+radius);
        glVertex2f(x-radius, y+radius);
    glEnd();
    #else
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(radius, radius, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(float)*2, points);
    glDrawArrays(GL_LINE_LOOP, 0, 16);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    #endif
    //*/

    return 0;
}

//绘图器
class box2D_draw:public b2Draw
{
public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        draw_polygon(vertices, vertexCount, color);
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        draw_polygon(vertices, vertexCount, color);
    }

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    {
        draw_circle(center.x, center.y, radius, color);
    }

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
    {
        draw_circle(center.x, center.y, radius, color);
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
        draw_line(p1.x, p1.y, p2.x, p2.y, color);
    }

    void DrawTransform(const b2Transform& xf)
    {
        //print("DrawTransform");
    }

    virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
    {
        draw_circle(p.x, p.y, size, color);
    }
};

//创建基本形状
b2Body* box2d_createBox(b2World* world, float x, float y, float width, float height, bool is_dynamic,
    int group, int category, int mask)
{
    b2Body* body = NULL;
    b2BodyDef body_def;

    body_def.type = is_dynamic ? b2_dynamicBody : b2_staticBody;
    body_def.position.Set(x, y);
    body_def.allowSleep = true;     //允许睡眠
    body_def.fixedRotation = true;  //不旋转
    body_def.linearDamping = 5.5f;  //重力衰减
    //body_def.angularDamping = 10.0f;
    //body_def.linearVelocity = b2Vec2(0, 10.0f);//重力，会砸掉其他砖块
    body = world->CreateBody(&body_def);

    b2PolygonShape shape;
    b2FixtureDef fixture_def;

    shape.SetAsBox(width, height);

    fixture_def.shape = &shape;         //模型绑定
    fixture_def.density = 1.0f;         //密度
    //fixture_def.isSensor = false;     //无法碰撞
    //fixture_def.restitution = 0.8f;   //弹力
    fixture_def.friction = 0.5f;        //摩擦力

    //碰撞过滤
    fixture_def.filter.groupIndex = group;
    fixture_def.filter.categoryBits = category;
    fixture_def.filter.maskBits = mask;
    b2Fixture* fixture = body->CreateFixture(&fixture_def);
    body->SetGravityScale(10.0f);
    body->ResetMassData();

    //b2MassData mass;
    //body->SetTransform( b2Vec2( 10, 20 ), 1 );
    //body->SetLinearVelocity(b2Vec2( 0, 10 ));//moving up and left 5 units per second
    //body->SetAngularVelocity(-90 * DEGTORAD);//90 degrees per second clockwise

    //永远保持正向矩形
    //body->SetFixedRotation(true);

    return body;
}

b2Body* box2d_createTile(b2World* world, float x, float y, float width, float height, bool is_dynamic,
    int group, int category, int mask)
{
    b2Body* body = NULL;
    b2BodyDef body_def;

    //创建body
    body_def.type = is_dynamic ? b2_dynamicBody : b2_staticBody;
    body_def.position.Set(x, y);
    body_def.allowSleep = true;     //允许睡眠
    body_def.fixedRotation = true;  //不旋转
    body_def.linearDamping = 5.5f;  //重力衰减
    //body_def.angularDamping = 10.0f;
    //body_def.linearVelocity = b2Vec2(0, 10.0f);//重力，会砸掉其他砖块
    body = world->CreateBody(&body_def);

    //创建碰撞边框
    b2EdgeShape shape;
    b2FixtureDef fixtureDef;

    fixtureDef.density = 2.0f;      //密度
    fixtureDef.friction = 0.0f;     //摩擦力，不然英雄会蹭在墙上
    fixtureDef.restitution = 0.125f;//弹力
    //碰撞过滤
    fixtureDef.filter.groupIndex = group;
    fixtureDef.filter.categoryBits = category;
    fixtureDef.filter.maskBits = mask;

    //top
    shape.Set(b2Vec2(-0.5f*PTM_RATIO, -0.5f*PTM_RATIO), b2Vec2(0.5f*PTM_RATIO, -0.5f*PTM_RATIO));
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    //bottom
    shape.Set(b2Vec2(-0.5f*PTM_RATIO, 0.5f*PTM_RATIO), b2Vec2(0.5f*PTM_RATIO, 0.5f*PTM_RATIO));
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    //left
    shape.Set(b2Vec2(-0.5f*PTM_RATIO, -0.4f*PTM_RATIO), b2Vec2(-0.5f*PTM_RATIO, 0.4f*PTM_RATIO));
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    //right
    shape.Set(b2Vec2(0.5f*PTM_RATIO, -0.4f*PTM_RATIO), b2Vec2(0.5f*PTM_RATIO, 0.4f*PTM_RATIO));
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    body->ResetMassData();

    return body;
}

b2Body* box2d_createCircle(b2World* world, float x, float y, float radius, bool is_dynamic,
    int group, int category, int mask)
{
    b2Body* body = NULL;
    b2BodyDef body_def;
    b2CircleShape  shape;
    b2FixtureDef fixture_def;

    body_def.type = is_dynamic ? b2_dynamicBody : b2_staticBody;
    body_def.position.Set(x, y);
    body_def.allowSleep = true;     //允许睡眠
    body_def.fixedRotation = true;  //不旋转
    body_def.linearDamping = 5.5f;  //重力衰减
    //body_def.angularDamping = 10.0f;
    //body_def.linearVelocity = b2Vec2(0, 10.0f);//重力，会砸掉其他砖块
    body = world->CreateBody(&body_def);

    //shape.m_p = b2Vec2(4, 4);// .SetAsBox(width, height);
    shape.m_radius = radius;

    fixture_def.shape = &shape;
    fixture_def.density = 1.0f;//width*height;    //密度
    //fixture_def.isSensor = false;
    //fixture_def.restitution = 0.8f;   //弹力
    fixture_def.friction = 0.5f;        //摩擦力

    //碰撞过滤
    fixture_def.filter.groupIndex = group;
    fixture_def.filter.categoryBits = category;
    fixture_def.filter.maskBits = mask;
    b2Fixture* fixture = body->CreateFixture(&fixture_def);
    body->SetGravityScale(10.0f);
    return body;
}


//重载查询类
class box2d_query: public b2QueryCallback
{
public:
    std::vector<b2Body*>* bodys;
    bool query_dynamic;
    bool query_static;
public:
    box2d_query(std::vector<b2Body*>* vec, bool _dynamic = true, bool _static = true)
        :bodys(vec), query_dynamic(_dynamic), query_static(_static)
    {

    }
    ~box2d_query()
    {
    }

    bool ReportFixture(b2Fixture* fixture)
    {
        if(fixture->GetBody()->GetType() == b2_dynamicBody)
        {
            if(query_dynamic)bodys->push_back(fixture->GetBody());
        }
        else
        {
            if(query_static)bodys->push_back(fixture->GetBody());
        }
        return true;
    }
};


class box2d_world
{
public:
    b2World m_world;
    box2D_draw m_draw;
    int m_width;
    int m_height;
    b2Body* map[256][256];
    b2Joint* mouseJoint;
    b2Body*  mouseBody;
public:
    box2d_world():m_world(b2Vec2(0.0f, 10.0f))
    {
        m_draw.SetFlags(
            b2Draw::e_shapeBit
            |b2Draw::e_jointBit
            //|b2Draw::e_aabbBit
            //|b2Draw::e_pairBit
            //|b2Draw::e_centerOfMassBit
            );
        m_world.SetDebugDraw(&m_draw);
        //m_world->SetContinuousPhysics(false);
        m_world.SetAllowSleeping(true);

        memset(map, 0, sizeof(map));
        mouseJoint = null;
        mouseBody = null;

    }
    ~box2d_world()
    {
        //delete m_world;
        //m_world = 0;
    }

    int set_size(int width, int height)
    {
        m_width = width;
        m_height = height;
        int n = 0;
        b2Body* body;

        for(int x=0; x<m_width; ++x)
        {
            body = create_tile(x*PTM_RATIO, PTM_RATIO, PTM_RATIO, PTM_RATIO);
        }

        for(int y=10; y<m_height; ++y)
        {
            for(int x=0; x<m_width; ++x)
            {
                body = 0;
                if(y<20){
                    n = random(100);
                    if(n<30)
                    {
                        body = create_tile(x*PTM_RATIO, y*PTM_RATIO, PTM_RATIO, PTM_RATIO);
                        if(n<30){
                            //body->GetFixtureList()->SetSensor(true);
                        }
                    }
                }
                else{
                    body = create_tile(x*PTM_RATIO, y*PTM_RATIO, PTM_RATIO, PTM_RATIO);
                }
                map[y][x] = body;
                //body->GetFixtureList()->SetDensity(2.0f);
            }
        }

        mouseBody = create_box(0, 0, PTM_RATIO, PTM_RATIO);
        mouseBody->SetType(b2_staticBody);//b2_dynamicBody);//b2_kinematicBody);
    }

    b2Body* create_box(float x, float y, float width, float height)
    {
        b2Body* body = box2d_createBox(&m_world, x+width/2.0f, y+height/2.0f, width/2.0f, height/2.0f, true, -1, 2, 3);
        body->GetFixtureList()->SetDensity(0.8f);
        body->ResetMassData();
        return body;
    }

    b2Body* create_tile(float x, float y, float width, float height)
    {
        b2Body* body = box2d_createTile(&m_world, x+width/2.0f, y+height/2.0f, width/2.0f, height/2.0f, false, 1, 1, 3);
        return body;
    }

    b2Body* create_circle(float x, float y, float radius)
    {
        b2Body* body = box2d_createCircle(&m_world, x, y, radius, true, 0, 0, 0);
        body->GetFixtureList()->SetDensity(1.0f);
        body->ResetMassData();
        return body;

    }

    void step()
    {
        float timeStep = 1.0f/60.0f;//40/1000.0f;  //the length of time passed to simulate (seconds)
        int velocityIterations = 8;  //how strongly to correct velocity
        int positionIterations = 1; //how strongly to correct position

        m_world.Step(timeStep, velocityIterations, positionIterations);
    }

    void paint()
    {
        m_world.DrawDebugData();
    }

    size_t query_box(std::vector<b2Body*>& ls, float x, float y, float width, float height, bool is_dynamic, bool is_static);
    size_t query_box(std::vector<b2Body*>& ls, const b2AABB& aabb, bool is_dynamic, bool is_static);
};

size_t box2d_world::query_box(std::vector<b2Body*>& ls, float x, float y, float width, float height, bool is_dynamic, bool is_static)
{
    b2AABB ab;
    ab.lowerBound.Set(x, y);
    ab.upperBound.Set(x+width, y+height);
    return query_box(ls, ab, is_dynamic, is_static);
}

size_t box2d_world::query_box(std::vector<b2Body*>& ls, const b2AABB& aabb, bool is_dynamic, bool is_static)
{
    box2d_query query(&ls, is_dynamic, is_static);
    m_world.QueryAABB(&query, aabb);
    return ls.size();
}



}//end namespace cgl

#endif BOX2D_WORLD_HPP_20170102113014

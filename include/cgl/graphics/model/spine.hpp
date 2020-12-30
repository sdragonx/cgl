/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spine.h

 2016-12-17 17:23:46

 对spine api的封装，不自动释放资源

*/
#ifndef SPINE_H_20161217172346
#define SPINE_H_20161217172346

#if defined(CGL_PLATFORM_CB2010)
#define CGL_SPINE_STATIC_LIB
#endif

#include <cgl/public.h>
#include <cgl/string.hpp>
#include <cgl/graphics/draw_context.hpp>
#include <cgl/graphics/teximage.hpp>
//#include <cgl/graphics/gl/glcontext.hpp>
#include <cgl/external/spinelib2.1025.hpp>

namespace cgl{

using namespace graphics;
using namespace spine;

#ifndef SPINE_WORLD_VERTEX_BUFFER
    #define SPINE_WORLD_VERTEX_BUFFER 512
#endif

//这个函数需要外部实现
extern graphics::teximage* spine_LoadImage(const cstring<char>& imageFile);

//从json加载模型
//path末位字符要加'/'
spSkeletonData* skeletonData_LoadFromJson(const char* path, const char* jsonFile, float scale);
spSkeletonData* skeletonData_LoadFromJsonData(const char* path, const char* jsonData, float scale);

//获得包围盒
vec4f skeleton_getBoundingBox(spSkeleton* skeleton, float scaleX = 1.0f, float scaleY = 1.0f);

//绘制
//返回true，表示继续绘制；false，不再绘制
typedef bool (*slot_on_paint)(draw_context& dc, spSlot* slot, void* owner);
int skeleton_paint(draw_context& dc, spSkeleton* skeleton, bool flipX, bool flipY, slot_on_paint on_paint, void* owner, float worldVertices[]);
int skeleton_drawBones(draw_context& dc, spSkeleton* skeleton, bool flipX, bool flipY, bool boneName);

//自定义皮肤函数
spSkin* skin_create(const char* name);
//创建并复制一个皮肤
spSkin* skin_clone(const char* name, spSkin* skeleton_skin);
spSkin* skin_clone(const char* name, spSkeletonData* skeletonData, const char* skin_name);
void skin_dispose(spSkin* self);
//spSkin* skin_appendAttachment(spSkin* skin, int slotIndex, spAttachment* attachment);

//合并两个皮肤，CTW局部换肤
spSkin* skin_append(spSkin* skin, spSkeletonData* skeletonData, const char* skin_name);

#if 0
class spine_slot
{
public:
    spSlot* const handle;
public:
    spine_slot():handle(null) { /*void*/ }
    spine_slot(spSlot* slot):handle(slot) { /*void*/ }

    spine_slot& operator=(spSlot* slot)
    {
        const_value(handle) = slot;
        return *this;
    }

    bool is_null()const
    {
        return !handle;
    }

    operator bool()const
    {
        return handle;
    }

    vec4f color()const
    {
        CGL_ASSERT(!is_null());
        return vec4f(handle->r, handle->g, handle->b, handle->a);
    }

    void color(float red, float green, float blue, float alpha)
    {
        handle->r = red;
        handle->r = red;
        handle->g = green;
        handle->b = blue;
        handle->a = alpha;
    }

    spSlotData* data()
    {
        return handle->data;
    }

    spBone* bone()
    {
        return handle->bone;
    }

    spAttachment* attachment()
    {
        return handle->attachment;
    }

    int paint(draw_context& dc)
    {
        vertex_t vertex;
        vec4ub color;
        if(!this->attachment()){
            return -1;
        }

        spSkeleton* skeleton = this->bone()->skeleton;

        color.red = static_cast<uint8_t>(skeleton->r * handle->r * 255);
        color.green = static_cast<uint8_t>(skeleton->g * handle->g * 255);
        color.blue = static_cast<uint8_t>(skeleton->b * handle->b * 255);
        color.alpha = static_cast<uint8_t>(skeleton->a * handle->a * 255);
        vertex.color = color;

        /*
        switch(attachment()->type)
        {
        case SP_ATTACHMENT_REGION:{
            spRegionAttachment* region = (spRegionAttachment*)attachment();
            //spRegionAttachment_computeWorldVertices(region, handle->bone, worldVertices);
            dc.bind_image2D((GLuint)region->rendererObject);
            dc.begin(draw_command::TRIANGLE_FAN);
            for(int i=0; i<8; i+=2)
            {
                vertex.x = _worldVertices[i];
                vertex.y = _worldVertices[i+1];
                vertex.u = region->uvs[i];
                vertex.v = region->uvs[i+1];
                dc.push_vertex(vertex);
            }
            dc.end();
            break;
            }
        case SP_ATTACHMENT_MESH:{
            spMeshAttachment* mesh = (spMeshAttachment*)attachment();
            spMeshAttachment_computeWorldVertices(mesh, handle, _worldVertices);

            dc.bind_image2D((GLuint)mesh->rendererObject);

            dc.begin(draw_command::TRIANGLES);
            for (int i = 0; i < mesh->trianglesCount; ++i) {
                int index = mesh->triangles[i] << 1;
                vertex.x = _worldVertices[index];
                vertex.y = _worldVertices[index + 1];
                vertex.u = mesh->uvs[index];
                vertex.v = mesh->uvs[index + 1];
                dc.push_vertex(vertex);
            }
            dc.end();
            break;
            }
        /
        case SP_ATTACHMENT_SKINNED_MESH:{
            spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment();
            spSkinnedMeshAttachment_computeWorldVertices(mesh, handle, _worldVertices);

            dc.bind_image2D((GLuint)mesh->rendererObject);

            int index;
            dc.begin(draw_command::TRIANGLES);
            for (int i = 0; i < mesh->trianglesCount; ++i) {
                index = mesh->triangles[i]<<1;
                vertex.x = _worldVertices[index];
                vertex.y = _worldVertices[index + 1];
                vertex.u = mesh->uvs[index];
                vertex.v = mesh->uvs[index + 1];
                dc.push_vertex(vertex);
            }
            dc.end();
            break;
            }

        default:
            break;
        } */
        return 0;
    }
};
#endif

class spine_skeletonData
{
public:
    spSkeletonData* const handle;
public:
    spine_skeletonData():handle(null){ /*void*/ }
    spine_skeletonData(spSkeletonData* skeletonData):handle(skeletonData) { /*void*/ }
    ~spine_skeletonData(){ /*void*/ }

    spine_skeletonData& operator=(spSkeletonData* skeletonData)
    {
        const_write(handle) = skeletonData;
        return *this;
    }

    spine_skeletonData& operator=(const spine_skeletonData& data)
    {
        const_write(handle) = data.handle;
        return *this;
    }

    int open(const char* path, const char* json, float scale = 1.0f)
    {
        if(handle){
            CGL_LOG("skeletonData is not null.");
            //this->dispose();
        }
        const_write(handle) = skeletonData_LoadFromJson(path, json, scale);
        return handle ? 0 : -1;
    }

    int open_memory(const char* path, const char* jsonData, float scale = 1.0f)
    {
        if(handle){
            CGL_LOG("skeletonData is not null.");
            //this->dispose();
        }
        const_write(handle) = skeletonData_LoadFromJsonData(path, jsonData, scale);
        return handle ? 0 : -1;
    }

    void dispose()
    {
        if(handle){
            spSkeletonData_dispose(const_write(handle));
            const_write(handle) = null;
        }
    }

    bool is_null()const
    {
        return !handle;
    }

    operator bool()const
    {
        return handle;
    }

    spAnimation* animations(int index)const
    {
        return handle ? handle->animations[index] : 0;
    }

    int animations_count()
    {
        return handle ? handle->animationsCount : 0;
    }

    spAnimation* find_animation(const char* name)
    {
        return handle ? spSkeletonData_findAnimation(handle, name) : 0;
    }

    spSlotData* slots(int index)
    {
        return handle->slots[index];
    }

    int slots_count()const
    {
        return handle ? handle->slotsCount : 0;
    }

    spSlotData* find_slot(const char* name)
    {
        return handle ? spSkeletonData_findSlot(handle, name) : 0;
    }

    int bones_count()const
    {
        CGL_ASSERT(handle);
        return handle->bonesCount;
    }

    spBoneData* bones(int index)
    {
        CGL_ASSERT(handle);
        return handle->bones[index];
    }

    spBoneData* find_bone(const char* name)
    {
        return spSkeletonData_findBone(handle, name);
    }

    spSkin* skins(int index)
    {
        CGL_ASSERT(!is_null());
        return handle->skins[index];
    }

    int skins_count()
    {
        return handle ? handle->skinsCount : 0;
    }

    spSkin* find_skin(const char* skin_name)
    {
        return handle ? spSkeletonData_findSkin(handle, skin_name) : 0;
    }

};

class spine_skeleton
{
public:
    spSkeleton* const handle;
    spSkin*    costom_skin;
    bool flipX;
    bool flipY;
public:
    spine_skeleton():handle(null), costom_skin(null), flipX(false), flipY(false) { /*void*/ }
    spine_skeleton(spSkeleton* skeleton):handle(skeleton), costom_skin(null), flipX(false), flipY(false) { /*void*/ }
    ~spine_skeleton() { /*void*/ }

    spine_skeleton operator=(spSkeleton* skeleton)
    {
        const_write(handle) = skeleton;
        return *this;
    }

    int create(spine_skeletonData& skeletonData)
    {
        if(handle){
            CGL_LOG("skeleton is not null.");
            //this->dispose();
        }

        if(skeletonData.is_null()){
            CGL_LOG_DEBUG("skeleton create : skeletonData is null.");
            return -1;
        }

        const_write(handle) = spSkeleton_create(skeletonData.handle);
        if(!handle){
            CGL_LOG_DEBUG("skeleton create : error.");
            return -1;
        }

        handle->flipX = 0;
        handle->flipY = 0;

        handle->x = 0;
        handle->y = 0;

        spSkeleton_setToSetupPose(handle);
        spSkeleton_updateWorldTransform(handle);
        return 0;
    }

    void dispose()
    {
        if(handle){
            spSkeleton_dispose(const_write(handle));
            const_write(handle) = null;
        }
        dispose_skin();
    }

    bool is_null()const
    {
        return !handle;
    }

    operator bool()const
    {
        return handle;
    }

    spine_skeletonData data()
    {
        return handle->data;
    }

    void flip(bool x, bool y)
    {
        if(handle){
            //handle->flipX = bool_toint(x);
            //handle->flipY = bool_toint(y);
            flipX = x;
            flipY = y;
            //spSkeleton_updateWorldTransform(m_skeleton);
        }
    }

    void scale(float x, float y)
    {
        if(handle){
            handle->root->scaleX = x;
            handle->root->scaleY = y;
        }
    }

    void update(float deltaTime)
    {
        CGL_ASSERT(handle);
        spSkeleton_update(handle, deltaTime);
    }

    vec4f bounding_box(float scaleX = 1.0f, float scaleY = 1.0f)
    {
        return this->is_null() ? vec4f() : skeleton_getBoundingBox(handle, scaleX, scaleY);
    }

    vec4f color()const
    {
        CGL_ASSERT(handle);
        return vec4f(handle->r, handle->g, handle->b, handle->a);
    }

    void set_color(float red, float green, float blue, float alpha)
    {
        CGL_ASSERT(handle);
        handle->r = red;
        handle->g = green;
        handle->b = blue;
        handle->a = alpha;
    }

    int slots_count()
    {
        CGL_ASSERT(handle);
        return handle->slotsCount;
    }

    spSlot* slots(int index)
    {
        CGL_ASSERT(handle);
        return handle->slots[index];
    }

    spSlot* find_slot(const char* name)
    {
        if(handle){
            return spSkeleton_findSlot(handle, name);
        }
        else{
            return null;
        }
    }

    spSlot* draw_order(int index)const
    {
        return handle->drawOrder[index];
    }

    int bones_count()const
    {
        CGL_ASSERT(handle);
        return handle->bonesCount;
    }

    spBone* bones(int index)
    {
        CGL_ASSERT(handle);
        return handle->bones[index];
    }

    spBone* find_bone(const char* name)
    {
        CGL_ASSERT(handle);
        return spSkeleton_findBone(handle, name);
    }

    spBone* root()
    {
        CGL_ASSERT(handle);
        return handle->root;
    }

    //附加到父骨骼上面
    int set_parent(spBone* bone)
    {
        if(bone){
            const_write(handle->root->parent) = bone;
            return 0;
        }
        else{
            return -1;
        }
    }

    int set_skin(spSkin* skin)
    {
        spSkeleton_setSkin(handle, skin);
        return 0;
    }

    int set_skin(const char* skin_name)
    {
        return spSkeleton_setSkinByName(handle, skin_name);
    }

    spSkin* skin()
    {
        return handle->skin;
    }

    //{{局部换肤，合并皮肤
    int create_skin(const char* skin_name = "custom_skin")
    {
        this->dispose_skin();
        //if(handle)this->set_skin((char*)null);//设置皮肤为空
        costom_skin = skin_create(skin_name);
        return 0;
    }

    //复制一个已存在的皮肤
    int clone_skin(const char* skin_name)
    {
        spSkin *pskin = data().find_skin(skin_name);
        if(pskin){
            dispose_skin();
            costom_skin = skin_clone("custom_skin", pskin);
            this->set_skin(costom_skin);
            //this->reset_slots();
        }
        return 0;
    }

    int append_skin(const char* skin_name)
    {
        skin_append(costom_skin, handle->data, skin_name);
        this->set_skin(costom_skin);
        //this->reset_slots();    //重置一下皮肤，不然显示错误
        return 0;
    }

    void dispose_skin()
    {
        if(handle){
            this->set_skin((char*)null);
        }
        if(costom_skin){
            skin_dispose(costom_skin);
            costom_skin = null;
        }
    }
    //}}

    void reset()
    {
        spSkeleton_setToSetupPose(handle);
    }

    void reset_bones()
    {
        spSkeleton_setBonesToSetupPose(handle);
    }

    void reset_slots()
    {
        spSkeleton_setSlotsToSetupPose(handle);
    }
};

class spine_animation
{
private:
    spAnimationStateData* m_stateData;
    //float worldVertices[SPINE_WORLD_VERTEX_BUFFER];
public:
    spAnimationState* const handle;
    spine_skeletonData skeletonData;
    spine_skeleton skeleton;
    bool debugBones;
    bool debugBoneName;
public:
    spine_animation() :
        m_stateData(null),
        handle(null),
        skeletonData(),
        skeleton(),
        debugBones(false),
        debugBoneName(false)
    {
    }
    ~spine_animation()
    {
        //this->dispose();
    }

    int create(spine_skeletonData& _skeletonData,
        const char* skin_name = "default",
        float default_mix = 0.3f)//默认动画过渡时间
    {
        if(handle){
            CGL_ASSERT(handle);
            CGL_LOG("animation is not null.");
            //this->dispose();//有疑问
        }

        skeletonData = _skeletonData;
        if(skeletonData.is_null()){
            CGL_LOG_DEBUG("skeletonData is null.");
            return -1;
        }

        /*
        spSkin* skin = skeletonData.find_skin(skin_name);
        if(skin){
            //skeletonData.set_skin(skin);
            //spSkeleton_setSkinByName
        }
        */

        skeleton.create(skeletonData);
        if(skeleton.is_null()){
            CGL_LOG_DEBUG("animation : skeleton create error.");
            return -1;
        }

        m_stateData = spAnimationStateData_create(skeletonData.handle);
        if(!m_stateData){
            this->dispose();
            CGL_LOG_DEBUG("animation : stateData create error.");
            return -1;
        }
        const_write(handle) = spAnimationState_create(m_stateData);
        if(!handle){
            this->dispose();
            CGL_LOG_DEBUG("animation : state create error.");
            return -1;
        }

        //动画过度时间
        m_stateData->defaultMix = default_mix;

        skeleton.flipY = true;

        //动画过度平滑切换
        //spAnimationStateData_setMixByName(m_stateData, "walk", "jump", 1.0f);
        //spAnimationStateData_setMixByName(m_stateData, "jump", "run", 1.0f);

        //m_skeletonNode->timeScale = 0.3f;   // 动画速度变缓
        //m_skeletonNode->debugBones = true;  // 打开骨骼调试

        //skeleton.handle->debugBones = true;

        return 0;
    }

    void dispose()
    {
        if(handle){
            spAnimationState_dispose(handle);
            const_write(handle) = null;
        }
        if(m_stateData){
            spAnimationStateData_dispose(m_stateData);
            m_stateData = null;
        }
        skeleton.dispose();
    }

    bool is_null()const
    {
        return !handle;
    }

    operator bool()const
    {
        return handle;
    }

    //当前动作是否重复
    bool is_loop()const
    {
        if(handle){
            spTrackEntry* entry = spAnimationState_getCurrent(handle, 0);
            if(entry && entry->animation){
                return entry->loop;
            }
        }
        return false;
    }

    //返回当前动作的名字
    spAnimation* animation()const
    {
        if(handle){
            spTrackEntry* entry = spAnimationState_getCurrent(handle, 0);
            if(entry && entry->animation){
                return entry->animation;
            }
        }
        return null;
    }

    //设置动作
    int set_animation(int trackIndex, spAnimation* animation, bool loop = true)
    {
        if(handle){
            //this->reset_bones();//防止残影，自定义皮肤会失效
            //spAnimationState_clearTracks(handle);
            spAnimationState_setAnimation(handle, trackIndex, animation, bool_toint(loop));
        }
        return 0;
    }

    int set_animation(int trackIndex, const char* name, bool loop = true)
    {
        if(handle){
            spAnimation* animation = skeletonData.find_animation(name);
            if(animation){
                return this->set_animation(trackIndex, animation, loop);
            }
        }
        return -1;
    }

    spTrackEntry* setEmptyAnimation (int trackIndex, float mixDuration) {
        //return spAnimationState_setEmptyAnimation(_state, trackIndex, mixDuration);
        //spAnimationState_setEmptyAnimations(_state, mixDuration);
        return null;
    }

    int set_animation(int trackIndex, int index, bool loop = true)
    {
        if(handle && index>=0 && index<skeletonData.animations_count()){
            return this->set_animation(trackIndex, skeletonData.animations(index), loop);
        }
        return -1;
    }

    const char* animation_name()const
    {
        spAnimation *ani = this->animation();
        if(ani){
            return ani->name;
        }
        else{
            return null;
        }
    }

    int animation_index(const char* name = null)
    {
        spAnimation* ani;
        if(name){
            ani = skeletonData.find_animation(name);
        }
        else{
            ani = this->animation();
        }
        return ani ? this->animation_index(ani) : -1;
    }

    int animation_index(spAnimation* animation)
    {
        for(int i=0; i<skeletonData.animations_count(); ++i){
            if(animation == skeletonData.animations(i)){
                return i;
            }
        }
        return -1;
    }

    //返回当前动作时间
    float animation_progress(int32_t trackIndex) const
    {
        CGL_ASSERT(handle);
        if (spTrackEntry* current = spAnimationState_getCurrent(handle, trackIndex)){
            //return (current->trackEnd != 0.0f) ? current->trackTime / current->trackEnd : 0.0f;
        }

        return 0.0f;
    }

    //重置
    void reset()
    {
        CGL_ASSERT(handle);
        spSkeleton_setToSetupPose(skeleton.handle);
    }

    void reset_bones()
    {
        CGL_ASSERT(handle);
        spSkeleton_setBonesToSetupPose(skeleton.handle);
    }

    void reset_slots()
    {
        CGL_ASSERT(handle);
        spSkeleton_setSlotsToSetupPose(skeleton.handle);
    }

    //停止所有动作
    void stop()
    {
        CGL_ASSERT(handle);
        spAnimationState_clearTracks(handle);
    }

    //停止动作
    void stop(int track)
    {
        CGL_ASSERT(handle);
        if(0 < track){
            spAnimationState_clearTrack(handle, track);
        }
        else{

        }
    }

    void flip(bool x, bool y)
    {
        CGL_ASSERT(handle);
        skeleton.flip(x, y);
    }

    //1.0/30.0表示每秒30帧，每次更新一帧
    int update(float deltaTime)//, float timeScale = 1.0f)
    {
        //CGL_ASSERT(handle);
        //if(m_state && this->animation())
        //if(m_state)// && m_state->tracksCount)
        if(handle)
        {
            skeleton.update(deltaTime);
            spAnimationState_update(handle, deltaTime);// * timeScale);
            spAnimationState_apply(handle, skeleton.handle);
            spSkeleton_updateWorldTransform(skeleton.handle);
        }
        return 0;
    }

    float time()
    {
        CGL_ASSERT(handle);
        return skeleton.handle->time;
    }

    float timeScale()const
    {
        CGL_ASSERT(handle);
        return handle->timeScale;
    }

    float set_timeScale(float scale)
    {
        CGL_ASSERT(handle);
        return handle->timeScale = scale;
    }

    int paint(graphics::draw_context& dc, slot_on_paint on_paint = null, void* owner = null)
    {
        if(!handle)return -1;
        float worldVertices[SPINE_WORLD_VERTEX_BUFFER];
        skeleton_paint(dc, skeleton.handle, skeleton.flipX, skeleton.flipY, on_paint, owner, worldVertices);
        if(debugBones){
            skeleton_drawBones(dc, skeleton.handle, skeleton.flipX, skeleton.flipY, debugBoneName);
        }
        return 0;
    }
};

/*
void SkeletonAnimation::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
    switch (type) {
    case SP_ANIMATION_START:
        if (startListener) startListener(trackIndex);
        break;
    case SP_ANIMATION_END:
        if (endListener) endListener(trackIndex);
        break;
    case SP_ANIMATION_COMPLETE:
        if (completeListener) completeListener(trackIndex, loopCount);
        break;
    case SP_ANIMATION_EVENT:
        if (eventListener) eventListener(trackIndex, event);
        break;
    }
}

void SkeletonAnimation::onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
    spTrackEntry* entry = spAnimationState_getCurrent(state, trackIndex);
    if (!entry->rendererObject) return;
    _TrackEntryListeners* listeners = (_TrackEntryListeners*)entry->rendererObject;
    switch (type) {
    case SP_ANIMATION_START:
        if (listeners->startListener) listeners->startListener(trackIndex);
        break;
    case SP_ANIMATION_END:
        if (listeners->endListener) listeners->endListener(trackIndex);
        break;
    case SP_ANIMATION_COMPLETE:
        if (listeners->completeListener) listeners->completeListener(trackIndex, loopCount);
        break;
    case SP_ANIMATION_EVENT:
        if (listeners->eventListener) listeners->eventListener(trackIndex, event);
        break;
    }
}
*/

}//end namespace cgl

#include "spine.cpp"

//---------------------------------------------------------------------------
#endif //SPINE_H_20161217172346

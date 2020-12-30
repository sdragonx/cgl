
#include "spine.hpp"

namespace spine{

//#ifdef CGL_SPINE_VERSION < 30000
#ifdef CGL_SPINE_STATIC_LIB

typedef struct _Entry
{
	int slotIndex;
	const char* name;
	spAttachment* attachment;
	_Entry* next;
}spSkinEntry;

typedef struct _spSkin
{
	spSkin super;
	_Entry* entries;
}spSkinEx;

#endif
//#endif

extern "C"{

//abandoned
void _spAtlasPage_disposeTexture (spAtlasPage* self)
{

}

#if 0
void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	//self->name == path

	GLuint tex = CTW_LoadTexture(path);
	glimage image;
	image.bind(tex);
	self->rendererObject = (void*)tex;
	self->width = image.width();
	self->height = image.height();
}
void _spAtlasPage_disposeTexture (spAtlasPage* self)
{
	if(self->rendererObject)
	{
		assert(0);
		//graphics.images.close((int)self->rendererObject);
		//_CTW_texManager.erase(self->name);
	}
}
char* _spUtil_readFile (const char* path, int* length)
{
	return _readFile(path, length);
}
#endif

}//end extern "C"
}//end namespace spine

namespace cgl{

using namespace spine;

//三角形索引
static const int spine_quadTriangles[6] = {0, 1, 2, 2, 3, 0};

//自定义skeleton加载函数
//atlas->rendererObject暂时未使用，当作传入图片路径的工具
struct spAttachmentLoaderEx {
	spAttachmentLoader super;
	//spAtlas* atlas;
	const char* path;
};

//自定义skin删除函数，不释放图元
void _Entry_disposeEx (_Entry* self) {
	//spAttachment_dispose(self->attachment);
	FREE(self->name);
	FREE(self);
}

_Entry* spine_skinFindEntryBySlotIndex(spSkin* skin, int slotIndex);

#if CGL_SPINE_VERSION < 30000

//加载、新建slot
spAttachment* spSlot_newAttachmentEx(spAttachmentLoader* loader, spSkin* skin, spAttachmentType type,
		const char* name, const char* path)
{
	spAttachmentLoaderEx *self = SUB_CAST(spAttachmentLoaderEx, loader);

	teximage* image = spine_LoadImage(cstring<char>(self->path) + path);
	if(!image)
	{
		image = spine_LoadImage(cstring<char>());
	}

	switch (type){
	case SP_ATTACHMENT_REGION:{
		spRegionAttachment* attachment;

		attachment = spRegionAttachment_create(name);
		attachment->rendererObject = reinterpret_cast<void*>(image->id);
		spRegionAttachment_setUVs(attachment, image->u1, image->v1, image->u2, image->v2, 0);
		attachment->width = image->width;
		attachment->height = image->height;
		attachment->regionOffsetX = 0.0f;
		attachment->regionOffsetY = 0.0f;
		attachment->regionWidth = image->width;
		attachment->regionHeight = image->height;
		attachment->regionOriginalWidth = image->width;
		attachment->regionOriginalHeight = image->height;
		attachment->path = path;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_MESH:
	{
		spMeshAttachment* attachment;

		attachment = spMeshAttachment_create(name);
		attachment->rendererObject = reinterpret_cast<void*>(image->id);
		//attachment->regionOffsetX = 0.0f;
		//attachment->regionOffsetY = 0.0f;
		attachment->regionOffsetX = image->x;
		attachment->regionOffsetY = image->y;

		attachment->regionWidth = image->width;
		attachment->regionHeight = image->height;
		attachment->regionOriginalWidth = image->width;
		attachment->regionOriginalHeight = image->height;
		attachment->regionU = image->u1;
		attachment->regionV = image->v1;
		attachment->regionU2 = image->u2;
		attachment->regionV2 = image->v2;
		attachment->regionRotate = 0;

		return SUPER(attachment);
	}
	case SP_ATTACHMENT_SKINNED_MESH:{
		spSkinnedMeshAttachment* attachment;

		attachment = spSkinnedMeshAttachment_create(name);
		attachment->rendererObject = reinterpret_cast<void*>(image->id);
		attachment->regionOffsetX = 0.0f;
		attachment->regionOffsetY = 0.0f;
		attachment->regionWidth = image->width;
		attachment->regionHeight = image->height;
		attachment->regionOriginalWidth = image->width;
		attachment->regionOriginalHeight = image->height;
		attachment->regionU = image->u1;
		attachment->regionV = image->v1;
		attachment->regionU2 = image->u2;
		attachment->regionV2 = image->v2;

		return SUPER(attachment);
	}
	case SP_ATTACHMENT_BOUNDING_BOX://未修改
		return SUPER(spBoundingBoxAttachment_create(name));
	default:
		CGL_LOG("spSlot_newAttachmentEx error");
		_spAttachmentLoader_setUnknownTypeError(loader, type);
		return 0;
	}
	//UNUSED(skin);
}
#else//3.0
spAttachment* spSlot_newAttachmentEx(spAttachmentLoader* loader, spSkin* skin, spAttachmentType type,
		const char* name, const char* path)
{
	spAttachmentLoaderEx *self = SUB_CAST(spAttachmentLoaderEx, loader);

	imageblock* image = spine_LoadImage(cstring<char>(self->path) + path);
	if(!image)
	{
		image = spine_LoadImage(cstring<char>());
	}

	switch (type){
	case SP_ATTACHMENT_REGION:{
		spRegionAttachment* attachment;

		attachment = spRegionAttachment_create(name);
		attachment->rendererObject = reinterpret_cast<void*>(image->image);
		spRegionAttachment_setUVs(attachment, image->u1, image->v1, image->u2, image->v2, 0);
		attachment->width = image->width;
		attachment->height = image->height;
		attachment->regionOffsetX = 0.0f;
		attachment->regionOffsetY = 0.0f;
		attachment->regionWidth = image->width;
		attachment->regionHeight = image->height;
		attachment->regionOriginalWidth = image->width;
		attachment->regionOriginalHeight = image->height;
		attachment->path = path;
		return SUPER(attachment);
	}
	case SP_ATTACHMENT_MESH:
	case SP_ATTACHMENT_LINKED_MESH:
	{
		spMeshAttachment* attachment;

		attachment = spMeshAttachment_create(name);
		attachment->rendererObject = reinterpret_cast<void*>(image->image);
		//attachment->regionOffsetX = 0.0f;
		//attachment->regionOffsetY = 0.0f;
		attachment->regionOffsetX = image->x;
		attachment->regionOffsetY = image->y;

		attachment->regionWidth = image->width;
		attachment->regionHeight = image->height;
		attachment->regionOriginalWidth = image->width;
		attachment->regionOriginalHeight = image->height;
		attachment->regionU = image->u1;
		attachment->regionV = image->v1;
		attachment->regionU2 = image->u2;
		attachment->regionV2 = image->v2;
		attachment->regionRotate = 0;

		return SUPER(SUPER(attachment));
	}
	case SP_ATTACHMENT_BOUNDING_BOX://未修改
		return SUPER(SUPER(spBoundingBoxAttachment_create(name)));
	case SP_ATTACHMENT_PATH:
		return SUPER(SUPER(spPathAttachment_create(name)));
	default:
		print_message("spSlot_newAttachmentEx error");
		_spAttachmentLoader_setUnknownTypeError(loader, type);
		return 0;
	}
	//UNUSED(skin);
}
#endif //CGL_SPINE_VERSION < 30000

spSkeletonData* skeletonData_LoadFromJson(const char* path, const char* json, float scale)
{
	spSkeletonData* skeleton_data = null;
	spAttachmentLoaderEx* loader = NEW(spAttachmentLoaderEx);
	#if CGL_SPINE_VERSION < 30000
	_spAttachmentLoader_init(SUPER(loader), _spAttachmentLoader_deinit, spSlot_newAttachmentEx);
	#else
	_spAttachmentLoader_init(SUPER(loader), _spAttachmentLoader_deinit, spSlot_newAttachmentEx, 0, 0);
	#endif

	loader->path = path;
	spSkeletonJson* skeletonJson = spSkeletonJson_createWithLoader(SUPER(loader));
	skeletonJson->scale = scale;
	char jsonFile[MAX_PATH] = {0};
	cscpy(jsonFile, path, MAX_PATH);
	cscat(jsonFile, json, MAX_PATH);

	skeleton_data = spSkeletonJson_readSkeletonDataFile(skeletonJson, jsonFile);
	if(!skeleton_data)
	{
		CGL_LOG(skeletonJson->error);
		spSkeletonJson_dispose(skeletonJson);
		return 0;
	}
	spSkeletonJson_dispose(skeletonJson);
	return skeleton_data;
}

spSkeletonData* skeletonData_LoadFromJsonData(const char* path, const char* jsonData, float scale)
{
    spSkeletonData* skeleton_data = null;
	spAttachmentLoaderEx* loader = NEW(spAttachmentLoaderEx);
	#if CGL_SPINE_VERSION < 30000
	_spAttachmentLoader_init(SUPER(loader), _spAttachmentLoader_deinit, spSlot_newAttachmentEx);
	#else
	_spAttachmentLoader_init(SUPER(loader), _spAttachmentLoader_deinit, spSlot_newAttachmentEx, 0, 0);
	#endif

	loader->path = path;
	spSkeletonJson* skeletonJson = spSkeletonJson_createWithLoader(SUPER(loader));
	skeletonJson->scale = scale;

	skeleton_data = spSkeletonJson_readSkeletonData(skeletonJson, jsonData);
	if(!skeleton_data)
	{
		CGL_LOG(skeletonJson->error);
		spSkeletonJson_dispose(skeletonJson);
		return 0;
	}
	spSkeletonJson_dispose(skeletonJson);
	return skeleton_data;
}

int skeleton_paint(draw_context& dc, spSkeleton* skeleton, bool flipX, bool flipY, slot_on_paint on_paint, void* owner, float worldVertices[])
{
	spAttachment* attachment = null;
	vtx3f vertex;
	vec4ub color;

	if(!skeleton){
		return -1;
	}

	//dc.clear_buffer();
	//dc.fill_mode(dc.LINE);
	//glDisable(GL_TEXTURE_2D);

	for(int slotIndex = 0; slotIndex<skeleton->slotsCount; ++slotIndex)
	{
		#if 0
		spine_slot slot;
		slot = skeleton->drawOrder[slotIndex];
		//attachment = slot->attachment;

		slot.paint(dc);

		#else

		spSlot* slot = skeleton->drawOrder[slotIndex];

		if(on_paint && !on_paint(dc, slot, owner)){
			continue;
		}

		attachment = slot->attachment;
		if (!attachment) {
			//print(slot->bone->data->name);
			continue;
		}

		color = dc.color;
		color.red   *= skeleton->r * slot->r;
		color.green *= skeleton->g * slot->g;
		color.blue  *= skeleton->b * slot->b;
		color.alpha *= skeleton->a * slot->a;
		vertex.color = color;

		/*
		switch(slot->data->additiveBlending){
		//switch(slot->data->blendMode){
		case SP_BLEND_MODE_ADDITIVE:
			dc.blendfunc(CGL_ONE, CGL_SRC_ALPHA);
			break;
		case SP_BLEND_MODE_MULTIPLY:
			dc.blendfunc(CGL_DST_COLOR , CGL_ONE_MINUS_SRC_ALPHA);
			break;
		case SP_BLEND_MODE_SCREEN:
			dc.blendfunc(CGL_ONE, CGL_ONE_MINUS_SRC_COLOR);
			break;
		default:
			dc.blendfunc(CGL_SRC_ALPHA, CGL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		*/

		#if CGL_SPINE_VERSION < 30000
		float fx = flipX ? -1.0f : 1.0f;
		float fy = flipY ? -1.0f : 1.0f;

		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment* region = (spRegionAttachment*)attachment;
			spRegionAttachment_computeWorldVertices(region, slot->bone, worldVertices);

			dc.bind_texture((int)region->rendererObject);
			dc.begin(CGL_TRIANGLE_FAN);
			for(int i=0; i<8; i+=2)
			{
				vertex.x = worldVertices[i] * fx;
				vertex.y = worldVertices[i+1] * fy;
				vertex.u = region->uvs[i];
				vertex.v = region->uvs[i+1];
				dc.vertex(vertex);
			}
			dc.end();
		}
		else if (attachment->type == SP_ATTACHMENT_MESH)
		{
			spMeshAttachment* mesh = (spMeshAttachment*)attachment;
			//if (mesh->super.verticesCount > SPINE_WORLD_VERTEX_BUFFER) continue;
			spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			dc.bind_texture((int)mesh->rendererObject);
			dc.begin(CGL_TRIANGLES);
			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;
				vertex.x = worldVertices[index] * fx;
				vertex.y = worldVertices[index + 1] * fy;
				vertex.u = mesh->uvs[index];
				vertex.v = mesh->uvs[index + 1];
				dc.vertex(vertex);
			}
			dc.end();
		}
		else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
		{
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)attachment;
			spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			dc.bind_texture((int)mesh->rendererObject);

			int index;
			dc.begin(CGL_TRIANGLES);
			for (int i = 0; i < mesh->trianglesCount; ++i) {
				index = mesh->triangles[i]<<1;
				vertex.x = worldVertices[index] * fx;
				vertex.y = worldVertices[index + 1] * fy;
				vertex.u = mesh->uvs[index];
				vertex.v = mesh->uvs[index + 1];
				dc.vertex(vertex);
			}
			dc.end();
		}
		#else//3.0x

		if (attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment* region = (spRegionAttachment*)attachment;
			spRegionAttachment_computeWorldVertices(region, slot->bone, worldVertices);

			dc.bind_image2D((GLuint)region->rendererObject);
			dc.begin(CGL_TRIANGLE_FAN);
			for(int i=0; i<8; i+=2)
			{
				vertex.x = worldVertices[i];
				vertex.y = worldVertices[i+1];
				vertex.u = region->uvs[i];
				vertex.v = region->uvs[i+1];
				dc.push_vertex(vertex);
			}
			dc.end();
		}
		else if (attachment->type == SP_ATTACHMENT_MESH || attachment->type == SP_ATTACHMENT_LINKED_MESH)
		{
			spMeshAttachment* mesh = (spMeshAttachment*)attachment;
			//if (mesh->super.verticesCount > SPINE_WORLD_VERTEX_BUFFER) continue;
			spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			dc.bind_image2D((GLuint)mesh->rendererObject);
			dc.begin(CGL_TRIANGLES);
			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;
				vertex.x = worldVertices[index];
				vertex.y = worldVertices[index + 1];
				vertex.u = mesh->uvs[index];
				vertex.v = mesh->uvs[index + 1];
				dc.push_vertex(vertex);
			}
			dc.end();
		}
		#endif
		#endif
	}//end for
	dc.update(0);
	return 0;
}

int skeleton_drawBones(draw_context& dc, spSkeleton* skeleton, bool flipX, bool flipY, bool boneName)
{
	float x, y;
	float vx, vy;

	dc.color = vec4ub(255, 0, 255, 255);

	//test
	float fx = flipX ? -1.0f : 1.0f;
	float fy = flipY ? -1.0f : 1.0f;

	//fx *= skeleton->root->scaleX;
	//fy *= skeleton->root->scaleY;

	for(int i = 0; i<skeleton->bonesCount; ++i)
	{
		spBone* bone = skeleton->bones[i];
		#if CGL_SPINE_VERSION < 30000
		vx = 2.0f * bone->m00 * fx;
		vy = 2.0f * bone->m10 * fy;
		#else//err
		vx = 3.0f * bone->a;
		vy = 3.0f * bone->c;
		#endif

		dc.bind_texture(0);
		dc.begin(CGL_TRIANGLES);
		x = fx * bone->worldX - vy;
		y = fy * bone->worldY + vx;
		dc.vertex(x, y, 0.0f);

		x = fx * bone->worldX + vy;
		y = fy * bone->worldY - vx;
		dc.vertex(x, y, 0.0f);

		#if CGL_SPINE_VERSION < 30000
		x = (bone->data->length * bone->m00 + bone->worldX) * fx;
		y = (bone->data->length * bone->m10 + bone->worldY) * fy;
		#else
		x = bone->data->length * bone->a + bone->worldX;
		y = bone->data->length * bone->c + bone->worldY;
		#endif
		dc.vertex(x, y, 0.0f);
		dc.end();
		if(boneName){
			dc.print(fx * bone->worldX, fy * bone->worldY, bone->data->name, -1);
		}
	}

	//原点
	dc.pen_width(4.0f);
	dc.color = vec4ub(0, 255, 0, 255);
	dc.draw_point(fx * skeleton->bones[0]->worldX, fy * skeleton->bones[0]->worldY);

	dc.update(0);
	return 0;
}

vec4f skeleton_getBoundingBox(spSkeleton* skeleton, float scaleX, float scaleY)
{
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
	float x, y;

	float _worldVertices[SPINE_WORLD_VERTEX_BUFFER] = {0};

	for (int i = 0; i < skeleton->slotsCount; ++i) {
		spSlot* slot = skeleton->slots[i];
		if (!slot->attachment) continue;
		int verticesCount;
		if (slot->attachment->type == SP_ATTACHMENT_REGION) {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			verticesCount = 8;
		}
		else if (slot->attachment->type == SP_ATTACHMENT_MESH) {
			spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			#if CGL_SPINE_VERSION < 30000
			verticesCount = mesh->verticesCount;
			#else
			verticesCount = mesh->super.worldVerticesLength;// >trianglesCount;
			#endif
		}
		#if CGL_SPINE_VERSION < 30000
		else if (slot->attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)slot->attachment;
			spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->uvsCount;
		}
		#endif
		else{
			continue;
		}
		for (int j = 0; j < verticesCount; j += 2) {
			x = _worldVertices[j] * scaleX,
			y = _worldVertices[j + 1] * scaleY;
			minX = std::min(minX, x);
			minY = std::min(minY, y);
			maxX = std::max(maxX, x);
			maxY = std::max(maxY, y);
		}
	}
	return vec4f(minX, minY, maxX - minX, maxY - minY);
}

/*
vec4f skeleton_getBoundingBox(spSkeleton* skeleton, float scaleX, float scaleY)
{
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
	float _worldVertices[SPINE_WORLD_VERTEX_BUFFER] = {0};
	for (int i = 0; i < skeleton->slotsCount; ++i) {
		spSlot* slot = skeleton->slots[i];
		if (!slot->attachment) continue;
		int verticesCount;
		if (slot->attachment->type == SP_ATTACHMENT_BOUNDING_BOX)
		{
			//if (slot->attachment->name == name)
			{
				spBoundingBoxAttachment* spa = (spBoundingBoxAttachment*)slot->attachment;
				spBoundingBoxAttachment_computeWorldVertices(spa, slot, _worldVertices);
				//spa->super.verticesCount
				verticesCount = spa->super.verticesCount;
				for (int ii = 0; ii < verticesCount; ii += 2) {
					float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
					minX = min(minX, x);
					minY = min(minY, y);
					maxX = max(maxX, x);
					maxY = max(maxY, y);
				}
			}
		}

	}
	//Vec2 position = getPosition();
	return vec4f(minX,  minY, maxX - minX, maxY - minY);
}

*/

//皮肤处理函数，支持局部换装

spSkin* skin_create(const char* name)
{
	return spSkin_create(name);
}

void skin_dispose(spSkin* self)
{
	_Entry* entry = SUB_CAST(_spSkin, self)->entries;
	while (entry) {
		_Entry* nextEntry = entry->next;
		_Entry_disposeEx(entry);
		entry = nextEntry;
	}

	FREE(self->name);
	FREE(self);
}

//查找皮肤入口
_Entry* skin_findEntryBySlotIndex(spSkin* skin, int slotIndex)
{
	_spSkin* skinData = (_spSkin*)skin;
	if(!skinData)return 0;
	_Entry* entry = skinData->entries;
	while(entry){
		if(entry->slotIndex == slotIndex){
			return entry;
		}
		entry = entry->next;
	}
	return null;
}

//创建或者复制一个skin
spSkin* skin_clone(const char* name, spSkin* skeleton_skin)
{
	spSkin *skin = null;
	_spSkin *skeleton_skinEx = (_spSkin*)skeleton_skin;
	if(!skeleton_skinEx){
		return null;
	}
	skin = skin_create(name);
	_Entry* entry = skeleton_skinEx->entries;
	while(entry){
		spSkin_addAttachment(skin, entry->slotIndex, entry->name, entry->attachment);
		entry = entry->next;
	}
	return skin;
}

spSkin* skin_clone(const char* name, spSkeletonData* skeletonData, const char* skin_name)
{
	if(skeletonData && skin_name)
	{
		spSkin* skeleton_skin = spSkeletonData_findSkin(skeletonData, skin_name);
		return skin_clone(name, skeleton_skin);
	}
	return null;
}

/*
spSkin* skin_appendAttachment(spSkin* skin, int slotIndex, spAttachment* attachment)
{
	spSkinEntry* entry = spine_skinFindEntryBySlotIndex(skin, slotIndex);
	if(!entry){
		spSkin_addAttachment(skin, entry->slotIndex, entry->name, entry->attachment);
	}
	else{
		skin_entry->attachment = entry->attachment;
	}
	return skin;
}
*/

spSkin* skin_append(spSkin* skin, spSkeletonData* skeletonData, const char* skin_name)
{
	spSkin* skeleton_skin = spSkeletonData_findSkin(skeletonData, skin_name);

	if(!skeleton_skin){
		return null;
	}

	_Entry* entry = ((_spSkin*)skeleton_skin)->entries;
	while(entry)
	{
		_Entry* skin_entry = skin_findEntryBySlotIndex(skin, entry->slotIndex);
		if(!skin_entry){
			//spAttachment* attachment = spine_attachmentClone(entry->attachment);
			spSkin_addAttachment(skin, entry->slotIndex, entry->name, entry->attachment);
		}
		else{
			skin_entry->attachment = entry->attachment;
		}
		entry = entry->next;
	}
	return skin;
}

}//end namespace cgl


/*

 spine2.1025.hpp

 sdragonx 2018-01-14 12:02:09

*/
#ifndef SPINE2_1025_HPP_20180114120209
#define SPINE2_1025_HPP_20180114120209

#pragma warn -8105

#include <stdlib.h>

#define SPINE_SHORT_NAMES

#ifndef __STDC_VERSION__
#define __STDC_VERSION__
#endif

#define CGL_SPINE_VERSION 2.1025

#define SPINE_2_1025_FILE(f) <cgl/external/spine2.1025/f>

namespace spine{

#define fmodf std::fmod
#define _strdup std::strdup

float acosf(float n)
{
	if(n < -1.0f)n = -1.0f;
	if(n > 1.0f)n = 1.0f;
	return std::acos(n);
}

#if defined CGL_SPINE_STATIC_LIB

#include SPINE_2_1025_FILE(spine.h)
#include SPINE_2_1025_FILE(extension.h)

#pragma link "spine2.1.25.lib"

#ifdef _DEBUG
//#pragma link "spine_debug.lib"
#else
//#pragma link "spine.lib"
#endif

#else

#define sinf std::sin
#define cosf std::cos
#define atan2f std::atan2
#define sqrtf std::sqrt

using std::sprintf;
using std::sscanf;
using std::FILE;
using std::fopen;

int _stricmp(const char* s1, const char* s2)
{
	return std::stricmp(s1, s2);
}

#include SPINE_2_1025_FILE(Animation.c)
#include SPINE_2_1025_FILE(AnimationState.c)
#include SPINE_2_1025_FILE(AnimationStateData.c)
#include SPINE_2_1025_FILE(Atlas.c)
#include SPINE_2_1025_FILE(AtlasAttachmentLoader.c)
#include SPINE_2_1025_FILE(Attachment.c)
#include SPINE_2_1025_FILE(AttachmentLoader.c)
#include SPINE_2_1025_FILE(Bone.c)
#include SPINE_2_1025_FILE(BoneData.c)
#include SPINE_2_1025_FILE(BoundingBoxAttachment.c)
#include SPINE_2_1025_FILE(Event.c)
#include SPINE_2_1025_FILE(EventData.c)
#include SPINE_2_1025_FILE(extension.c)
#include SPINE_2_1025_FILE(IkConstraint.c)
#include SPINE_2_1025_FILE(IkConstraintData.c)
#include SPINE_2_1025_FILE(Json.c)
#include SPINE_2_1025_FILE(MeshAttachment.c)
#include SPINE_2_1025_FILE(RegionAttachment.c)
#include SPINE_2_1025_FILE(Skeleton.c)
#include SPINE_2_1025_FILE(SkeletonBounds.c)
#include SPINE_2_1025_FILE(SkeletonData.c)
#include SPINE_2_1025_FILE(SkeletonJson.c)
#include SPINE_2_1025_FILE(Skin.c)
#include SPINE_2_1025_FILE(SkinnedMeshAttachment.c)
#include SPINE_2_1025_FILE(Slot.c)
#include SPINE_2_1025_FILE(SlotData.c)

#endif //CGL_SPINE_STATIC_LIB

}// end namespace spine

#endif//SPINE2_1025_HPP_20180114120209

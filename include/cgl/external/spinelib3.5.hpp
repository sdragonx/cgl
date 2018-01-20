#ifndef SPINE_LIB_HPP
#define SPINE_LIB_HPP

#include <stdlib.h>
#define fmodf std::fmod
#define _strdup std::strdup

#include <cgl/cgl.h>
using namespace cgl;

//extern void print(const cstring<char> & s);

float acosf(float n)
{
	/*
	if(0.0f<=n && n<=1.0f){
		return std::acos(n);
	}
	*/
	if(n < -1.0f)n = -1.0f;
	if(n > 1.0f)n = 1.0f;
	return std::acos(n);

	//else
	{
		//print(cstring<char>(n));
	}
	//return 0.0f;
}

int _stricmp(const char* s1, const char* s2)
{
    return std::stricmp(s1, s2);
}

#ifndef __STDC_VERSION__
#define __STDC_VERSION__
#endif

#define SPINE_VERSION 3.5

namespace spine{

#if SPINE_VERSION <= 0203

#include <spine/Animation.c>
#include <spine/AnimationState.c>
#include <spine/AnimationStateData.c>
#include <spine/Atlas.c>
#include <spine/AtlasAttachmentLoader.c>
#include <spine/Attachment.c>
#include <spine/AttachmentLoader.c>
#include <spine/Bone.c>
#include <spine/BoneData.c>
#include <spine/BoundingBoxAttachment.c>
#include <spine/Event.c>
#include <spine/EventData.c>
#include <spine/extension.c>
#include <spine/IkConstraint.c>
#include <spine/IkConstraintData.c>
#include <spine/Json.c>
#include <spine/MeshAttachment.c>
#include <spine/RegionAttachment.c>
#include <spine/Skeleton.c>
#include <spine/SkeletonBounds.c>
#include <spine/SkeletonData.c>
#include <spine/SkeletonJson.c>
#include <spine/Skin.c>
#include <spine/SkinnedMeshAttachment.c>
#include <spine/Slot.c>
#include <spine/SlotData.c>

#elif SPINE_VERSION == 225

#include <spine/Animation.c> 
#include <spine/AnimationState.c> 
#include <spine/AnimationStateData.c> 
#include <spine/Atlas.c> 
#include <spine/AtlasAttachmentLoader.c> 
#include <spine/Attachment.c> 
#include <spine/AttachmentLoader.c> 
#include <spine/Bone.c> 
#include <spine/BoneData.c> 
#include <spine/BoundingBoxAttachment.c> 
#include <spine/Event.c> 
#include <spine/EventData.c> 
#include <spine/extension.c> 
#include <spine/IkConstraint.c> 
#include <spine/IkConstraintData.c> 
#include <spine/Json.c> 
#include <spine/Json.h> 
#include <spine/MeshAttachment.c> 
#include <spine/RegionAttachment.c> 
#include <spine/Skeleton.c> 
#include <spine/SkeletonBounds.c> 
#include <spine/SkeletonData.c> 
#include <spine/SkeletonJson.c> 
#include <spine/Skin.c> 
#include <spine/SkinnedMeshAttachment.c> 
#include <spine/Slot.c> 
#include <spine/SlotData.c> 


#else

#include <spine/Animation.c>
#include <spine/AnimationState.c>
#include <spine/AnimationStateData.c>
#include <spine/Atlas.c>
#include <spine/AtlasAttachmentLoader.c>
#include <spine/Attachment.c>
#include <spine/AttachmentLoader.c>
#include <spine/Bone.c>
#include <spine/BoneData.c>
#include <spine/BoundingBoxAttachment.c>
#include <spine/Event.c>
#include <spine/EventData.c>
#include <spine/extension.c>
#include <spine/IkConstraint.c>
#include <spine/IkConstraintData.c>
#include <spine/Json.c>
#include <spine/Json.h>
#include <spine/kvec.h>
#include <spine/MeshAttachment.c>
#include <spine/PathAttachment.c>
#include <spine/PathConstraint.c>
#include <spine/PathConstraintData.c>
#include <spine/RegionAttachment.c>
#include <spine/Skeleton.c>
//#include <spine/SkeletonBinary.c>
#include <spine/SkeletonBounds.c>
#include <spine/SkeletonData.c>
#include <spine/SkeletonJson.c>
#include <spine/Skin.c>
#include <spine/Slot.c>
#include <spine/SlotData.c>
#include <spine/TransformConstraint.c>
#include <spine/TransformConstraintData.c>
#include <spine/VertexAttachment.c>

#endif

}// end namespace spine

#endif//

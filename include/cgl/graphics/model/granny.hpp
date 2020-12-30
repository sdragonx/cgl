/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 granny.hpp

 2020-04-28 10:19:10

*/
#ifndef GRANNY_HPP_20200428101910
#define GRANNY_HPP_20200428101910

#include <cgl/public.h>
#include <cgl/io.hpp>
#include <cgl/graphics/gl/glcontext.hpp>

//extern "C"{
#include <cgl/external/granny.h>
//}
#pragma link "granny2.lib"

namespace cgl{
namespace graphics{

//
// base
//

//granny member type to OpenGL type
GLuint granny_glType(granny_member_type t)
{
    switch(t){
    case GrannyReal32Member:
        return GL_FLOAT;
    case GrannyInt8Member:
        return GL_BYTE;
    case GrannyUInt8Member:
    //case GrannyBinormalInt8Member:
    //GrannyNormalUInt8Member:
        return GL_UNSIGNED_BYTE;
    case GrannyInt16Member:
        return GL_SHORT;
    case GrannyUInt16Member:
        return GL_UNSIGNED_SHORT;
    //GrannyBinormalInt16Member,
    //GrannyNormalUInt16Member,
    case GrannyInt32Member:
        return GL_INT;
    case GrannyUInt32Member:
        return GL_UNSIGNED_INT;
    case GrannyReal16Member:
        return GL_HALF_FLOAT;
    //case GrannyBool32Member:
    //    return GL_BOOL;
    default:
        return 0;
    }
}

// granny index type to OpenGL type
//granny_mesh_indexType
GLuint granny_indexType(granny_mesh* mesh)
{
    switch(GrannyGetMeshBytesPerIndex(mesh)){
    case 2:
        return GL_UNSIGNED_SHORT;
    case 4:
        return GL_UNSIGNED_INT;
    default:
        throw "granny_indexType error.";
    }
}

void draw_elements(granny_mesh* mesh)
{
    int size = GrannyGetMeshIndexCount(mesh);
    void* indices = GrannyGetMeshIndices(mesh);
    glDrawElements(GL_TRIANGLES, size, granny_indexType(mesh), indices);
}

//granny_pngbt33332_vertex
void draw_elements(int shape, const granny_pngbt33332_vertex* vs, const int* indices, int size)
{
    glVertexPointer(3, GL_FLOAT, sizeof(granny_pngbt33332_vertex), vs);
    glTexCoordPointer(2, GL_FLOAT, sizeof(granny_pngbt33332_vertex), vs->UV);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawElements(shape, size, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void draw_mesh(granny_mesh* mesh, std::vector<granny_pnt332_vertex>& vertices)
{
    glVertexPointer(3, GL_FLOAT, sizeof(granny_pnt332_vertex), vertices[0].Position);
    glTexCoordPointer(2, GL_FLOAT, sizeof(granny_pnt332_vertex), vertices[0].UV);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    draw_elements(mesh);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//转换蒙皮动画顶点
void deformer_vertices(std::vector<granny_pnt332_vertex>& dest, granny_mesh* mesh, granny_mesh_binding* binding, granny_world_pose* worldPose)
{
    void* source = GrannyGetMeshVertices(mesh);
    int count = GrannyGetMeshVertexCount(mesh);

    dest.resize(count);

    float *mat = (float*)GrannyGetWorldPoseComposite4x4Array(worldPose);

    granny_mesh_deformer* deformer = GrannyNewMeshDeformer(
        GrannyGetMeshVertexType(mesh),
        GrannyPNT332VertexType,
        GrannyDeformPositionNormal,
        GrannyDontAllowUncopiedTail);

    granny_int32x const * boneIndices = GrannyGetMeshBindingFromBoneIndices(binding);

    GrannyDeformVertices(deformer,
        boneIndices,
        mat,
        count,
        source,
        &dest[0]);

    GrannyFreeMeshDeformer(deformer);
}

// draw granny_file_info->Meshs[]
int draw_mesh(granny_mesh* mesh)
{
    void * vs = GrannyGetMeshVertices(mesh);
    granny_data_type_definition* type = GrannyGetMeshVertexType(mesh);
    //void* ids = GrannyGetMeshIndices(mesh);

    //granny_debug(type);

    //绑定顶点格式
    int stride = GrannyGetTotalObjectSize(type);
    granny_data_type_definition* t = type;
    int offset = 0;
    byte_t *pointer = (byte_t*)vs;
    while(t->Type != GrannyEndMember){
        if(!strcmp(t->Name, GrannyVertexPositionName)){
            glVertexPointer(t->ArrayWidth, granny_glType(t->Type), stride, pointer);
            glEnableClientState(GL_VERTEX_ARRAY);
        }
        else if(!strcmp(t->Name, "TextureCoordinates0")){
            glTexCoordPointer(t->ArrayWidth, granny_glType(t->Type), stride, pointer);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        offset += GrannyGetMemberTypeSize(t);
        pointer += GrannyGetMemberTypeSize(t);
        ++t;
    }

    draw_elements(mesh);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void sample_draw(granny_mesh* mesh)
{
    granny_tri_material_group* triangleGroups = GrannyGetMeshTriangleGroups(mesh);

    int numTriangles = GrannyGetMeshTriangleCount(mesh);
    int numSurfaces = GrannyGetMeshTriangleGroupCount(mesh);

    std::vector<granny_pngbt33332_vertex> vertices;
    int numVertices = GrannyGetMeshVertexCount(mesh);
    vertices.resize(numVertices);
    //ok for now we load gr2 with Tangents and Binormals
    GrannyCopyMeshVertices(mesh, GrannyPNGBT33332VertexType, &vertices[0]);

    int nIndices = GrannyGetMeshIndexCount(mesh);
    std::vector<int> indices;
    indices.resize(nIndices);
    GrannyCopyMeshIndices(mesh, 4, &indices[0]);

    //draw_elements(GL_TRIANGLES, &vertices[0], &indices[0], indices.size());
    for(unsigned int i = 0; i < numSurfaces; i++)
    {
        //triangles[i].resize(triangleGroups[i].TriCount);
        //surfaces[i].numTriangles = triangleGroups[i].TriCount;
        draw_elements(GL_TRIANGLES, &vertices[0], &indices[triangleGroups[i].TriFirst*3], triangleGroups[i].TriCount*3);
//
        //draw_elements(GL_TRIANGLES, &vertices[0], &indices[triangleGroups[i].TriFirst*1+0], triangleGroups[i].TriCount*3);
        /*
        for (unsigned int j = 0; j < triangleGroups[i].TriCount; ++j)
        {
            int w = (triangleGroups[i].TriFirst + j)*3 ;
//			triangles[i][j].resize(3);
//			triangles[i][j][0] = ind[w];
//			triangles[i][j][1] = ind[w+1];
//			triangles[i][j][2] = ind[w+2];
//                draw_triangle(&vertices[0], indices[w], indices[w+1], indices[w+2]);
            int ids[3] = {indices[w], indices[w+1], indices[w+2]};
            draw_elements(GL_TRIANGLES, &vertices[0], ids, 3);// &indices[triangleGroups[i].TriFirst*3], triangleGroups[i].TriCount*3);
        }
        //*/
    }
}

void draw_skeleton( granny_skeleton *skeleton, granny_world_pose *worldPose )
{
	int boneCount = skeleton->BoneCount;
	granny_bone *bones = skeleton->Bones;

    glBindTexture(GL_TEXTURE_2D, 0);

    glPointSize(4.0f);
	// Draw points at all the bone locations
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_POINTS );
	for( int i = 0; i < boneCount; ++i ) {
		float *transform = GrannyGetWorldPose4x4( worldPose, i );
		glVertex3fv(&transform[12]);
	}
	glEnd();


    glLineWidth(2.0f);
	glBegin(GL_LINES);
	for( int i = 0; i < boneCount; ++i ) {
		float *transform = GrannyGetWorldPose4x4( worldPose, i );

		// Draw a line connecting each child bone to its parent
		if( bones[i].ParentIndex != GrannyNoParentBone ) {
			float *parentTransform = GrannyGetWorldPose4x4( worldPose, bones[i].ParentIndex );

			glColor3f( 1.0f, 1.0f, 1.0f );
			glVertex3fv( &transform[12] );

			glColor3f( 0.7f, 0.7f, 0.7f );
			glVertex3fv( &parentTransform[12] );
		}

		// Draw a scaled axis system at each of the bones
		float scale = 1.0f;

		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3fv( &transform[12] );
		glVertex3f( transform[12] + scale*transform[0], transform[13] + scale*transform[1], transform[14] + scale*transform[2] );

		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3fv( &transform[12] );
		glVertex3f( transform[12] + scale*transform[4], transform[13] + scale*transform[5], transform[14] + scale*transform[6] );

		glColor3f( 0.0f, 0.0f, 1.0f );
		glVertex3fv( &transform[12] );
		glVertex3f( transform[12] + scale*transform[8], transform[13] + scale*transform[9], transform[14] + scale*transform[10] );
	}
	glEnd();

    // Restore gl state
    glColor3f(1.0f, 1.0f, 1.0f);
}

//
// debug info
//

void granny_debug(granny_data_type_definition* t)
{
    CGL_LOG("GrannyGetMemberUnitSize  = %i", GrannyGetMemberUnitSize(t));
    CGL_LOG("GrannyGetMemberTypeSize  = %i", GrannyGetMemberTypeSize(t));
    CGL_LOG("GrannyGetTotalObjectSize = %i", GrannyGetTotalObjectSize(t));
    CGL_LOG("GrannyGetTotalTypeSize   = %i", GrannyGetTotalTypeSize(t));
    while(t->Type != GrannyEndMember){
        CGL_LOG("name: %-20s width: %i ctype: %-16s type: %-40s",
            t->Name,
            t->ArrayWidth,
            GrannyGetMemberCTypeName(t->Type),
            GrannyGetMemberTypeName(t->Type)
            );

        //print(GrannyGetMemberMarshalling(t), GrannyGetObjectMarshalling(t));
        t++;
    }
}

void granny_debug(granny_animation* animation)
{
    CGL_LOG("    Animation : %s  loops: %i", animation->Name, animation->DefaultLoopCount);
    CGL_LOG("      Duration: %f TimeStep: %f Oversampling: %f", animation->Duration, animation->TimeStep, animation->Oversampling);
    CGL_LOG("    TrackGroups: %i", animation->TrackGroupCount);
    for(int i=0; i<animation->TrackGroupCount; ++i){
        CGL_LOG("      TrackGroup: %s", animation->TrackGroups[i]->Name);
    }
}

void granny_debug(granny_file_info* info)
{
    CGL_LOG("filename: %s", info->FromFileName);
    //Texture
    CGL_LOG("  TextureCount: %i", info->TextureCount);
    for(int i=0; i<info->TextureCount; ++i){
        CGL_LOG("    Texture %i: %s", i, info->Textures[i]->FromFileName);
    }
    //Material
    CGL_LOG("  MaterialCount: %i", info->MaterialCount);
    for(int i=0; i<info->MaterialCount; ++i){
        CGL_LOG("    Material %i: %s", i, info->Materials[i]->Name);
    }
    //Skeleton
    CGL_LOG("  SkeletonCount: %i", info->SkeletonCount);
    for(int i=0; i<info->SkeletonCount; ++i){
        CGL_LOG("    Skeleton %i: %s", i, info->Skeletons[i]->Name);
    }
    //Mesh
    CGL_LOG("  MesheCount: %i", info->MeshCount);
    for(int i=0; i<info->MeshCount; ++i){
        CGL_LOG("    Mesh %i: %s", i, info->Meshes[i]->Name);
    }
    //Model
    CGL_LOG("  ModelCount: %i", info->ModelCount);
    for(int i=0; i<info->ModelCount; ++i){
        CGL_LOG("    Model %i: %s", i, info->Models[i]->Name);
    }
    //Animation
    CGL_LOG("  AnimationCount: %i", info->AnimationCount);
    for(int i=0; i<info->AnimationCount; ++i){
        granny_debug(info->Animations[i]);
    }
}

//
// class
//


struct grannyTexture
{
    char *name;
    int id;
};

typedef std::vector<grannyTexture*> grannyTextures;

class grannyFile
{
public:
    granny_file* handle;
    granny_file_info* data;
    grannyTextures textures;
    int tex;    //测试用纹理

    grannyFile() : handle(), data(), textures(), tex()
    {

    }

    ~grannyFile()
    {
        this->close();
    }

    int open(const char* filename)
    {
        std::vector<byte_t> buf;
        io::load_file(buf, filename);
        handle = GrannyReadEntireFileFromMemory(buf.size(), reinterpret_cast<char*>(&buf[0]));
        data = GrannyGetFileInfo(handle);
        granny_debug(data);
        return 0;
    }

    void close()
    {
        if(handle){
            GrannyFreeFile(handle);
        }
        data = null;
        textures.clear();
    }

    void paint()
    {
        for(int i=0; i<data->MeshCount; ++i){
            granny_mesh* mesh = data->Meshes[i];
            draw_mesh(mesh);
        }
    }
};

class grannyMesh
{
public:
    granny_mesh *handle;
    granny_mesh_binding *binding;
    granny_mesh_deformer *deformer;
    grannyTextures textures;
    granny_skeleton *skeleton;

public:
    grannyMesh() : handle(), binding(), deformer(), textures()
    {

    }

    ~grannyMesh()
    {
        this->dispose();
    }

    void create(granny_mesh *mesh, granny_model_instance *inModel, const grannyTextures& tex)
    {
        this->dispose();

        handle = mesh;

        skeleton = GrannyGetSourceSkeleton( inModel );
        binding = GrannyNewMeshBinding( mesh, skeleton, skeleton );

        textures.resize(mesh->MaterialBindingCount);
        for( int i = 0; i < textures.size(); ++i ) {
            //mesh->textureReferences[i] = zGrannyFindTexture( inScene, grannyMesh->MaterialBindings[i].Material );
        }

        //int vertexCount = GrannyGetMeshVertexCount( grannyMesh );

        if( GrannyMeshIsRigid(mesh) ) {
            #if 0
            granny_pnt332_vertex *tempVertices = new granny_pnt332_vertex[vertexCount];
            GrannyCopyMeshVertices( grannyMesh, GrannyPNT332VertexType, tempVertices );

            mesh->displayList = glGenLists(1);
            if( mesh->displayList ) {
                glNewList( mesh->displayList, GL_COMPILE );

                glEnableClientState( GL_VERTEX_ARRAY );
                glEnableClientState( GL_NORMAL_ARRAY );
                glEnableClientState( GL_TEXTURE_COORD_ARRAY );

                //zGrannyRenderMesh( mesh, tempVertices );

                glDisableClientState( GL_VERTEX_ARRAY );
                glDisableClientState( GL_NORMAL_ARRAY );
                glDisableClientState( GL_TEXTURE_COORD_ARRAY );

                glEndList();
            }
            else {
                assert(0);
                //ErrorMessage( "Unable to allocate GL display list for ZGrannyMesh %s - this ZGrannyMesh will not render properly.", grannyMesh->Name );
            }

            delete [] tempVertices;
            #endif

            // Since this is a rigid ZGrannyMesh, we won't need a deformer.
            deformer = 0;
        }
        else {
    		deformer = GrannyNewMeshDeformer(GrannyGetMeshVertexType(mesh),
                GrannyPNT332VertexType,
                GrannyDeformPositionNormal,
                GrannyDontAllowUncopiedTail);
            if( !deformer ) {
                //assert(0);
                //ErrorMessage( "Granny didn't find a matching deformer for the vertex format used by ZGrannyMesh \"%s\".  This ZGrannyMesh won't be rendered properly.", grannyMesh->Name );
            }
        }
    }

    void dispose()
    {
        handle = null;
        if(binding){
            GrannyFreeMeshBinding(binding);
            binding = null;
        }
        if(deformer){
            GrannyFreeMeshDeformer(deformer);
            deformer = null;
        }
        textures.clear();
    }

};


/*
//查找可用纹理
ZGrannyTexture *zGrannyFindTexture( ZGrannyScene *scene, granny_material *grannyMaterial ) {
	granny_texture *grannyTexture = GrannyGetMaterialTextureByType( grannyMaterial, GrannyDiffuseColorTexture );
	if( grannyTexture ) {
		// Look through all the textures in the file for a match-by-name.
		for(int i = 0; i < scene->textureCount; ++i ) {
			ZGrannyTexture &texture = scene->textures[i];
			if( strcmp(texture.name, grannyTexture->FromFileName) == 0 ) {
				return(&texture);
			}
		}
	}

	return(0);
}*/


class grannyControl
{
public:
    granny_control* handle;
    float timeMax;
    float timeStep;

    grannyControl() : handle()
    {
    }

    ~grannyControl()
    {
        this->dispose();
    }

    int create(granny_model_instance* instance, granny_animation* animation)
    {
        this->dispose();
        handle = GrannyPlayControlledAnimation( 0.0f, animation, instance );
		if( handle ) {
            GrannySetControlActive(handle, true);
            GrannySetControlSpeed(handle, 1.0f);
            GrannySetControlWeight(handle, 1.0f);
            timeMax = animation->Duration;
            timeStep = animation->TimeStep;
        }

        /*
        //创建第二个控制点
        granny_model* model = GrannyGetSourceModel(instance);
        DetailedControl = NULL;
        granny_int32x TrackGroupIndex;
        if(GrannyFindTrackGroupForModel(animation, model->Name, &TrackGroupIndex))
        {
            granny_controlled_animation_builder* Builder = GrannyBeginControlledAnimation(0.0f, animation);
            assert(Builder);

            GrannySetTrackGroupTarget(Builder, TrackGroupIndex, instance);
            GrannySetTrackGroupLOD(Builder, TrackGroupIndex, true, 1.0f);
            DetailedControl = GrannyEndControlledAnimation(Builder);
            GrannySetControlLoopCount( DetailedControl, 10 );
            GrannySetControlWeight(DetailedControl, 1.0);
        }//*/

        return 0;
    }

    void create(granny_model_instance* instance, granny_animation_binding* binding, granny_animation* animation)
    {
        this->dispose();
        handle = GrannyPlayControlledAnimationBinding( 0.0f, animation, binding, instance );
		if( handle ) {
            //GrannySetControlActive(handle, true);
        }
    }

    void dispose()
    {
        if(handle){
            GrannyFreeControl(handle);
            //GrannyFreeControlOnceUnused( control );
            handle = null;
        }
    }

    void active(bool value)
    {
        if(handle){
            GrannySetControlActive(handle, value);
        }
    }

    bool is_active()const
    {
        return handle ? GrannyControlIsActive(handle) : false;
    }

    //以下这些貌似没用
    int loop_count()
    {
        return handle ? GrannyGetControlLoopCount(handle) : 0;
    }

    void set_loop_count(int value)
    {
        if(handle){
            GrannySetControlLoopCount( handle, value );
        }
    }

    int frame()
    {
        return handle ? GrannyGetControlLoopIndex(handle) : 0;
    }

    void update(int value)
    {
        if(handle){
            GrannySetControlLoopIndex( handle, value );
        }
    }

//    float duration()const
//    {
//        return handle ? GrannyGetControlDuration(handle) : 0.0f;
//    }

    void update()
    {
        if(handle){
            int id = frame();
            int max = loop_count();
            if(++id >= max){
                id = 0;
            }
            //print(id);
            //GrannyCompleteControlAt(handle, std::clock());
            //update(id);
            //GrannyRecenterControlClocks(handle, 0.2 * id);
            //GrannyEaseControlIn(handle, 0.2, true);

        }
    }
};


class grannyModel
{
public:
    granny_model_instance   *instance;
    granny_world_pose       *worldPose;
    granny_local_pose       *localPose;
    std::vector<grannyMesh> meshes;

    mat4f mat;

    grannyControl control;
    std::vector<granny_pnt332_vertex> vertices;
    float localTime;

public:
    grannyModel() :
        instance(),
        worldPose(),
        localPose(),
        meshes(),
        control(),
        vertices()
    {
        localTime = 0;
    }

    ~grannyModel()
    {

    }

    void create( granny_model *model, const grannyTextures& texlist )
    {
        this->dispose();

        int boneCount = model->Skeleton->BoneCount;

        instance = GrannyInstantiateModel( model );
        worldPose = GrannyNewWorldPose( boneCount );
        localPose = GrannyNewLocalPose( boneCount );

        GrannyGetModelInitialPlacement4x4( model, mat.data );

        meshes.resize( model->MeshBindingCount );
        for( size_t i = 0; i < meshes.size(); ++i ) {
            meshes[i].create(model->MeshBindings[i].Mesh, instance, texlist);
        }
    }

    void dispose()
    {
        if(worldPose){
            GrannyFreeWorldPose(worldPose);
            worldPose = null;
        }
        if(localPose){
            GrannyFreeLocalPose(localPose);
            localPose = null;
        }
        meshes.clear();
        if(instance){
            GrannyFreeModelInstance(instance);
            instance = null;
        }
    }

    void set_animation(granny_animation* animation)
    {
        control.create(instance, animation);
    }

    void update(float t)
    {
        //GrannySetModelClock(instance, fmod((std::clock()/10000.0), 4.0));
        localTime += control.timeStep;
        if(localTime > control.timeMax)localTime = 0;
        //print(control.timeMax);
        GrannySetModelClock(instance, localTime);

        granny_model* model = GrannyGetSourceModel(instance);
        granny_skeleton *skeleton = GrannyGetSourceSkeleton(instance);
        granny_mesh* mesh = model->MeshBindings->Mesh;

        granny_int32x boneIndex = 0;
//        if (!GrannyFindBoneByName(skeleton, "Bip01", &boneIndex))
//        {
//            printf("Error: unable to find neck bone\n");
//            return ;
//        }

        GrannySampleModelAnimations(instance, boneIndex, skeleton->BoneCount, localPose);
        GrannyBuildWorldPose(skeleton, boneIndex, skeleton->BoneCount, localPose, NULL, worldPose);

        //update vertices
        deformer_vertices(vertices, meshes[0].handle, meshes[0].binding, worldPose);



        /*
        granny_matrix_4x4 ModelTransform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        GrannySampleModelAnimationsAccelerated(instance, skeleton->BoneCount, (float*)ModelTransform, localPose, worldPose);

        int const *ToBoneIndices = GrannyGetMeshBindingToBoneIndices(meshes[0].binding);
        granny_matrix_4x4 CompositeBuffer[64];
        int const NumMeshBones = GrannyGetMeshBindingBoneCount(meshes[0].binding);//36ge
        GrannyBuildIndexedCompositeBuffer(
            GrannyGetMeshBindingToSkeleton(meshes[0].binding),
            worldPose,
            ToBoneIndices, NumMeshBones,
            CompositeBuffer);
        */
    }

    int paint(int tex)
    {
        granny_model* model = GrannyGetSourceModel(instance);

        //luna.gl.bind_texture(tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        draw_mesh(model->MeshBindings->Mesh, vertices);

        //draw_mesh( model->MeshBindings->Mesh );
        for( int i = 0; i < meshes.size(); ++i ) {

            //draw_mesh( meshes[i].handle);
        }

        //luna.gl.bind_texture(0);
        draw_skeleton(model->Skeleton, worldPose);
        for( int i = 0; i < meshes.size(); ++i ) {
            //zGrannyRenderSkeleton(meshes[i].skeleton, worldPose);
        }
    }


};

class grannyAnimation
{
public:
	granny_camera camera;
    std::vector<grannyModel> models;

    grannyAnimation() : camera(), models()
    {

    }

    ~grannyAnimation()
    {

    }

    void create(grannyFile& file)
    {
        models.resize(file.data->ModelCount);
        for(int i=0; i<models.size(); ++i){
            granny_model* model = file.data->Models[i];
            models[i].create(model, file.textures);
        }

    }

    void dispose()
    {
        models.clear();
    }

    void set_animation( granny_animation* animation )
    {
        for(size_t i=0; i<models.size(); ++i){
            models[i].set_animation(animation);
        }
    }

    void update()
    {
        for(size_t i=0; i<models.size(); ++i){
            models[i].update(0.1);
        }
    }

    void paint(int tex)
    {
        for(size_t i=0; i<models.size(); ++i){
            models[i].paint(tex);
        }
    }

};





}//end namespace graphics
}//end namespace cgl

#endif //GRANNY_HPP_20200428101910

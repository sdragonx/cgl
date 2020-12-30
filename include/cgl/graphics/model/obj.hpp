/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 obj.hpp

 2020-01-05 13:27:09

*/
#ifndef MODEL_OBJ_HPP_20200105132709
#define MODEL_OBJ_HPP_20200105132709

#include <cgl/graphics/gl/gl.hpp>
#include <vector>

namespace cgl{
namespace graphics{
//namespace model{

class model_obj
{
public:
    struct FACE
    {
        int v;
        int vt;
        int vn;
    };

    struct MATERIAL
    {
        vec3f ka, kd, ks;
        int texture;
    };

    std::vector<vec3f> vertices;
    std::vector<vec2f> uvs;
    std::vector<vec3f> normals;
    std::vector<FACE> face;

public:
    int load(const char* file)
    {
        using namespace std;
        char buf[1024] = {0};
        FILE* f = fopen(file, "r");

        if(!f){
            return -1;
        }

        while(!feof(f)){
            std::fgets(buf, 1024, f);
            if(buf[0] && buf[0] != '#'){
                parse_line(buf);
            }
        }
        fclose(f);

        f = fopen("0.mtl", "r");
        if(!f){
            return -1;
        }
        while(!feof(f)){
            std::fgets(buf, 1024, f);
            if(buf[0] && buf[0] != '#'){
                parse_mtl(buf);
            }
        }
        fclose(f);
        return 0;
    }

    void paint()
    {
        #ifndef CGL_PLATFORM_GLES
        glPushMatrix();
        glScalef(64, 64, 64);
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        for(size_t i=0; i<face.size(); ++i){
            glTexCoord2fv(uvs[face[i].vt-1].data);
            glVertex3fv(vertices[face[i].v-1].data);
            glNormal3fv(normals[face[i].vn-1].data);
        }
        glEnd();
        glPopMatrix();
        #endif
    }

private:
    void parse_line(const char* line)
    {
        if(line[0] == 'v'){
            float x, y, z;
            if(line[1] == ' '){//vertex
                sscanf(line + 2, "%f %f %f", &x, &y, &z);
                vertices.push_back(vec3f(x, y, z));
            }
            else if(line[1] == 't'){//texture coord
                sscanf(line + 3, "%f %f", &x, &y);
                uvs.push_back(vec2f(x, y));
            }
            else if(line[1] == 'n'){//normal
                sscanf(line + 3, "%f %f %f", &x, &y, &z);
                normals.push_back(vec3f(x, y, z));
            }
        }
        else if(line[0] == 'f'){//face
            //f 160//160 10//10 1//1
            FACE f[4] = {0};
            int p = sscanf(line + 2, "%i/%i/%i%*c%i/%i/%i%*c%i/%i/%i%*c%i/%i/%i",
                &f[0].v, &f[0].vt, &f[0].vn,
                &f[1].v, &f[1].vt, &f[1].vn,
                &f[2].v, &f[2].vt, &f[2].vn,
                &f[3].v, &f[3].vt, &f[3].vn);

            face.push_back(f[0]);
            face.push_back(f[1]);
            face.push_back(f[2]);
            face.push_back(f[3]);

        }

    }

    // ka ambient color
    // kd diffuse color
    // ks specular color

    void parse_mtl(const char* line)
    {
        char name[8] = {0};

        if(line[0] == 'v'){
            //float x, y, z;
        }
    }



};

//}//end namespace model
}//end namespace graphics
}//end namespace cgl

#endif //MODEL_OBJ_HPP_20200105132709

/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glmultisample

 2019-06-17 07:13:48

*/
#ifndef GLMULTISAMPLE_20190617071348
#define GLMULTISAMPLE_20190617071348

#include "gl.hpp"

namespace cgl{
namespace graphics{


class glmultisample
{
protected:
    GLuint framebuffer;
    GLuint colorbuffer;
    GLuint depth_stencil;
    int buffer_mode;        //1 renderbuffer; 2 texture2D

    bool m_is_enabled;
    int m_width;
    int m_height;

    enum {
        RENDER_BUFFER_MODE = 1,
        TEXTURE_BUFFER_MODE = 2
    };

public:
    glmultisample() : framebuffer(), colorbuffer(), depth_stencil()
    {
        m_is_enabled = false;
        m_width = m_height = 0;
    }

    static int sample_buffers()
    {
        int n;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &n);
        return n;
    }

    //Context reports MSAA is available with %i samples
    static int samples()
    {
        int n;
        glGetIntegerv(GL_SAMPLES, &n);
        return n;
    }

    static int max_samples()
    {
        int samples;
        glGetIntegerv(GL_MAX_SAMPLES, &samples);
        return samples;
    }

    int create(GLuint width, GLuint height, GLuint level = 4)
    {
        if(glTexImage2DMultisample){
            return create_texturebuffer(width, height, level);
        }
        else{
            return create_renderbuffer(width, height, level);
        }
    }

    void dispose()
    {
        if(framebuffer){
            glDeleteFramebuffers(1, &framebuffer);
        }

        if(buffer_mode == RENDER_BUFFER_MODE){
            if(colorbuffer){
                glDeleteRenderbuffers(1, &colorbuffer);
            }
            if(depth_stencil){
                glDeleteRenderbuffers(1, &depth_stencil);
            }
        }
        else{
            if(colorbuffer){
                glDeleteTextures(1, &colorbuffer);
            }
            if(depth_stencil){
                glDeleteTextures(1, &depth_stencil);
            }
        }

        framebuffer = colorbuffer = depth_stencil = 0;
        m_is_enabled = false;
        m_width = m_height = 0;
    }

    void enable()
    {
        glEnable(GL_MULTISAMPLE);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        m_is_enabled = true;
    }

    void disable()
    {
        glDisable(GL_MULTISAMPLE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_is_enabled = false;
    }

    bool is_enabled()const
    {
        return m_is_enabled;
    }

    void active()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    }

    void bitblt(int x, int y, int width, int height)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer); // Make sure your multisampled FBO is the read framebuffer
        glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_BACK);
        glBlitFramebuffer(x, y, width, height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    //¥¥Ω®‰÷»æª∫≥Â
    int create_renderbuffer(int width, int height, GLuint level)
    {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        //color
        glGenRenderbuffers(1, &colorbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
        if(0 == level){
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
        }
        else{
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, level, GL_RGBA8, width, height);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //depth
        glGenRenderbuffers(1, &depth_stencil);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, level, GL_DEPTH_STENCIL, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_width = width;
        m_height = height;
        buffer_mode = TEXTURE_BUFFER_MODE;

        return 0;
    }

    //¥¥Ω®Œ∆¿Ì‰÷»æª∫≥Â
    int create_texturebuffer(GLuint width, GLuint height, GLuint level)
    {
        if(!colorbuffer){
            glGenTextures( 1, &colorbuffer );
        }
        glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, colorbuffer );
        glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, level, GL_RGBA8, width, height, false );

        if(!framebuffer){
            glGenFramebuffers( 1, &framebuffer );
        }
        glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorbuffer, 0 );

        if(!depth_stencil){
            glGenTextures( 1, &depth_stencil);
        }
        glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, depth_stencil );
        glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, level, GL_DEPTH_STENCIL, width, height, false );

        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth_stencil, 0 );
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthStencil,0);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texDepthStencil,0);

        GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if( result == GL_FRAMEBUFFER_COMPLETE) {
            printf("multisample complete!\n");
        }
        else{
            printf("multisample faild!\n");
        }

        glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, 0 );
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        m_width = width;
        m_height = height;
        buffer_mode = TEXTURE_BUFFER_MODE;

        return 0;

//        glGenRenderbuffers(1, &rbo);
//        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//        glRenderbufferStorageMultisample(GL_RENDERBUFFER, level, GL_DEPTH24_STENCIL8, width, height);
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }



};

}//end namespace graphics
}//end namespace cgl

#endif //GLMULTISAMPLE_20190617071348


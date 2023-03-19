//-----------------------------------------------------------------------------
// About imgui_impl_opengl3_loader.h:
//
// We embed our own OpenGL loader to not require user to provide their own or to have to use ours,
// which proved to be endless problems for users.
// Our loader is custom-generated, based on gl3w but automatically filtered to only include
// enums/functions that we use in our imgui_impl_opengl3.cpp source file in order to be small.
//
// YOU SHOULD NOT NEED TO INCLUDE/USE THIS DIRECTLY. THIS IS USED BY imgui_impl_opengl3.cpp ONLY.
// THE REST OF YOUR APP SHOULD USE A DIFFERENT GL LOADER: ANY GL LOADER OF YOUR CHOICE.
//
// IF YOU GET BUILD ERRORS IN THIS FILE (commonly macro redefinitions or function redefinitions):
// IT LIKELY MEANS THAT YOU ARE BUILDING 'imgui_impl_opengl3.cpp' OR INCUDING 'imgui_impl_opengl3_loader.h'
// IN THE SAME COMPILATION UNIT AS ONE OF YOUR FILE WHICH IS USING A THIRD-PARTY OPENGL LOADER.
// (e.g. COULD HAPPEN IF YOU ARE DOING A UNITY/JUMBO BUILD, OR INCLUDING .CPP FILES FROM OTHERS)
// YOU SHOULD NOT BUILD BOTH IN THE SAME COMPILATION UNIT.
// BUT IF YOU REALLY WANT TO, you can '#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM' and imgui_impl_opengl3.cpp
// WILL NOT BE USING OUR LOADER, AND INSTEAD EXPECT ANOTHER/YOUR LOADER TO BE AVAILABLE IN THE COMPILATION UNIT.
//
// Regenerate with:
//   python gl3w_gen.py --output ../imgui/backends/imgui_impl_opengl3_loader.h --ref ../imgui/backends/imgui_impl_opengl3.cpp ./extra_symbols.txt
//
// More info:
//   https://github.com/dearimgui/gl3w_stripped
//   https://github.com/ocornut/imgui/issues/4445
//-----------------------------------------------------------------------------

/*
 * This file was generated with gl3w_gen.py, part of imgl3w
 * (hosted at https://github.com/dearimgui/gl3w_stripped)
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __gl3w_h_
#define __gl3w_h_


 // Adapted from KHR/khrplatform.h to avoid including entire file.
#ifndef __khrplatform_h_
typedef          float         khronos_float_t;
typedef signed   char          khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;
#ifdef _WIN64
typedef signed   long long int khronos_intptr_t;
typedef signed   long long int khronos_ssize_t;
#else
typedef signed   long  int     khronos_intptr_t;
typedef signed   long  int     khronos_ssize_t;
#endif

#if defined(_MSC_VER) && !defined(__clang__)
typedef signed   __int64       khronos_int64_t;
typedef unsigned __int64       khronos_uint64_t;
#elif (defined(__clang__) || defined(__GNUC__)) && (__cplusplus < 201100)
#include <stdint.h>
typedef          int64_t       khronos_int64_t;
typedef          uint64_t      khronos_uint64_t;
#else
typedef signed   long long     khronos_int64_t;
typedef unsigned long long     khronos_uint64_t;
#endif
#endif  // __khrplatform_h_

#ifndef __gl_glcorearb_h_
#define __gl_glcorearb_h_ 1
#ifdef __cplusplus
extern "C" {
#endif
    /*
    ** Copyright 2013-2020 The Khronos Group Inc.
    ** SPDX-License-Identifier: MIT
    **
    ** This header is generated from the Khronos OpenGL / OpenGL ES XML
    ** API Registry. The current version of the Registry, generator scripts
    ** used to make the header, and the header can be found at
    **   https://github.com/KhronosGroup/OpenGL-Registry
    */
#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif
    /* glcorearb.h is for use with OpenGL core profile implementations.
    ** It should should be placed in the same directory as gl.h and
    ** included as <GL/glcorearb.h>.
    **
    ** glcorearb.h includes only APIs in the latest OpenGL core profile
    ** implementation together with APIs in newer ARB extensions which
    ** can be supported by the core profile. It does not, and never will
    ** include functionality removed from the core profile, such as
    ** fixed-function vertex and fragment processing.
    **
    ** Do not #include both <GL/glcorearb.h> and either of <GL/gl.h> or
    ** <GL/glext.h> in the same source file.
    */
    /* Generated C header for:
     * API: gl
     * Profile: core
     * Versions considered: .*
     * Versions emitted: .*
     * Default extensions included: glcore
     * Additional extensions included: _nomatch_^
     * Extensions removed: _nomatch_^
     */
#ifndef GL_VERSION_1_0
    typedef void GLvoid;
    typedef unsigned int GLenum;

    typedef khronos_float_t GLfloat;
    typedef int GLint;
    typedef int GLsizei;
    typedef unsigned int GLbitfield;
    typedef double GLdouble;
    typedef unsigned int GLuint;
    typedef unsigned char GLboolean;
    typedef khronos_uint8_t GLubyte;
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_TRIANGLES                      0x0004
#define GL_ONE                            1
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_FRONT_AND_BACK                 0x0408
#define GL_POLYGON_MODE                   0x0B40
#define GL_CULL_FACE                      0x0B44
#define GL_DEPTH_TEST                     0x0B71
#define GL_STENCIL_TEST                   0x0B90
#define GL_VIEWPORT                       0x0BA2
#define GL_BLEND                          0x0BE2
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_TEXTURE_2D                     0x0DE1
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_RGBA                           0x1908
#define GL_FILL                           0x1B02
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_LINEAR                         0x2601
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_VALIDATE_STATUS                0x8B83
    typedef void (APIENTRYP PFNGLPOLYGONMODEPROC) (GLenum face, GLenum mode);
    typedef void (APIENTRYP PFNGLSCISSORPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
    typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
    typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
    typedef void (APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
    typedef void (APIENTRYP PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    typedef void (APIENTRYP PFNGLDISABLEPROC) (GLenum cap);
    typedef void (APIENTRYP PFNGLENABLEPROC) (GLenum cap);
    typedef void (APIENTRYP PFNGLFLUSHPROC) (void);
    typedef void (APIENTRYP PFNGLPIXELSTOREIPROC) (GLenum pname, GLint param);
    typedef void (APIENTRYP PFNGLREADPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
    typedef GLenum(APIENTRYP PFNGLGETERRORPROC) (void);
    typedef void (APIENTRYP PFNGLGETINTEGERVPROC) (GLenum pname, GLint* data);
    typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGPROC) (GLenum name);
    typedef GLboolean(APIENTRYP PFNGLISENABLEDPROC) (GLenum cap);
    typedef void (APIENTRYP PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
    typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
    typedef void (APIENTRYP PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glPolygonMode(GLenum face, GLenum mode);
    GLAPI void APIENTRY glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    GLAPI void APIENTRY glTexParameteri(GLenum target, GLenum pname, GLint param);
    GLAPI void APIENTRY glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
    GLAPI void APIENTRY glClear(GLbitfield mask);
    GLAPI void APIENTRY glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLAPI void APIENTRY glDisable(GLenum cap);
    GLAPI void APIENTRY glEnable(GLenum cap);
    GLAPI void APIENTRY glFlush(void);
    GLAPI void APIENTRY glPixelStorei(GLenum pname, GLint param);
    GLAPI void APIENTRY glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
    GLAPI GLenum APIENTRY glGetError(void);
    GLAPI void APIENTRY glGetIntegerv(GLenum pname, GLint* data);
    GLAPI const GLubyte* APIENTRY glGetString(GLenum name);
    GLAPI GLboolean APIENTRY glIsEnabled(GLenum cap);
    GLAPI void APIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    GLAPI void APIENTRY glValidateProgramPipeline(GLuint pipeline);
#endif
#endif /* GL_VERSION_1_0 */
#ifndef GL_VERSION_1_1
    typedef khronos_float_t GLclampf;
    typedef double GLclampd;
#define GL_TEXTURE_BINDING_2D             0x8069
    typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices);
    typedef void (APIENTRYP PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
    typedef void (APIENTRYP PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint* textures);
    typedef void (APIENTRYP PFNGLGENTEXTURESPROC) (GLsizei n, GLuint* textures);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
    GLAPI void APIENTRY glBindTexture(GLenum target, GLuint texture);
    GLAPI void APIENTRY glDeleteTextures(GLsizei n, const GLuint* textures);
    GLAPI void APIENTRY glGenTextures(GLsizei n, GLuint* textures);
#endif
#endif /* GL_VERSION_1_1 */
#ifndef GL_VERSION_1_3
#define GL_TEXTURE0                       0x84C0
#define GL_ACTIVE_TEXTURE                 0x84E0
    typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glActiveTexture(GLenum texture);
#endif
#endif /* GL_VERSION_1_3 */
#ifndef GL_VERSION_1_4
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_FUNC_ADD                       0x8006
    typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC) (GLenum mode);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    GLAPI void APIENTRY glBlendEquation(GLenum mode);
#endif
#endif /* GL_VERSION_1_4 */
#ifndef GL_VERSION_1_5
    typedef khronos_ssize_t GLsizeiptr;
    typedef khronos_intptr_t GLintptr;
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_STREAM_DRAW                    0x88E0
    typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
    typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
    typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
    typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glBindBuffer(GLenum target, GLuint buffer);
    GLAPI void APIENTRY glDeleteBuffers(GLsizei n, const GLuint* buffers);
    GLAPI void APIENTRY glGenBuffers(GLsizei n, GLuint* buffers);
    GLAPI void APIENTRY glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    GLAPI void APIENTRY glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
#endif
#endif /* GL_VERSION_1_5 */
#ifndef GL_VERSION_2_0
    typedef char GLchar;
    typedef khronos_int16_t GLshort;
    typedef khronos_int8_t GLbyte;
    typedef khronos_uint16_t GLushort;
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_UPPER_LEFT                     0x8CA2
    typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum modeRGB, GLenum modeAlpha);
    typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
    typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
    typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
    typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
    typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
    typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
    typedef void (APIENTRYP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
    typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
    typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
    typedef GLint(APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
    typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
    typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
    typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
    typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
    typedef GLint(APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
    typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVPROC) (GLuint index, GLenum pname, GLint* params);
    typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, void** pointer);
    typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
    typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
    typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
    typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    GLAPI void APIENTRY glAttachShader(GLuint program, GLuint shader);
    GLAPI void APIENTRY glCompileShader(GLuint shader);
    GLAPI GLuint APIENTRY glCreateProgram(void);
    GLAPI GLuint APIENTRY glCreateShader(GLenum type);
    GLAPI void APIENTRY glDeleteProgram(GLuint program);
    GLAPI void APIENTRY glDeleteShader(GLuint shader);
    GLAPI void APIENTRY glDetachShader(GLuint program, GLuint shader);
    GLAPI void APIENTRY glDisableVertexAttribArray(GLuint index);
    GLAPI void APIENTRY glEnableVertexAttribArray(GLuint index);
    GLAPI GLint APIENTRY glGetAttribLocation(GLuint program, const GLchar* name);
    GLAPI void APIENTRY glGetProgramiv(GLuint program, GLenum pname, GLint* params);
    GLAPI void APIENTRY glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
    GLAPI void APIENTRY glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
    GLAPI void APIENTRY glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
    GLAPI GLint APIENTRY glGetUniformLocation(GLuint program, const GLchar* name);
    GLAPI void APIENTRY glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
    GLAPI void APIENTRY glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
    GLAPI void APIENTRY glLinkProgram(GLuint program);
    GLAPI void APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    GLAPI void APIENTRY glUseProgram(GLuint program);
    GLAPI void APIENTRY glUniform1i(GLint location, GLint v0);
    GLAPI void APIENTRY glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    GLAPI void APIENTRY glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
    GLAPI void APIENTRY glValidateProgram(GLuint program);
#endif
#endif /* GL_VERSION_2_0 */
#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1
    typedef khronos_uint16_t GLhalf;
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_CLIP_DISTANCE0                 0x3000
#define GL_CLIP_DISTANCE1                 0x3001
#define GL_CLIP_DISTANCE2                 0x3002
#define GL_CLIP_DISTANCE3                 0x3003
#define GL_CLIP_DISTANCE4                 0x3004
#define GL_CLIP_DISTANCE5                 0x3005
#define GL_CLIP_DISTANCE6                 0x3006
#define GL_CLIP_DISTANCE7                 0x3007
#define GL_MAX_CLIP_DISTANCES             0x0D32
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_CONTEXT_FLAGS                  0x821E
#define GL_COMPRESSED_RED                 0x8225
#define GL_COMPRESSED_RG                  0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_CLAMP_READ_COLOR               0x891C
#define GL_FIXED_ONLY                     0x891D
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_1D_ARRAY               0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY         0x8C19
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY         0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY       0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TEXTURE_SHARED_SIZE            0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED           0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_GREEN_INTEGER                  0x8D95
#define GL_BLUE_INTEGER                   0x8D96
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_BGR_INTEGER                    0x8D9A
#define GL_BGRA_INTEGER                   0x8D9B
#define GL_SAMPLER_1D_ARRAY               0x8DC0
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW        0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_1D                 0x8DC9
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY           0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D        0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY  0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_QUERY_WAIT                     0x8E13
#define GL_QUERY_NO_WAIT                  0x8E14
#define GL_QUERY_BY_REGION_WAIT           0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT        0x8E16
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_COLOR_ATTACHMENT16             0x8CF0
#define GL_COLOR_ATTACHMENT17             0x8CF1
#define GL_COLOR_ATTACHMENT18             0x8CF2
#define GL_COLOR_ATTACHMENT19             0x8CF3
#define GL_COLOR_ATTACHMENT20             0x8CF4
#define GL_COLOR_ATTACHMENT21             0x8CF5
#define GL_COLOR_ATTACHMENT22             0x8CF6
#define GL_COLOR_ATTACHMENT23             0x8CF7
#define GL_COLOR_ATTACHMENT24             0x8CF8
#define GL_COLOR_ATTACHMENT25             0x8CF9
#define GL_COLOR_ATTACHMENT26             0x8CFA
#define GL_COLOR_ATTACHMENT27             0x8CFB
#define GL_COLOR_ATTACHMENT28             0x8CFC
#define GL_COLOR_ATTACHMENT29             0x8CFD
#define GL_COLOR_ATTACHMENT30             0x8CFE
#define GL_COLOR_ATTACHMENT31             0x8CFF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_FRAMEBUFFER_SRGB               0x8DB9
#define GL_HALF_FLOAT                     0x140B
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_COMPRESSED_RED_RGTC1           0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1    0x8DBC
#define GL_COMPRESSED_RG_RGTC2            0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2     0x8DBE
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_RG8                            0x822B
#define GL_RG16                           0x822C
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_DEPTH_BUFFER_BIT 0x00000100
typedef void (APIENTRYP PFNGLCOLORMASKIPROC) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (APIENTRYP PFNGLGETBOOLEANI_VPROC) (GLenum target, GLuint index, GLboolean* data);
typedef void (APIENTRYP PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint* data);
typedef void (APIENTRYP PFNGLENABLEIPROC) (GLenum target, GLuint index);
typedef void (APIENTRYP PFNGLDISABLEIPROC) (GLenum target, GLuint index);
typedef GLboolean(APIENTRYP PFNGLISENABLEDIPROC) (GLenum target, GLuint index);
typedef void (APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum primitiveMode);
typedef void (APIENTRYP PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void (APIENTRYP PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
typedef void (APIENTRYP PFNGLCLAMPCOLORPROC) (GLenum target, GLenum clamp);
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERPROC) (GLuint id, GLenum mode);
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERPROC) (void);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIIVPROC) (GLuint index, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint index, GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IPROC) (GLuint index, GLint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IPROC) (GLuint index, GLint x, GLint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIPROC) (GLuint index, GLuint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIPROC) (GLuint index, GLuint x, GLuint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IVPROC) (GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IVPROC) (GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IVPROC) (GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IVPROC) (GLuint index, const GLint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIVPROC) (GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIVPROC) (GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIVPROC) (GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIVPROC) (GLuint index, const GLuint* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4BVPROC) (GLuint index, const GLbyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4SVPROC) (GLuint index, const GLshort* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UBVPROC) (GLuint index, const GLubyte* v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4USVPROC) (GLuint index, const GLushort* v);
typedef void (APIENTRYP PFNGLGETUNIFORMUIVPROC) (GLuint program, GLint location, GLuint* params);
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONPROC) (GLuint program, GLuint color, const GLchar* name);
typedef GLint(APIENTRYP PFNGLGETFRAGDATALOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLUNIFORM1UIPROC) (GLint location, GLuint v0);
typedef void (APIENTRYP PFNGLUNIFORM2UIPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (APIENTRYP PFNGLUNIFORM3UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (APIENTRYP PFNGLUNIFORM4UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (APIENTRYP PFNGLUNIFORM1UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM2UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM3UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLUNIFORM4UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, const GLuint* params);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, GLuint* params);
typedef void (APIENTRYP PFNGLCLEARBUFFERIVPROC) (GLenum buffer, GLint drawbuffer, const GLint* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERUIVPROC) (GLenum buffer, GLint drawbuffer, const GLuint* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERFVPROC) (GLenum buffer, GLint drawbuffer, const GLfloat* value);
typedef void (APIENTRYP PFNGLCLEARBUFFERFIPROC) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
typedef GLboolean(APIENTRYP PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean(APIENTRYP PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef GLenum(APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void* (APIENTRYP PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLboolean(APIENTRYP PFNGLISVERTEXARRAYPROC) (GLuint array);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI void APIENTRY glGetBooleani_v(GLenum target, GLuint index, GLboolean* data);
GLAPI void APIENTRY glGetIntegeri_v(GLenum target, GLuint index, GLint* data);
GLAPI void APIENTRY glEnablei(GLenum target, GLuint index);
GLAPI void APIENTRY glDisablei(GLenum target, GLuint index);
GLAPI GLboolean APIENTRY glIsEnabledi(GLenum target, GLuint index);
GLAPI void APIENTRY glBeginTransformFeedback(GLenum primitiveMode);
GLAPI void APIENTRY glEndTransformFeedback(void);
GLAPI void APIENTRY glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
GLAPI void APIENTRY glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
GLAPI void APIENTRY glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
GLAPI void APIENTRY glClampColor(GLenum target, GLenum clamp);
GLAPI void APIENTRY glBeginConditionalRender(GLuint id, GLenum mode);
GLAPI void APIENTRY glEndConditionalRender(void);
GLAPI void APIENTRY glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
GLAPI void APIENTRY glGetVertexAttribIiv(GLuint index, GLenum pname, GLint* params);
GLAPI void APIENTRY glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params);
GLAPI void APIENTRY glVertexAttribI1i(GLuint index, GLint x);
GLAPI void APIENTRY glVertexAttribI2i(GLuint index, GLint x, GLint y);
GLAPI void APIENTRY glVertexAttribI3i(GLuint index, GLint x, GLint y, GLint z);
GLAPI void APIENTRY glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY glVertexAttribI1ui(GLuint index, GLuint x);
GLAPI void APIENTRY glVertexAttribI2ui(GLuint index, GLuint x, GLuint y);
GLAPI void APIENTRY glVertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY glVertexAttribI1iv(GLuint index, const GLint* v);
GLAPI void APIENTRY glVertexAttribI2iv(GLuint index, const GLint* v);
GLAPI void APIENTRY glVertexAttribI3iv(GLuint index, const GLint* v);
GLAPI void APIENTRY glVertexAttribI4iv(GLuint index, const GLint* v);
GLAPI void APIENTRY glVertexAttribI1uiv(GLuint index, const GLuint* v);
GLAPI void APIENTRY glVertexAttribI2uiv(GLuint index, const GLuint* v);
GLAPI void APIENTRY glVertexAttribI3uiv(GLuint index, const GLuint* v);
GLAPI void APIENTRY glVertexAttribI4uiv(GLuint index, const GLuint* v);
GLAPI void APIENTRY glVertexAttribI4bv(GLuint index, const GLbyte* v);
GLAPI void APIENTRY glVertexAttribI4sv(GLuint index, const GLshort* v);
GLAPI void APIENTRY glVertexAttribI4ubv(GLuint index, const GLubyte* v);
GLAPI void APIENTRY glVertexAttribI4usv(GLuint index, const GLushort* v);
GLAPI void APIENTRY glGetUniformuiv(GLuint program, GLint location, GLuint* params);
GLAPI void APIENTRY glBindFragDataLocation(GLuint program, GLuint color, const GLchar* name);
GLAPI GLint APIENTRY glGetFragDataLocation(GLuint program, const GLchar* name);
GLAPI void APIENTRY glUniform1ui(GLint location, GLuint v0);
GLAPI void APIENTRY glUniform2ui(GLint location, GLuint v0, GLuint v1);
GLAPI void APIENTRY glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI void APIENTRY glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI void APIENTRY glUniform1uiv(GLint location, GLsizei count, const GLuint* value);
GLAPI void APIENTRY glUniform2uiv(GLint location, GLsizei count, const GLuint* value);
GLAPI void APIENTRY glUniform3uiv(GLint location, GLsizei count, const GLuint* value);
GLAPI void APIENTRY glUniform4uiv(GLint location, GLsizei count, const GLuint* value);
GLAPI void APIENTRY glTexParameterIiv(GLenum target, GLenum pname, const GLint* params);
GLAPI void APIENTRY glTexParameterIuiv(GLenum target, GLenum pname, const GLuint* params);
GLAPI void APIENTRY glGetTexParameterIiv(GLenum target, GLenum pname, GLint* params);
GLAPI void APIENTRY glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint* params);
GLAPI void APIENTRY glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value);
GLAPI void APIENTRY glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value);
GLAPI void APIENTRY glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
GLAPI void APIENTRY glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GLAPI const GLubyte* APIENTRY glGetStringi(GLenum name, GLuint index);
GLAPI GLboolean APIENTRY glIsRenderbuffer(GLuint renderbuffer);
GLAPI void APIENTRY glBindRenderbuffer(GLenum target, GLuint renderbuffer);
GLAPI void APIENTRY glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
GLAPI void APIENTRY glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
GLAPI void APIENTRY glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
GLAPI GLboolean APIENTRY glIsFramebuffer(GLuint framebuffer);
GLAPI void APIENTRY glBindFramebuffer(GLenum target, GLuint framebuffer);
GLAPI void APIENTRY glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
GLAPI void APIENTRY glGenFramebuffers(GLsizei n, GLuint* framebuffers);
GLAPI GLenum APIENTRY glCheckFramebufferStatus(GLenum target);
GLAPI void APIENTRY glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI void APIENTRY glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLAPI void APIENTRY glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI void APIENTRY glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
GLAPI void APIENTRY glGenerateMipmap(GLenum target);
GLAPI void APIENTRY glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI void APIENTRY glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI void* APIENTRY glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI void APIENTRY glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
GLAPI void APIENTRY glBindVertexArray(GLuint array);
GLAPI void APIENTRY glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
GLAPI void APIENTRY glGenVertexArrays(GLsizei n, GLuint* arrays);
GLAPI GLboolean APIENTRY glIsVertexArray(GLuint array);
#endif
#endif /* GL_VERSION_3_0 */
#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1
#define GL_PRIMITIVE_RESTART              0x8F9D
#endif /* GL_VERSION_3_1 */
#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1
    typedef struct __GLsync* GLsync;
    typedef khronos_uint64_t GLuint64;
    typedef khronos_int64_t GLint64;
    typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
    typedef void (APIENTRYP PFNGLGETINTEGER64I_VPROC) (GLenum target, GLuint index, GLint64* data);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
#endif
#endif /* GL_VERSION_3_2 */
#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1
#define GL_SAMPLER_BINDING                0x8919
    typedef void (APIENTRYP PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
#ifdef GL_GLEXT_PROTOTYPES
    GLAPI void APIENTRY glBindSampler(GLuint unit, GLuint sampler);
#endif
#endif /* GL_VERSION_3_3 */
#ifndef GL_VERSION_4_1
    typedef void (APIENTRYP PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat* data);
    typedef void (APIENTRYP PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble* data);
#endif /* GL_VERSION_4_1 */
#ifndef GL_VERSION_4_3
    typedef void (APIENTRY* GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif /* GL_VERSION_4_3 */
#ifndef GL_VERSION_4_5
#define GL_CLIP_ORIGIN                    0x935C
    typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKI_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint* param);
    typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKI64_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint64* param);
#endif /* GL_VERSION_4_5 */
#ifndef GL_ARB_bindless_texture
    typedef khronos_uint64_t GLuint64EXT;
#endif /* GL_ARB_bindless_texture */
#ifndef GL_ARB_cl_event
    struct _cl_context;
    struct _cl_event;
#endif /* GL_ARB_cl_event */
#ifndef GL_ARB_clip_control
#define GL_ARB_clip_control 1
#endif /* GL_ARB_clip_control */
#ifndef GL_ARB_debug_output
    typedef void (APIENTRY* GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif /* GL_ARB_debug_output */
#ifndef GL_EXT_EGL_image_storage
    typedef void* GLeglImageOES;
#endif /* GL_EXT_EGL_image_storage */
#ifndef GL_EXT_direct_state_access
    typedef void (APIENTRYP PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
    typedef void (APIENTRYP PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
    typedef void (APIENTRYP PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, void** params);
    typedef void (APIENTRYP PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
    typedef void (APIENTRYP PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, void** param);
#endif /* GL_EXT_direct_state_access */
#ifndef GL_NV_draw_vulkan_image
    typedef void (APIENTRY* GLVULKANPROCNV)(void);
#endif /* GL_NV_draw_vulkan_image */
#ifndef GL_NV_gpu_shader5
    typedef khronos_int64_t GLint64EXT;
#endif /* GL_NV_gpu_shader5 */
#ifndef GL_NV_vertex_buffer_unified_memory
    typedef void (APIENTRYP PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT* result);
#endif /* GL_NV_vertex_buffer_unified_memory */
#ifdef __cplusplus
}
#endif
#endif

#ifndef GL3W_API
#define GL3W_API
#endif

#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GL3W_OK 0
#define GL3W_ERROR_INIT -1
#define GL3W_ERROR_LIBRARY_OPEN -2
#define GL3W_ERROR_OPENGL_VERSION -3

    typedef void (*GL3WglProc)(void);
    typedef GL3WglProc(*GL3WGetProcAddressProc)(const char* proc);

    /* gl3w api */
    GL3W_API int imgl3wInit(void);
    GL3W_API int imgl3wInit2(GL3WGetProcAddressProc proc);
    GL3W_API int imgl3wIsSupported(int major, int minor);
    GL3W_API GL3WglProc imgl3wGetProcAddress(const char* proc);

    /* gl3w internal state */
    union GL3WProcs {
        GL3WglProc ptr[58];
        struct {
            PFNGLACTIVETEXTUREPROC            ActiveTexture;
            PFNGLATTACHSHADERPROC             AttachShader;
            PFNGLBINDBUFFERPROC               BindBuffer;
            PFNGLBINDSAMPLERPROC              BindSampler;
            PFNGLBINDTEXTUREPROC              BindTexture;
            PFNGLBINDVERTEXARRAYPROC          BindVertexArray;
            PFNGLBLENDEQUATIONPROC            BlendEquation;
            PFNGLBLENDEQUATIONSEPARATEPROC    BlendEquationSeparate;
            PFNGLBLENDFUNCSEPARATEPROC        BlendFuncSeparate;
            PFNGLBUFFERDATAPROC               BufferData;
            PFNGLBUFFERSUBDATAPROC            BufferSubData;
            PFNGLCLEARPROC                    Clear;
            PFNGLCLEARCOLORPROC               ClearColor;
            PFNGLCOMPILESHADERPROC            CompileShader;
            PFNGLCREATEPROGRAMPROC            CreateProgram;
            PFNGLCREATESHADERPROC             CreateShader;
            PFNGLDELETEBUFFERSPROC            DeleteBuffers;
            PFNGLDELETEPROGRAMPROC            DeleteProgram;
            PFNGLDELETESHADERPROC             DeleteShader;
            PFNGLDELETETEXTURESPROC           DeleteTextures;
            PFNGLDELETEVERTEXARRAYSPROC       DeleteVertexArrays;
            PFNGLDETACHSHADERPROC             DetachShader;
            PFNGLDISABLEPROC                  Disable;
            PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
            PFNGLDRAWELEMENTSPROC             DrawElements;
            PFNGLDRAWELEMENTSBASEVERTEXPROC   DrawElementsBaseVertex;
            PFNGLENABLEPROC                   Enable;
            PFNGLENABLEVERTEXATTRIBARRAYPROC  EnableVertexAttribArray;
            PFNGLFLUSHPROC                    Flush;
            PFNGLGENBUFFERSPROC               GenBuffers;
            PFNGLGENTEXTURESPROC              GenTextures;
            PFNGLGENVERTEXARRAYSPROC          GenVertexArrays;
            PFNGLGETATTRIBLOCATIONPROC        GetAttribLocation;
            PFNGLGETERRORPROC                 GetError;
            PFNGLGETINTEGERVPROC              GetIntegerv;
            PFNGLGETPROGRAMINFOLOGPROC        GetProgramInfoLog;
            PFNGLGETPROGRAMIVPROC             GetProgramiv;
            PFNGLGETSHADERINFOLOGPROC         GetShaderInfoLog;
            PFNGLGETSHADERIVPROC              GetShaderiv;
            PFNGLGETSTRINGPROC                GetString;
            PFNGLGETSTRINGIPROC               GetStringi;
            PFNGLGETUNIFORMLOCATIONPROC       GetUniformLocation;
            PFNGLGETVERTEXATTRIBPOINTERVPROC  GetVertexAttribPointerv;
            PFNGLGETVERTEXATTRIBIVPROC        GetVertexAttribiv;
            PFNGLISENABLEDPROC                IsEnabled;
            PFNGLLINKPROGRAMPROC              LinkProgram;
            PFNGLPIXELSTOREIPROC              PixelStorei;
            PFNGLPOLYGONMODEPROC              PolygonMode;
            PFNGLREADPIXELSPROC               ReadPixels;
            PFNGLSCISSORPROC                  Scissor;
            PFNGLSHADERSOURCEPROC             ShaderSource;
            PFNGLTEXIMAGE2DPROC               TexImage2D;
            PFNGLTEXPARAMETERIPROC            TexParameteri;
            PFNGLUNIFORM1IPROC                Uniform1i;
            PFNGLUNIFORMMATRIX4FVPROC         UniformMatrix4fv;
            PFNGLUSEPROGRAMPROC               UseProgram;
            PFNGLVERTEXATTRIBPOINTERPROC      VertexAttribPointer;
            PFNGLVIEWPORTPROC                 Viewport;
            PFNGLVALIDATEPROGRAMPIPELINEPROC    ValidateProgram;
            PFNGLBLENDFUNCPROC                                   BlendFunc;
        } gl;
    };

    GL3W_API extern union GL3WProcs imgl3wProcs;

    /* OpenGL functions */
#define glActiveTexture                   imgl3wProcs.gl.ActiveTexture
#define glAttachShader                    imgl3wProcs.gl.AttachShader
#define glBindBuffer                      imgl3wProcs.gl.BindBuffer
#define glBindSampler                     imgl3wProcs.gl.BindSampler
#define glBindTexture                     imgl3wProcs.gl.BindTexture
#define glBindVertexArray                 imgl3wProcs.gl.BindVertexArray
#define glBlendEquation                   imgl3wProcs.gl.BlendEquation
#define glBlendEquationSeparate           imgl3wProcs.gl.BlendEquationSeparate
#define glBlendFuncSeparate               imgl3wProcs.gl.BlendFuncSeparate
#define glBufferData                      imgl3wProcs.gl.BufferData
#define glBufferSubData                   imgl3wProcs.gl.BufferSubData
#define glClear                           imgl3wProcs.gl.Clear
#define glClearColor                      imgl3wProcs.gl.ClearColor
#define glCompileShader                   imgl3wProcs.gl.CompileShader
#define glCreateProgram                   imgl3wProcs.gl.CreateProgram
#define glCreateShader                    imgl3wProcs.gl.CreateShader
#define glDeleteBuffers                   imgl3wProcs.gl.DeleteBuffers
#define glDeleteProgram                   imgl3wProcs.gl.DeleteProgram
#define glDeleteShader                    imgl3wProcs.gl.DeleteShader
#define glDeleteTextures                  imgl3wProcs.gl.DeleteTextures
#define glDeleteVertexArrays              imgl3wProcs.gl.DeleteVertexArrays
#define glDetachShader                    imgl3wProcs.gl.DetachShader
#define glDisable                         imgl3wProcs.gl.Disable
#define glDisableVertexAttribArray        imgl3wProcs.gl.DisableVertexAttribArray
#define glDrawElements                    imgl3wProcs.gl.DrawElements
#define glDrawElementsBaseVertex          imgl3wProcs.gl.DrawElementsBaseVertex
#define glEnable                          imgl3wProcs.gl.Enable
#define glEnableVertexAttribArray         imgl3wProcs.gl.EnableVertexAttribArray
#define glFlush                           imgl3wProcs.gl.Flush
#define glGenBuffers                      imgl3wProcs.gl.GenBuffers
#define glGenTextures                     imgl3wProcs.gl.GenTextures
#define glGenVertexArrays                 imgl3wProcs.gl.GenVertexArrays
#define glGetAttribLocation               imgl3wProcs.gl.GetAttribLocation
#define glGetError                        imgl3wProcs.gl.GetError
#define glGetIntegerv                     imgl3wProcs.gl.GetIntegerv
#define glGetProgramInfoLog               imgl3wProcs.gl.GetProgramInfoLog
#define glGetProgramiv                    imgl3wProcs.gl.GetProgramiv
#define glGetShaderInfoLog                imgl3wProcs.gl.GetShaderInfoLog
#define glGetShaderiv                     imgl3wProcs.gl.GetShaderiv
#define glGetString                       imgl3wProcs.gl.GetString
#define glGetStringi                      imgl3wProcs.gl.GetStringi
#define glGetUniformLocation              imgl3wProcs.gl.GetUniformLocation
#define glGetVertexAttribPointerv         imgl3wProcs.gl.GetVertexAttribPointerv
#define glGetVertexAttribiv               imgl3wProcs.gl.GetVertexAttribiv
#define glIsEnabled                       imgl3wProcs.gl.IsEnabled
#define glLinkProgram                     imgl3wProcs.gl.LinkProgram
#define glPixelStorei                     imgl3wProcs.gl.PixelStorei
#define glPolygonMode                     imgl3wProcs.gl.PolygonMode
#define glReadPixels                      imgl3wProcs.gl.ReadPixels
#define glScissor                         imgl3wProcs.gl.Scissor
#define glShaderSource                    imgl3wProcs.gl.ShaderSource
#define glTexImage2D                      imgl3wProcs.gl.TexImage2D
#define glTexParameteri                   imgl3wProcs.gl.TexParameteri
#define glUniform1i                       imgl3wProcs.gl.Uniform1i
#define glUniformMatrix4fv                imgl3wProcs.gl.UniformMatrix4fv
#define glUseProgram                      imgl3wProcs.gl.UseProgram
#define glVertexAttribPointer             imgl3wProcs.gl.VertexAttribPointer
#define glViewport                        imgl3wProcs.gl.Viewport
#define glValidateProgram                             imgl3wProcs.gl.ValidateProgram
#define glBlendFunc                                   imgl3wProcs.gl.BlendFunc

#ifdef __cplusplus
}
#endif

#endif

#ifdef IMGL3W_IMPL
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

    static HMODULE libgl;
    typedef PROC(__stdcall* GL3WglGetProcAddr)(LPCSTR);
    static GL3WglGetProcAddr wgl_get_proc_address;

    static int open_libgl(void)
    {
        libgl = LoadLibraryA("opengl32.dll");
        if (!libgl)
            return GL3W_ERROR_LIBRARY_OPEN;
        wgl_get_proc_address = (GL3WglGetProcAddr)GetProcAddress(libgl, "wglGetProcAddress");
        return GL3W_OK;
    }

    static void close_libgl(void) { FreeLibrary(libgl); }
    static GL3WglProc get_proc(const char* proc)
    {
        GL3WglProc res;
        res = (GL3WglProc)wgl_get_proc_address(proc);
        if (!res)
            res = (GL3WglProc)GetProcAddress(libgl, proc);
        return res;
    }
#elif defined(__APPLE__)
#include <dlfcn.h>

    static void* libgl;
    static int open_libgl(void)
    {
        libgl = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY | RTLD_LOCAL);
        if (!libgl)
            return GL3W_ERROR_LIBRARY_OPEN;
        return GL3W_OK;
    }

    static void close_libgl(void) { dlclose(libgl); }

    static GL3WglProc get_proc(const char* proc)
    {
        GL3WglProc res;
        *(void**)(&res) = dlsym(libgl, proc);
        return res;
    }
#else
#include <dlfcn.h>

    static void* libgl;
    static GL3WglProc(*glx_get_proc_address)(const GLubyte*);

    static int open_libgl(void)
    {
        libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_LOCAL);
        if (!libgl)
            return GL3W_ERROR_LIBRARY_OPEN;
        *(void**)(&glx_get_proc_address) = dlsym(libgl, "glXGetProcAddressARB");
        return GL3W_OK;
    }

    static void close_libgl(void) { dlclose(libgl); }

    static GL3WglProc get_proc(const char* proc)
    {
        GL3WglProc res;
        res = glx_get_proc_address((const GLubyte*)proc);
        if (!res)
            *(void**)(&res) = dlsym(libgl, proc);
        return res;
    }
#endif

    static struct { int major, minor; } version;

    static int parse_version(void)
    {
        if (!glGetIntegerv)
            return GL3W_ERROR_INIT;
        glGetIntegerv(GL_MAJOR_VERSION, &version.major);
        glGetIntegerv(GL_MINOR_VERSION, &version.minor);
        if (version.major < 3)
            return GL3W_ERROR_OPENGL_VERSION;
        return GL3W_OK;
    }

    static void load_procs(GL3WGetProcAddressProc proc);

    int imgl3wInit(void)
    {
        int res = open_libgl();
        if (res)
            return res;
        atexit(close_libgl);
        return imgl3wInit2(get_proc);
    }

    int imgl3wInit2(GL3WGetProcAddressProc proc)
    {
        load_procs(proc);
        return parse_version();
    }

    int imgl3wIsSupported(int major, int minor)
    {
        if (major < 3)
            return 0;
        if (version.major == major)
            return version.minor >= minor;
        return version.major >= major;
    }

    GL3WglProc imgl3wGetProcAddress(const char* proc) { return get_proc(proc); }

    static const char* proc_names[] = {
        "glActiveTexture",
        "glAttachShader",
        "glBindBuffer",
        "glBindSampler",
        "glBindTexture",
        "glBindVertexArray",
        "glBlendEquation",
        "glBlendEquationSeparate",
        "glBlendFuncSeparate",
        "glBufferData",
        "glBufferSubData",
        "glClear",
        "glClearColor",
        "glCompileShader",
        "glCreateProgram",
        "glCreateShader",
        "glDeleteBuffers",
        "glDeleteProgram",
        "glDeleteShader",
        "glDeleteTextures",
        "glDeleteVertexArrays",
        "glDetachShader",
        "glDisable",
        "glDisableVertexAttribArray",
        "glDrawElements",
        "glDrawElementsBaseVertex",
        "glEnable",
        "glEnableVertexAttribArray",
        "glFlush",
        "glGenBuffers",
        "glGenTextures",
        "glGenVertexArrays",
        "glGetAttribLocation",
        "glGetError",
        "glGetIntegerv",
        "glGetProgramInfoLog",
        "glGetProgramiv",
        "glGetShaderInfoLog",
        "glGetShaderiv",
        "glGetString",
        "glGetStringi",
        "glGetUniformLocation",
        "glGetVertexAttribPointerv",
        "glGetVertexAttribiv",
        "glIsEnabled",
        "glLinkProgram",
        "glPixelStorei",
        "glPolygonMode",
        "glReadPixels",
        "glScissor",
        "glShaderSource",
        "glTexImage2D",
        "glTexParameteri",
        "glUniform1i",
        "glUniformMatrix4fv",
        "glUseProgram",
        "glVertexAttribPointer",
        "glViewport",
        "glValidateProgram",
        "glBlendFunc",
    };

    GL3W_API union GL3WProcs imgl3wProcs;

    static void load_procs(GL3WGetProcAddressProc proc)
    {
        size_t i;
        for (i = 0; i < ARRAY_SIZE(proc_names); i++)
            imgl3wProcs.ptr[i] = proc(proc_names[i]);
    }

#ifdef __cplusplus
}
#endif
#endif

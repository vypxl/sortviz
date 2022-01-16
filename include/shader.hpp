#ifndef __shader_hpp_
#define __shader_hpp_

#include <string>
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

class Shader {
private:
    GLuint vert;
    GLuint frag;
    GLuint prog;

    std::string readFile(const std::string &filePath);
    int compileShader(GLuint id, const std::string &src);
public:
    ~Shader();

    int loadFromFile(const std::string &vertexShaderFilename, const std::string &fragmenShaderFileName);
    void bind();
    void unbind();
    void setUniform(const std::string &name, float x);
    GLuint getNativeHandle();
};

#endif

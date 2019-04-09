#ifndef __ftfont_hpp_
#define __ftfont_hpp_

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include "shader.hpp"

class FtFont {
private:
    struct Character {
        GLuint texID;
        unsigned int w, h;
        int bearingX, bearingY;
        long int advance;
    };
    int size;

    Shader shader;
    GLuint attr_pos;

    GLuint vbo;

    std::map<GLchar, Character> characters;
public:
    FtFont(std::string fontFilename, int size, int window_width, int window_height);
    ~FtFont();

    void setWindow(int width, int height);
    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
};

#endif

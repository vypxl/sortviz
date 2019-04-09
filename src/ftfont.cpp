#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ftfont.hpp"

FtFont::FtFont(std::string fontFilename, int size, int window_width, int window_height) {
    this->size = size;
    // Load shader
    shader.loadFromFile("shaders/text.vert", "shaders/text.frag");
    attr_pos = glGetAttribLocation(shader.getNativeHandle(), "pos_uv");
    setWindow(window_width, window_height);

    // Load font via freetype
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "failed to initialize freetype library" << std::endl;
        exit(1);
    }

    FT_Face face;
    if (FT_New_Face(ft, fontFilename.c_str(), 0, &face)) {
        std::cout << "failed to initialize freetype face" << std::endl;
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_ALPHA, GL_UNSIGNED_BYTE, 
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character charr = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x
        };

        characters.insert(std::pair<GLchar, Character>(c, charr));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FtFont::~FtFont() { }

void FtFont::setWindow(int width, int height) {
    shader.bind();
    glm::mat4 projection = glm::ortho(0.f, static_cast<GLfloat>(width), 0.f, static_cast<GLfloat>(height));
    glUniformMatrix4fv(glGetUniformLocation(shader.getNativeHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void FtFont::renderText(std::string text, GLfloat _x, GLfloat _y, GLfloat scale) {
    shader.bind();
    glActiveTexture(GL_TEXTURE0);

    GLfloat x = _x;
    GLfloat y = _y;
    
    for (GLchar c : text) {
        Character ch = characters[c];

        GLfloat posX = x + ch.bearingX * scale;
        GLfloat posY = y - (ch.h - ch.bearingY) * scale;
        GLfloat width = ch.w * scale;
        GLfloat height = ch.h * scale;

        GLfloat vertices[6][4] = {
            { posX        , posY + height, 0.0, 0.0 },
            { posX        , posY         , 0.0, 1.0 },
            { posX + width, posY         , 1.0, 1.0 },

            { posX        , posY + height, 0.0, 0.0 },
            { posX + width, posY         , 1.0, 1.0 },
            { posX + width, posY + height, 1.0, 0.0 },
        };

        glBindTexture(GL_TEXTURE_2D, ch.texID);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glVertexAttribPointer(attr_pos, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(attr_pos);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(attr_pos);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        x += (ch.advance >> 6) * scale;

        if (c == '\n') {
            y -= size * scale;
            x = _x;
        }
    }
}

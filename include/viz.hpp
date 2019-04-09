#ifndef __viz_hpp_
#define __viz_hpp_

#include <glad/glad.h>
#include <SDL/SDL.h>

#include "shader.hpp"
#include "ftfont.hpp"
#include "sort.hpp"
#include "sorts.hpp"
#include "arraywrapper.hpp"

class Viz {
public:
    struct Stats{
        int frame;
    };

    enum Styles {
        Varietyloop = 0,
        VarietyDots = 1,

        /* Register new style here */
        /* DO NOT TOUCH */__count_
    };

private:
    struct Style{
        const char *vertexShader;
        const char *fragmentShader;
        const int gl_draw_method;
    };

    const Style styles[2] {
        Style{ "shaders/varietyloop.vert", "shaders/varietyloop.frag", GL_LINE_LOOP },
        Style{ "shaders/varietyloop.vert", "shaders/varietyloop.frag", GL_POINTS },

        /* Define new style here */
    };

    const std::string styleNames[2] {
        "Variety Loop",
        "Variety Points",

        /* Set new style display name here */
    };

    static const int dataSizesCount = 7;
    const int dataSizes[dataSizesCount] { 16, 256, 1000, 5000, 50000, 500000, 1000000 };

    ArrayWrapper *data = nullptr;
    bool running = false;
    int width = 1920;
    int height = 1080;
    
    int current_style = 0;
    int current_sort = 0;
    int current_data_size = 2;

    std::string infotext;
    FtFont *font;

    Shader shader;

    GLuint vaoId;
    GLuint dataBuffer;
    GLuint indexBuffer;

    int attr_x;
    int attr_i;

    Stats stats{0};
    Sort* sort = nullptr;

    void keyDown(SDL_keysym *keysym);
    void update();
    void draw();
    void drawData();
    void drawInfoText();

public:
    /// initialize the display
    int init();
    /// load a visual style from the list
    void changeStyle(Viz::Styles which);
    /// change current sort
    void setSort(Sorts::Sorts which);
    void setDataSize(int newsize);
    void loop();
};

#endif

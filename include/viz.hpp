#pragma once

#include <string>
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <glad/glad.h>
#include "SFML/Graphics.hpp"

#include "sort.hpp"
#include "arraywrapper.hpp"


class Viz {
public:
    struct Stats{
        int frame;
    };

    enum Styles {
        Colorloop = 0,
        None = 1
    };

private:
    struct Style{
        const char *vertexShader;
        const char *fragmentShader;
        const int gl_draw_method;
    };

    const Style styles[2] {
        Style{ "shaders/colorloop.vert", "shaders/colorloop.frag", GL_LINE_LOOP },
        Style{ "_.vert", "_.frag", GL_POINTS }
    };

    int data_size = 1000;
    ArrayWrapper *data = nullptr;
    bool running = false;
    int width = 1920;
    int height = 1080;
    int current_style;
    sf::Window window;
    sf::Shader shader;

    uint vaoId;
    uint dataBuffer;
    uint indexBuffer;

    int attr_x;
    int attr_i;

    Stats stats{0};
    Sort* sort = nullptr;

    void update();
    void draw();
    void drawData();

public:
    /// initialize the display
    int init();
    /// load a shader from the list
    void changeStyle(Viz::Styles which);
    /// change current sort
    template <typename T>
    void setSort() {
        static_assert(std::is_base_of<Sort, T>::value, "setSort must receive a Sort subclass!");

        // if (sort != nullptr) sort->stop();
        delete sort;
        this->sort = new T(data);
    };
    void setDataSize(int newsize);
    int getDataSize() { return data_size; }
    void loop();
};

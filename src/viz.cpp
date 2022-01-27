#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#include "viz.hpp"

void Viz::update() {
    std::ostringstream str;
    str 
        << "Sorting Algorithm: " << Sorts::names[current_sort]
        << " | Style: " << styleNames[current_style] 
        << std::endl 
        << data->stats 
        << std::endl 
        << sort->stats;

    infotext = str.str();
#ifdef __EMSCRIPTEN__
    sort->em_tick();
#endif
}

void Viz::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    font->renderText(infotext, 10, height - 26, 1.f);
    drawData();

    SDL_GL_SwapWindow(window);
}

void Viz::drawData() {
    shader.bind();
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->data(), GL_STREAM_DRAW);
    glVertexAttribPointer(attr_x, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glVertexAttribPointer(attr_i, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(attr_x);
    glEnableVertexAttribArray(attr_i);

    glDrawArrays(styles[current_style].gl_draw_method, 0, data->size());

    glDisableVertexAttribArray(attr_x);
    glDisableVertexAttribArray(attr_i);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Viz::init() {
#ifdef __EMSCRIPTEN__
    EmscriptenWebGLContextAttributes __attrs;
    emscripten_webgl_init_context_attributes(&__attrs);
    __attrs.majorVersion = 2; __attrs.minorVersion = 0;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE __ctx = emscripten_webgl_create_context("#canvas", &__attrs);
    emscripten_webgl_make_context_current(__ctx);
#endif
    // Initialize SDL
    int width = 1920;
    int height = 1080;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    window = SDL_CreateWindow("Sortviz",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    context = SDL_GL_CreateContext(window);
#ifndef __EMSCRIPTEN__
        // Initialize glew
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
#endif
    // Initialize OpenGL
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize infotext
    font = new FtFont("res/RobotoMono-Regular.ttf", 16, width, height);

    // Initialize VBOs
    glGenBuffers(1, &dataBuffer);
    glGenBuffers(1, &indexBuffer);
    
    // Initialize data, sort and style
    setDataSize(dataSizes[current_data_size]);
    changeStyle(Styles::Varietyloop);
    return 0;
}

void Viz::setDataSize(int data_size) {
    ArrayWrapper *old_data = data;

    // Initialize data and buffers
    data = new ArrayWrapper(data_size);
    
    // refresh index buffer
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->indices(), GL_STATIC_DRAW);

    shader.setUniform("size", float(data->size()));
    setSort(static_cast<Sorts::Sorts>(current_sort));

    delete old_data;
}

void Viz::changeStyle(Viz::Styles which) {
    current_style = which;
    if (shader.loadFromFile(styles[current_style].vertexShader, styles[current_style].fragmentShader) == GL_FALSE) {
        std::cerr << "Failed to load shaders, style id: " << which << std::endl;
        return;
    }
    shader.bind();
    attr_x = glGetAttribLocation(shader.getNativeHandle(), "x");
    attr_i = glGetAttribLocation(shader.getNativeHandle(), "i");

    shader.setUniform("size", float(data->size()));
}

void Viz::setSort(Sorts::Sorts which) {
    if (sort != nullptr) sort->stop();
    delete sort;
    sort = Sorts::create(which, data);
    sort->reset();
    sort->start();
}

#define MODEQ(var, val) var = var < 0 ? val - abs(var % val) : var % val

void Viz::keyDown(SDL_Keysym *keysym) {
    switch (keysym->sym) {
        case SDLK_SPACE:
            sort->stop();
            sort->data->shuffle();
            sort->reset();
            sort->data->stats.reset();
            sort->start();
            break;
        case SDLK_p:
            sort->toggle_pause();
            break;
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_UP:
            current_style++;
            MODEQ(current_style, Styles::__count_);
            changeStyle(static_cast<Styles>(current_style));
            break;
        case SDLK_DOWN:
            current_style--;
            MODEQ(current_style, Styles::__count_);
            changeStyle(static_cast<Styles>(current_style));
            break;
        case SDLK_LEFT:
            current_sort--;
            MODEQ(current_sort, Sorts::__count_);
            setSort(static_cast<Sorts::Sorts>(current_sort));
            break;
        case SDLK_RIGHT:
            current_sort++;
            MODEQ(current_sort, Sorts::__count_);
            setSort(static_cast<Sorts::Sorts>(current_sort));
            break;
        case SDLK_q:
            current_data_size--;
            MODEQ(current_data_size, dataSizesCount);
            setDataSize(dataSizes[current_data_size]);
            break;
        case SDLK_e:
            current_data_size++;
            MODEQ(current_data_size, dataSizesCount);
            setDataSize(dataSizes[current_data_size]);
            break;
        default:
            break;
    } 
}
#undef MODEQ

void Viz::loopf() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
            keyDown(&event.key.keysym);
            break;
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
              case SDL_WINDOWEVENT_RESIZED:
                width = event.window.data1;
                height = event.window.data2;

                glViewport(0, 0, width, height);
                font->setWindow(width, height);
                break;
            }
            break;
        }
    }

    update();
    draw();
}

void _loopf(void *self) {
    ((Viz*)self)->loopf();
}

void Viz::loop() {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(_loopf, this, 0, 1);
#else
    running = true;
    while (running) {
        loopf();
        SDL_Delay(10);
    }
#endif
    delete font;
    SDL_Quit();
}


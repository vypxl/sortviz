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

    SDL_GL_SwapBuffers();
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

void Viz::drawInfoText() {
    // infotext_shader.bind();
    
    
    /*
    SDL_Color clrFg = {255,255,255,0};
    SDL_Color clrBg = {0x1f,0x1f,0x1f,255};
    SDL_Surface *sText = SDL_DisplayFormatAlpha(TTF_RenderUTF8_Blended(font, infotext.c_str(), clrFg));
    GLint nColors = sText->format->BytesPerPixel;

    GLenum textureFormat;
	if(nColors == 4) {
		if(sText->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else textureFormat = GL_BGRA;
	} else {
		if(sText->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else textureFormat = GL_BGR;
	}

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // glTexImage2D(GL_TEXTURE_2D, 0, nColors, sText->w, sText->h, 0, textureFormat, GL_UNSIGNED_BYTE, sText->pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, nColors, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, nullptr);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sText->w, sText->h, textureFormat, GL_UNSIGNED_BYTE, sText->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float bColor[] = { 0.f, 0.f, 0.f, 0.f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);

    glEnable(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER, infotext_vbo);
    glVertexAttribPointer(infotext_shader_attr_pos, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(infotext_shader_attr_pos);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(infotext_shader_attr_pos);
    
    glDisable(GL_TEXTURE_2D);
    SDL_FreeSurface(sText);
    */
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
    const SDL_VideoInfo* info = nullptr;
    int width = 1920;
    int height = 1080;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    info = SDL_GetVideoInfo( );
    if(!info) {
        std::cerr << "Video query failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    if(SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_NOFRAME) == 0) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        return 1;
    }
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

void Viz::keyDown(SDL_keysym *keysym) {
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
        case SDL_VIDEORESIZE:
            width = event.resize.w;
            height = event.resize.h;
            
            if(SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_NOFRAME) == 0) {
                std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
                exit(1);
            }
            glViewport(0, 0, width, height);
            font->setWindow(width, height);
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


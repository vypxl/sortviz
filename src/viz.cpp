#include <iostream>
#include <algorithm>
#include <thread>

#include "viz.hpp"

void Viz::update() {
    std::cout << "frame " << (stats.frame++) << "\n\t" << sort->data->stats << std::endl;
}

void Viz::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    drawData();
    window.display();
}

void Viz::drawData() {

    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->data(), GL_STREAM_DRAW);
    glVertexAttribPointer(attr_x, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glVertexAttribPointer(attr_i, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(attr_x);
    glEnableVertexAttribArray(attr_i);

    glDrawArrays(styles[current_style].gl_draw_method, 0, data->size());

    glDisableVertexAttribArray(attr_x);
    glDisableVertexAttribArray(attr_i);
}

int Viz::init() {
    // Initialize window
    window.create(sf::VideoMode(width, height), "Sorting Vizualization", sf::Style::None);
    window.setFramerateLimit(100);
    window.setActive(true);

    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        desktop.width/2 - window.getSize().x/2 + 1920 / 2, 
        desktop.height/2 - window.getSize().y/2
    ));

    // Initialize glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Initialize VAOs
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    
    setDataSize(1000);
    return 0;
}

void Viz::setDataSize(int data_size) {
    delete data;
    // Initialize data and buffers
    data = new ArrayWrapper(data_size);
    // data buffer
    glGenBuffers(1, &dataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->data(), GL_STREAM_DRAW);

    // index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->indices(), GL_STATIC_DRAW);

    shader.setUniform("size", float(data->size()));
}

void Viz::changeStyle(Viz::Styles which) {
    current_style = which;
    if (!shader.loadFromFile(styles[current_style].vertexShader, styles[current_style].fragmentShader)) {
        std::cerr << "Failed to load shaders, style id: " << which << std::endl;
        return;
    }
    attr_x = glGetAttribLocation(shader.getNativeHandle(), "x");
    attr_i = glGetAttribLocation(shader.getNativeHandle(), "i");

    shader.setUniform("size", float(data->size()));
    sf::Shader::bind(&shader);
}

void Viz::loop() {
    running = true;
    while (running) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) running = false;
            if (ev.type == sf::Event::KeyPressed) {
                switch (ev.key.code) {
                    case sf::Keyboard::Q:
                        running = false;
                        break;
                    case sf::Keyboard::S:
                        sort->stop();
                        sort->data->shuffle();
                        sort->reset();
                        sort->data->stats.reset();
                        sort->start();
                        break;
                    case sf::Keyboard::Space:
                        sort->toggle_pause();
                        break;
                    default:
                        break;
                } 
            }
        }

        update();
        draw();

    }

    window.close();
}


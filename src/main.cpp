#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <glad/glad.h>
#include "SFML/Graphics.hpp"

const int SIZE = 10000;
const float RADIUS = 0.8;
const double M_TAU = M_PI * 2;

const int DELAY = 1;

class Sort {
private:
    std::thread t;
    bool terminate = false;

    void loop() {
        while (true) {
            if (terminate) return;
            step();
        }
    }

protected:
    void delay() {
        if (DELAY > 0) std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

    virtual void step() {};

public:
    float data[SIZE];
    float indices[SIZE];
    
    Sort() {
        for (int i = 0; i < SIZE; i++) {
            data[i] = (float)i;
            indices[i] = (float)i;
        }
    }

    void shuffle() {
        std::random_shuffle(std::begin(data), std::end(data));
    }
    
    void start() {
        t = std::thread(&Sort::loop, this);
    }
    void stop() {
        terminate = true;
        t.join();
    }
    
    virtual void reset() {};
};

class SelectionSort : public Sort {
protected:
    void step() {
        if (idx >= SIZE) {
            return;
        }
        int m = idx;
        
        for (int i = idx; i < SIZE; i++) {
            if (data[i] < data[m]) {
                m = i;
            }
        }

        float tmp = data[idx];
        data[idx] = data[m];
        data[m] = tmp;

        idx++;
        delay();
    }
public:
    int idx = 0;
    void reset() {
        idx = 0;
    }
};

class Viz {
private:
    bool running = false;
    int width = 1920;
    int height = 1080;
    int frame = 0;
    sf::Window window;
    sf::Shader shader;

    uint vaoId;
    uint dataBuffer;
    uint indexBuffer;

    int attr_x;
    int attr_i;

    Sort* sort;

    void update() {

    }

    void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.0f, 0.f);
        drawData();
        window.display();
        printf("frame %d\n", frame++);
    }

    void drawData() {

        glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sort->data), sort->data, GL_STREAM_DRAW);
        glVertexAttribPointer(attr_x, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
        glVertexAttribPointer(attr_i, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(attr_x);
        glEnableVertexAttribArray(attr_i);

        glDrawArrays(GL_LINE_LOOP, 0, SIZE);
        // glDrawArrays(GL_POINTS, 0, SIZE);

        glDisableVertexAttribArray(attr_x);
        glDisableVertexAttribArray(attr_i);


        // sf::CircleShape c(1);
        // c.setFillColor(sf::Color::White);
        // for (int i = 0; i < SIZE; i++) {
        //     double ang = (double)i / SIZE * M_TAU;
        //     const int dist = RADIUS - ((double)abs(i - sort->data[i]) / SIZE) * RADIUS;
        //     c.setPosition(dist * cos(ang), dist * sin(ang));

        //     int hue = (double)sort->data[i] / SIZE * 360;
        //     c.setFillColor(sf::Color(HSVtoRGB(hue, 1, 1)));
        //     window.draw(c);
        // }
    }

public:
    Viz(Sort* sort) {
        this->sort = sort;
    }

    int init() {
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

        // Initialize shaders
        if (!shader.loadFromFile("src/vertex_shader.vert", "src/fragment_shader.frag")) {
            std::cerr << "Failed to load shaders" << std::endl;
            return -1;
        }
        shader.setUniform("size", float(SIZE));
        shader.setUniform("radius", RADIUS);
        sf::Shader::bind(&shader);
        // sf::Glsl::Vec2 offset(window.getSize().x / 2, window.getSize().y / 2);
        // shader.setUniform("offset", &offset);

        // Initialize attribute positions
        attr_x = glGetAttribLocation(shader.getNativeHandle(), "x");
        attr_i = glGetAttribLocation(shader.getNativeHandle(), "i");

        // Initialize buffers
        // data buffer
        glGenBuffers(1, &dataBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sort->data), sort->data, GL_STREAM_DRAW);

        // index buffer
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sort->indices), sort->indices, GL_STATIC_DRAW);

        // Start sorting thread
        sort->start();

        return 0;
    }

    void loop() {
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
                        case sf::Keyboard::Space:
                            sort->shuffle();
                            sort->reset();
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
};

int main() {
    SelectionSort sort;
    Viz v(&sort);
    if (v.init() != 0) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    };
    v.loop();
    sort.stop();
    return 0;
}

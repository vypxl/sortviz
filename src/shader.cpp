#include <fstream>
#include <iostream>
#include <vector>

#include "shader.hpp"

std::string Shader::readFile(const std::string &filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

int Shader::compileShader(GLuint id, const std::string &src) {
    const char *csrc = src.c_str();
    glShaderSource(id, 1, &csrc, nullptr);
    glCompileShader(id);

    int result = 0;
    int logLength;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    logLength = logLength > 1 ? logLength : 1;
    std::vector<char> error(logLength);
    glGetShaderInfoLog(id, logLength, nullptr, &error[0]);

    if (result == GL_FALSE) {
        std::cout << "shader compilation result: " << result << std::endl << " error log: " << &error[0] << std::endl;
    }

    return result;
}

int Shader::loadFromFile(const std::string &vertexShaderFilename, const std::string &fragmenShaderFileName) {
    unbind();
    glDeleteProgram(prog);
    prog = 0;

    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertSrc = readFile(vertexShaderFilename);
    std::string fragSrc = readFile(fragmenShaderFileName);

    compileShader(vert, vertSrc);
    compileShader(frag, fragSrc);

    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    int result = 0;
    int logLength;
    glGetProgramiv(prog, GL_LINK_STATUS, &result);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);

    logLength = logLength > 1 ? logLength : 1;
    std::vector<char> error(logLength);
    glGetProgramInfoLog(prog, logLength, nullptr, &error[0]);

    if (result == GL_FALSE) {
        std::cout << "program linking result: " << result << std::endl << " error log: " << &error[0] << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return result;
}

void Shader::bind() {
    glUseProgram(prog);
}

void Shader::unbind() {
    int cur;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
    if (cur == (int)prog) 
        glUseProgram(0);
}

GLuint Shader::getNativeHandle() {
    return prog;
}

void Shader::setUniform(const std::string &name, float x) {
    int loc = glGetUniformLocation(prog, name.c_str());
    glUniform1f(loc, x);
}

Shader::~Shader() {
    unbind();
    glDeleteProgram(prog);
}

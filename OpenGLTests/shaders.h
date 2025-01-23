#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

const char* get_vertex_shader_source();
const char* get_vertex_shader_lit_source();
const char* get_fragment_shader_source();
const char* get_fragment_shader_lit_source();

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMatrix(const std::string& name, const float* value) const;
    void setVec3(const std::string& name, const float* value) const;
};
#include "shaders.h"

const char* get_vertex_shader_source() {
    return "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        //"layout (location = 1) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec4 vertexColor;\n"
        "out vec2 texCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 perspective;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = perspective * view * model * vec4(aPos, 1.0);\n"
        "   vertexColor = vec4((aPos.x + 0.5)/1.5, (aPos.y + 0.5)/1.5, (aPos.z + 0.5)/1.5, 1.0);\n"
        "   texCoord = aTexCoord;\n"
        "}\0";
}

const char* get_vertex_shader_lit_source() {
    return 
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"
            "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
}

const char* get_fragment_shader_source() {
    return "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "in vec4 vertexColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, texCoord) * vertexColor;\n"
        //"   FragColor = vertexColor;\n"
    "}\n\0";
}

const char* get_fragment_shader_lit_source() {
    return
        "#version 330 core\n"
        "out vec4 FragColor;\n"

        "uniform vec3 objectColor;\n"
        "uniform vec3 lightColor;\n"

        "void main()\n"
        "{\n"
        "FragColor = vec4(lightColor * objectColor, 1.0);\n"
        "}\0";
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string& name, const float* value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}
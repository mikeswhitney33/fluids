#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* v_path, const char* f_path) {
        std::string v_code, f_code;
        std::ifstream v_file, f_file;

        v_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        f_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            v_file.open(v_path);
            f_file.open(f_path);
            std::stringstream v_ss, f_ss;

            v_ss << v_file.rdbuf();
            f_ss << f_file.rdbuf();

            v_file.close();
            f_file.close();

            v_code = v_ss.str();
            f_code = f_ss.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char* vs_code = v_code.c_str();
        const char* fs_code = f_code.c_str();
        unsigned int vert, frag;

        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vs_code, NULL);
        glCompileShader(vert);
        checkCompileError(vert, "VERTEX");

        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fs_code, NULL);
        glCompileShader(frag);
        checkCompileError(frag, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vert);
        glAttachShader(ID, frag);
        glLinkProgram(ID);
        checkCompileError(ID, "PROGRAM");

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    void checkCompileError(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if(type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(shader, 1024, NULL< infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------------- --" << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------------- --" << std::endl;
            }
        }
    }
};


#endif

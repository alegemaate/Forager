#include "./gpu_program.h"

#include <asw/asw.h>
#include <fstream>

#include "../utils/utils.h"

void GpuProgram::initProgramFromFiles(const std::vector<std::string>& paths)
{
    std::vector<std::string> shaders;

    for (const auto& path : paths)
    {
#ifdef __EMSCRIPTEN__
        const std::string filename = "assets/shaders/es/" + path;
#else
        const std::string filename = "assets/shaders/core/" + path;
#endif

        const std::string content = readFromFile(filename);
        if (content.empty())
        {
            asw::log::info("Shader file '{}' not found.", filename);
            return;
        }
        shaders.push_back(content);
    }

    initProgram(shaders);
}

void GpuProgram::initProgram(const std::vector<std::string>& shaders)
{
    // GLSL program
    programId = glCreateProgram();

    for (const auto& content : shaders)
    {
        // Vertex shader
        GLuint shaderId = 0;
        if (shaderIds.empty())
        {
            shaderId = glCreateShader(GL_VERTEX_SHADER);
        }
        else
        {
            shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        }
        glShaderSource(shaderId, 1, (const char**)&content, nullptr);
        glCompileShader(shaderId);
        validateShader(shaderId);
        glAttachShader(programId, shaderId);
        shaderIds.push_back(shaderId);
    }

    glLinkProgram(programId);
    validateProgram(programId);
}

std::string GpuProgram::readFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        asw::util::abort_on_error("Cannot find file " + path + " \n Please check your files and try again");
    }

    std::string str;

    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return str;
}

void GpuProgram::validateShader(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        const unsigned int BUFFER_SIZE = 1024;
        GLchar             infoLog[BUFFER_SIZE];
        glGetShaderInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);

        asw::log::info("Shader {} compile log: \n{}", shader, infoLog);
    }
}

void GpuProgram::validateProgram(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        const unsigned int BUFFER_SIZE = 1024;
        GLchar             infoLog[BUFFER_SIZE];
        glGetProgramInfoLog(program, BUFFER_SIZE, nullptr, infoLog);

        asw::log::info("Program {} compile log: \n{}", program, infoLog);
    }
}

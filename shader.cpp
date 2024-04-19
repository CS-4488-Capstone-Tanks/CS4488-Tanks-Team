#include "shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(): program(0) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
}

Shader::Shader(unsigned int prog) : program(prog) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
}

Shader::Shader(Shader&& other) {
    program = other.program;
    other.program = 0;
    uniforms = std::move(other.uniforms);
}

Shader::~Shader() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

Shader& Shader::operator=(Shader&& other) {
    program = other.program;
    other.program = 0;
    uniforms = std::move(other.uniforms);
    return *this;
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::bindTexture(const char* name, int location, GLenum type, unsigned int texture) {
    if (hasUniform(name)) {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(type, texture);
        glUniform1i(uniforms.at(name), location);
    }
}

bool Shader::hasUniform(const char* name) const {
    return uniforms.find(name) != uniforms.end();
}

void Shader::compile(const char* vertex, const char* fragment) {
    // Create the two shader modules
    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);

    // Set their source to the passed in strings
    glShaderSource(vshader, 1, &vertex, nullptr);
    glShaderSource(fshader, 1, &fragment, nullptr);

    // Compile the shaders modules
    glCompileShader(vshader);
    glCompileShader(fshader);

    // Ensure they compiled correctly
    int success;
    const int logSize = 2048;
    char compilerOutput[logSize];

    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vshader, logSize, nullptr, compilerOutput);
        std::string message = "Vertex shader failed to compile:\n\n";
        message += compilerOutput;

        glDeleteShader(vshader);
        glDeleteShader(fshader);

        throw std::runtime_error(message);
    }

    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fshader, logSize, nullptr, compilerOutput);
        std::string message = "Fragment shader failed to compile:\n\n";
        message += compilerOutput;

        glDeleteShader(vshader);
        glDeleteShader(fshader);

        throw std::runtime_error(message);
    }

    // Now that we know they compiled correctly, link the modules into the final shader program
    program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);

    glLinkProgram(program);

    // Ensure it linked correctly
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, logSize, nullptr, compilerOutput);

        std::string message = "Shader program failed to link:\n\n";
        message += compilerOutput;

        glDeleteShader(vshader);
        glDeleteShader(fshader);
        glDeleteProgram(program);

        throw std::runtime_error(message);
    }

    // The shader modules are no longer required
    glDeleteShader(vshader);
    glDeleteShader(fshader);


    // Read out all the uniforms (variables we can set on the shader)

    GLint numUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLchar uniformName[256];
    for(GLint i = 0; i < numUniforms; i++) {
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, sizeof(uniformName), &length, & size, &type, uniformName);
        GLint location = glGetUniformLocation(program, uniformName);

        uniforms[uniformName] = location;
    }
}

Shader Shader::fromSource(const char* vertex, const char* fragment) {
    Shader returnval;
    returnval.compile(vertex, fragment);
    return returnval;
}

Shader Shader::fromFolder(const std::filesystem::path& path) {
    if (
        !std::filesystem::is_directory(path) ||
        !std::filesystem::exists(path / "vertex.glsl") ||
        !std::filesystem::exists(path / "vertex.glsl")
    ) {
        throw std::runtime_error("Shader must be a directory containing vertex.glsl and fragment.glsl");
    }

    std::ifstream vertstream(path / "vertex.glsl");
    std::ifstream fragstream(path / "fragment.glsl");

    std::stringstream ss;

    ss << vertstream.rdbuf();

    std::string vertSrc = ss.str();

    ss.clear();

    ss << fragstream.rdbuf();

    std::string fragSrc = ss.str();

    vertstream.close();
    fragstream.close();

    return fromSource(vertSrc.c_str(), fragSrc.c_str());
}

unsigned int Shader::location(const char* name) const {
    if (hasUniform(name)) {
        return uniforms.at(name);
    }
    else {
        std::string msg = "No such uniform on shader: ";
        msg += name;
        throw std::logic_error(msg);
    }
}

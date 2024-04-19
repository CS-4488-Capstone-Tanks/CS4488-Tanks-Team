#ifndef TANKS_SHADER_H
#define TANKS_SHADER_H


#include <QOpenGLExtraFunctions>

#include <unordered_map>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Shaders used to live inside the Renderer, but grew too complicated and became their own class.
 * They use just a touch of template metaprogramming, which I'll heavily document, since it's an extremely
 * complicated part of C++.
 *
 * Also, for correctness reasons, Shaders are movable, but not copyable. So this is invalid code:
 *
 * Shader s1(0);
 * Shader s2 = s1;  // err, illegal, can't copy the shader to a new location
 *
 * But they can be moved, like this
 *
 * Shader s1(0);
 * Shader s2 = std::move(s1);
 *
 * But be warned, this will invalidate s1, so don't use it after moving it
 *
 * @author Tyson Cox
 */
class Shader : private QOpenGLExtraFunctions {
    unsigned int program;
    std::unordered_map<std::string, unsigned int> uniforms;

    void compile(const char* vertex, const char* fragment);
public:
    Shader();
    Shader(unsigned int prog);
    Shader(Shader&& other);

    static Shader fromSource(const char* vertex, const char* fragment);
    static Shader fromFolder(const std::filesystem::path& path);

    ~Shader();

    Shader& operator=(Shader&& other);

    /** Set this as the active shader, and the next draw call will use this shader */
    void use();

    /**
     * Check if the shader has a uniform variable
     * @param name the name of the variable
     * @return whether the uniform exists
     */
    bool hasUniform(const char* name) const;

    /**
     * Get the location of a uniform variable
     * @param name The name of the uniform
     * @return the numeric location of the uniform
     * @throws std::logic_error if no such uniform is present
     */
    unsigned int location(const char* name) const;

    /**
     * Bind a texture, so that the shader will use that texture on the next draw call
     * @param name The name of the sampler uniform to bind to
     * @param location The location of the texture (i.e tex 0, tex 1 - only used in multi-texturing)
     * @param type The type of the texture (probably GL_TEXTURE_2D most of the time)
     * @param texture The index of the texture
     */
    void bindTexture(const char* name, int location, GLenum type, unsigned int texture);


    /* So, this is a template. They're a special type of function/class/method you can make in C++, declared
     * with the "template" keyword. They also have to have their body defined in the header, you (mostly) can't do
     * it in the .cpp file
     *
     * The idea is that, normally, you'd say something like
     *
     *  void myfunc(int x);
     *
     * but what if we also want to take a float too? We could make a second function, but what if we want to
     * take any type of parameter. That's where templates come in. They essentially tell the compiler
     * "Please automatically generate for me, a function for each type that I call this function with"
     *
     *  template<typename ParamType>   // we're saying there's some type called ParamType the compiler will figure out
     *  void myFunc(ParamType input);  // and now we're using it like a regular type
     *
     * That function call be called with ints, floats, unsigned char, std::string, whatever.
     * */

    /**
     * Set the value of a uniform
     * @tparam T The type of the value passed in - should be automatically deduced
     * @param name The name of the uniform to set
     * @param value The value to set the uniform to
     */
    template<typename T>
    void setUniform(const char* name, T&& value) {

        /* This is a piece of template metaprogramming. We want to check what type value is, so we
         * can call the right GL function for it. The problem is that in C++, these are technically
         * different types:
         *
         * int
         * int&
         * const int&
         *
         * And so we wanna get rid of all that nonsense, and get at just the core type of int,
         * which is what this does
         */
        using realtype = std::decay_t<T>;

        /* Lastly, we use constexpr if - which is evaluated at compile time. So this basically says
         *
         * "if it's a float, compile this function so that all it does is call glUniform1f"
         *
         * the other branches of the constexpr if don't even end up in the final compiled function
         * */
        if constexpr (std::is_same_v<realtype, float>) { glUniform1f(uniforms.at(name), value); }
        else if constexpr (std::is_same_v<realtype, int>) { glUniform1i(uniforms.at(name), value); }
        else if constexpr (std::is_same_v<realtype, unsigned int>) { glUniform1ui(uniforms.at(name), value); }
        else if constexpr (std::is_same_v<realtype, glm::vec2>) { glUniform2fv(uniforms.at(name), 1, glm::value_ptr(value)); }
        else if constexpr (std::is_same_v<realtype, glm::vec3>) { glUniform3fv(uniforms.at(name), 1, glm::value_ptr(value)); }
        else if constexpr (std::is_same_v<realtype, glm::vec4>) { glUniform4fv(uniforms.at(name), 1, glm::value_ptr(value)); }
        else if constexpr (std::is_same_v<realtype, glm::mat2>) { glUniformMatrix2fv(uniforms.at(name), 1, GL_FALSE, glm::value_ptr(value)); }
        else if constexpr (std::is_same_v<realtype, glm::mat3>) { glUniformMatrix3fv(uniforms.at(name), 1, GL_FALSE, glm::value_ptr(value)); }
        else if constexpr (std::is_same_v<realtype, glm::mat4>) { glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, glm::value_ptr(value)); }
        else {
            throw std::logic_error("Invalid shader uniform type");
        }
    }

    template<typename T>
    void setUniformIf(const char* name, T&& value) {
        if (hasUniform(name)) {
            setUniform(name, std::forward<T>(value));

            // The forward call here ^ just makes it call the next template with the same type of T that was
            // passed in. So if you called it with a const int&, it will call the next template with a
            // const int&
        }
    }

};

#endif //TANKS_SHADER_H

#ifndef TANKS_RENDERER_H
#define TANKS_RENDERER_H

#include <QOpenGLExtraFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <glm/glm.hpp>

#include <vector>
#include <filesystem>

// Forward declared so that this header doesn't need to include the game object header
class GameObject;

/**
 * @brief The renderer is a QWidget responsible for drawing 3D graphics to the window
 * @author Tyson Cox
 *
 * It inherits from the QOpenGLWidget to create a drawing context, and it also inherits
 * from the QOpenGLExtraFunctions to gain access to the GL drawing functions required
 */
class Renderer : public QOpenGLWidget, public QOpenGLExtraFunctions {
public:
    enum class CameraMode {
        Static,
        Chasing,
        Periscope,
        Orbiting
    };
protected:
    enum class DrawCommandType {
        Player,
        Enemy,
        Obstacle,
        Bullet
    };

    struct DrawCommand {
        DrawCommandType type;
        glm::mat4 transform;
        glm::vec3 forwardPoint;
    };

    struct Mesh {
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
        int vertexCount;
        int indexCount;
    };

    struct Shader {
        unsigned int program;
        std::unordered_map<std::string, unsigned int> uniforms;

        bool hasUniform(const char* name) const;
    };

    std::unordered_map<std::string, Mesh> meshes;
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, unsigned int> textures;

    // The list of draw commands for the last complete frame, and the currently being built frame
    // They are separate to ensure it never draws a half frame
    std::vector<DrawCommand> lastFrame;
    std::vector<DrawCommand> curFrame;

    CameraMode camMode;
    float cameraTime;

    glm::mat4 view;
    glm::mat4 projection;

    unsigned int textureFromFile(const std::filesystem::path& path);

    Mesh meshFromFile(const std::filesystem::path& path);
    void meshDestroy(Mesh& mesh);

    Shader shaderFromSource(const char* vertex, const char* fragment);
    void shaderDestroy(Shader& shader);

    bool textureExists(const char* name);
    bool textureExists(const std::string& name);

    void advanceCamera();

    void drawMesh(const Mesh& mesh, const glm::mat4& mvp, unsigned int texture = 0, float* color = nullptr);

    // The following are configuration parameters that can be easily tweaked
    static const float constexpr cameraFOVDegrees = 45.0f;

    static const float constexpr backgroundRed = 0.0f;
    static const float constexpr backgroundGreen = 0.0f;
    static const float constexpr backgroundBlue = 0.0f;

    static const float constexpr cameraRadius = 20.0f;
    static const float constexpr cameraSpeed = 0.01f;

    static const glm::vec3 constexpr cameraTopLookPos = glm::vec3(0, 0, 0);
    static const glm::vec3 constexpr cameraTopPosition = glm::vec3(0, 17, -25);

    static const float constexpr cameraChaseDistance = 25.0f;

    static const float constexpr periscopeHeight = 0.25f;

    static const float constexpr groundScale = 50.0f;
    static const float constexpr groundHeight = -0.5f;
public:

    ~Renderer() override;

    /**
     * Draw an object to the screen
     * @param object the object to draw
     */
    void drawObject(const GameObject* object);

    /** Called when all objects for the frame have been drawn */
    void doneWithFrame();

    void setCameraMode(CameraMode mode);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};


#endif //TANKS_RENDERER_H

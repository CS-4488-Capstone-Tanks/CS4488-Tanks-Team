#ifndef TANKS_RENDERER_H
#define TANKS_RENDERER_H

#include <QOpenGLExtraFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <glm/glm.hpp>

#include <vector>

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
        int vertexCount;
    };

    std::unordered_map<std::string, Mesh> meshes;

    // The list of draw commands for the last complete frame, and the currently being built frame
    // They are separate to ensure it never draws a half frame
    std::vector<DrawCommand> lastFrame;
    std::vector<DrawCommand> curFrame;

    unsigned int playerTexture;
    unsigned int enemyTexture;

    unsigned int texturedShader;

    bool usingPeriscope;
    static const glm::vec3 constexpr cameraTopPosition = glm::vec3(0, 10, -5);

    glm::mat4 view;
    glm::mat4 projection;

    Mesh meshFromFile(const char* path);
    void meshDestroy(Mesh& mesh);
    unsigned int shaderFromSource(const char* vertex, const char* fragment);


    // The following are configuration parameters that can be easily tweaked
    static const float constexpr cameraFOVDegrees = 45.0f;

    static const float constexpr backgroundRed = 0.0f;
    static const float constexpr backgroundGreen = 0.0f;
    static const float constexpr backgroundBlue = 0.0f;

public:
    ~Renderer() override;

    /**
     * Draw an object to the screen
     * @param object the object to draw
     */
    void drawObject(GameObject* object);

    /** Called when all objects for the frame have been drawn */
    void doneWithFrame();

    /** Toggle the renderer to display the scene in first person */
    void togglePeriscope();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};


#endif //TANKS_RENDERER_H

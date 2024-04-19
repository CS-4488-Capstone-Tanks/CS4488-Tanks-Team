#ifndef TANKS_RENDERER_H
#define TANKS_RENDERER_H

#include <QOpenGLExtraFunctions>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <glm/glm.hpp>

#include <vector>
#include <filesystem>

// Forward declared so that this header doesn't need to include the game object header
class GameObject;

#include "Obstacle.h"
#include "shader.h"

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
        Orbiting,
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
        ObstacleType obstacleType;
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


    std::unordered_map<std::string, Mesh> meshes;
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, unsigned int> textures;
    std::unordered_map<std::string, unsigned int> cubemaps;

    // The list of draw commands for the last complete frame, and the currently being built frame
    // They are separate to ensure it never draws a half frame
    std::vector<DrawCommand> lastFrame;
    std::vector<DrawCommand> curFrame;

    CameraMode camMode;
    float cameraTime;
    glm::vec3 freeCamPos;
    glm::vec3 freeCamAngle;

    glm::mat4 view;
    glm::mat4 projection;

    /**
     * @brief Load a texture from a file, and create a GL texture for it
     * @param path The path to the file
     * @return The GL id of the texture
     */
    unsigned int textureFromFile(const std::filesystem::path& path);

    /**
     * @brief Loads a texture from a file, but stores it as a cubemap
     * @param path The path to the file
     * @return The GL id of the texture
     */
    unsigned int cubemapFromFile(const std::filesystem::path& path);

    /**
     * @brief Load a mesh from a file, and create a Mesh struct for it
     * @param path The path to the file
     * @return A mesh struct containing the mesh's GL buffers/vertex array
     * @throws std::runtime_error if the mesh fails to load
     */
    Mesh meshFromFile(const std::filesystem::path& path);

    /**
     * @brief Destroys a mesh, cleaning up any GPU buffers/vertex arrays
     * @param mesh the mesh to destroy
     */
    void meshDestroy(Mesh& mesh);

    /**
     * @brief a utility function to check if a texture is known to the renderer
     * @param name The texture to look for
     * @return whether the texture exists
     */
    bool textureExists(const char* name);

    /**
     * @brief a utility function to check if a texture is known to the renderer
     * @param name The texture to look for
     * @return whether the texture exists
     */
    bool textureExists(const std::string& name);

    /**
     * @brief Used to compute the camera position when in orbiting camera mode
     */
    void advanceCamera();

    /**
     * Executes a draw command to draw a player tank
     * @param cmd the command to execute
     */
    void drawPlayerTank(const DrawCommand& cmd);

    /**
     * Executes a draw command to draw an enemy tank
     * @param cmd the command to execute
     */
    void drawEnemyTank(const DrawCommand& cmd);

    /**
     * Executes a draw command to draw a projectile
     * @param cmd the command to execute
     */
    void drawProjectile(const DrawCommand& cmd);

    /**
     * Executes a draw command to draw an obstacle
     * @param cmd the command to execute
     */
    void drawObstacle(const DrawCommand& cmd);

    /**  Draws the ground plane */
    void drawGround();

    /** Draws the skybox */
    void drawSkybox();

    /**
     * Handles setting any parameters that any dynamic cameras need for the current frame
     */
    void frameSetCamera();

    /**
     * @brief Handles binding the shader/mesh and executing the draw call
     * @param mesh The mesh to display
     * @param mvp The model-view-projection matrix for the the draw
     * @param texture The texture, if available, or 0 (the GL id for no texture) if not
     * @param color an array of three floats for RGB, of nullptr if not available
     */
    void drawMesh(const Mesh& mesh, const glm::mat4& mvp, unsigned int texture = 0, float* color = nullptr);

    /**
     * @brief Handles any special case adjustments that some draw calls may require (such as nudging some meshes)
     * @param cmd The command to adjust
     */
    void specialCaseAdjusment(DrawCommand& cmd);

    // The following are configuration parameters that can be easily tweaked

    // The field of view of the camera, in degrees
    static const float constexpr cameraFOVDegrees = 45.0f;

    // The color of the window's background (visible when nothing is drawn)
    static const float constexpr backgroundRed = 0.0f;
    static const float constexpr backgroundGreen = 0.0f;
    static const float constexpr backgroundBlue = 0.0f;

    // The radius the orbiting camera orbits, and its speed
    static const float constexpr cameraRadius = 20.0f;
    static const float constexpr cameraSpeed = 0.01f;

    // The position the static camera looks at, and where it is
    static const glm::vec3 constexpr cameraTopLookPos = glm::vec3(0, 0, 0);
    static const glm::vec3 constexpr cameraTopPosition = glm::vec3(0, 17, -25);

    // The distance backward the chasing camera follows
    static const float constexpr cameraChaseDistance = 25.0f;

    // How high off the ground the first person (periscope) view is set
    static const float constexpr periscopeHeight = 0.25f;

    // How big, and far up/down the ground is set
    static const float constexpr groundScale = 25.0f;
    static const float constexpr groundHeight = -0.5f;

    // How large and dense the grass is
    static const int constexpr grassShells = 16;
    static const float constexpr grassShellHeightStep = 0.04f;
    static const float constexpr grassShellDensityStep = 0.15f;
    static const float constexpr grassScale = 400.0f;

    // How large the skybox mesh is
    static const float constexpr skyboxSize = 200.0f;

    // Where the scene's light source is, and how bright unlit surfaces are (the ambient light intensity)
    static const glm::vec3 constexpr lightPos = glm::vec3(10, 5, 7);
    static const float constexpr ambientLightIntensity = 0.2;
public:

    ~Renderer() override;

    /**
     * Draw an object to the screen
     * @param object the object to draw
     */
    void drawObject(const GameObject* object);

    /** Called when all objects for the frame have been drawn */
    void doneWithFrame();

    /**
     * Set the renderer's camera mode. Note that cameras currently are not deltatime synced,
     * and so are frame-rate dependent.
     * @param mode The Renderer::CameraMode to use
     */
    void setCameraMode(CameraMode mode);


    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};


#endif //TANKS_RENDERER_H

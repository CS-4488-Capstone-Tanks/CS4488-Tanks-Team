#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>


static const char* texturedVertexSource = R"(
#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoord;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(pos, 1.0f);
    texCoord = tex;
})";

static const char* texturedFragmentSource = R"(
#version 450

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D imageTexture;

void main() {
    fragColor = texture(imageTexture, texCoord);
})";



Renderer::Mesh Renderer::meshFromFile(const char* path) {
    Mesh m{};

    // TODO: once assimp is linked, load the model from the file and handle
    // processing the vertex attributes/uploading the data

    // Create the vertex array and buffer
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);

    // Bind them, and upload the data to the GPU buffer
    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);


    return m;
}

void Renderer::meshDestroy(Renderer::Mesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}

unsigned int Renderer::shaderFromSource(const char* vertex, const char* fragment) {
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
    unsigned int program = glCreateProgram();
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

    return program;
}

void Renderer::doneWithFrame() {
    lastFrame = curFrame;
    curFrame.clear();
}

void Renderer::togglePeriscope() {
    usingPeriscope = !usingPeriscope;

    if (!usingPeriscope) {
        // TODO: handle setting the camera to its top down view, once we know where the coordinates for that are
    }
}


void Renderer::drawObject(GameObject* object) {
    DrawCommand cmd{};

    /* TODO: uncomment this once objects are implemented
    switch(object->getType()) {
        case Player:
            cmd.type = DrawCommandType::Player;
            break;
        case Enemy:
            cmd.type = DrawCommandType::Enemy;
            break;
        case Bullet:
            cmd.type = DrawCommandType::Bullet;
            break;
        case Obstacle:
            cmd.type = DrawCommandType::Obstacle;
            break;
    }

    // Compute the basis vectors of the object's rotation using the cross product
    // of its forward direction, and the world's up vector
    glm::vec3 pos = object->getPosition();
    glm::vec3 objectForward = object->getDirection();
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    glm::vec3 objectRight = glm::normalize(glm::cross(worldUp, objectForward));

    cmd.transform[0] = glm::vec4(objectRight, 0.0f);
    cmd.transform[1] = glm::vec4(worldUp, 0.0f);
    cmd.transform[2] = glm::vec4(objectForward, 0.0f);
    cmd.transform[3] = glm::vec4(pos, 1.0f);
    */

    curFrame.emplace_back(cmd);
}

void Renderer::paintGL() {
    QOpenGLWidget::paintGL();

    // Clear both the color buffer and depth buffer, preparing to draw an entirely fresh frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // If the periscope is enabled, make a pass through the draw commands to ensure
    // we find where the player is and set the camera to be there for first person view
    if (usingPeriscope) {
       for(auto& cmd : lastFrame) {
           if (cmd.type == DrawCommandType::Player) {
               glm::vec3 playerPos = glm::vec3(cmd.transform[3][0], cmd.transform[3][1], cmd.transform[3][2]);
               glm::vec3 up = glm::vec3(0, 1, 0);

               view = glm::lookAt(cmd.forwardPoint, playerPos, up);

               break;
           }
       }
    }

    // Compute half the mvp, the last bit computed per object
    glm::mat4 vp = projection * view;
    glm::mat4 mvp;

    // Use the appropriate shader for drawing
    glUseProgram(texturedShader);

    for(auto& cmd : lastFrame) {
        mvp = vp * cmd.transform;

        switch(cmd.type) {
            // TODO: I can't handle drawing meshes until I have more info about how meshes are loaded
            case DrawCommandType::Player:
                break;
            case DrawCommandType::Enemy:
                break;
            case DrawCommandType::Obstacle:
                break;
            case DrawCommandType::Bullet:
                break;
        }
    }
}

void Renderer::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);

    // If the window size changes, we have to recreate the projection
    projection = glm::perspective(
        glm::radians(cameraFOVDegrees),
        (float)w / float(h),
        0.1f,
        100.0f
    );
}

void Renderer::initializeGL() {
    QOpenGLWidget::initializeGL();
    QOpenGLExtraFunctions::initializeOpenGLFunctions();

    // Set the background color of the window when nothing is on it
    glClearColor(backgroundRed, backgroundBlue, backgroundGreen, 1.0f);

    // This produces the perspective projection matrix for doing a standard 3D scene
    // The arguments to this are:
    // 1. The field of view of the camera, in radians
    // 2. The ratio of the window's width to height
    // 3. The "near plane", or basically if something gets closer than this, it's invisible
    // 4. The "far plane", or if something gets farther than this, it's invisible
    projection = glm::perspective(
        glm::radians(cameraFOVDegrees),
        (float)this->width() / (float)this->height(),
        0.1f,
        100.0f
    );

    // Leave the view as the identity matrix for now
    view = glm::mat4(1.0f);

    usingPeriscope = false;

    try {
        texturedShader = shaderFromSource(texturedVertexSource, texturedFragmentSource);
    }
    catch (std::exception& ex) {
        std::string msg = "The following error occurred while initializing the renderer:\n\n";
        msg += ex.what();

        throw std::runtime_error(msg);
    }
}

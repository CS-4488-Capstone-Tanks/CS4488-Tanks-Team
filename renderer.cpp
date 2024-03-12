#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <filesystem>


//#include "gameobject.h"

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



Renderer::Mesh Renderer::meshFromFile(const std::filesystem::path& path) {
    Mesh m{};

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string msg = "Failed to load mesh: ";
        msg += path;
        throw std::runtime_error(msg);
    }

    if (scene->mNumMeshes > 1) {
        std::cout << "WARN: Model file " << path << " has more than 1 mesh, only the first will be used\n";
    }

    // Create the vertex array and buffer
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);

    // Bind them, and upload the data to the GPU buffer
    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);

    const aiMesh* mesh = scene->mMeshes[0];

    // OpenGL now needs all that data in one big buffer. Easier to interleave it now,
    // then deal with the math of storing them separately

    std::vector<float> interleavedBuffer;
    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->HasPositions()) {
            interleavedBuffer.push_back(mesh->mVertices[i].x);
            interleavedBuffer.push_back(mesh->mVertices[i].y);
            interleavedBuffer.push_back(mesh->mVertices[i].z);
        }
        if (mesh->HasTextureCoords(0)) { // Assuming the first set of texture coordinates
            interleavedBuffer.push_back(mesh->mTextureCoords[0][i].x);
            interleavedBuffer.push_back(mesh->mTextureCoords[0][i].y);
        }
        if (mesh->HasNormals()) {
            interleavedBuffer.push_back(mesh->mNormals[i].x);
            interleavedBuffer.push_back(mesh->mNormals[i].y);
            interleavedBuffer.push_back(mesh->mNormals[i].z);
        }
    }

    // Upload our data to the GPU
    // Store it in the array buffer
    // Pass how big our data is, and the pointer to it
    // Lastly, it won't change often, so we use static draw
    glBufferData(GL_ARRAY_BUFFER, interleavedBuffer.size() * sizeof(float), interleavedBuffer.data(), GL_STATIC_DRAW);

    size_t bufferOffset = 0;

    if (mesh->HasPositions()) {
        // Tell the vertex array that we want to:
        // Enable attribute 0
        // Which has three components (a vec3)
        // of type float
        // that is not normalized
        // has total data size per element of 3 * sizeof(float)
        // and starts at a certain offset in the buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)bufferOffset);
        glEnableVertexAttribArray(0);
        bufferOffset += 3 * sizeof(float);
    }

    if (mesh->HasTextureCoords(0)) {
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)bufferOffset);
        glEnableVertexAttribArray(1);
        bufferOffset += 2 * sizeof(float);
    }

    if (mesh->HasNormals()) {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)bufferOffset);
        glEnableVertexAttribArray(2);
    }

    // Unbind our VAO so we don't accidentally modify it later
    glBindVertexArray(0);

    m.vertexCount = mesh->mNumVertices;

    return m;
}

void Renderer::meshDestroy(Renderer::Mesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);

    mesh = {};
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
        view = glm::lookAt(cameraTopPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }
}


void Renderer::drawObject(GameObject* object) {
    DrawCommand cmd{};

    /* TODO: uncomment this once objects are implemented
    switch(object->getType()) {
        case GameObjectType::PlayerTank:
            cmd.type = DrawCommandType::Player;
            break;
        case GameObjectType::EnemyTank:
            cmd.type = DrawCommandType::Enemy;
            break;
        case GameObjectType::Projectile:
            cmd.type = DrawCommandType::Bullet;
            break;
        case GameObjectType::Obstacle:
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

    cmd.forwardPoint = pos + glm::normalize(objectForward);
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

    // Get the numeric identifier of our uniform, for uploading data to
    int mvpLocation = glGetUniformLocation(texturedShader, "mvp");

    for(auto& cmd : lastFrame) {
        mvp = vp * cmd.transform;

        Mesh m{};

        switch(cmd.type) {
            case DrawCommandType::Player:
                m = meshes["tank"];            // find the mesh we care about
                break;
            case DrawCommandType::Enemy:
                m = meshes["tank"];
                break;
            case DrawCommandType::Obstacle:
                m = meshes["obstacle"];
                break;
            case DrawCommandType::Bullet:
                m = meshes["bullet"];
                break;
        }

        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));  // set our mvp matrix for this draw
        glDrawArrays(GL_TRIANGLES, 0, m.vertexCount);                       // execute a draw call
    }
}

void Renderer::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);

    // If the window size changes, we have to recreate the projection. See initializeGL for details
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

    view = glm::lookAt(cameraTopPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    usingPeriscope = false;

    try {
        texturedShader = shaderFromSource(texturedVertexSource, texturedFragmentSource);

        // Ensure the data exists as an empty mesh, so at worst, if the meshes aren't on disk, we just
        // don't draw them instead of crashing or something
        meshes["tank"] = {};
        meshes["obstacle"] = {};
        meshes["bullet"] = {};

        if (std::filesystem::exists("assets/models")) {
            for(const auto& entry : std::filesystem::directory_iterator("assets/models")) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    Mesh loaded = meshFromFile(path);

                    meshes[path.stem()] = loaded;
                }
            }
        }
        else {
            throw std::runtime_error("No assets/models directory is available. Can't draw any 3D meshes");
        }
    }
    catch (std::exception& ex) {
        std::string msg = "The following error occurred while initializing the renderer:\n\n";
        msg += ex.what();

        throw std::runtime_error(msg);
    }
}

Renderer::~Renderer() {
    // Clean up after yourself
    for(auto& kvpair : meshes) {
        meshDestroy(kvpair.second);
    }

    meshes.clear();
}

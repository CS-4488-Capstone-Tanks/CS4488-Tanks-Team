#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QImage>

#include <iostream>
#include <filesystem>



#include "gameobject.h"

// These are the magic names that the renderer looks for when loading assets

static const std::string TANK_MESH_FILE = "tank";
static const std::string OBSTACLE_MESH_FILE = "obstacle";
static const std::string BULLET_MESH_FILE = "bullet";

static const std::string PLAYER_TEXTURE_FILE = "player";
static const std::string ENEMY_TEXTURE_FILE = "enemy";
static const std::string OBSTACLE_TEXTURE_FILE = "obstacle";





static const char* texturedVertexSource = R"(
#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 texCoord;
out vec3 normal;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(pos, 1.0f);
    texCoord = tex;
    normal = norm;
})";

static const char* coloredFragmentSource = R"(
#version 450

in vec2 texCoord;

out vec4 fragColor;

uniform vec3 color;

void main() {
    fragColor = vec4(color, 1.0f);
})";

static const char* texturedFragmentSource = R"(
#version 450

in vec2 texCoord;
in vec3 normal;

out vec4 fragColor;

uniform sampler2D albedo;

vec3 lightPos = vec3(10, 5, 7);
float ambient = 0.2;

void main() {
    float surfaceAlignment = clamp(dot(normalize(normal), normalize(lightPos)), 0.0f, 1.0f);
    vec4 color = texture(albedo, texCoord) * (surfaceAlignment + ambient);
    color += vec4(ambient, ambient, ambient, 0.0f);

    fragColor = vec4(
        color[0],
        color[1],
        color[2],
        1.0f
    );
})";



Renderer::Mesh Renderer::meshFromFile(const std::filesystem::path& path) {
    Mesh m{};

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string msg = "Failed to load mesh: ";
        msg += path.string();
        throw std::runtime_error(msg);
    }

    if (scene->mNumMeshes > 1) {
        std::cout << "WARN: Model file " << path << " has more than 1 mesh, only the first will be used\n";
    }

    // Create the vertex array and buffer
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    // Bind them, and upload the data to the GPU buffer
    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);

    const aiMesh* mesh = scene->mMeshes[0];

    // OpenGL now needs all that data in one big buffer. Easier to interleave it now,
    // then deal with the math of storing them separately

    std::vector<float> interleavedBuffer;
    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        interleavedBuffer.push_back(mesh->mVertices[i].x);
        interleavedBuffer.push_back(mesh->mVertices[i].y);
        interleavedBuffer.push_back(mesh->mVertices[i].z);

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

    std::vector<unsigned int> indices;
    for(size_t i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];

        for(size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }


    // Upload our data to the GPU
    // Store it in the array buffer
    // Pass how big our data is, and the pointer to it
    // Lastly, it won't change often, so we use static draw
    glBufferData(GL_ARRAY_BUFFER, interleavedBuffer.size() * sizeof(float), interleavedBuffer.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Stride is the total size of a vertex in bytes
    GLsizei stride = 3 * sizeof(float);

    if (mesh->HasTextureCoords(0)) { stride += 2 * sizeof(float); }
    if (mesh->HasNormals()) { stride += 3 * sizeof(float); }


    size_t bufferOffset = 0;

    // Tell the vertex array that we want to:
    // Enable attribute 0
    // Which has three components (a vec3)
    // of type float
    // that is not normalized
    // has total data size per vertex of stride
    // and starts at a certain offset in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)bufferOffset);
    glEnableVertexAttribArray(0);
    bufferOffset += 3 * sizeof(float);

    if (mesh->HasTextureCoords(0)) {
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)bufferOffset);
        glEnableVertexAttribArray(1);
        bufferOffset += 2 * sizeof(float);
    }

    if (mesh->HasNormals()) {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)bufferOffset);
        glEnableVertexAttribArray(2);
    }

    // Unbind our VAO so we don't accidentally modify it later
    glBindVertexArray(0);

    m.vertexCount = mesh->mNumVertices;
    m.indexCount = indices.size();

    return m;
}

void Renderer::meshDestroy(Renderer::Mesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);

    mesh = {};
}

Renderer::Shader Renderer::shaderFromSource(const char* vertex, const char* fragment) {
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

    Shader shader {
        .program = program
    };

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

        shader.uniforms[uniformName] = location;
    }

    return shader;
}

void Renderer::shaderDestroy(Shader& shader) {
    glDeleteProgram(shader.program);
}

void Renderer::doneWithFrame() {
    lastFrame = curFrame;
    curFrame.clear();
    update();
}

void Renderer::drawObject(const GameObject* object) {
    DrawCommand cmd{};

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

    if (glm::length(objectForward) < 0.001f) {
        objectForward = glm::vec3(0, 0, -1);
    }
    else {
        objectForward = glm::normalize(objectForward);
    }

    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    glm::vec3 objectRight = glm::normalize(glm::cross(worldUp, objectForward));

    cmd.transform[0] = glm::vec4(objectRight, 0.0f);
    cmd.transform[1] = glm::vec4(worldUp, 0.0f);
    cmd.transform[2] = glm::vec4(objectForward, 0.0f);
    cmd.transform[3] = glm::vec4(pos, 1.0f);

    cmd.forwardPoint = pos + glm::normalize(objectForward);

    curFrame.emplace_back(cmd);
}

void Renderer::paintGL() {
    QOpenGLWidget::paintGL();

    // Clear both the color buffer and depth buffer, preparing to draw an entirely fresh frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // If the periscope is enabled, make a pass through the draw commands to ensure
    // we find where the player is and set the camera to be there for first person view

    switch (camMode) {
        case CameraMode::Static: break;
        case CameraMode::Periscope:
            for(auto& cmd : lastFrame) {
                if (cmd.type == DrawCommandType::Player) {
                    glm::vec3 playerPos = glm::vec3(cmd.transform[3][0], cmd.transform[3][1], cmd.transform[3][2]);
                    glm::vec3 up = glm::vec3(0, 1, 0);

                    view = glm::lookAt(cmd.forwardPoint, playerPos, up);

                    break;
                }
            }
            break;
        case CameraMode::Chasing:
            break;
        case CameraMode::Orbiting:
           advanceCamera();
           break;
    }

    // Compute half the mvp, the last bit computed per object
    glm::mat4 vp = projection * view;
    glm::mat4 mvp;


    for(auto& cmd : lastFrame) {
        mvp = vp * cmd.transform;

        Mesh m{};

        bool hasTexture = false;
        unsigned int texture = 0;
        float color[] = { 1.0f, 1.0f, 1.0f };

        switch(cmd.type) {
            case DrawCommandType::Player:
                if (camMode == CameraMode::Periscope) {
                    continue;
                }

                m = meshes[TANK_MESH_FILE];            // find the mesh we care about

                color[0] = 0.0f; // green
                color[1] = 1.0f,
                color[2] = 0.0f;

                if ((hasTexture = textureExists(PLAYER_TEXTURE_FILE))) {
                    texture = textures[PLAYER_TEXTURE_FILE];
                }

                break;
            case DrawCommandType::Enemy:
                m = meshes[TANK_MESH_FILE];

                color[0] = 1.0f; // red
                color[1] = 0.0f,
                color[2] = 0.0f;

                if ((hasTexture = textureExists(ENEMY_TEXTURE_FILE))) {
                    texture = textures[ENEMY_TEXTURE_FILE];
                }

                break;
            case DrawCommandType::Obstacle:
                m = meshes[OBSTACLE_MESH_FILE];

                color[0] = 0.58; // brown
                color[1] = 0.29f,
                color[2] = 0.0f;

                if ((hasTexture = textureExists(OBSTACLE_TEXTURE_FILE))) {
                    texture = textures[OBSTACLE_TEXTURE_FILE];
                }

                break;
            case DrawCommandType::Bullet:
                m = meshes[BULLET_MESH_FILE];
                color[0] = 1.0f; // yellow
                color[1] = 1.0f;
                color[2] = 0.0f;

                break;
        }

        Shader shader = hasTexture ? shaders["textured"] : shaders["colored"];

        // Use the appropriate shader for drawing
        glUseProgram(shader.program);

        if (!shader.hasUniform("mvp")) {
            std::cerr << "Invalid shader, has no mvp uniform\n";
        }

        if (shader.hasUniform("color")) {
            glUniform3fv(shader.uniforms["color"], 1, color);
        }

        if (shader.hasUniform("albedo")) {
            if (!hasTexture) {
                std::cerr << "Shader requested a texture, but not found\n";
                continue;
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(shader.uniforms["albedo"], 0);
        }

        glBindVertexArray(m.vao);

        glUniformMatrix4fv(shader.uniforms["mvp"], 1, GL_FALSE, glm::value_ptr(mvp));  // set our mvp matrix for this draw
        glDrawElements(GL_TRIANGLES, m.indexCount, GL_UNSIGNED_INT, 0);
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

    glEnable(GL_DEPTH_TEST);

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

    //view = glm::lookAt(cameraTopPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    setCameraMode(CameraMode::Static);

    try {
        shaders["textured"] = shaderFromSource(texturedVertexSource, texturedFragmentSource);
        shaders["colored"] = shaderFromSource(texturedVertexSource, coloredFragmentSource);

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

                    meshes[path.stem().string()] = loaded;
                }
            }
        }
        else {
            throw std::runtime_error("No assets/models directory is available. Can't draw any 3D meshes");
        }

        if (std::filesystem::exists("assets/textures")) {
            for(const auto& entry : std::filesystem::directory_iterator("assets/textures")) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    unsigned int loaded = textureFromFile(path);

                    textures[path.stem().string()] = loaded;
                }
            }
        }
        else {
            std::cerr << "Renderer: No textures available, using coloring instead\n";
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

    for(auto& kvpair : shaders) {
        shaderDestroy(kvpair.second);
    }

    shaders.clear();

    for(auto& kvpair : textures) {
        glDeleteTextures(1, &kvpair.second);
    }

    textures.clear();
}

bool Renderer::textureExists(const char* name) {
    return textures.find(name) != textures.end();
}

bool Renderer::textureExists(const std::string& name) {
    return textures.find(name) != textures.end();
}

unsigned int Renderer::textureFromFile(const std::filesystem::path& path) {
    // Use QT to load the image
    QImage image(QString::fromStdString(path.string()));
    if (image.isNull()) {
        std::cerr << "Failed to load image: " << path << "\n";
        return 0;
    }

    // OpenGL requires specific data formatting
    QImage glImage = image.mirrored().convertToFormat(QImage::Format_RGBA8888);

    // Generate a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters, like how the texture should be resized and what to do if it doesn't quite fit right
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void Renderer::advanceCamera() {
    glm::vec3 cameraPos = cameraTopPosition;
    cameraPos[0] = cos(cameraTime) * cameraRadius;
    cameraPos[2] = sin(cameraTime) * cameraRadius;
    cameraTime += cameraSpeed;

    view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Renderer::setCameraMode(Renderer::CameraMode mode) {
    camMode = mode;

    if (camMode == CameraMode::Static) {
        view = glm::lookAt(cameraTopPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }
}

bool Renderer::Shader::hasUniform(const char* name) const {
    return uniforms.find(name) != uniforms.end();
}

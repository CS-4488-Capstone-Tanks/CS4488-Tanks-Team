#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include <iostream>
#include <filesystem>
#include <algorithm>



#include "gameobject.h"

// These are the magic names that the renderer looks for when loading assets

static const char* TANK_MESH_FILE = "tank";
static const char* BULLET_MESH_FILE = "bullet";
static const char* GROUND_MESH_FILE = "ground";
static const char* SKY_MESH_FILE = "skybox";

static const char* PLAYER_TEXTURE_FILE = "player";
static const char* ENEMY_TEXTURE_FILE = "enemy";
static const char* GROUND_TEXTURE_FILE = "ground";

static const char* SKY_CUBEMAP_FOLDER = "bluecloud";


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


static const char* skyboxVertexSource = R"(
#version 450

layout (location = 0) in vec3 pos;
out vec3 texcoord;

uniform mat4 vp;

void main() {
    texcoord = pos;
    vec4 p = vp * vec4(pos, 1.0f);
    gl_Position = p.xyww;
})";

static const char* skyboxFragmentSource = R"(
#version 450

in vec3 texcoord;
out vec4 fragColor;

uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, texcoord);
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
    if (!object) {
        std::cerr << "Renderer: received null object\n";
        return;
    }

    DrawCommand cmd{};

    switch(object->getType()) {
        case GameObjectType::PlayerTank:
            cmd.type = DrawCommandType::Player;
            break;
        case GameObjectType::EnemyTank:
            cmd.type = DrawCommandType::Enemy;
            break;
        case GameObjectType::PlayerProjectile:
            cmd.type = DrawCommandType::Bullet;
            break;
        case GameObjectType::EnemyProjectile:
            cmd.type = DrawCommandType::Bullet;
            break;
        case GameObjectType::Obstacle:
            cmd.type = DrawCommandType::Obstacle;
            break;
        case GameObjectType::None:
            std::cerr << "Renderer: Can't draw none-type object\n";
            return;
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

    if (cmd.type == DrawCommandType::Obstacle) {
        cmd.obstacleType = dynamic_cast<const Obstacle*>(object)->getObstacleType();
    }

    specialCaseAdjusment(cmd);

    curFrame.emplace_back(cmd);
}

void Renderer::paintGL() {
    QOpenGLWidget::paintGL();

    // Clear both the color buffer and depth buffer, preparing to draw an entirely fresh frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handle setting any camera properties needed for this frame
    frameSetCamera();

    // Always draw the ground
    drawGround();

    // Loop through the commands, and draw each object at its appropriate location/type/etc
    for(auto& cmd : lastFrame) {

        switch(cmd.type) {
            case DrawCommandType::Player:
                drawPlayerTank(cmd);
                break;
            case DrawCommandType::Enemy:
                drawEnemyTank(cmd);
                break;
            case DrawCommandType::Obstacle:
                drawObstacle(cmd);
                break;
            case DrawCommandType::Bullet:
                drawProjectile(cmd);
                break;
        }
    }

    // Always draw the skybox
    drawSkybox();
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

    // Enable depth buffering/testing (so that objects behind others aren't drawn over top of them)
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling, so that the GPU only draws the faces the camera can see
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

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

    setCameraMode(CameraMode::Static);

    try {
        shaders["textured"] = shaderFromSource(texturedVertexSource, texturedFragmentSource);
        shaders["colored"] = shaderFromSource(texturedVertexSource, coloredFragmentSource);
        shaders["skybox"] = shaderFromSource(skyboxVertexSource, skyboxFragmentSource);

        // Ensure the data exists as an empty mesh, so at worst, if the meshes aren't on disk, we just
        // don't draw them instead of crashing or something
        meshes[TANK_MESH_FILE] = {};
        meshes[BULLET_MESH_FILE] = {};
        meshes[SKY_MESH_FILE] = {};

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

        if (std::filesystem::exists("assets/cubemaps")) {
            for(const auto& entry : std::filesystem::directory_iterator("assets/cubemaps")) {
                if (entry.is_directory()) {
                    const auto& path = entry.path();
                    unsigned int loaded = cubemapFromFile(path);

                    cubemaps[path.stem().string()] = loaded;
                }
            }
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

    for(auto& kvpair : cubemaps) {
        glDeleteTextures(1, &kvpair.second);
    }

    cubemaps.clear();
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
        std::cerr << "Renderer: Failed to load image: " << path << "\n";
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


/**
 * This function is used as part of the sorting process for the faces of a cubemap when loading one
 * @param path the path of the image
 * @return the desired position in the list
 */
int cubemapFaceOrder(const std::filesystem::path& path) {
    const static std::unordered_map<std::string, int> order = {
        {"right", 0},
        {"left", 1},
        {"top", 2},
        {"bottom", 3},
        {"front", 4},
        {"back", 5}
    };

    std::string name = path.stem().string();

    if (order.find(name) == order.end()) {
        std::string msg = "Unknown face for cubemap: " + path.string();
        throw std::runtime_error(msg.c_str());
    }

    return order.at(name);
}

/**
 * This is the actual sorting function used as a key to std::sort for the faces of the cubemapw
 * @param a the first face
 * @param b the second face
 * @return whether the first is less than the second
 */
bool cubmapFaceCompare(const std::filesystem::path& a, const std::filesystem::path& b) {
    return cubemapFaceOrder(a) < cubemapFaceOrder(b);
}

unsigned int Renderer::cubemapFromFile(const std::filesystem::path& path) {
    if (!is_directory(path)) {
        throw std::runtime_error("Cubemaps must be a directory containing exactly six images and one optional meta.json file");
    }

    QFile* metafile = nullptr;
    QByteArray metabytes;
    QJsonDocument meta;
    std::vector<std::filesystem::path> images;

    for(const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            auto imgpath = entry.path();

            if (imgpath.extension() == ".json") {
                metafile = new QFile(imgpath);

                if (!metafile->open(QIODevice::ReadOnly | QIODevice::Text)) {
                    delete metafile;
                    std::cerr << "Renderer: failed to open cubemap json metafile: " << imgpath << "\n";
                    continue;
                }

                metabytes = metafile->readAll();
                meta = QJsonDocument::fromJson(metabytes);
            }
            else {
                images.push_back(entry.path());
            }
        }
    }

    if (images.size() != 6) {
        throw std::runtime_error("Cubemaps must be a directory containing exactly six images and one optional meta.json file");
    }

    std::sort(images.begin(), images.end(), cubmapFaceCompare);

    // Generate a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for(size_t i = 0; i < images.size(); i++) {
        const auto& imgpath = images[i];

        // Use QT to load the image
        QImage image(QString::fromStdString(imgpath.string()));
        if (image.isNull()) {
            std::cerr << "Renderer: Failed to load cubemap image " << i << ": " << imgpath << "\n";
            glDeleteTextures(1, &texture);
            return 0;
        }


        bool flipHorizontal = true;
        bool flipVertical = false;

        if (meta.isObject()) {
            auto mainobj = meta.object();

            if (mainobj.contains(imgpath.stem().c_str())) {
                auto faceobjvalue = mainobj.value(imgpath.stem().c_str());

                if (faceobjvalue.isObject()) {
                    auto faceobj = faceobjvalue.toObject();

                    auto fh = faceobj.value("flipHorizontal");
                    auto fv = faceobj.value("flipVertical");

                    if (fh.isBool()) { flipHorizontal = fh.toBool(); }
                    if (fv.isBool()) { flipVertical = fv.toBool(); }
                }
            }
        }

        // OpenGL requires specific data formatting
        QImage glImage = image.mirrored(flipHorizontal, flipVertical).convertToFormat(QImage::Format_RGBA8888);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    delete metafile;
    return texture;
}

void Renderer::advanceCamera() {
    glm::vec3 cameraPos = cameraTopPosition;
    cameraPos[0] = cos(cameraTime) * cameraRadius;
    cameraPos[2] = sin(cameraTime) * cameraRadius;
    cameraTime += cameraSpeed;

    view = glm::lookAt(cameraPos, cameraTopLookPos, glm::vec3(0, 1, 0));
}

void Renderer::setCameraMode(Renderer::CameraMode mode) {
    camMode = mode;

    if (camMode == CameraMode::Static) {
        view = glm::lookAt(cameraTopPosition, cameraTopLookPos, glm::vec3(0, 1, 0));
    }
}

void Renderer::drawMesh(const Renderer::Mesh& mesh, const glm::mat4& mvp, unsigned int texture, float* passedColor) {
    float color[] = { 1.0f, 1.0f, 1.0f };

    if (passedColor != nullptr) {
        memcpy(color, passedColor, 3 * sizeof(float));
    }

    bool hasTexture = texture != 0;

    Shader shader = hasTexture ? shaders["textured"] : shaders["colored"];

    // Use the appropriate shader for drawing
    glUseProgram(shader.program);

    if (!shader.hasUniform("mvp")) {
        std::cerr << "Renderer: Invalid shader, has no mvp uniform\n";
    }

    if (shader.hasUniform("color")) {
        glUniform3fv(shader.uniforms["color"], 1, color);
    }

    if (shader.hasUniform("albedo")) {
        if (!hasTexture) {
            std::cerr << "Renderer: Shader requested a texture, but not found\n";
            return;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(shader.uniforms["albedo"], 0);
    }

    glBindVertexArray(mesh.vao);

    glUniformMatrix4fv(shader.uniforms["mvp"], 1, GL_FALSE, glm::value_ptr(mvp));  // set our mvp matrix for this draw
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

void Renderer::specialCaseAdjusment(Renderer::DrawCommand& cmd) {
    if (cmd.type != DrawCommandType::Obstacle) {
        return;
    }

    // Trees need scooted down to be flat with the ground
    // if at some point, some enterprising individual fixes the alignment in the mesh itself,
    // this is no longer required
    if (cmd.obstacleType == ObstacleType::Tree) {
        cmd.transform = glm::translate(cmd.transform, glm::vec3(0.0f, -0.5f, 0.0f));
    }
}

void Renderer::drawPlayerTank(const Renderer::DrawCommand& cmd) {
    // Skip drawing the player in periscope mode, so we're not looking at its
    // insides. Alternatively, can enable backface culling
    if (camMode == CameraMode::Periscope) {
        return;
    }

    Mesh m = meshes[TANK_MESH_FILE];

    float color[] = {0.0f, 1.0f, 0.0f}; // green
    unsigned int texture = 0;
    glm::mat4 mvp = projection * view * cmd.transform;

    if (textureExists(PLAYER_TEXTURE_FILE)) {
        texture = textures[PLAYER_TEXTURE_FILE];
    }

    drawMesh(m, mvp, texture, color);
}

void Renderer::drawEnemyTank(const Renderer::DrawCommand& cmd) {
    Mesh m = meshes[TANK_MESH_FILE];

    float color[] = {1.0f, 0.0f, 0.0f}; // red
    unsigned int texture = 0;
    glm::mat4 mvp = projection * view * cmd.transform;

    if (textureExists(ENEMY_TEXTURE_FILE)) {
        texture = textures[ENEMY_TEXTURE_FILE];
    }

    drawMesh(m, mvp, texture, color);
}

void Renderer::drawProjectile(const Renderer::DrawCommand& cmd) {
    Mesh m = meshes[BULLET_MESH_FILE];

    float color[] = {1.0f, 1.0f, 0.0f}; // yellow
    glm::mat4 mvp = projection * view * cmd.transform;


    drawMesh(m, mvp, 0, color);
}

void Renderer::drawObstacle(const Renderer::DrawCommand& cmd) {
    Mesh m{};
    std::string obstacleTypeName = Obstacle::convertObstacleTypeToName(cmd.obstacleType);

    float color[] {0.58, 0.29f, 0.0f}; // brown
    glm::mat4 mvp = projection * view * cmd.transform;
    unsigned int texture = 0;

    if (obstacleTypeName.empty() || meshes.find(obstacleTypeName) == meshes.end()) {
        std::string errorName = obstacleTypeName.empty() ? "(empty)" : obstacleTypeName;
        std::cerr << "Renderer: No obstacle by type " << errorName << "\n";
        return;
    }
    else {
        m = meshes[obstacleTypeName];

        if (textureExists(obstacleTypeName)) {
            texture = textures[obstacleTypeName];
        }
    }

    drawMesh(m, mvp, texture, color);
}

void Renderer::drawGround() {
    if (meshes.find(GROUND_MESH_FILE) != meshes.end()) {
        const auto& mesh = meshes[GROUND_MESH_FILE];

        mat4 groundTransform = mat4(1.0f);
        groundTransform = glm::scale(groundTransform, glm::vec3(groundScale, 0.0f, groundScale));

        groundTransform[3][1] = groundHeight;

        glm::mat4 mvp = projection * view * groundTransform;

        float groundColor[] = {0.25, 0.25, 0.25};

        int groundTex = textures.find(GROUND_TEXTURE_FILE) == textures.end() ? 0 : textures[GROUND_TEXTURE_FILE];

        drawMesh(mesh, mvp, groundTex, groundColor);
    }
}

void Renderer::frameSetCamera() {
    switch (camMode) {
        case CameraMode::Static: break; // No changes needed for a static camera
        case CameraMode::Periscope:     // In periscope mode, set our camera inside the player tank
            for(auto& cmd : lastFrame) {
                if (cmd.type == DrawCommandType::Player) {
                    glm::vec3 camPos = glm::vec3(cmd.transform[3][0], cmd.transform[3][1] + periscopeHeight, cmd.transform[3][2]);
                    glm::vec3 up = glm::vec3(0, 1, 0);

                    glm::vec3 lookPoint = cmd.forwardPoint + glm::vec3(0, periscopeHeight, 0);

                    view = glm::lookAt(camPos, lookPoint, up);

                    break;
                }
            }
            break;
        case CameraMode::Chasing:   // For chasing mode, set our camera above the player tank
            for(auto& cmd : lastFrame) {
                if (cmd.type == DrawCommandType::Player) {
                    glm::vec3 camPos = glm::vec3(cmd.transform[3][0], cmd.transform[3][1], cmd.transform[3][2]);
                    glm::vec3 up = glm::vec3(0, 1, 0);

                    glm::vec3 lookPoint = camPos;

                    glm::vec3 forwardDir = cmd.forwardPoint - camPos;

                    camPos += glm::vec3(0.0f, cameraTopPosition[1], 0.0f);
                    camPos -= forwardDir * cameraChaseDistance;

                    view = glm::lookAt(camPos, lookPoint, up);

                    break;
                }
            }
            break;
        case CameraMode::Orbiting:  // For orbiting mode, calculate the next camera position
            advanceCamera();
            break;
    }
}

void Renderer::drawSkybox() {
    if (shaders.find("skybox") == shaders.end()) { return; }
    if (cubemaps.find(SKY_CUBEMAP_FOLDER) == cubemaps.end()) { return; }
    if (meshes.find(SKY_MESH_FILE) == meshes.end()) { return; }

    // Disable writing to the depth buffer, so it draws behind everything always
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    const auto& shader = shaders.at("skybox");
    unsigned int skybox = cubemaps.at(SKY_CUBEMAP_FOLDER);
    const auto& mesh = meshes.at(SKY_MESH_FILE);

    if (!shader.hasUniform("vp")) {
        std::cerr << "Renderer: Invalid skybox shader, has no vp uniform\n";
    }

    if (!shader.hasUniform("skybox")) {
        std::cerr << "Renderer: Invalid skybox shader, has no skybox samplerCube uniform\n";
    }

    glm::mat4 vp = projection * view;
    vp = glm::scale(vp, glm::vec3(skyboxSize, skyboxSize, skyboxSize));

    glUseProgram(shader.program);
    glBindVertexArray(mesh.vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

    glUniformMatrix4fv(shader.uniforms.at("vp"), 1, GL_FALSE, glm::value_ptr(vp));  // set our vp matrix for this draw
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}


bool Renderer::Shader::hasUniform(const char* name) const {
    return uniforms.find(name) != uniforms.end();
}

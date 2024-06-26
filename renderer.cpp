#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>

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
uniform mat4 mv;

void main() {
    gl_Position = mvp * vec4(pos, 1.0f);
    texCoord = tex;
    normal = mat3(mv) * norm;
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
uniform vec3 lightPos;
uniform float ambient;

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

static const char* groundVertexSource = R"(
#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 texCoord;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    texCoord = tex;
    worldPos = vec3(model * vec4(pos, 1.0));
}
)";

// The noise functions from this shader taken from https://www.shadertoy.com/view/fsf3DN
static const char* groundFragmentSource = R"(
#version 450

in vec2 texCoord;
in vec3 worldPos;

uniform float grassDensity;
uniform float grassScale;
uniform vec3 color;
//uniform sampler2D albedo;

out vec4 fragColor;

float random(vec2 uv){
    return fract(sin(dot(uv, vec2(12.9898,78.233))) * 43758.5453);
}

float noise(in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main() {
    float noiseValue = noise(texCoord * grassScale);

    //fragColor = vec4(noiseValue, noiseValue, noiseValue, 1.0f);

    if (noiseValue < grassDensity) {
        discard;
    }

    //fragColor = texture(albedo, texCoord);
    fragColor = vec4(color * grassDensity + 0.1, 1.0);
})";





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
        shaders["textured"] = Shader::fromSource(texturedVertexSource, texturedFragmentSource);
        shaders["colored"] = Shader::fromSource(texturedVertexSource, coloredFragmentSource);
        shaders["skybox"] = Shader::fromSource(skyboxVertexSource, skyboxFragmentSource);
        shaders["ground"] = Shader::fromSource(groundVertexSource, groundFragmentSource);

        // Ensure the data exists as an empty mesh, so at worst, if the meshes aren't on disk, we just
        // don't draw them instead of crashing or something
        meshes[TANK_MESH_FILE] = {};
        meshes[BULLET_MESH_FILE] = {};
        meshes[SKY_MESH_FILE] = {};

        if (std::filesystem::exists("assets/models")) {
            for(const auto& entry : std::filesystem::directory_iterator("assets/models")) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path();
                    meshes[path.stem().string()] = Mesh(path);
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
                    textures[path.stem().string()] = Texture::fromFile(path);
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
                    textures[path.stem().string()] = Texture::cubemapFromFolder(path);
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
    // These classes' destructors handle the cleanup
    meshes.clear();
    shaders.clear();
    textures.clear();
}

bool Renderer::textureExists(const char* name) {
    return textures.find(name) != textures.end();
}

bool Renderer::textureExists(const std::string& name) {
    return textures.find(name) != textures.end();
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

void Renderer::drawMesh(Mesh& mesh, const glm::mat4& meshTransform, Texture* texture, float* passedColor) {
    glm::vec3 color(1.0f, 1.0f, 1.0f);

    if (passedColor != nullptr) {
        color[0] = passedColor[0];
        color[1] = passedColor[1];
        color[2] = passedColor[2];
    }

    bool hasTexture = texture != nullptr;

    Shader& shader = hasTexture ? shaders.at("textured") : shaders.at("colored");

    shader.use();

    glm::mat4 mvp = projection * view * meshTransform;
    glm::mat4 mv = view * meshTransform;

    shader.setUniformIf("mvp", mvp);
    shader.setUniformIf("mv", mv);
    shader.setUniformIf("color", color);
    shader.setUniformIf("lightPos", lightPos);
    shader.setUniformIf("ambient", ambientLightIntensity);
    shader.bindTexture("albedo", 0, texture);

    mesh.draw();
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

    Mesh& m = meshes[TANK_MESH_FILE];

    float color[] = {0.0f, 1.0f, 0.0f}; // green
    Texture* texture = nullptr;

    if (textureExists(PLAYER_TEXTURE_FILE)) {
        texture = &textures[PLAYER_TEXTURE_FILE];
    }

    drawMesh(m, cmd.transform, texture, color);
}

void Renderer::drawEnemyTank(const Renderer::DrawCommand& cmd) {
    Mesh& m = meshes[TANK_MESH_FILE];

    float color[] = {1.0f, 0.0f, 0.0f}; // red
    Texture* texture = nullptr;

    if (textureExists(ENEMY_TEXTURE_FILE)) {
        texture = &textures[ENEMY_TEXTURE_FILE];
    }

    drawMesh(m, cmd.transform, texture, color);
}

void Renderer::drawProjectile(const Renderer::DrawCommand& cmd) {
    Mesh& m = meshes[BULLET_MESH_FILE];

    float color[] = {1.0f, 1.0f, 0.0f}; // yellow

    drawMesh(m, cmd.transform, 0, color);
}

void Renderer::drawObstacle(const Renderer::DrawCommand& cmd) {
    std::string obstacleTypeName = Obstacle::convertObstacleTypeToName(cmd.obstacleType);

    float color[] {0.58, 0.29f, 0.0f}; // brown
    Texture* texture = nullptr;

    if (obstacleTypeName.empty() || meshes.find(obstacleTypeName) == meshes.end()) {
        std::string errorName = obstacleTypeName.empty() ? "(empty)" : obstacleTypeName;
        std::cerr << "Renderer: No obstacle by type " << errorName << "\n";
        return;
    }
    else {
        if (textureExists(obstacleTypeName)) {
            texture = &textures[obstacleTypeName];
        }

        drawMesh(meshes[obstacleTypeName], cmd.transform, texture, color);
    }

}

void Renderer::drawGround() {
    if (meshes.find(GROUND_MESH_FILE) != meshes.end()) {
        auto& mesh = meshes[GROUND_MESH_FILE];

        mat4 groundTransform = mat4(1.0f);
        groundTransform = glm::scale(groundTransform, glm::vec3(groundScale, 1.0f, groundScale));
        groundTransform = glm::translate(groundTransform, glm::vec3(0, groundHeight, 0));

        Texture* groundTex = textureExists(GROUND_TEXTURE_FILE) ? nullptr : &textures[GROUND_TEXTURE_FILE];
        glm::vec3 groundColor(0.0f, 0.75f, 0.0f);

        auto& shader = shaders.at("ground");
        shader.use();

        float grassDensitySum = 0.0f;
        float grassHeightSum = 0.0f;
        for(size_t i = 0; i < grassShells; i++) {

            glm::mat4 model = glm::translate(groundTransform, glm::vec3(0, grassHeightSum, 0));
            glm::mat4 mvp = projection * view * model;

            shader.setUniformIf("grassDensity", grassDensitySum);
            shader.setUniformIf("grassScale", grassScale);
            shader.setUniformIf("mvp", mvp);
            shader.setUniformIf("model", model);
            shader.setUniformIf("view", view);
            shader.setUniformIf("projection", projection);
            shader.setUniformIf("color", groundColor);

            shader.bindTexture("albedo", 0, groundTex);

            mesh.draw();

            grassDensitySum += grassShellDensityStep;
            grassHeightSum += grassShellHeightStep;
        }
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

                    camPos += glm::vec3(0.0f, cameraChaseHeight, 0.0f);
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
    if (textures.find(SKY_CUBEMAP_FOLDER) == textures.end()) { return; }
    if (meshes.find(SKY_MESH_FILE) == meshes.end()) { return; }

    // Disable writing to the depth buffer, because we're going to draw behind
    // everything else, so we don't want to overwrite the depth buffer (and screw up
    // the visible order). Done this way to minimize overdraw
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    auto& shader = shaders.at("skybox");
    Texture& skybox = textures.at(SKY_CUBEMAP_FOLDER);
    auto& mesh = meshes.at(SKY_MESH_FILE);

    if (!shader.hasUniform("vp")) {
        std::cerr << "Renderer: Invalid skybox shader, has no vp uniform\n";
    }

    if (!shader.hasUniform("skybox")) {
        std::cerr << "Renderer: Invalid skybox shader, has no skybox samplerCube uniform\n";
    }

    glm::mat4 vp = projection * view;
    vp = glm::scale(vp, glm::vec3(skyboxSize, skyboxSize, skyboxSize));

    shader.use();
    shader.bindTexture("skybox", 0, skybox);
    shader.setUniformIf("vp", vp);

    mesh.draw();

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}



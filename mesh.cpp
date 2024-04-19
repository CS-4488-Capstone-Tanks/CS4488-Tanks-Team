#include "mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh() : vao(0), vbo(0), ebo(0), vertexCount(0), indexCount(0) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
}

Mesh::Mesh(const std::filesystem::path& path) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string msg = "Failed to load mesh: ";
        msg += path.string();
        throw std::runtime_error(msg);
    }

    if (scene->mNumMeshes > 1) {
        std::cerr << "WARN: Model file " << path << " has more than 1 mesh, only the first will be used\n";
    }

    // Create the vertex array and buffer
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind them, and upload the data to the GPU buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

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

    vertexCount = mesh->mNumVertices;
    indexCount = indices.size();
}

Mesh::~Mesh() {
    if (vao != 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);

    }
}

void Mesh::draw() {
    if (vao != 0) {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
}

Mesh::Mesh(Mesh&& other) noexcept {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    vertexCount = other.vertexCount;
    indexCount = other.indexCount;
    memset(&other, 0, sizeof(Mesh));
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    vertexCount = other.vertexCount;
    indexCount = other.indexCount;
    memset(&other, 0, sizeof(Mesh));
    return *this;
}

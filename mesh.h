#ifndef TANKS_MESH_H
#define TANKS_MESH_H

#include <QOpenGLExtraFunctions>

#include <filesystem>

/**
 * Handles loading a mesh from disk, storing its GPU resource handles, and cleaning
 * up when destroyed. The class is not copyable, but it is movable with std::move
 *
 * @author Tyson Cox
 */
class Mesh : private QOpenGLExtraFunctions {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int vertexCount;
    int indexCount;
public:
    Mesh();
    Mesh(const std::filesystem::path& path);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    ~Mesh();

    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) noexcept;

    /** Draw the mesh */
    void draw();
};

#endif //TANKS_MESH_H

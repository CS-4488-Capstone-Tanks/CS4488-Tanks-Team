#ifndef TANKS_TEXTURE_H
#define TANKS_TEXTURE_H

#include <QOpenGLExtraFunctions>

#include <filesystem>

/**
 * Handles loading a 2D texture or cubemap from disk, storing its
 * type and GPU resource handles, and cleaning them up when destroyed.
 * The class is not copyable, but it is movable with std::move
 *
 * @author Tyson Cox
 */
class Texture : private QOpenGLExtraFunctions {
    GLenum textype;
    unsigned int texture;

    void loadTex(const std::filesystem::path& path);
    void loadCubemap(const std::filesystem::path& path);
public:
    Texture();
    Texture(const Texture& other) = delete;
    Texture(Texture&& other);
    ~Texture();

    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept;

    /** Get the texture's type (suitable GLenum for passing into glBindTexture) */
    GLenum type();
    /** Get the texture's GPU resource handle */
    unsigned int handle();

    /** Load a regular texture from a file */
    static Texture fromFile(const std::filesystem::path& path);

    /** Load a cubemap from a folder containing exactly 6 images, and up to one optional json metadata file */
    static Texture cubemapFromFolder(const std::filesystem::path& path);
};

#endif //TANKS_TEXTURE_H

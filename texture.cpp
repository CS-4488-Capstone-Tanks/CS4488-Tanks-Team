#include "texture.h"

#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include <iostream>

Texture::Texture() : texture(0) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
}

Texture::Texture(Texture&& other) : texture(other.texture), textype(other.textype) {
    QOpenGLExtraFunctions::initializeOpenGLFunctions();
    other.texture = 0;
}

Texture::~Texture() {
    if (texture != 0) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
}

Texture& Texture::operator=(Texture&& other) noexcept{
    texture = other.texture;
    textype = other.textype;
    other.texture = 0;
    return *this;
}

GLenum Texture::type() {
    return textype;
}

unsigned int Texture::handle() {
    return texture;
}

Texture Texture::fromFile(const std::filesystem::path& path) {
    Texture t;
    t.textype = GL_TEXTURE_2D;
    t.loadTex(path);
    return t;
}

Texture Texture::cubemapFromFolder(const std::filesystem::path& path) {
    Texture t;
    t.textype = GL_TEXTURE_CUBE_MAP;
    t.loadCubemap(path);
    return t;
}

void Texture::loadTex(const std::filesystem::path& path) {
    // Use QT to load the image
    QImage image(QString::fromStdString(path.string()));
    if (image.isNull()) {
        std::string msg = "Renderer: Failed to load image: ";
        msg += path.string();
        throw std::runtime_error(msg);
    }

    // OpenGL requires specific data formatting
    QImage glImage = image.mirrored().convertToFormat(QImage::Format_RGBA8888);

    // Generate a texture
    glGenTextures(1, &texture);
    glBindTexture(textype, texture);

    // Set texture parameters, like how the texture should be resized and what to do if it doesn't quite fit right
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
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

void Texture::loadCubemap(const std::filesystem::path& path) {
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
    glGenTextures(1, &texture);
    glBindTexture(textype, texture);

    for(size_t i = 0; i < images.size(); i++) {
        const auto& imgpath = images[i];

        // Use QT to load the image
        QImage image(QString::fromStdString(imgpath.string()));
        if (image.isNull()) {
            std::string msg = "Renderer: Failed to load cubemap image ";
            msg += ": ";
            msg += imgpath.string();

            glDeleteTextures(1, &texture);

            throw std::runtime_error(msg);
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
}



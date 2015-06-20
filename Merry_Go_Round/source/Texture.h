//
// Created by johannes on 6/20/15.
//

#ifndef COMPUTER_GRAPHICS_TEXTURE_H
#define COMPUTER_GRAPHICS_TEXTURE_H


class Texture {
public:
    Texture(const char* filename);
    ~Texture();
    GLuint TextureID;
    GLuint TextureUniform;
    unsigned char *data;
    unsigned int width, height;
private:
    int loadTexture(const char* filename);
    void setupTexture();
};


#endif //COMPUTER_GRAPHICS_TEXTURE_H

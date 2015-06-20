//
// Created by johannes on 6/20/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <iostream>
#include "Texture.h"

using namespace std;

Texture::Texture(const char *filename) {
    if (loadTexture(filename) == 0) {
        cerr << "could not load texture.";
    } else
        setupTexture();
}

int Texture::loadTexture(const char *filename) {
        printf("Reading image %s.\n", filename);

        /* Header data of BMP file */
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;

        /* Open the file */
        FILE* file = fopen(filename,"rb");

        if (!file)
        {
            printf("%s could not be opened.\n", filename);
            return 0;
        }

        /* Parse the header (i.e. try to read first 54 bytes) */
        if (fread(header, 1, 54, file) != 54)
        {
            printf("Not a correct BMP file.\n");
            fclose (file);
            return 0;
        }

        /* BMP files begin with "BM" */
        if (header[0]!='B' || header[1]!='M')
        {
            printf("Not a correct BMP file.\n");
            fclose(file);
            return 0;
        }

        /* Make sure file is 24bpp */
        if (*(int*)&(header[0x1E]) !=0)  /* No compression */
        {
            printf("Not a correct BMP file\n");
            return 0;
        }

        if (*(int*)&(header[0x1C]) != 24)  /* 24 bits per pixel */
        {
            printf("Not a correct BMP file\n");
            return 0;
        }

        /* Read image information */
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

        /* Guess size if not provided, assuming one byte
         * each for RGB */
        if (imageSize == 0)
            imageSize = width * height * 3;

        /* Adjust for BMP header */
        if (dataPos == 0)
            dataPos = 54;

        /* Allocate memory for RGB data */
        data = (unsigned char*)malloc(sizeof(unsigned char) * (imageSize+1));

        /* Read data from file */
        fread(data, 1, imageSize, file);

        fclose (file);
        return 1;
}

void Texture::setupTexture() {

    /* Create texture name and store in handle */
    glGenTextures(1, &TextureID);

    /* Bind texture */
    glBindTexture(GL_TEXTURE_2D, TextureID);

    /* Load texture image into memory */
    glTexImage2D(GL_TEXTURE_2D,     /* Target texture */
                 0,                 /* Base level */
                 GL_RGB,            /* Each element is RGB triple */
                 width,    /* Texture dimensions */
                 height,
                 0,                 /* Border should be zero */
                 GL_BGR,            /* Data storage format for BMP file */
                 GL_UNSIGNED_BYTE,  /* Type of pixel data, one byte per channel */
                 data);    /* Pointer to image data  */

    /* Next set up texturing parameters */

    /* Repeat texture on edges when tiling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* Linear interpolation for magnification */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Trilinear MIP mapping for minification */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    free(data);
}

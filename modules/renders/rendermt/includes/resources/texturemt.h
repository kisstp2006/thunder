#ifndef TEXTUREMT_H
#define TEXTUREMT_H

#include <resources/texture.h>

#include "wrappermt.h"

class TextureMt : public Texture {
    A_OVERRIDE(TextureMt, Texture, Resources)

    A_NOPROPERTIES()
    A_NOMETHODS()
    A_NOENUMS()

public:
    TextureMt();

    MTL::Texture *nativeHandle();

private:
    void readPixels(int x, int y, int width, int height) override;

    void updateTexture();

    void uploadTexture(uint32_t slice);

    MTL::PixelFormat pixelFormat();

private:
    MTL::Texture *m_native;

};

#endif // TEXTUREMT_H

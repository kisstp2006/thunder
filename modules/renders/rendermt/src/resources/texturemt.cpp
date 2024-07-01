#include "resources/texturemt.h"

#include <cstring>

#include "commandbuffermt.h"

TextureMt::TextureMt() :
        m_native(nullptr) {

}

MTL::Texture *TextureMt::nativeHandle() {
    switch(state()) {
        case Unloading: {
            if(m_native) {
                m_native->release();
            }
            m_native = nullptr;

            switchState(ToBeDeleted);
        } break;
        case ToBeUpdated: {
            updateTexture();

            switchState(Ready);
        } break;
        default: break;
    }

    return m_native;
}

void TextureMt::readPixels(int x, int y, int width, int height) {
    bool depth = (format() == Depth);

    if(sides() != 0) {
        // Surface &dst = surface(0);

        // glReadPixels(x, y, width, height,
        //              (depth) ? GL_DEPTH_COMPONENT : GL_RGBA,
        //              (depth) ? GL_FLOAT : GL_UNSIGNED_BYTE, dst[0].data());
        // CheckGLError();
    }
}

void TextureMt::updateTexture() {
    if(m_native == nullptr) {
        MTL::TextureDescriptor *textureDesc = MTL::TextureDescriptor::alloc()->init();
        textureDesc->setWidth(m_width);
        textureDesc->setHeight(m_height);
        textureDesc->setPixelFormat(pixelFormat());
        textureDesc->setTextureType(isCubemap() ? MTL::TextureTypeCube : MTL::TextureType2D);
        textureDesc->setStorageMode(MTL::StorageModeManaged);
        textureDesc->setUsage(isRender() ?
                                  (MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead) :
                                  (MTL::ResourceUsageSample | MTL::ResourceUsageRead));
        textureDesc->setMipmapLevelCount(mipCount());

        //MTL::SamplerDescriptor

        m_native = WrapperMt::device()->newTexture(textureDesc);
    }

    if(isCubemap()) {
        for(uint32_t n = 0; n < 6; n++) {
            uploadTexture(n);
        }
    } else {
        uploadTexture(0);
    }
}

void TextureMt::uploadTexture(uint32_t slice) {
    if(!isRender()) {
        const Surface &image = surface(slice);
        int bpp = 4; // bytes per pixel (probably issues with compressed formats)

        for(uint32_t i = 0; i < image.size(); i++) {
            int32_t w = (m_width >> i);
            int32_t h = (m_height >> i);
            m_native->replaceRegion(MTL::Region(0, 0, w, h), i, slice, image[i].data(), w * bpp, w * h * bpp);
        }
    }
}

MTL::PixelFormat TextureMt::pixelFormat() {
    switch(format()) {
    case R8: return MTL::PixelFormatR8Unorm;
    case RGB10A2: return MTL::PixelFormatRGB10A2Unorm;
    case RGBA32Float: return MTL::PixelFormatRGBA32Float;
    case R11G11B10Float: return MTL::PixelFormatRG11B10Float;
    case Depth: return (depthBits() == 16) ? MTL::PixelFormatDepth16Unorm : MTL::PixelFormatDepth24Unorm_Stencil8;
    default: break;
    }

    return MTL::PixelFormatRGBA8Unorm;
}

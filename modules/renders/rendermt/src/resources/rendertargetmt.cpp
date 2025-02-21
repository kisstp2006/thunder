#include "resources/rendertargetmt.h"

#include "resources/texturemt.h"

#include "commandbuffermt.h"

RenderTargetMt::RenderTargetMt() :
        m_descriptor(nullptr) {

}

MTL::RenderPassDescriptor *RenderTargetMt::nativeHandle() {
    switch(state()) {
        case Unloading: {
            if(m_descriptor != nullptr) {
                m_descriptor->release();
            }
            m_descriptor = nullptr;

            setState(ToBeDeleted);
            return nullptr;
        } break;
        case ToBeUpdated: {
            if(m_descriptor != nullptr) {
                m_descriptor->release();
            }

            m_descriptor = MTL::RenderPassDescriptor::alloc()->init();

            for(uint32_t i = 0; i < colorAttachmentCount(); i++) {
                TextureMt *color = static_cast<TextureMt *>(colorAttachment(i));
                if(color) {
                    MTL::Texture *handle = color->nativeHandle();

                    MTL::RenderPassColorAttachmentDescriptor *desc = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();
                    desc->setLevel(m_currentLevel);
                    desc->setTexture(handle);

                    m_descriptor->colorAttachments()->setObject(desc, i);
                }
            }

            TextureMt *depth = static_cast<TextureMt *>(depthAttachment());
            if(depth) {
                MTL::Texture *handle = depth->nativeHandle();

                MTL::RenderPassDepthAttachmentDescriptor *desc = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();
                desc->setLevel(m_currentLevel);
                desc->setTexture(handle);

                m_descriptor->setDepthAttachment(desc);
            }

            setState(Ready);
        } break;
        default: break;
    }

    return m_descriptor;
}

void RenderTargetMt::setNativeHandle(MTL::RenderPassDescriptor *descriptor) {
    m_descriptor = descriptor;

    setState(Ready);
}

void RenderTargetMt::setLevel(uint32_t level) {
    m_currentLevel = level;
}

void RenderTargetMt::setClearColor(const Vector4 &color) {

}

void RenderTargetMt::setClearDepth(float depth) {

}

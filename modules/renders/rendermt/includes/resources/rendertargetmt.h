#ifndef RENDERTARGETMT_H
#define RENDERTARGETMT_H

#include <resources/rendertarget.h>

#include "wrappermt.h"

class RenderTargetMt : public RenderTarget {
    A_OVERRIDE(RenderTargetMt, RenderTarget, Resources)

    A_NOPROPERTIES()
    A_NOMETHODS()
    A_NOENUMS()

public:
    RenderTargetMt();

    MTL::RenderPassDescriptor *nativeHandle();
    void setNativeHandle(MTL::RenderPassDescriptor *descriptor);

    void setLevel(uint32_t level);

private:
    MTL::RenderPassDescriptor *m_descriptor;

    uint32_t m_currentLevel;

};

#endif // RENDERTARGETMT_H

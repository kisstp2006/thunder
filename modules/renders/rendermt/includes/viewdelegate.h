#ifndef VIEWDELEGATE_H
#define VIEWDELEGATE_H

#include "wrappermt.h"

class RenderMtSystem;

class ViewDelegate : public MTK::ViewDelegate {
public:
    explicit ViewDelegate(RenderMtSystem *system);

private:
    void drawInMTKView(MTK::View *view) override;

private:
    RenderMtSystem *m_render;

    MTL::CommandQueue *m_queue;

};

#endif // VIEWDELEGATE_H

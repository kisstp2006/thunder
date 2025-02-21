#ifndef VIEWDELEGATE_H
#define VIEWDELEGATE_H

#include "wrappermt.h"

class RenderMtSystem;
class Viewport;

class ViewDelegate : public MTK::ViewDelegate {
public:
    explicit ViewDelegate(RenderMtSystem *system, Viewport *viewport);

private:
    void drawInMTKView(MTK::View *view) override;

private:
    RenderMtSystem *m_render;

    Viewport *m_viewport;

    MTL::CommandQueue *m_queue;

};

#endif // VIEWDELEGATE_H

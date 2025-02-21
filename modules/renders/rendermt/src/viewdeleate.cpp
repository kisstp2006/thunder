#include "viewdelegate.h"

#include "rendermtsystem.h"
#if defined(SHARED_DEFINE)
#include "editor/viewport/viewport.h"
#endif
ViewDelegate::ViewDelegate(RenderMtSystem *system, Viewport *viewport) :
        m_render(system),
        m_viewport(viewport),
        m_queue(WrapperMt::device()->newCommandQueue()) {

}

void ViewDelegate::drawInMTKView(MTK::View *view) {
    NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer *cmd = m_queue->commandBuffer();

    view->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 0.0, 1.0));

    m_render->setCurrentView(view, cmd);

    // Render cycle here
#if defined(SHARED_DEFINE)
    m_viewport->onDraw();
#endif
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();
    cmd->waitUntilScheduled();

    pool->release();
}

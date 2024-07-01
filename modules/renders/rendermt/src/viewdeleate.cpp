#include "viewdelegate.h"

#include "rendermtsystem.h"

ViewDelegate::ViewDelegate(RenderMtSystem *system) :
        m_render(system),
        m_queue(WrapperMt::device()->newCommandQueue()) {

}

void ViewDelegate::drawInMTKView(MTK::View *view) {
    NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer *cmd = m_queue->commandBuffer();

    m_render->setCurrentView(view, cmd);

    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}

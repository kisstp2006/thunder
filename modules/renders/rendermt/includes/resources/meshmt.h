#ifndef MESHMT_H
#define MESHMT_H

#include <list>

#include <resources/mesh.h>
#include <amath.h>

#include "wrappermt.h"

class CommandBufferMt;

class MeshMt : public Mesh {
    A_OVERRIDE(MeshMt, Mesh, Resources)

    A_NOPROPERTIES()
    A_NOMETHODS()

public:
    MeshMt();

    MTL::Buffer *vertexBuffer();

    MTL::Buffer *indexBuffer();

protected:
    void bind();

    void update();

protected:
    MTL::Buffer *m_vertexBuffer;

    MTL::Buffer *m_indexBuffer;

};

#endif // MESHMT_H

#include "resources/meshmt.h"

#include "commandbuffermt.h"

MeshMt::MeshMt() :
        m_vertexBuffer(nullptr),
        m_indexBuffer(nullptr) {

}

MTL::Buffer *MeshMt::vertexBuffer() {
    bind();

    return m_vertexBuffer;
}

MTL::Buffer *MeshMt::indexBuffer() {
    bind();

    return m_indexBuffer;
}

void MeshMt::bind() {
    switch(state()) {
        case ToBeUpdated: {
            update();

            switchState(Ready);
        } break;
        case Ready: break;
        case Unloading: {
            if(m_vertexBuffer != nullptr) {
                m_vertexBuffer->release();
            }
            m_vertexBuffer = nullptr;

            if(m_indexBuffer != nullptr) {
                m_indexBuffer->release();
            }
            m_indexBuffer = nullptr;

            switchState(ToBeDeleted);
            return;
        }
        default: return;
    }
}

void MeshMt::update() {
    if(!indices().empty()) {
        if(m_indexBuffer != nullptr) {
            m_indexBuffer->release();
        }

        size_t size = sizeof(uint32_t) * indices().size();

        m_indexBuffer = WrapperMt::device()->newBuffer(size, MTL::ResourceStorageModeManaged);
        memcpy(m_indexBuffer->contents(), indices().data(), size);

        m_indexBuffer->didModifyRange(NS::Range::Make(0, size));
    }

    if(!vertices().empty()) {
        uint32_t vCount = vertices().size();

        uint32_t size = sizeof(Vector3) * vCount;
        if(!uv0().empty()) size += sizeof(Vector2) * vCount;
        if(!colors().empty()) size += sizeof(Vector4) * vCount;
        if(!normals().empty()) size += sizeof(Vector3) * vCount;
        if(!tangents().empty()) size += sizeof(Vector3) * vCount;
        if(!weights().empty()) size += sizeof(Vector4) * vCount;
        if(!bones().empty()) size += sizeof(Vector4) * vCount;

        m_vertexBuffer = WrapperMt::device()->newBuffer(size, MTL::ResourceStorageModeManaged);

        uint8_t *ptr = reinterpret_cast<uint8_t *>(m_vertexBuffer->contents());

        size_t length = sizeof(Vector3) * vCount;
        size_t offset = 0;

        memcpy(ptr, vertices().data(), length);

        offset += length;

        if(!uv0().empty()) {
            length = sizeof(Vector2) * vCount;
            memcpy(&ptr[offset], uv0().data(), length);
            offset += length;
        }
        if(!colors().empty()) {
            length = sizeof(Vector4) * vCount;
            memcpy(&ptr[offset], colors().data(), length);
            offset += length;
        }
        if(!normals().empty()) {
            length = sizeof(Vector3) * vCount;
            memcpy(&ptr[offset], normals().data(), length);
            offset += length;
        }
        if(!tangents().empty()) {
            length = sizeof(Vector3) * vCount;
            memcpy(&ptr[offset], tangents().data(), length);
            offset += length;
        }
        if(!bones().empty()) {
            length = sizeof(Vector4) * vCount;
            memcpy(&ptr[offset], bones().data(), length);
            offset += length;
        }
        if(!weights().empty()) {
            length = sizeof(Vector4) * vCount;
            memcpy(&ptr[offset], weights().data(), length);
        }

        m_vertexBuffer->didModifyRange(NS::Range::Make(0, size));
    }
}

#ifndef MATERIALMT_H
#define MATERIALMT_H

#include <unordered_map>
#include <list>

#include <resources/material.h>

#include <engine.h>

#include "wrappermt.h"

class CommandBufferMt;

class MaterialInstanceMt : public MaterialInstance {
public:
    MaterialInstanceMt(Material *material);

    ~MaterialInstanceMt();

    bool bind(CommandBufferMt *buffer, uint32_t layer);

private:
    static void setBlendState(const Material::BlendState &state);

    static void setRasterState(const Material::RasterState &state);

    static void setDepthState(const Material::DepthState &state);

    static void setStencilState(const Material::StencilState &state);

private:
    Material::BlendState m_blendState;

    Material::DepthState m_depthState;

    Material::StencilState m_stencilState;

    MTL::Buffer *m_instanceBuffer;

};

class MaterialMt : public Material {
    A_OVERRIDE(MaterialMt, Material, Resources)

    A_NOPROPERTIES()
    A_NOMETHODS()
    A_NOENUMS()

    enum ShaderType {
        VertexStatic      = 1,
        VertexSkinned,
        VertexParticle,
        VertexLast,

        FragmentDefault,
        FragmentVisibility,
        FragmentLast,

        GeometryDefault,
        GeometryLast
    };

    typedef std::unordered_map<uint32_t, uint32_t> ObjectMap;

public:
    void loadUserData(const VariantMap &data) override;

    uint32_t bind(uint32_t layer, uint16_t vertex);

    uint32_t getProgramState(uint16_t type);

    Textures &textures() { return m_textures; }

protected:
    uint32_t buildShader(uint16_t type, const std::string &src = std::string());

    MaterialInstance *createInstance(SurfaceType type = SurfaceType::Static) override;

    static void setDepthState(const DepthState &state);

    static void setStencilState(const StencilState &state);

private:
    friend class MaterialInstanceMt;

    ObjectMap m_programs;

    std::map<uint16_t, std::string> m_shaderSources;

};

#endif // MATERIALMT_H

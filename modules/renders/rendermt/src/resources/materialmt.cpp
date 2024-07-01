#include "resources/materialmt.h"

#include <cstring>

#include "commandbuffermt.h"

#include "components/transform.h"

#include "resources/texturemt.h"

#include <log.h>

namespace  {
    const char *gVisibility("Visibility");
    const char *gDefault("Default");

    const char *gStatic("Static");
    const char *gSkinned("Skinned");
    const char *gParticle("Particle");

    const char *gGeometry("Geometry");
};

void MaterialMt::loadUserData(const VariantMap &data) {
    Material::loadUserData(data);

    static map<string, uint32_t> pairs = {
        {gVisibility, FragmentVisibility},
        {gDefault, FragmentDefault},

        {gStatic, VertexStatic},
        {gSkinned, VertexSkinned},
        {gParticle, VertexParticle},

        {gGeometry, GeometryDefault}
    };

    for(auto &pair : pairs) {
        auto it = data.find(pair.first);
        if(it != data.end()) {
            m_shaderSources[pair.second] = (*it).second.toString();
        }
    }

    switchState(ToBeUpdated);
}

uint32_t MaterialMt::getProgramState(uint16_t type) {
    switch(state()) {
        case Unloading: {
            // for(auto it : m_programs) {
            //     glDeleteProgram(it.second);
            // }
            // m_programs.clear();

            switchState(ToBeDeleted);
        } break;
        case ToBeUpdated: {
            // for(auto it : m_programs) {
            //     glDeleteProgram(it.second);
            // }
            // m_programs.clear();

            MTL::RenderPipelineDescriptor *renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();

            uint32_t geometry = 0;
            auto itg = m_shaderSources.find(GeometryDefault);
            if(itg != m_shaderSources.end()) {
                buildShader(itg->first, itg->second);
            }

            for(uint16_t v = Static; v < VertexLast; v++) {
                auto itv = m_shaderSources.find(v);
                if(itv != m_shaderSources.end()) {
                    for(uint16_t f = FragmentDefault; f < FragmentLast; f++) {
                        auto itf = m_shaderSources.find(f);
                        if(itf != m_shaderSources.end()) {
                            uint32_t vertex = buildShader(itv->first, itv->second);
                            uint32_t fragment = buildShader(itf->first, itf->second);

                            vector<uint32_t> shaders = {vertex, fragment};
                            if(geometry > 0) {
                                shaders.push_back(geometry);
                            }

                            //m_programs[v * f] = buildProgram(shaders);
                        }
                    }
                }
            }

            switchState(Ready);
        } break;
        default: break;
    }

    auto it = m_programs.find(type);
    if(it != m_programs.end()) {
        return it->second;
    }
    return 0;
}

uint32_t MaterialMt::bind(uint32_t layer, uint16_t vertex) {
    if((layer & CommandBuffer::DEFAULT || layer & CommandBuffer::SHADOWCAST) && m_blendState.enabled) {
        return 0;
    }
    if(layer & CommandBuffer::TRANSLUCENT && !m_blendState.enabled) {
        return 0;
    }

    uint16_t type = MaterialMt::FragmentDefault;
    if((layer & CommandBuffer::RAYCAST) || (layer & CommandBuffer::SHADOWCAST)) {
        type = FragmentVisibility;
    }

    uint32_t program = getProgramState(vertex * type);
    if(!program) {
        return 0;
    }

    return program;
}

uint32_t MaterialMt::buildShader(uint16_t type, const string &src) {
    // uint32_t t = 0;
    // if(type >= FragmentDefault && type < FragmentLast) {
    //     t = GL_FRAGMENT_SHADER;
    // } else if(type >= VertexStatic && type < VertexLast) {
    //     t = GL_VERTEX_SHADER;
    // } else if(type >= GeometryDefault && type < GeometryLast) {
    //     return 0;
    // }

    // uint32_t shader = glCreateShader(t);
    // if(shader) {
    //     const char *data = src.c_str();

    //     glShaderSource(shader, 1, &data, nullptr);
    //     glCompileShader(shader);

    //     checkShader(shader);
    // }

    // return shader;
    return 0;
}

//uint32_t MaterialMt::buildProgram(const vector<uint32_t> &shaders) {
    // uint32_t result = glCreateProgram();
    // if(result) {
    //     for(auto it : shaders) {
    //         glAttachShader(result, it);
    //     }

    //     glLinkProgram(result);

    //     for(auto it : shaders) {
    //         glDetachShader(result, it);
    //         glDeleteShader(it);
    //     }

    //     checkProgram(result);

    //     glUseProgram(result);
    //     uint8_t t = 0;
    //     for(auto &it : m_textures) {
    //         int32_t location = glGetUniformLocation(result, it.name.c_str());
    //         if(location > -1) {
    //             glUniform1i(location, t);
    //         }
    //         t++;
    //     }
    // }

    // return result;
//    return 0;
//}

MaterialInstance *MaterialMt::createInstance(SurfaceType type) {
    MaterialInstanceMt *result = new MaterialInstanceMt(this);

    initInstance(result);

    if(result) {
        result->setSurfaceType(type);
    }

    return result;
}

inline int32_t convertAction(int32_t action) {
    // switch(action) {
    //     case Material::ActionType::Keep: return GL_KEEP;
    //     case Material::ActionType::Clear: return GL_ZERO;
    //     case Material::ActionType::Replace: return GL_REPLACE;
    //     case Material::ActionType::Increment: return GL_INCR;
    //     case Material::ActionType::IncrementWrap: return GL_INCR_WRAP;
    //     case Material::ActionType::Decrement: return GL_DECR;
    //     case Material::ActionType::DecrementWrap: return GL_DECR_WRAP;
    //     case Material::ActionType::Invert: return GL_INVERT;
    //     default: break;
    // }

    return action;
}

inline MTL::BlendOperation convertBlendMode(int32_t mode) {
    switch(mode) {
        case Material::BlendOp::Add: return MTL::BlendOperationAdd;
        case Material::BlendOp::Subtract: return MTL::BlendOperationSubtract;
        case Material::BlendOp::ReverseSubtract: return MTL::BlendOperationReverseSubtract;
        case Material::BlendOp::Min: return MTL::BlendOperationMin;
        case Material::BlendOp::Max: return MTL::BlendOperationMax;
        default: break;
    }

    return MTL::BlendOperationAdd;
}

inline MTL::BlendFactor convertBlendFactor(int32_t factor) {
    switch(factor) {
        case Material::BlendFactor::Zero: return MTL::BlendFactorZero;
        case Material::BlendFactor::One: return MTL::BlendFactorOne;
        case Material::BlendFactor::SourceColor: return MTL::BlendFactorSourceColor;
        case Material::BlendFactor::OneMinusSourceColor: return MTL::BlendFactorOneMinusSourceColor;
        case Material::BlendFactor::DestinationColor: return MTL::BlendFactorDestinationColor;
        case Material::BlendFactor::OneMinusDestinationColor: return MTL::BlendFactorOneMinusDestinationColor;
        case Material::BlendFactor::SourceAlpha: return MTL::BlendFactorSource1Alpha;
        case Material::BlendFactor::OneMinusSourceAlpha: return MTL::BlendFactorOneMinusSource1Alpha;
        case Material::BlendFactor::DestinationAlpha: return MTL::BlendFactorDestinationAlpha;
        case Material::BlendFactor::OneMinusDestinationAlpha: return MTL::BlendFactorOneMinusDestinationAlpha;
        case Material::BlendFactor::SourceAlphaSaturate: return MTL::BlendFactorSourceAlphaSaturated;
        case Material::BlendFactor::ConstantColor: return MTL::BlendFactorBlendColor;
        case Material::BlendFactor::OneMinusConstantColor: return MTL::BlendFactorOneMinusBlendColor;
        case Material::BlendFactor::ConstantAlpha: return MTL::BlendFactorBlendAlpha;
        case Material::BlendFactor::OneMinusConstantAlpha: return MTL::BlendFactorOneMinusBlendAlpha;
        default: break;
    }

    return MTL::BlendFactorOne;
}

MaterialInstanceMt::MaterialInstanceMt(Material *material) :
        MaterialInstance(material),
        m_instanceBuffer(nullptr) {

    MaterialMt *m = static_cast<MaterialMt *>(material);
    m_blendState = m->m_blendState;

    // Blending
    m_blendState.colorOperation = convertBlendMode(m_blendState.colorOperation);
    m_blendState.alphaOperation = convertBlendMode(m_blendState.alphaOperation);

    m_blendState.sourceColorBlendMode = convertBlendFactor(m_blendState.sourceColorBlendMode);
    m_blendState.sourceAlphaBlendMode = convertBlendFactor(m_blendState.sourceAlphaBlendMode);

    m_blendState.destinationColorBlendMode = convertBlendFactor(m_blendState.destinationColorBlendMode);
    m_blendState.destinationAlphaBlendMode = convertBlendFactor(m_blendState.destinationAlphaBlendMode);

    // Depth
    m_depthState = m->m_depthState;
    m_depthState.compareFunction = 0x0200 | m_depthState.compareFunction;

    // Stencil
    m_stencilState = m->m_stencilState;
    m_stencilState.compareFunctionBack = 0x0200 | m_stencilState.compareFunctionBack;
    m_stencilState.compareFunctionFront = 0x0200 | m_stencilState.compareFunctionFront;

    m_stencilState.failOperationBack = convertAction(m_stencilState.failOperationBack);
    m_stencilState.failOperationFront = convertAction(m_stencilState.failOperationFront);

    m_stencilState.zFailOperationBack = convertAction(m_stencilState.zFailOperationBack);
    m_stencilState.zFailOperationFront = convertAction(m_stencilState.zFailOperationFront);

    m_stencilState.passOperationBack = convertAction(m_stencilState.passOperationBack);
    m_stencilState.passOperationFront = convertAction(m_stencilState.passOperationFront);

}

MaterialInstanceMt::~MaterialInstanceMt() {
    if(m_instanceBuffer != nullptr) {
        m_instanceBuffer->release();
        m_instanceBuffer = 0;
    }
}

bool MaterialInstanceMt::bind(CommandBufferMt *buffer, uint32_t layer) {
    MaterialMt *material = static_cast<MaterialMt *>(m_material);
    uint32_t program = material->bind(layer, m_surfaceType + 1);
    if(program) {
//         glUseProgram(program);

//         uint32_t materialType = material->materialType();

//         if(m_instanceBuffer == 0) {
//             glGenBuffers(1, &m_instanceBuffer);
//         }

//         /*if(m_uniformDirty || index > 0)*/ {
//             uint32_t offset = index * gMaxUBO;

//             vector<uint8_t> &gpuBuffer = m_batchBuffer.empty() ? rawUniformBuffer() : m_batchBuffer;

//             if(materialType == Material::Surface) {
//                 glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
//                 glBufferData(GL_SHADER_STORAGE_BUFFER, gpuBuffer.size(), gpuBuffer.data(), GL_DYNAMIC_DRAW);
//                 glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//             }

//             m_uniformDirty = false;
//         }
// #ifdef THUNDER_MOBILE
//         glBindBufferBase(GL_UNIFORM_BUFFER, LOCAL_BIND, m_instanceBuffer);
// #else
//         if(materialType == Material::Surface) {
//             glBindBufferBase(GL_SHADER_STORAGE_BUFFER, LOCAL_BIND, m_instanceBuffer);
//         } else {
//             glBindBufferBase(GL_UNIFORM_BUFFER, LOCAL_BIND, m_instanceBuffer);
//         }
// #endif

        uint8_t i = 0;
        for(auto &it : material->textures()) {
            Texture *tex = it.texture;
            Texture *tmp = texture(it.name.c_str());

            if(tmp) {
                tex = tmp;
            } else {
                tmp = buffer->texture(it.name.c_str());
                if(tmp) {
                    tex = tmp;
                }
            }

            // if(tex) {
            //     glActiveTexture(GL_TEXTURE0 + i);
            //     uint32_t texture = GL_TEXTURE_2D;
            //     if(tex->isCubemap()) {
            //         texture = GL_TEXTURE_CUBE_MAP;
            //     }

            //     glBindTexture(texture, static_cast<TextureGL *>(tex)->nativeHandle());
            // }
            i++;
        }

        Material::RasterState rasterState;
/*
        rasterState.cullingMode = GL_BACK;

        if(layer & CommandBuffer::SHADOWCAST) {
            rasterState.cullingMode = GL_FRONT;
        } else if(!material->doubleSided() && !(layer & CommandBuffer::RAYCAST)) {
            if(materialType != Material::LightFunction) {
                rasterState.cullingMode = GL_BACK;
            }
        } else {
            rasterState.enabled = false;
        }
*/
        setRasterState(rasterState);

        Material::BlendState blendState = m_blendState;
        if(layer & CommandBuffer::RAYCAST) {
            // blendState.sourceColorBlendMode = GL_ONE;
            // blendState.sourceAlphaBlendMode = GL_ONE;

            // blendState.destinationColorBlendMode = GL_ZERO;
            // blendState.destinationAlphaBlendMode = GL_ZERO;
        }

        setBlendState(blendState);

        setDepthState(m_depthState);

        setStencilState(m_stencilState);

        return true;
    }

    return false;
}

void MaterialInstanceMt::setBlendState(const Material::BlendState &state) {
    // if(state.enabled) {
    //     glEnable(GL_BLEND);

    //     glBlendFuncSeparate(state.sourceColorBlendMode, state.destinationColorBlendMode, state.sourceAlphaBlendMode, state.destinationAlphaBlendMode);

    //     glBlendEquationSeparate(state.colorOperation, state.alphaOperation);
    // } else {
    //     glDisable(GL_BLEND);
    // }
}

void MaterialInstanceMt::setRasterState(const Material::RasterState &state) {
    // if(state.enabled) {
    //     glEnable(GL_CULL_FACE);

    //     glCullFace(state.cullingMode);
    // } else {
    //     glDisable(GL_CULL_FACE);
    // }
}

void MaterialInstanceMt::setDepthState(const Material::DepthState &state) {
    // if(state.enabled) {
    //     glEnable(GL_DEPTH_TEST);

    //     glDepthMask(state.writeEnabled);

    //     glDepthFunc(state.compareFunction);
    // } else {
    //     glDisable(GL_DEPTH_TEST);
    // }
}

void MaterialInstanceMt::setStencilState(const Material::StencilState &state) {
    // if(state.enabled) {
    //     glEnable(GL_STENCIL_TEST);

    //     glStencilMask(state.writeMask);

    //     glStencilFuncSeparate(GL_BACK, state.compareFunctionBack, state.reference, state.readMask);
    //     glStencilFuncSeparate(GL_FRONT, state.compareFunctionFront, state.reference, state.readMask);

    //     glStencilOpSeparate(GL_BACK, state.failOperationBack, state.zFailOperationBack, state.passOperationBack);
    //     glStencilOpSeparate(GL_FRONT, state.failOperationFront, state.zFailOperationFront, state.passOperationFront);
    // } else {
    //     glDisable(GL_STENCIL_TEST);
    // }
}

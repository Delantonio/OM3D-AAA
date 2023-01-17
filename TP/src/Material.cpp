#include "Material.h"

#include <glad/glad.h>

#include <algorithm>
#include "utils.h"

namespace OM3D {

Material::Material() {
}

void Material::set_program(std::shared_ptr<Program> prog) {
    _program = std::move(prog);
}

void Material::set_blend_mode(BlendMode blend) {
    _blend_mode = blend;
}

void Material::set_depth_test_mode(DepthTestMode depth) {
    _depth_test_mode = depth;
}

void Material::set_texture(u32 slot, std::shared_ptr<Texture> tex) {
    if(const auto it = std::find_if(_textures.begin(), _textures.end(), [&](const auto& t) { return t.second == tex; }); it != _textures.end()) {
        it->second = std::move(tex);
    } else {
        _textures.emplace_back(slot, std::move(tex));
    }
}

void Material::set_cull_mode(CullMode cull) {
    _cull_mode = cull;
}

void Material::bind() const {
    switch(_blend_mode) {
        case BlendMode::None:
            glDisable(GL_BLEND);
        break;

        case BlendMode::Alpha:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
        
        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            // glBlendFunc(GL_ONE, GL_ONE);
        break;
    }

    switch(_depth_test_mode) {
        case DepthTestMode::None:
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
        break;

        case DepthTestMode::Equal:
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_EQUAL);
        break;

        case DepthTestMode::Standard:
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            // We are using reverse-Z
            glDepthFunc(GL_GEQUAL);
        break;

        case DepthTestMode::Reversed:
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            // We are using reverse-Z
            glDepthFunc(GL_LEQUAL);
        break;
    }
    
    switch (_cull_mode) {
    case CullMode::None:
        glDisable(GL_CULL_FACE);
        break;
    case CullMode::Front:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
        break;
    case CullMode::Back:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        break;
    case CullMode::BackAndFront:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);
        glFrontFace(GL_CCW);
        break;
    }

    for(const auto& texture : _textures) {
        texture.second->bind(texture.first);
    }
    _program->bind();
}


Material Material::empty_material(std::shared_ptr<Program> program, std::vector<std::shared_ptr<Texture>> textures)
{
    Material material;
    material.set_program(program);
    for (u32 i = 0; i < textures.size(); ++i)
    {
        material.set_texture(i, textures[i]);
        // textures[i]->print_handle();
    }
    return material;
}

std::shared_ptr<Material> Material::empty_material() {
    static std::weak_ptr<Material> weak_material;
    auto material = weak_material.lock();
    if(!material) {
        material = std::make_shared<Material>();
        material->_program = Program::from_files("gbuffer.frag", "basic.vert");
        weak_material = material;
    }
    return material;
}

Material Material::textured_material() {
    Material material;
    material._program = Program::from_files("gbuffer.frag", "basic.vert", {"TEXTURED"});
    return material;
}

Material Material::textured_normal_mapped_material() {
    Material material;
    material._program = Program::from_files("gbuffer.frag", "basic.vert", std::array<std::string, 2>{"TEXTURED", "NORMAL_MAPPED"});
    return material;
}


}

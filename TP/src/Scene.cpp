#include "Scene.h"

#include <glad/glad.h>
#include <TypedBuffer.h>

#include <shader_structs.h>
#include "Camera.h"
#include "Material.h"
#include "glm/ext/matrix_transform.hpp"

namespace OM3D {

Scene::Scene() {
}

void Scene::add_object(SceneObject obj) {
    _objects.emplace_back(std::move(obj));
}

void Scene::add_object(PointLight obj) {
    _point_lights.emplace_back(std::move(obj));
}

void Scene::render(const Camera& camera) const {
    // Fill and bind frame data buffer
    TypedBuffer<shader::FrameData> buffer(nullptr, 1);
    {
        auto mapping = buffer.map(AccessType::WriteOnly);
        mapping[0].camera.view_proj = camera.view_proj_matrix();
        mapping[0].point_light_count = u32(_point_lights.size());
        mapping[0].sun_color = glm::vec3(1.0f, 1.0f, 1.0f);
        mapping[0].sun_dir = glm::normalize(_sun_direction);
    }
    buffer.bind(BufferUsage::Uniform, 0);

    // Fill and bind lights buffer
    TypedBuffer<shader::PointLight> light_buffer(nullptr, std::max(_point_lights.size(), size_t(1)));
    {
        auto mapping = light_buffer.map(AccessType::WriteOnly);
        for(size_t i = 0; i != _point_lights.size(); ++i) {
            const auto& light = _point_lights[i];
            mapping[i] = {
                light.position(),
                light.radius(),
                light.color(),
                0.0f
            };
        }
    }
    light_buffer.bind(BufferUsage::Storage, 1);

    Frustum frustum = camera.build_frustum();

    // Render every object
    for(const SceneObject& obj : _objects) {
        obj.material()->set_cull_mode(CullMode::Back);
        obj.material()->set_depth_test_mode(DepthTestMode::Standard);
        // obj.material()->set_blend_mode(BlendMode::Alpha);
        auto mesh = obj.mesh();
        glm::vec4 center = obj.transform() * glm::vec4(mesh->bounding_sphere.center, 1.0f);
        center -= glm::vec4(camera.position(), 0.0f);
        if (frustum.intersect(glm::vec3(center.x, center.y, center.z), mesh->bounding_sphere.radius)) {
            obj.render();
        }
    }
}

void Scene::render_triangle(const Camera &camera) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.7f, 0.8f, 0.0f);

    // Fill and bind frame data buffer
    TypedBuffer<shader::FrameData> buffer(nullptr, 1);
    {
        auto mapping = buffer.map(AccessType::WriteOnly);
        mapping[0].camera.view_proj = camera.view_proj_matrix();
        mapping[0].point_light_count = u32(_point_lights.size());
        mapping[0].sun_color = glm::vec3(1.0f, 1.0f, 1.0f);
        mapping[0].sun_dir = glm::normalize(_sun_direction);
    }
    buffer.bind(BufferUsage::Uniform, 0);

    // Fill and bind lights buffer
    TypedBuffer<shader::PointLight> light_buffer(nullptr, std::max(_point_lights.size(), size_t(1)));
    {
        auto mapping = light_buffer.map(AccessType::WriteOnly);
        for(size_t i = 0; i != _point_lights.size(); ++i) {
            const auto& light = _point_lights[i];
            mapping[i] = {
                light.position(),
                light.radius(),
                light.color(),
                0.0f
            };
        }
    }
    light_buffer.bind(BufferUsage::Storage, 1);
    
    // penser à cull les lights

    // Render screen triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

// void Scene::render_triangle(const Camera &camera, std::shared_ptr<SceneObject> light_sphere) const
void Scene::render_triangle(const Camera &camera, SceneObject &light_sphere) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.7f, 0.8f, 0.0f);

    // Fill and bind frame data buffer
    TypedBuffer<shader::FrameData> buffer(nullptr, 1);
    {
        auto mapping = buffer.map(AccessType::WriteOnly);
        mapping[0].camera.view_proj = camera.view_proj_matrix();
        mapping[0].point_light_count = u32(_point_lights.size());
        mapping[0].sun_color = glm::vec3(1.0f, 1.0f, 1.0f);
        mapping[0].sun_dir = glm::normalize(_sun_direction);
    }
    buffer.bind(BufferUsage::Uniform, 0);

    // Fill and bind lights buffer
    TypedBuffer<shader::PointLight> light_buffer(nullptr, std::max(_point_lights.size(), size_t(1)));
    {
        auto mapping = light_buffer.map(AccessType::WriteOnly);
        for(size_t i = 0; i != _point_lights.size(); ++i) {
            const auto& light = _point_lights[i];
            mapping[i] = {
                light.position(),
                light.radius(),
                light.color(),
                0.0f
            };
        }
    }
    light_buffer.bind(BufferUsage::Storage, 1);
    
    Frustum frustum = camera.build_frustum();

    for (size_t i = 0; i != _point_lights.size(); ++i) {
        const auto& light = _point_lights[i];
        glm::mat4 transform = glm::translate(light_sphere.transform(), light.position());
        transform = glm::scale(transform, glm::vec3(light.radius()));
        light_sphere.set_transform(transform);

        light_sphere.material()->set_cull_mode(CullMode::Back);
        light_sphere.material()->set_depth_test_mode(DepthTestMode::None);
        light_sphere.material()->set_blend_mode(BlendMode::Additive);

        auto mesh = light_sphere.mesh();
        glm::vec4 center = light_sphere.transform() * glm::vec4(mesh->bounding_sphere.center, 1.0f);
        center -= glm::vec4(camera.position(), 0.0f);

        if (frustum.intersect(glm::vec3(center.x, center.y, center.z), mesh->bounding_sphere.radius)) {
            light_sphere.render();
        }
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}

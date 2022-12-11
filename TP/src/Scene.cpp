#include "Scene.h"

#include <TypedBuffer.h>

#include <algorithm>
#include <iostream>
#include <shader_structs.h>
#include "Camera.h"
#include "SceneObject.h"

namespace OM3D {

Scene::Scene() {
}

void Scene::add_object(SceneObject obj, size_t instance) {
    if (instance >= _objects_by_instance.size()) {
        _objects_by_instance.resize(instance + 1);
    }
    _objects_by_instance[instance].emplace_back(std::move(obj));
}

void Scene::add_object(SceneObject obj) {
    _objects.emplace_back(std::move(obj));
}

void Scene::add_object(PointLight obj) {
    _point_lights.emplace_back(std::move(obj));
}

void Scene::show_instances() {
    for (size_t i = 0; i < _objects_by_instance.size(); ++i)
    {
        auto &objects = _objects_by_instance[i];
        std::cout << "Objects " << i << " has " << objects.size() << " instances" << std::endl;
    }
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
    
    // Render every object (old way)

    // for(const SceneObject& obj : _objects) {
    //     obj.material()->set_cull_mode(CullMode::Back);
    //     auto mesh = obj.mesh();
    //     glm::vec4 center = obj.transform() * glm::vec4(mesh->bounding_sphere.center, 1.0f);
    //     center -= glm::vec4(camera.position(), 0.0f);
    //     if (frustum.intersect(glm::vec3(center.x, center.y, center.z), mesh->bounding_sphere.radius)) {
    //         obj.render();
    //     }
    // }
    
    for (size_t i = 0; i < _objects_by_instance.size(); ++i)
    {
        //auto objects = _objects_by_instance[i];
        std::vector<glm::mat4> transforms;
        int count = 0;
        for (auto &obj : _objects_by_instance[i])
        {
            // obj.material()->set_uniform(HASH("model"), obj.transform());
            //auto mesh = obj.mesh();
            glm::vec4 center = obj.transform() * glm::vec4(obj.mesh()->bounding_sphere.center, 1.0f);
            center -= glm::vec4(camera.position(), 0.0f);
            if (frustum.intersect(glm::vec3(center.x, center.y, center.z), obj.mesh()->bounding_sphere.radius)) {
                // do not render, add in a buffer to be rendered later in instance
                // add transform to transform ssbo
                obj.prepare_render(); 
                transforms.push_back(obj.transform());
                count++;
            }
        }
        
        // bind transform ssbo
        // 
        TypedBuffer<glm::mat4> transform_buffer(nullptr, std::max(transforms.size(), size_t(1)));
        {
            auto mapping = transform_buffer.map(AccessType::WriteOnly);
            for(size_t i = 0; i != transforms.size(); ++i) {
                mapping[i] = transforms[i];
            }
        }
        transform_buffer.bind(BufferUsage::Storage, 2);
        
        _objects_by_instance[i][0].render(count);
    }
}

}

#ifndef SCENE_H
#define SCENE_H

#include <SceneObject.h>
#include <PointLight.h>
#include <Camera.h>

#include <vector>
#include <memory>
#include "ParticleSystem.h"

namespace OM3D {

class Scene : NonMovable {

    public:
        Scene();

        static Result<std::unique_ptr<Scene>> from_gltf(const std::string& file_name);

        void render(const Camera& camera) const;
        void render_triangle(const Camera &camera) const;
        // void render_triangle(const Camera &camera, std::shared_ptr<SceneObject> light_sphere) const;
        void render_triangle(const Camera &camera, SceneObject &light_sphere) const;
        void render_particles(const Camera &camera, std::shared_ptr<ParticleSystem> particle_system, const float &dt) const;

        void add_object(SceneObject obj);
        void add_object(PointLight obj);
        
        const std::vector<SceneObject>& objects() const
        {
            return _objects;
        }
        
        SceneObject get_first_object()
        {
            return std::move(_objects[0]);
        }
        
        glm::vec3 sun_direction() const
        {
            return _sun_direction;
        }

    private:
        std::vector<SceneObject> _objects;
        std::vector<PointLight> _point_lights;
        glm::vec3 _sun_direction = glm::vec3(0.2f, 1.0f, 0.1f);
};

}

#endif // SCENE_H

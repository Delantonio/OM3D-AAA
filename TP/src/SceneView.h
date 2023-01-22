#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <Scene.h>
#include <Camera.h>
#include "ParticleSystem.h"
#include "SceneObject.h"

namespace OM3D {

class SceneView {
    public:
        SceneView(const Scene* scene = nullptr);

        Camera& camera();
        const Camera& camera() const;

        void render() const;
        void render_triangle() const;
        // void render_triangle(std::shared_ptr<SceneObject> light_sphere) const;
        void render_lights(SceneObject &light_sphere) const;
        
        void render_particles(const float &delta_time, std::shared_ptr<ParticleSystem> particle_system, const bool &gui_lit) const;

    private:
        const Scene* _scene = nullptr;
        Camera _camera;

};

}

#endif // SCENEVIEW_H

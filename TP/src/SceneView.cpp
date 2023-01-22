#include "SceneView.h"
#include <memory>

namespace OM3D {

SceneView::SceneView(const Scene* scene) : _scene(scene) {
}

Camera& SceneView::camera() {
    return _camera;
}

const Camera& SceneView::camera() const {
    return _camera;
}

void SceneView::render() const {
    if(_scene) {
        _scene->render(_camera);
    }
}

void SceneView::render_triangle() const {
    if(_scene) {
        _scene->render_triangle(_camera);
    }
}
void SceneView::render_lights(SceneObject &light_sphere) const {
    if(_scene) {
        _scene->render_lights(_camera, light_sphere);
    }
}

void SceneView::render_particles(
    const float &delta_time, std::shared_ptr<ParticleSystem> particle_system,
    const bool &gui_lit) const
{
    if(_scene) {
        _scene->render_particles(_camera, particle_system, delta_time, gui_lit);
    }
}

}

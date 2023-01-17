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
// void SceneView::render_triangle(std::shared_ptr<SceneObject> light_sphere) const {
void SceneView::render_triangle(SceneObject &light_sphere) const {
    if(_scene) {
        _scene->render_triangle(_camera, light_sphere);
    }
}

}

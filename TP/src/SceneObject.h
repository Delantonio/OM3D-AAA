#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <StaticMesh.h>
#include <Material.h>

#include <memory>

#include <glm/matrix.hpp>

namespace OM3D {

class SceneObject : NonCopyable {

    public:
        SceneObject(std::shared_ptr<StaticMesh> mesh = nullptr, std::shared_ptr<Material> material = nullptr);

        void prepare_render() const;
        void render() const;
        void render(int count) const;

        void set_transform(const glm::mat4& tr);
        const glm::mat4& transform() const;
        
        std::shared_ptr<StaticMesh> mesh() const
        {
            return _mesh;
        }
        
        std::shared_ptr<Material> material() const
        {
            return _material;
        }
        
        int get_indices_count() const
        {
            std::cout << "salut" << std::endl;
            return _mesh->get_indices_count();
        }

    private:
        glm::mat4 _transform = glm::mat4(1.0f);

        std::shared_ptr<StaticMesh> _mesh;
        std::shared_ptr<Material> _material;
};

}

#endif // SCENEOBJECT_H

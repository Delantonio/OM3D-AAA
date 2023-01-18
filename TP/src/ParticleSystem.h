#pragma once

#include <Particle.h>
#include <vector>
#include "Camera.h"
#include "Material.h"
#include "StaticMesh.h"
#include "TypedBuffer.h"
#include <vector>
#include "glm/ext/matrix_float4x4.hpp"
#include <shader_structs.h>
#include "Program.h"
#include <glad/glad.h>

#include <Vertex.h>

namespace OM3D {
    
    
    /*
        b   c

        a   d
    
    1st triangle a d b
    2nd triangle d b c
    triangles_strip
    */

// const float ParticleVertices[] = {
//     0.0, 0.0, 0.0, 0.0, 0.0, // position , uv
//     1.0, 0.0, 0.0, 1.0, 0.0, // position , uv
//     0.0, 1.0, 0.0, 0.0, 1.0, // position , uv
//     1.0, 1.0, 0.0, 1.0, 1.0, // position , uv
// };


// const MeshData ParticleMesh{ParticleVertex, ParticleIndices};


// const float ParticleVertices[] = {
//     0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, // position, normal, uv
//     1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 1.0, 0.0, // position, normal, uv
//     0.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, // position, normal, uv
//     1.0, 1.0, 0.0, 0.0, 0.0, -1.0, 1.0, 1.0, // position, normal, uv
// };

// const int ParticleIndices[] = {
//     0, 1, 2,
//     1, 2, 3
// };

class ParticleSystem
{
    public:
        ParticleSystem() = default;

        ParticleSystem(std::shared_ptr<Program> program_compute, std::shared_ptr<Material> material, const std::vector<Particle> &particles);

        void update(float dt);
        void render();
        void map_particles();
        
        void bind_render() const;
        void bind_compute() const;

        // void add_particle(const Particle& particle);
        // void add_particles(const std::vector<Particle>& particles);

        void set_transform(const glm::mat4& tr);
        const glm::mat4& transform() const;

    // private:
    public:
        std::shared_ptr<Program> _program_compute;
        std::shared_ptr<Material> _render_material;
        // TypedBuffer<shader::Particle> _particle_buffer_compute;
        TypedBuffer<Vertex> _particle_vertex_buffer;
        TypedBuffer<u32> _particle_index_buffer;
        std::vector<Particle> _particles;
        glm::mat4 _transform = glm::mat4(1.0f);
};

}

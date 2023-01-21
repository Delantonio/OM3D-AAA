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

        // Update the particles with compute shader
        void update(float dt);
        // Render the particles with the render material (instanced rendering)
        void render();
        
        void texture_from_file(const std::string &texture_path);
        
        void set_transform(const glm::mat4& tr);
        const glm::mat4& transform() const;

    // private:
    public:
        std::shared_ptr<Program> _program_compute;
        std::shared_ptr<Material> _render_material;

        // Particle Mesh buffers (vertex and index)
        TypedBuffer<Vertex> _particle_vertex_buffer;
        TypedBuffer<u32> _particle_index_buffer;
        
        // CPU representation of the particles
        std::vector<Particle> _particles;

        // GPU representation of the particles
        TypedBuffer<shader::Particle> _particle_buffer_compute;
        
        std::shared_ptr<Texture> _particle_texture;
        
        glm::mat4 _transform = glm::mat4(1.0f);
};

}

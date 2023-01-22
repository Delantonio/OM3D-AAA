#include "ParticleSystem.h"
#include <cstddef>
#include <memory>
#include "Camera.h"
#include "Material.h"
#include "PointLight.h"
#include "Framebuffer.h"


namespace OM3D {

    ParticleSystem::ParticleSystem(std::shared_ptr<Program> program_compute,
                                   std::shared_ptr<Material> material,
                                   const std::vector<Particle> &particles,
                                   const std::vector<PointLight> &lights)
    {
        _program_compute = std::move(program_compute);
        _render_material = std::move(material);

        _particles = particles;

        // Particle Mesh
        const std::vector<Vertex> ParticleVertex = {
            Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                   glm::vec2(0.0, 0.0)),
            Vertex(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                   glm::vec2(1.0, 0.0)),
            Vertex(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                   glm::vec2(0.0, 1.0)),
            Vertex(glm::vec3(1.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                   glm::vec2(1.0, 1.0)),
        };
        const std::vector<u32> ParticleIndices = { 0, 1, 2, 1, 2, 3 };

        _particle_vertex_buffer = TypedBuffer<Vertex>(ParticleVertex);
        _particle_index_buffer = TypedBuffer<u32>(ParticleIndices);

        _particle_buffer_compute = TypedBuffer<shader::Particle>(
            nullptr, std::max(particles.size(), size_t(1)));

        {
            auto mapping = _particle_buffer_compute.map(AccessType::ReadWrite);
            for (size_t i = 0; i != particles.size(); ++i)
            {
                const auto &particle = particles[i];
                mapping[i].color = particle._color;
                mapping[i].velocity = particle._velocity;
                mapping[i].duration = particle._duration;
                mapping[i].force = particle._force;
                mapping[i].seed = particle._seed;
                mapping[i].center = particle._center;
                mapping[i].luminosity = particle._luminosity;
                mapping[i].origin = particle._origin;
                mapping[i].age = particle._age;
                mapping[i].light.color = particle._light_color;
                mapping[i].light.radius = particle._light_radius;
            }
        }
        // Fill and bind lights buffer
        _particle_buffer_light = TypedBuffer<shader::PointLight>(
            nullptr, std::max(lights.size(), size_t(1)));
        {
            auto mapping = _particle_buffer_light.map(AccessType::ReadWrite);
            for (size_t i = 0; i != lights.size(); ++i)
            {
                const auto &light = lights[i];
                mapping[i] = { light.position(), light.radius(), light.color(),
                               0.0f };
            }
        }
        _particle_texture = nullptr;
}

ParticleSystem::ParticleSystem( glm::vec3 center,
                                glm::vec3 size,
                                float number_of_particles,
                                glm::vec2 window_size)
{
    for (size_t i = 0;  i < number_of_particles; i++)
    {
        Particle p;
        p._color = glm::vec4(.3f, 1.0f, 0.0f, 1.0f);
        p._velocity = glm::vec3(0.0f, 1.0f, 0.0f);
        p._duration = 5.0f + (25.0f * static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        p._force = glm::vec3(0.0f, 0.0f, 0.0f);
        float seed = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        p._seed = seed;
        p._center = center + glm::vec3( size.x * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5),
                               size.y * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5),
                               size.z * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5) );
        p._luminosity = 1.0f;
        p._origin = p._center;
        p._age = p._duration;
        p._light_color = glm::vec3(255.0f, 255.0f, 0.0f);
        p._light_radius = 100.0f;
        _particles.push_back(p);
        
        {
            PointLight light;
            light.set_position(glm::vec3(p._center));
            light.set_color(glm::vec3(255.0f, 255.0f, 0.0f));
            light.set_radius(1.0f);
            //scene->add_object(std::move(light));
            lights.push_back(std::move(light));
        }
    }
    
    
    Texture particles_texture(window_size, ImageFormat::RGBA16_FLOAT);
    Framebuffer particles_framebuffer(nullptr, std::array{&particles_texture});
    
    auto particles_render_program = Program::from_files("particle.frag", "particle.vert");
    _program_compute = Program::from_file("particle.comp");
    auto particles_material_raw = OM3D::Material::empty_material(particles_render_program, {});
    _render_material = std::make_shared<Material>(particles_material_raw);

    // Particle Mesh
    const std::vector<Vertex> ParticleVertex = {
        Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                glm::vec2(0.0, 0.0)),
        Vertex(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                glm::vec2(1.0, 0.0)),
        Vertex(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                glm::vec2(0.0, 1.0)),
        Vertex(glm::vec3(1.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0),
                glm::vec2(1.0, 1.0)),
    };
    const std::vector<u32> ParticleIndices = { 0, 1, 2, 1, 2, 3 };

    _particle_vertex_buffer = TypedBuffer<Vertex>(ParticleVertex);
    _particle_index_buffer = TypedBuffer<u32>(ParticleIndices);

    _particle_buffer_compute = TypedBuffer<shader::Particle>(
        nullptr, std::max(_particles.size(), size_t(1)));

    {
        auto mapping = _particle_buffer_compute.map(AccessType::ReadWrite);
        for (size_t i = 0; i != _particles.size(); ++i)
        {
            const auto &particle = _particles[i];
            mapping[i].color = particle._color;
            mapping[i].velocity = particle._velocity;
            mapping[i].duration = particle._duration;
            mapping[i].force = particle._force;
            mapping[i].seed = particle._seed;
            mapping[i].center = particle._center;
            mapping[i].luminosity = particle._luminosity;
            mapping[i].origin = particle._origin;
            mapping[i].age = particle._age;
        }
    }
    // Fill and bind lights buffer
    _particle_buffer_light = TypedBuffer<shader::PointLight>(
        nullptr, std::max(lights.size(), size_t(1)));
    {
        auto mapping = _particle_buffer_light.map(AccessType::ReadWrite);
        for (size_t i = 0; i != lights.size(); ++i)
        {
            const auto &light = lights[i];
            mapping[i] = { light.position(), light.radius(), light.color(),
                            0.0f };
        }
    }
    _particle_texture = nullptr;
}

void ParticleSystem::texture_from_file(const std::string &texture_path)
{
    Result<TextureData> texture_data = TextureData::from_file(texture_path);
    ALWAYS_ASSERT(texture_data.is_ok, "Unable to load texture from file");

    _particle_texture = std::make_shared<Texture>(std::move(texture_data.value));
    
    _render_material->set_texture(HASH("in_texture"), _particle_texture);
}

void ParticleSystem::update(float dt)
{
    _program_compute->bind();

    _particle_buffer_compute.bind(BufferUsage::Storage, 1);

    _particle_buffer_light.bind(BufferUsage::Storage, 2);

    _program_compute->set_uniform(HASH("dt"), dt);
    glDispatchCompute(align_up_to(_particles.size(), 8) / 8, 1, 1);
}

void ParticleSystem::render()
{
    // _render_material->bind();

    _particle_buffer_compute.bind(BufferUsage::Storage, 1);
    
    _render_material->set_cull_mode(CullMode::None);
    _render_material->set_depth_test_mode(DepthTestMode::Standard);
    _render_material->set_blend_mode(BlendMode::None);

    _particle_vertex_buffer.bind(BufferUsage::Attribute);
    _particle_index_buffer.bind(BufferUsage::Index);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
    // Vertex normal
    // glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(3 * sizeof(float)));
    // Vertex uv
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                          reinterpret_cast<void *>(6 * sizeof(float)));
    // // Tangent / bitangent sign
    // glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(8 * sizeof(float)));
    // // Vertex color
    // glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex),
    //                       reinterpret_cast<void *>(12 * sizeof(float)));

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // glEnableVertexAttribArray(3);
    // glEnableVertexAttribArray(4);

    int instance_count = _particles.size();
    glDrawElementsInstanced(
        GL_TRIANGLE_STRIP,
        int(_particle_index_buffer.element_count()),
        GL_UNSIGNED_INT, nullptr, instance_count);
}

} // namespace OM3D
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include "glm/ext/matrix_float4x4.hpp"

class Particle
{
    public:
        // Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 color, float radius, float age, float lifetime)
        //     : _position(position), _velocity(velocity), _color(color), _radius(radius), _age(age), _lifetime(lifetime)
        // {}
        Particle() = default;

        Particle(glm::mat4 transform, glm::vec3 velocity, glm::vec4 color, float radius, float lifetime, glm::vec3 center)
            : _transform(transform), _velocity(velocity), _color(color), _center(center), _radius(radius), _age(0.0f), _lifetime(lifetime)
        {}

        glm::mat4 _transform;
        // glm::vec3 _position;
        glm::vec3 _velocity;
        glm::vec3 _force;
        glm::vec4 _color;
        glm::vec3  _center;

        float _radius;
        float _age;
        float _lifetime;
};
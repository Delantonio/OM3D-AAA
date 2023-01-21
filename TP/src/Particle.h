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

        // Particle(glm::mat4 transform, glm::vec3 velocity, glm::vec4 color, float radius, float lifetime, glm::vec3 center)
        //     : _transform(transform), _velocity(velocity), _color(color), _center(center), _radius(radius), _age(0.0f), _lifetime(lifetime)
        // {}

        Particle(glm::vec4 color, glm::vec3 velocity,
                 float duration, glm::vec3 force, float seed, glm::vec3 center,
                 float luminosity)
            : _color(color)
            , _velocity(velocity)
            , _duration(duration)
            , _force(force)
            , _seed(seed)
            , _center(center)
            , _luminosity(luminosity)
            , _origin(center)
            , _age(10.0f)
        {}

        glm::vec4 _color;
        glm::vec3 _velocity;
        float _duration;
        glm::vec3 _force;
        float _seed;
        glm::vec3  _center;
        float _luminosity;
        glm::vec3 _origin;
        float _age;
        glm::vec3 _light_color;
        float _light_radius;
};
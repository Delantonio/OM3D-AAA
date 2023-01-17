#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

class Particle
{
    public:
        Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 color, float radius, float age, float lifetime)
            : _position(position), _velocity(velocity), _color(color), _radius(radius), _age(age), _lifetime(lifetime)
        {}

        glm::vec3 _position;
        glm::vec3 _velocity;
        glm::vec3 _force;
        glm::vec4 _color;

        float _radius;
        float _age;
        float _lifetime;
};
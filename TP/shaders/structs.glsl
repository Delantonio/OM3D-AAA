struct CameraData {
    mat4 view_proj;
    mat4 view;
    mat4 proj;
};

struct FrameData {
    CameraData camera;

    vec3 sun_dir;
    uint point_light_count;

    vec3 sun_color;
    float numParticles;
};

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float padding_1;
};

struct Particle {
    mat4 transform;
    vec4 color;
    vec3 velocity;
    float age;
    vec3 force;
    float lifetime;
    vec3 center;
    float luminosity;
};

// struct Particle {
//     vec3 position;
//     float radius;
//     vec3 velocity;
//     float age;
//     vec3 force;
//     float lifetime;
//     vec4 color;
// };

struct ParticleVertex {
    vec3 position;
    vec4 color;
    vec2 uv;
};


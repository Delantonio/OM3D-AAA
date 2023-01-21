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
    vec4 color;
    vec3 velocity;
    float duration;
    vec3 force;
    float seed;
    vec3 center;
    float luminosity;
    vec3 origin;
    float age;
};

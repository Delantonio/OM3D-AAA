#include <cstddef>
#include <glad/glad.h>
#include <memory>
#include "SceneObject.h"
#include "TypedBuffer.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <graphics.h>
#include <SceneView.h>
#include <Texture.h>
#include <Framebuffer.h>
#include <ImGuiRenderer.h>
#include <Material.h>

#include <imgui/imgui.h>


using namespace OM3D;

static float delta_time = 0.0f;
const glm::uvec2 window_size(1600, 900);


void glfw_check(bool cond) {
    if(!cond) {
        const char* err = nullptr;
        glfwGetError(&err);
        std::cerr << "GLFW error: " << err << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void update_delta_time() {
    static double time = 0.0;
    const double new_time = program_time();
    delta_time = float(new_time - time);
    time = new_time;
}

void process_inputs(GLFWwindow* window, Camera& camera) {
    static glm::dvec2 mouse_pos;

    glm::dvec2 new_mouse_pos;
    glfwGetCursorPos(window, &new_mouse_pos.x, &new_mouse_pos.y);

    {
        glm::vec3 movement = {};
        if(glfwGetKey(window, 'W') == GLFW_PRESS) {
            movement += camera.forward();
        }
        if(glfwGetKey(window, 'S') == GLFW_PRESS) {
            movement -= camera.forward();
        }
        if(glfwGetKey(window, 'D') == GLFW_PRESS) {
            movement += camera.right();
        }
        if(glfwGetKey(window, 'A') == GLFW_PRESS) {
            movement -= camera.right();
        }

        float speed = 10.0f;
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed *= 10.0f;
        }

        if(movement.length() > 0.0f) {
            const glm::vec3 new_pos = camera.position() + movement * delta_time * speed;
            camera.set_view(glm::lookAt(new_pos, new_pos + camera.forward(), camera.up()));
        }
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        const glm::vec2 delta = glm::vec2(mouse_pos - new_mouse_pos) * 0.01f;
        if(delta.length() > 0.0f) {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), delta.x, glm::vec3(0.0f, 1.0f, 0.0f));
            rot = glm::rotate(rot, delta.y, camera.right());
            camera.set_view(glm::lookAt(camera.position(), camera.position() + (glm::mat3(rot) * camera.forward()), (glm::mat3(rot) * camera.up())));
        }

    }

    mouse_pos = new_mouse_pos;
}


std::unique_ptr<Scene> create_default_scene() {
    auto scene = std::make_unique<Scene>();

    // Load default cube model
    auto result = Scene::from_gltf(std::string(data_path) + "cube.glb");
    ALWAYS_ASSERT(result.is_ok, "Unable to load default scene");
    scene = std::move(result.value);

    // Add lights
    {
        PointLight light;
        light.set_position(glm::vec3(1.0f, 2.0f, 4.0f));
        light.set_color(glm::vec3(0.0f, 10.0f, 0.0f));
        light.set_radius(100.0f);
        scene->add_object(std::move(light));
    }
    {
        PointLight light;
        light.set_position(glm::vec3(1.0f, 2.0f, -4.0f));
        light.set_color(glm::vec3(10.0f, 0.0f, 0.0f));
        light.set_radius(50.0f);
        scene->add_object(std::move(light));
    }

    return scene;
}

std::shared_ptr<StaticMesh> create_light_sphere() {
    auto scene = std::make_unique<Scene>();

    auto result = Scene::from_gltf(std::string(data_path) + "sphere.glb");
    ALWAYS_ASSERT(result.is_ok, "Unable to load default scene");
    scene = std::move(result.value);

    auto sphere = scene->get_first_object();
    // sphere = std::move(sphere);
    // std::cout << sphere.mesh()->bounding_sphere.radius << std::endl;

    return sphere.mesh();
}

std::unique_ptr<Scene> create_forest_scene() {
    auto scene = std::make_unique<Scene>();

    auto result = Scene::from_gltf(std::string(data_path) + "forest.glb");
    ALWAYS_ASSERT(result.is_ok, "Unable to load default scene");
    scene = std::move(result.value);

    // Add lights
    {
        PointLight light;
        light.set_position(glm::vec3(-13.0f, 6.0f, 2.0f));
        light.set_color(glm::vec3(0.0f, 255.0f, 0.0f));
        light.set_radius(1000.0f);
        scene->add_object(std::move(light));
    }
    {
        PointLight light;
        light.set_position(glm::vec3(50.0f, 75.0f, 150.0f));
        light.set_color(glm::vec3(255.0f, 0.0f, 0.0f));
        light.set_radius(500.0f);
        scene->add_object(std::move(light));
    }

    return scene;
}


int main(int, char**) {
    DEBUG_ASSERT([] { std::cout << "Debug asserts enabled" << std::endl; return true; }());

    glfw_check(glfwInit());
    DEFER(glfwTerminate());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "TP window", nullptr, nullptr);
    glfw_check(window);
    DEFER(glfwDestroyWindow(window));

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    init_graphics();

    ImGuiRenderer imgui(window);

    // std::unique_ptr<Scene> scene = create_default_scene();
    // SceneView scene_view(scene.get());
    

    std::unique_ptr<Scene> scene = create_forest_scene();
    SceneView scene_view(scene.get());
    
    // Set the camera for the forest scene
    glm::mat4 view = glm::lookAt(glm::vec3(50.0f, 75.0f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scene_view.camera().set_view(view);
    
    // glm::mat4 view = glm::lookAt(glm::vec3(-13.0f, 6.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // scene_view.camera().set_view(view);
    

    auto tonemap_program = Program::from_file("tonemap.comp");
    
    auto debug_program = Program::from_files("debug.frag", "screen.vert");
    auto render_sun_program = Program::from_files("render_sun.frag", "screen.vert");
    auto render_light_program = Program::from_files("render_light.frag", "basic.vert");


    // Texture depth(window_size, ImageFormat::Depth32_FLOAT);
    // Texture lit(window_size, ImageFormat::RGBA16_FLOAT);
    Texture color(window_size, ImageFormat::RGBA8_UNORM);
    // Framebuffer main_framebuffer(&depth, std::array{&lit});
    // Framebuffer tonemap_framebuffer(nullptr, std::array{&color});

    // Texture gcolor(window_size, ImageFormat::RGBA8_sRGB);
    // Texture gnormal(window_size, ImageFormat::RGBA8_UNORM);

    auto depth = std::make_shared<Texture>(window_size, ImageFormat::Depth32_FLOAT);
    auto gcolor = std::make_shared<Texture>(window_size, ImageFormat::RGBA8_sRGB);
    auto gnormal = std::make_shared<Texture>(window_size, ImageFormat::RGBA8_UNORM);
    Texture glit(window_size, ImageFormat::RGBA16_FLOAT);
    Framebuffer gbuffer(depth.get(), std::array{gcolor.get(), gnormal.get()});
    // Framebuffer gbuffer(&depth, std::array{&gcolor, &gnormal});
    Framebuffer renderbuffer(nullptr, std::array{&glit});
    Framebuffer tonemap_framebuffer(nullptr, std::array{&color});
    bool gui_albedo = false;
    bool gui_normal = false;
    bool gui_depth = false;
    
    auto debug_color = OM3D::Material::empty_material(debug_program, {gcolor});
    auto debug_normal = OM3D::Material::empty_material(debug_program, {gnormal});
    auto debug_depth = OM3D::Material::empty_material(debug_program, {depth});

    auto sun_material_raw = OM3D::Material::empty_material(render_sun_program, {gcolor, gnormal, depth});
    auto sun_material = std::make_shared<Material>(sun_material_raw);

    auto light_material_raw = OM3D::Material::empty_material(render_light_program, {gcolor, gnormal, depth});
    auto light_material = std::make_shared<Material>(light_material_raw);

    auto sphere_mesh = create_light_sphere();
    // auto sphere_material = std::make_shared<Material>(Material::empty_material(render_program, {}));
    // auto sphere = std::make_shared<SceneObject>(sphere_mesh, sphere_material);
    auto sphere = std::make_shared<SceneObject>(sphere_mesh, light_material);
            
    debug_color.set_cull_mode(CullMode::None);
    debug_normal.set_cull_mode(CullMode::None);
    debug_depth.set_cull_mode(CullMode::None);
    
    sun_material->set_cull_mode(CullMode::None);
    sun_material->set_blend_mode(BlendMode::Alpha);
    
    light_material->set_cull_mode(CullMode::None);
    light_material->set_blend_mode(BlendMode::Additive);
    // render_material.set_blend_mode(BlendMode::Additive);

    for(;;) {
        glfwPollEvents();
        if(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            break;
        }

        update_delta_time();

        if(const auto& io = ImGui::GetIO(); !io.WantCaptureMouse && !io.WantCaptureKeyboard) {
            process_inputs(window, scene_view.camera());
        }
    
        // Render the scene
        {
            // main_framebuffer.bind();
            // scene_view.render();
            
            // bind gbuffer
            gbuffer.bind(true);

            // use gbuffer shader
            scene_view.render();
            
            // unbind gbuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        
        if(gui_albedo)
        {
            renderbuffer.bind();
            debug_color.bind();
            debug_program->set_uniform("depth", (unsigned int)false);
            scene_view.render_triangle();

            tonemap_program->bind();
            glit.bind(0);
            color.bind_as_image(1, AccessType::WriteOnly);
            
            glDispatchCompute(align_up_to(window_size.x, 8), align_up_to(window_size.y, 8), 1);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            tonemap_framebuffer.blit();
        }
        else if(gui_normal)
        {
            debug_normal.bind();
            debug_program->set_uniform("depth", (unsigned int)false);
            scene_view.render_triangle();
        }
        else if(gui_depth)
        {
            debug_depth.bind();
            debug_program->set_uniform("depth", (unsigned int)true);
            scene_view.render_triangle();
        }
        else {
            renderbuffer.bind();
            sun_material->bind();
            scene_view.render_triangle();
            // osef de ce bind material ici, il est fait dans render_lights
            light_material->bind();
            scene_view.render_lights(*sphere.get());

            // Apply a tonemap in compute shader
            tonemap_program->bind();
            glit.bind(0);
            color.bind_as_image(1, AccessType::WriteOnly);
            
            glDispatchCompute(align_up_to(window_size.x, 8) / 8, align_up_to(window_size.y, 8) / 8, 1);

            // Blit tonemap result to screen
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            tonemap_framebuffer.blit();
        }


        // GUI
        imgui.start();
        {
            char buffer[1024] = {};
            if(ImGui::InputText("Load scene", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                auto result = Scene::from_gltf(buffer);
                if(!result.is_ok) {
                    std::cerr << "Unable to load scene (" << buffer << ")" << std::endl;
                } else {
                    scene = std::move(result.value);
                    scene_view = SceneView(scene.get());
                }
            }

            if(ImGui::Checkbox("albedo", &gui_albedo))
            {
                gui_normal = false;
                gui_depth = false;
                //boolean = !boolean;
            }
            if(ImGui::Checkbox("normals", &gui_normal))
            {
                gui_albedo = false;
                gui_depth = false;
                //boolean = !boolean;
            }
            if(ImGui::Checkbox("depth", &gui_depth))
            {
                gui_albedo = false;
                gui_normal = false;
                //boolean = !boolean;
            }
        }
        imgui.finish();

        glfwSwapBuffers(window);
    }

    scene = nullptr; // destroy scene and child OpenGL objects
}

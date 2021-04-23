#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>


#include "src/window.hpp"
#include "src/camera.hpp"
#include "src/components/transform.hpp"

/**
 * TO COMPILE: (directly)
 *      g++ SingleCube.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp ./../../include/src/shading/material.cpp ./../../include/src/components/transform.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o SingleCube
 *
 * WITHOUT EXTRA WARNINGS:
 *      g++ SingleCube.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp ./../../include/src/shading/material.cpp ./../../include/src/components/transform.cpp -Wall -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o SingleCube
 *
 * WITH MAKEFILE: (you must be at root folder)
 *      make targetFile=SingleCube targetFolder=Test-template
*/

const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "out vec3 normal;\n"
    "out vec4 fragPos;"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    // send output to fragment shader in view space
    "   texCoord = aTexCoord;\n"
    "   normal = (view * model * vec4(aNormal, 0.0)).xyz;\n"
    "   fragPos = view * model * vec4(aPos.xyz, 1.0);\n"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec2 texCoord;\n"
    "in vec3 normal;\n"
    "in vec4 fragPos;\n"
    "out vec4 FragColor;\n"
    // textures
    "uniform sampler2D ourTexture;\n"
    // structs
    "struct Light {\n"
        // position should be in view space
    "   vec3 position, color;\n"
    "   vec3 ambient, diffuse, specular;\n"
    "};\n"
    "struct Material {\n"
        "vec3 ambient, diffuse, specular;\n"
        "float shininess;\n"
    "};\n"
    "uniform Light light;\n"
    "uniform Material material;\n"
    "void main() {\n"
    // diffuse compute
    "   vec3 lightDir = normalize(light.position - fragPos.xyz);\n"
    "   float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);\n"
    // specular compute
    "   vec3 viewDir = normalize(vec3(0) - fragPos.xyz);\n"
    "   float specular = pow( clamp( dot( reflect(-lightDir, normal), viewDir), 0.0, 1.0), material.shininess);\n"
    // color
    "   vec3 color;\n"
    "   color = light.ambient * material.ambient;\n"
    "   color += light.diffuse * intensity * material.diffuse * light.color;\n"
    "   color += light.specular * material.specular * light.color * specular;\n"
    "   FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0);\n"
    "}\n";

const std::string SRC_FRAGMENT_LIGHT =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 lightColor;\n"
    "void main() {\n"
    "   FragColor = vec4(lightColor, 1.0);\n"
    "}\n";

struct LightData {
    glm::vec3 ambient, diffuse, specular;
    glm::vec3 position, color;
};

struct CubeData {
    glm::vec3 diffuse, ambient, specular;
    Transform transform;
    float shininess;
};

float getRandom() {
    int r = rand() % 10;
    return (float)r / 10.0f;
}

class SingleCube : public Window {
    public:
        SingleCube(const std::string& wnd, const int& w, const int h) 
            : Window(wnd, w, h), mainCamera(), 
              deltaTime(0), prevTime(0) {
                // init light data
                lightData.position = glm::vec3(0.0f);
                lightData.ambient = glm::vec3(0.5f);
                lightData.diffuse = glm::vec3(1.0f);
                lightData.specular = lightData.color = glm::vec3(1.0f);
                // init random cube's material properties
                cubeData.transform.SetPosition(glm::vec3(0));
                cubeData.transform.SetRotation(glm::vec3(0));
                cubeData.diffuse.r = getRandom();
                cubeData.diffuse.g = getRandom();
                cubeData.diffuse.b = getRandom();
                cubeData.ambient = cubeData.diffuse;
                cubeData.specular = glm::vec3(0.5f);
                cubeData.shininess = 32.0f;
            }

        ~SingleCube() { 
            std::cout << "[Multiple-Cubes::FreesMemory]" << std::endl;
            
            delete cube;
            delete lightSource;
        }

        void CreateLight(float* vertices, const u_long& vsize, const u_long& esize) {
            Material s(SRC_VERTEX, SRC_FRAGMENT_LIGHT);

            lightSource = new Mesh(vertices, vsize, esize);
            lightSource->Use();
            // position attribute
            lightSource->SetAttribute(0, 3, GL_FLOAT, false, esize, 0);
            // normals
            lightSource->SetAttribute(1, 3, GL_FLOAT, false, esize, 3 * sizeof(float));
            // uv's attribute
            lightSource->SetAttribute(2, 2, GL_FLOAT, false, esize, 6 * sizeof(float));
                
            s.Use();
            lightSource->SetMaterial(s);
        }
        
        void UpdateMouseLook() {
            // jaw
            if (Input::PressedLEFT())
                mainCamera.Rotate(glm::vec3(0, -360 * deltaTime * sensitivity, 0));
            if (Input::PressedRIGHT())
                mainCamera.Rotate(glm::vec3(0, 360 * deltaTime * sensitivity, 0));
            // pitch
            if (Input::PressedUP())
                mainCamera.Rotate(glm::vec3(360 * deltaTime * sensitivity, 0, 0));
            if (Input::PressedDOWN())
                mainCamera.Rotate(glm::vec3(-360 * deltaTime * sensitivity, 0, 0));
        }

        void HandleInput() {
            if (Input::PressedW())
                // move forward (since camera faces towards negative z)
                mainCamera.Move(mainCamera.GetForward() * deltaTime);
            if (Input::PressedS())
                // move backwards (camera faces towards negative z)
                mainCamera.Move(-mainCamera.GetForward() * deltaTime);
            if (Input::PressedA())
                // move left
                mainCamera.Move(-mainCamera.GetRight() * deltaTime);
            if (Input::PressedD())
                // move right
                mainCamera.Move(mainCamera.GetRight() * deltaTime);

            if (Input::PressedESC())
                Close();

            UpdateMouseLook();
        }

        /**
         * Excercise is to create basic phong lighting in view space
        */
        void OnMainLoopInit() {
            float rect[] = {
                // vertices       //normals      // uv's
                // front face
                0.5f, 0.5f, 0,    0, 0, 1.0f,    1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, 0,   0, 0, 1.0f,    0.0f, 1.0f,  
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f,  
                -0.5f, -0.5f, 0,  0, 0, 1.0f,    0.0f, 0.0f,  // triangle
                -0.5, 0.5f, 0,    0, 0, 1.0f,    0.0f, 1.0f,
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f,
                // back face
                0.5f, 0.5f, -1,   0, 0, -1.0f,   1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 0, -1.0f,   0.0f, 1.0f,  
                0.5f, -0.5f, -1,  0, 0, -1.0f,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1, 0, 0, -1.0f,   0.0f, 0.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 0, -1.0f,   0.0f, 1.0f,
                0.5f, -0.5f, -1,  0, 0, -1.0f,   1.0f, 0.0f,
                // right face
                0.5f, 0.5f, 0,    1.0f, 0, 0,   0.0f, 1.0f,  // triangle
                0.5f, 0.5f, -1,   1.0f, 0, 0,   1.0f, 1.0f,  
                0.5f, -0.5f, 0,   1.0f, 0, 0,   0.0f, 0.0f,  
                0.5f, -0.5f, -1,  1.0f, 0, 0,   1.0f, 0.0f,  // triangle
                0.5f, 0.5f, -1,   1.0f, 0, 0,   1.0f, 1.0f,  
                0.5f, -0.5f, 0,   1.0f, 0, 0,   0.0f, 0.0f,
                // left face
                -0.5f, 0.5f, 0,   -1.0f, 0, 0,   1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  -1.0f, 0, 0,   0.0f, 1.0f,  
                -0.5f, -0.5f, 0,  -1.0f, 0, 0,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1, -1.0f, 0, 0,   0.0f, 0.0f,  // triangle
                -0.5f, -0.5f, 0,  -1.0f, 0, 0,   1.0f, 0.0f,
                -0.5f, 0.5f, -1,  -1.0f, 0, 0,   0.0f, 1.0f,
                // top face
                -0.5f, 0.5f, 0,   0, 1.0f, 0,   0.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 1.0f, 0,   1.0f, 1.0f,  
                0.5f, 0.5f, 0,    0, 1.0f, 0,   0.0f, 0.0f,  
                0.5f, 0.5f, -1,   0, 1.0f, 0,   1.0f, 0.0f,  // triangle
                0.5f, 0.5f, 0,    0, 1.0f, 0,   0.0f, 0.0f,
                -0.5f, 0.5f, -1,  0, 1.0f, 0,   1.0f, 1.0f,
                // bottom face
                -0.5f, -0.5f, 0,   0, -1.0f, 0,   1.0f, 1.0f,  // triangle
                -0.5f, -0.5f, -1,  0, -1.0f, 0,   0.0f, 1.0f,  
                0.5f, -0.5f, 0,    0, -1.0f, 0,   1.0f, 0.0f,  
                0.5f, -0.5f, -1,   0, -1.0f, 0,   0.0f, 0.0f,  // triangle
                0.5f, -0.5f, 0,    0, -1.0f, 0,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1,  0, -1.0f, 0,   0.0f, 1.0f
            };
            const u_long componentsSize = 8 * sizeof(float);
            Texture mainTex(OLD_MAN_IMG, Texture::TexFormat::RGB, Texture::PixelFormat::RGB);
            Material material(SRC_VERTEX, SRC_FRAGMENT);
            
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            cube = new Mesh(rect, sizeof(rect), componentsSize);

            // position attribute
            cube->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
            // normals
            cube->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
            // uv's attribute
            cube->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
            
            cube->Use();
            // to bind with current VAO
            mainTex.Use();
            material.Use();
            glActiveTexture(GL_TEXTURE0);
            material.SetUniform<int>("ourTexture", 0);
            
            // put here to make sure VAO's been binded
            cube->SetTexture(mainTex);
            cube->SetMaterial(material);
            
            CreateLight(rect, sizeof(rect), componentsSize);
        }

        void OnRenderLight(const glm::mat4& view, const glm::mat4& proj) {
            glm::mat4 model(1.0f);
            /*
            float t = (float)glfwGetTime();
            lightData.position.z = sin(t) * LIGHT_RADIUS_ROT;
            lightData.position.x = cos(t) * LIGHT_RADIUS_ROT;
            lightData.position.y = 0.0f;
            // color
            lightData.color.r = sin(t * 2.0f);
            lightData.color.g = sin(t * 0.7f);
            lightData.color.b = sin(t * 1.3f);
            */
            // only as white color
            lightData.position.x = lightData.position.y = lightData.position.z = 1;
            lightData.color.r = lightData.color.g = lightData.color.b = 1;

            lightData.ambient = lightData.color * AMBIENT_FACTOR;

            model = glm::translate(model, lightData.position);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            // send pos to cubes shader in view space
            lightData.position = 
                view * model * glm::vec4(lightData.position, 1.0f);

            // for light's shader            
            lightSource->Render();
            lightSource->SetUniform<glm::mat4x4>("model", model);
            lightSource->SetUniform<glm::mat4x4>("view", view);
            lightSource->SetUniform<glm::mat4x4>("projection", proj);
            lightSource->SetUniform<glm::vec3>("lightColor", lightData.color);
            lightSource->Draw();
        }

        void OnRender() {
            // transformations
            glm::mat4 model;
            glm::mat4 view = mainCamera.GetViewMatrix();
            float angle; 
            glm::mat4 proj = 
                glm::perspective(45.0, (double)GetAspectRatio(), 0.1, 100.0);
            
            OnRenderLight(view, proj);
            
            /*
            //angle = glm::radians(20.0f);
            model = glm::translate(glm::mat4(1.0f), cubeData.transform.Position());
            model = glm::rotate(model, angle, glm::vec3(1, 0.3f, 0.5f));
            */
            angle = (float)glfwGetTime();
            cubeData.transform.SetRotation(glm::vec3(0.0f, 0.0f, angle));
            model = cubeData.transform.ModelMatrix();
            cube->Render();
            // update uniforms
            cube->SetUniform<glm::mat4x4>("view", view);
            cube->SetUniform<glm::mat4x4>("projection", proj);
            cube->SetUniform<glm::mat4x4>("model", model);
            cube->SetUniform<glm::vec3>("material.diffuse", cubeData.diffuse);
            cube->SetUniform<glm::vec3>("material.ambient", cubeData.ambient);
            cube->SetUniform<glm::vec3>("material.specular", cubeData.specular);
            cube->SetUniform<float>("material.shininess", cubeData.shininess);
            
            cube->SetUniform<glm::vec3>("light.position", lightData.position);
            cube->SetUniform<glm::vec3>("light.diffuse", lightData.diffuse);
            cube->SetUniform<glm::vec3>("light.ambient", lightData.ambient);
            cube->SetUniform<glm::vec3>("light.specular", lightData.specular);
            cube->SetUniform<glm::vec3>("light.color", lightData.color);
            
            cube->Draw();

            // update delta time
            float currTime = (float)glfwGetTime();
            deltaTime = currTime - prevTime;
            prevTime = currTime;
        }
    
    private:
        LightData lightData;
        CubeData cubeData;
        Camera mainCamera;
        Mesh* cube;
        Mesh* lightSource;
        float deltaTime, prevTime;

        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const float LIGHT_RADIUS_ROT = 2, LIGHT_SPEED = 1, AMBIENT_FACTOR = 0.25f;
        // NOTE: this path is relative to the console's location!!!
        const std::string OLD_MAN_IMG = "assets/oldman_meme.jpg";
};

int main() {
    SingleCube wnd("LearnOpenGL", 800, 600);
    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "meshes.h" // 添加对网格头文件的引用

// Window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    worldPos = vec3(model * vec4(aPos, 1.0));
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
in vec3 worldPos;

out vec4 FragColor;

// 用于控制颜色渐变的参数
uniform float maxDistance;
uniform vec3 startColor;
uniform vec3 endColor;
uniform bool useGradient;

void main()
{
    if (useGradient) {
        // 基于世界坐标计算距离，用于颜色渐变
        float distZ = abs(worldPos.z);
        float distX = abs(worldPos.x);
        float dist = max(distZ, distX);
        
        // 计算颜色渐变
        float t = clamp(dist / maxDistance, 0.0, 1.0);
        vec3 color = mix(startColor, endColor, t);
        
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = vec4(ourColor, 1.0);
    }
}
)";

// Camera parameters
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstPersonView = false;  // 是否使用第一人称视角
bool keyTPressed = false;      // 跟踪T键状态，用于防止重复触发

// Car parameters
glm::vec3 carPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float carRotation = 0.0f;
float carSpeed = 0.0f;
float carMaxSpeed = 37.5f;         // 再提高5倍 (从7.5f增加到37.5f)
float carAcceleration = 0.75f;     // 再提高5倍 (从0.15f增加到0.75f)
float carDeceleration = 0.2f;      // 相应调整减速度
float carTurnSpeed = 25.0f;        // 增加转向速度 (从15.0f增加到25.0f)
float carLength = 4.0f;            // 车辆长度，用于计算转向
float wheelAngle = 0.0f;           // 前轮转向角度
float maxWheelAngle = 35.0f;       // 最大前轮转向角度

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Input state
bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;

// Input processing
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Update key states
    keyW = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    keyA = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    keyS = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    keyD = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    
    // 切换视角（按T键）
    bool keyTCurrentState = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    if (keyTCurrentState && !keyTPressed)
    {
        firstPersonView = !firstPersonView;
    }
    keyTPressed = keyTCurrentState;
}

// Update car position and rotation based on input
void updateCar()
{
    // Forward/Backward
    if (keyW)
    {
        // Accelerate forward
        carSpeed += carAcceleration * deltaTime;
        if (carSpeed > carMaxSpeed)
            carSpeed = carMaxSpeed;
    }
    else if (keyS)
    {
        // Accelerate backward
        carSpeed -= carAcceleration * deltaTime;
        if (carSpeed < -carMaxSpeed)
            carSpeed = -carMaxSpeed;
    }
    else
    {
        // Decelerate if no input
        if (carSpeed > 0)
        {
            carSpeed -= carDeceleration * deltaTime;
            if (carSpeed < 0)
                carSpeed = 0;
        }
        else if (carSpeed < 0)
        {
            carSpeed += carDeceleration * deltaTime;
            if (carSpeed > 0)
                carSpeed = 0;
        }
    }

    // 计算实际速度
    float speedFactor = fabs(carSpeed) * 100.0f;
    float direction = (carSpeed > 0) ? 1.0f : -1.0f;
    
    // 更新前轮转向角度
    if (keyA && fabs(carSpeed) > 0.001f)
    {
        wheelAngle += 120.0f * deltaTime;
        if (wheelAngle > maxWheelAngle)
            wheelAngle = maxWheelAngle;
    }
    else if (keyD && fabs(carSpeed) > 0.001f)
    {
        wheelAngle -= 120.0f * deltaTime;
        if (wheelAngle < -maxWheelAngle)
            wheelAngle = -maxWheelAngle;
    }
    else
    {
        // 前轮回正
        if (wheelAngle > 0)
        {
            wheelAngle -= 90.0f * deltaTime;
            if (wheelAngle < 0)
                wheelAngle = 0;
        }
        else if (wheelAngle < 0)
        {
            wheelAngle += 90.0f * deltaTime;
            if (wheelAngle > 0)
                wheelAngle = 0;
        }
    }
    
    // 模拟前轮驱动转向 - 前轮改变方向，后轮作为转向轴心
    if (fabs(carSpeed) > 0.001f) // 只有在移动时才转向
    {
        float steeringAngle = 0.0f;
        
        // 计算转向角度 - 基于前轮转向角度
        steeringAngle = (wheelAngle / maxWheelAngle) * carTurnSpeed * deltaTime * direction;
            
        if (steeringAngle != 0.0f)
        {
            // 更新车辆旋转角度
            carRotation += steeringAngle;
            
            // 计算车辆当前朝向的单位向量
            float radians = glm::radians(carRotation);
            glm::vec3 carDirection(sin(radians), 0.0f, cos(radians));
            
            // 前轮驱动模式 - 车辆位置向前轮方向移动
            carPosition += direction * speedFactor * carDirection * deltaTime;
            
            // 模拟车辆漂移效果 - 速度越快，漂移越明显
            if (fabs(steeringAngle) > 0.1f && speedFactor > 200.0f)
            {
                // 计算横向漂移分量 (与车辆方向垂直)
                glm::vec3 driftDirection(-carDirection.z, 0.0f, carDirection.x);
                float driftAmount = steeringAngle * 0.01f * speedFactor;
                carPosition += driftDirection * driftAmount * deltaTime;
            }
        }
        else
        {
            // 直线行驶
            float radians = glm::radians(carRotation);
            carPosition.x += direction * speedFactor * sin(radians) * deltaTime;
            carPosition.z += direction * speedFactor * cos(radians) * deltaTime;
        }
    }
}

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Driving Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Compile shaders
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Check vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation error: " << infoLog << std::endl;
    }
    
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation error: " << infoLog << std::endl;
    }
    
    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
    }
    
    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Create car meshes
    std::vector<CubeMesh> carMeshes = createCar();
    
    // 创建地面网格 - 简单的平面
    CubeMesh groundMesh = createCube(20.0f, 0.1f, 20.0f, glm::vec3(0.2f, 0.8f, 0.2f));
    
    // 创建蜿蜒的路障
    BarrierConfig barrierConfig;
    barrierConfig.segmentLength = 5.0f;      // 每个路障段的长度
    barrierConfig.spacing = 2.0f;            // 路障段之间的间隔
    barrierConfig.width = 0.3f;              // 路障宽度
    barrierConfig.height = 0.8f;             // 路障高度
    barrierConfig.depth = 1.0f;              // 路障深度
    barrierConfig.roadWidth = 10.5f;         // 道路宽度增加到1.5倍(原来是7.0f)
    barrierConfig.segmentsPerTurn = 10;      // 每10段后拐弯(原来是5)，使直行更远
    barrierConfig.turnAngle = 5.0f;         // 拐弯角度改为30度(原来是90度)，使拐弯更平缓
    barrierConfig.startColor = glm::vec3(1.0f, 0.3f, 0.0f);  // 起始颜色（橙色）
    barrierConfig.endColor = glm::vec3(0.0f, 0.3f, 1.0f);    // 结束颜色（蓝色）
    
    CubeMesh barriersMesh = createBarriers(barrierConfig, 200);  // 生成200段路障(增加到200段，使路径更长)
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process input
        processInput(window);
        
        // Update car position and rotation
        updateCar();
        
        // Render
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader program
        glUseProgram(shaderProgram);
        
        // Create transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        
        // 计算车辆当前方向向量
        glm::vec3 carDirectionVector(sin(glm::radians(carRotation)), 0.0f, cos(glm::radians(carRotation)));
        
        // 根据视角模式设置相机位置
        if (firstPersonView)
        {
            // 第一人称视角（司机位置）
            // 将相机放在车内，稍微高一点的位置，并朝向车头方向
            cameraPos = carPosition + glm::vec3(0.0f, 1.3f, 0.0f); // 1.3是司机眼睛高度
            cameraFront = carDirectionVector;
        }
        else
        {
            // 第三人称跟随视角（原来的视角）
            cameraPos = carPosition - carDirectionVector * 10.0f + glm::vec3(0.0f, 5.0f, 0.0f);
            cameraFront = glm::normalize(carPosition - cameraPos + glm::vec3(0.0f, 0.5f, 0.0f));
        }
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // Set transformation matrices in shader
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        
        // 设置颜色渐变参数
        int useGradientLoc = glGetUniformLocation(shaderProgram, "useGradient");
        int maxDistanceLoc = glGetUniformLocation(shaderProgram, "maxDistance");
        int startColorLoc = glGetUniformLocation(shaderProgram, "startColor");
        int endColorLoc = glGetUniformLocation(shaderProgram, "endColor");
        
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        // 绘制地面
        glUniform1i(useGradientLoc, 0); // 不使用渐变颜色
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -2.0f, 0.0f)); // 稍微往下移一点，避免z-fighting
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(groundModel));
        glBindVertexArray(groundMesh.VAO);
        glDrawElements(GL_TRIANGLES, groundMesh.indexCount, GL_UNSIGNED_INT, 0);
        
        // 绘制路障（使用颜色渐变）
        glUniform1i(useGradientLoc, 1); // 使用渐变颜色
        float maxDistance = barrierConfig.segmentLength * barrierConfig.segmentsPerTurn * 20.0f; // 适应200段路障的距离
        glUniform1f(maxDistanceLoc, maxDistance);
        glUniform3f(startColorLoc, barrierConfig.startColor.r, barrierConfig.startColor.g, barrierConfig.startColor.b);
        glUniform3f(endColorLoc, barrierConfig.endColor.r, barrierConfig.endColor.g, barrierConfig.endColor.b);
        
        glm::mat4 barrierModel = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(barrierModel));
        glBindVertexArray(barriersMesh.VAO);
        glDrawElements(GL_TRIANGLES, barriersMesh.indexCount, GL_UNSIGNED_INT, 0);
        
        // 为汽车部件关闭渐变颜色
        glUniform1i(useGradientLoc, 0);
        
        // Draw car parts
        for (size_t i = 0; i < carMeshes.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, carPosition);
            model = glm::rotate(model, glm::radians(carRotation), glm::vec3(0.0f, 1.0f, 0.0f));
            
            // Apply additional transformations based on part index
            switch(i) {
                case 0: break; // Main body
                case 1: // Top
                    model = glm::translate(model, glm::vec3(0.0f, 0.75f, -0.5f));
                    break;
                case 2: // Front-left wheel
                    model = glm::translate(model, glm::vec3(-1.1f, -0.2f, 1.2f));
                    model = glm::rotate(model, glm::radians(wheelAngle), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case 3: // Front-right wheel
                    model = glm::translate(model, glm::vec3(1.1f, -0.2f, 1.2f));
                    model = glm::rotate(model, glm::radians(wheelAngle), glm::vec3(0.0f, 1.0f, 0.0f));
                    break;
                case 4: // Rear-left wheel
                    model = glm::translate(model, glm::vec3(-1.1f, -0.2f, -1.2f));
                    break;
                case 5: // Rear-right wheel
                    model = glm::translate(model, glm::vec3(1.1f, -0.2f, -1.2f));
                    break;
            }
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(carMeshes[i].VAO);
            glDrawElements(GL_TRIANGLES, carMeshes[i].indexCount, GL_UNSIGNED_INT, 0);
        }
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up resources
    for (auto& mesh : carMeshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    
    // 清理地面网格资源
    glDeleteVertexArrays(1, &groundMesh.VAO);
    glDeleteBuffers(1, &groundMesh.VBO);
    glDeleteBuffers(1, &groundMesh.EBO);
    
    // 清理路障资源
    glDeleteVertexArrays(1, &barriersMesh.VAO);
    glDeleteBuffers(1, &barriersMesh.VBO);
    glDeleteBuffers(1, &barriersMesh.EBO);
    
    glDeleteProgram(shaderProgram);
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
} 

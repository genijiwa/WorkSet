#include "meshes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <random>

// 车轮的转向角度 - 需要从main.cpp中访问
extern float wheelAngle;

// 创建立方体网格函数
CubeMesh createCube(float width, float height, float depth, glm::vec3 color)
{
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    float halfDepth = depth * 0.5f;
    
    // 定义8个顶点
    std::vector<float> vertices = {
        // 位置                // 颜色
        -halfWidth, -halfHeight, -halfDepth,  color.r, color.g, color.b,  // 0 前左下
         halfWidth, -halfHeight, -halfDepth,  color.r, color.g, color.b,  // 1 前右下
         halfWidth,  halfHeight, -halfDepth,  color.r, color.g, color.b,  // 2 前右上
        -halfWidth,  halfHeight, -halfDepth,  color.r, color.g, color.b,  // 3 前左上
        -halfWidth, -halfHeight,  halfDepth,  color.r, color.g, color.b,  // 4 后左下
         halfWidth, -halfHeight,  halfDepth,  color.r, color.g, color.b,  // 5 后右下
         halfWidth,  halfHeight,  halfDepth,  color.r, color.g, color.b,  // 6 后右上
        -halfWidth,  halfHeight,  halfDepth,  color.r, color.g, color.b   // 7 后左上
    };
    
    // 定义索引
    std::vector<unsigned int> indices = {
        // 前面
        0, 1, 2,
        2, 3, 0,
        // 后面
        4, 5, 6,
        6, 7, 4,
        // 左面
        0, 3, 7,
        7, 4, 0,
        // 右面
        1, 2, 6,
        6, 5, 1,
        // 上面
        3, 2, 6,
        6, 7, 3,
        // 下面
        0, 1, 5,
        5, 4, 0
    };
    
    CubeMesh mesh;
    mesh.indexCount = indices.size();

    // 生成并绑定VAO、VBO和EBO
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    // 绑定并填充VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // 绑定并填充EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return mesh;
}

// 创建汽车所有部分
std::vector<CubeMesh> createCar()
{
    std::vector<CubeMesh> carMeshes;
    
    // Car body (main cube)
    carMeshes.push_back(createCube(2.0f, 0.8f, 4.0f, glm::vec3(0.2f, 0.2f, 0.8f)));
    
    // Car top (smaller cube)
    carMeshes.push_back(createCube(1.5f, 0.7f, 2.0f, glm::vec3(0.2f, 0.2f, 0.9f)));
    
    // Wheels
    glm::vec3 wheelColor(0.2f, 0.2f, 0.2f);
    float wheelWidth = 0.4f;
    float wheelRadius = 0.5f;
    
    // Front wheels
    carMeshes.push_back(createCube(wheelWidth, wheelRadius*2, wheelRadius*2, wheelColor)); // Front-left
    carMeshes.push_back(createCube(wheelWidth, wheelRadius*2, wheelRadius*2, wheelColor)); // Front-right
    
    // Rear wheels
    carMeshes.push_back(createCube(wheelWidth, wheelRadius*2, wheelRadius*2, wheelColor)); // Rear-left
    carMeshes.push_back(createCube(wheelWidth, wheelRadius*2, wheelRadius*2, wheelColor)); // Rear-right
    
    return carMeshes;
}

// 创建路障
std::vector<CubeMesh> createRoadblock(float x, float z)
{
    std::vector<CubeMesh> meshes;
    
    // 创建路障的底座
    CubeMesh baseMesh = createCube(1.0f, 0.5f, 0.3f, glm::vec3(1.0f, 0.3f, 0.0f)); // 橙色底座
    meshes.push_back(baseMesh);
    
    // 添加条纹
    CubeMesh stripeMesh = createCube(1.1f, 0.1f, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f)); // 白色条纹
    meshes.push_back(stripeMesh);
    
    return meshes;
}

// 创建简单的建筑物
std::vector<CubeMesh> createBuilding(float height, glm::vec3 color)
{
    std::vector<CubeMesh> meshes;
    
    // 创建主体
    CubeMesh bodyMesh = createCube(5.0f, height, 5.0f, color);
    meshes.push_back(bodyMesh);
    
    // 为一些建筑添加屋顶
    if (height < 15.0f) // 只给矮建筑添加屋顶
    {
        CubeMesh roofMesh = createCube(5.5f, 1.0f, 5.5f, glm::vec3(0.3f, 0.3f, 0.3f)); // 深灰色屋顶
        meshes.push_back(roofMesh);
    }
    
    return meshes;
}

// 创建蜿蜒的路障
CubeMesh createBarriers(const BarrierConfig& config, int totalSegments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    float halfWidth = config.width * 0.5f;
    float halfHeight = config.height * 0.5f;
    float halfDepth = config.depth * 0.5f;
    float halfRoadWidth = config.roadWidth * 0.5f;
    
    // 基本路障立方体的顶点（相对于原点）
    std::vector<float> baseVertices = {
        // 位置 (x, y, z)
        -halfWidth, -halfHeight, -halfDepth,
         halfWidth, -halfHeight, -halfDepth,
         halfWidth,  halfHeight, -halfDepth,
        -halfWidth,  halfHeight, -halfDepth,
        -halfWidth, -halfHeight,  halfDepth,
         halfWidth, -halfHeight,  halfDepth,
         halfWidth,  halfHeight,  halfDepth,
        -halfWidth,  halfHeight,  halfDepth
    };
    
    // 基本立方体的索引（一个立方体有12个三角形，36个索引点）
    std::vector<unsigned int> baseCubeIndices = {
        // 前面
        0, 1, 2, 2, 3, 0,
        // 后面
        4, 5, 6, 6, 7, 4,
        // 左面
        0, 3, 7, 7, 4, 0,
        // 右面
        1, 2, 6, 6, 5, 1,
        // 上面
        3, 2, 6, 6, 7, 3,
        // 下面
        0, 1, 5, 5, 4, 0
    };
    
    // 跟踪当前路段的方向和位置
    glm::vec3 direction(0.0f, 0.0f, 1.0f); // 初始方向是Z轴正向
    glm::vec3 right(1.0f, 0.0f, 0.0f);     // 右向量，用于确定路障的左右位置
    glm::vec3 position(0.0f, 0.0f, 0.0f);  // 当前位置
    
    int vertexOffset = 0;
    bool turnRight = false; // 用于交替左右拐弯

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int segment = 0; segment < totalSegments; segment++)
    {
        // 每segmentsPerTurn段后改变方向
        if (segment > 0 && segment % config.segmentsPerTurn == 0)
        {
            // 创建旋转矩阵
            std::uniform_real_distribution<float> angle_distribution(config.turnAngle,config.turnAngle + 30.0f);
            float random_angle = angle_distribution(gen);

            float angle = glm::radians(random_angle);
            
            // 交替左右拐弯，使路径呈蜿蜒状
            if (turnRight) {
                angle = -angle; // 向右拐弯
            }

            std::uniform_int_distribution<int> turn_distribution(0, 1);
            int random_turn = turn_distribution(gen);

            turnRight = random_turn==0? turnRight : !turnRight; // 切换下一次的拐弯方向
            
            glm::mat3 rotationMatrix = glm::mat3(
                glm::cos(angle), 0.0f, glm::sin(angle),
                0.0f, 1.0f, 0.0f,
                -glm::sin(angle), 0.0f, glm::cos(angle)
            );
            
            // 旋转方向和右向量
            direction = rotationMatrix * direction;
            right = rotationMatrix * right;
        }
        
        // 计算左右路障的位置
        glm::vec3 leftPosition = position - right * halfRoadWidth;
        glm::vec3 rightPosition = position + right * halfRoadWidth;
        
        // 添加左路障
        for (size_t i = 0; i < 8; i++)
        {
            // 获取基本顶点
            float x = baseVertices[i * 3];
            float y = baseVertices[i * 3 + 1];
            float z = baseVertices[i * 3 + 2];
            
            // 计算世界坐标中的位置
            glm::vec3 worldPos = leftPosition + right * x + glm::vec3(0.0f, 1.0f, 0.0f) * y + direction * z;
            
            // 添加顶点位置
            vertices.push_back(worldPos.x);
            vertices.push_back(worldPos.y);
            vertices.push_back(worldPos.z);
            
            // 添加颜色 (使用默认颜色，颜色渐变会在着色器中处理)
            vertices.push_back(1.0f);
            vertices.push_back(0.3f);
            vertices.push_back(0.0f);
        }
        
        // 添加右路障
        for (size_t i = 0; i < 8; i++)
        {
            // 获取基本顶点
            float x = baseVertices[i * 3];
            float y = baseVertices[i * 3 + 1];
            float z = baseVertices[i * 3 + 2];
            
            // 计算世界坐标中的位置
            glm::vec3 worldPos = rightPosition + right * x + glm::vec3(0.0f, 1.0f, 0.0f) * y + direction * z;
            
            // 添加顶点位置
            vertices.push_back(worldPos.x);
            vertices.push_back(worldPos.y);
            vertices.push_back(worldPos.z);
            
            // 添加颜色 (使用默认颜色，颜色渐变会在着色器中处理)
            vertices.push_back(1.0f);
            vertices.push_back(0.3f);
            vertices.push_back(0.0f);
        }
        
        // 为左路障添加索引
        for (size_t i = 0; i < baseCubeIndices.size(); i++)
        {
            indices.push_back(baseCubeIndices[i] + vertexOffset);
        }
        
        // 为右路障添加索引
        for (size_t i = 0; i < baseCubeIndices.size(); i++)
        {
            indices.push_back(baseCubeIndices[i] + vertexOffset + 8);
        }
        
        // 更新顶点偏移
        vertexOffset += 16; // 8个顶点 * 2个路障
        
        // 沿方向移动到下一段
        position += direction * (config.segmentLength + config.spacing);
    }
    
    // 创建mesh
    CubeMesh mesh;
    mesh.indexCount = indices.size();
    
    // 生成并绑定VAO、VBO和EBO
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    // 绑定并填充VBO
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // 绑定并填充EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return mesh;
} 

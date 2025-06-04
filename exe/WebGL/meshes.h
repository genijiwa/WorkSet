#ifndef MESHES_H
#define MESHES_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// 立方体网格结构体
struct CubeMesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    int indexCount;
};

// 路障配置结构体
struct BarrierConfig {
    float segmentLength;      // 每段路障的长度
    float spacing;            // 路障之间的间距
    float width;              // 路障的宽度
    float height;             // 路障的高度
    float depth;              // 路障的深度
    float roadWidth;          // 道路宽度（左右路障之间的距离）
    int segmentsPerTurn;      // 每多少段后拐弯
    float turnAngle;          // 拐弯的角度
    glm::vec3 startColor;     // 起始颜色
    glm::vec3 endColor;       // 结束颜色
};

// 创建立方体网格
CubeMesh createCube(float width, float height, float depth, glm::vec3 color);

// 创建汽车的所有部分
std::vector<CubeMesh> createCar();

// 创建路障
std::vector<CubeMesh> createRoadblock(float x, float z);

// 创建建筑物
std::vector<CubeMesh> createBuilding(float height, glm::vec3 color);

// 创建蜿蜒的路障
CubeMesh createBarriers(const BarrierConfig& config, int totalSegments);

#endif // MESHES_H 
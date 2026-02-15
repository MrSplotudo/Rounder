#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "../third_party/tinyobjloader/tiny_obj_loader.h"
#include "../engine/vulkan_vertex.h"
#include <vector>
#include <string>
#include <iostream>

void loadOBJ(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        std::cout << "Failed to load OBJ: " << filepath << std::endl;
        return;
    }

    for (const auto& index : shapes[0].mesh.indices) {
        Vertex vertex;

        vertex.position = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]
        };

        vertex.UV = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            attrib.texcoords[2 * index.texcoord_index + 1]
        };

        vertices.push_back(vertex);
        indices.push_back(indices.size());
    }

    std::cout << "Loaded " << filepath << ": " << vertices.size() << " vertices" << std::endl;
}
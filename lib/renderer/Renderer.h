#pragma once

#include "unordered_map"
#include "array"
#include "memory"
#include "list"

#include "VertexObject.h"
#include "renderer/Shader.h"

#define ASSERT(x) if (!(x)) __builtin_trap();

// Clears all existing OpenGL error.
// Executest the given function and check for errors.
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

namespace renderer {
    struct SizeStruct
    {
        // First Array pointer, second size
        int Verticies;
        int Indices;
    };

    void Init();
    void Draw(const float width, const float height, 
            const SizeStruct sizes, const Vertex *vertecies, const unsigned int *indicies);

    class Renderer 
    {
        public:
            static std::unordered_map<char, VertexObject*> map;
            static Shader *shader;
            static unsigned int vertexArray; 
            static unsigned int indicies;

            static void clear();
            static void parseObjects(Vertex* vertecies, unsigned int* indicies);
            static SizeStruct calcCount();
    };
};

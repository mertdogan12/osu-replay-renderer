#pragma once

#include "Texture.h"

#include "string"
#include "memory"

#include "glm/glm.hpp"

namespace renderer {
    struct Vertex 
    {
        glm::vec2 Coords;
        glm::vec4 Rgba;
        glm::vec2 TexCoords;
        float TextureIndex;
    };

    class VertexObject
    {
        public:
            VertexObject(const float x, const float y, 
                    const float scaleX, const float scaleY, const std::string& texturePath);

            Vertex Vertexs[4];

            static const float DEFAULT;

            Texture VertexTexture;
            void BindTexture(unsigned int slot = 0);
            void ChangeCoords(float x = -1.0f, float y = -1.0f, 
                    const float scaleX = -1.0f, const float scaleY = -1.0f);

            float posX, posY;
            float posScaleX, posScaleY;

        private:
            void setTextureIndex(float index);
    };
}

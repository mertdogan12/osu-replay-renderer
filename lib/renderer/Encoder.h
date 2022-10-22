#pragma once

#include "string"
#include "GL/glew.h"

namespace renderer 
{
    class Encoder 
    {
        // TODO char data --> GLbyte* data
        public: 
            Encoder(std::string filePath, const unsigned int width, const unsigned int height);
            void Write(char data, int size);

        private:
           FILE* ffmpeg; 
    };
}

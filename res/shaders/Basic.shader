#shader vertex
#version 400 core
  
layout(location = 1) in vec2 position;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCord;
layout(location = 4) in float texIndex;

out vec2 v_TexCoords;
out vec4 v_Color;
out float v_TexIndex;

uniform mat4 u_MVP;
 
void main()
{
    gl_Position = u_MVP * vec4(position, 1.0, 1.0);
    v_TexCoords = texCord;
    v_Color = color;
    v_TexIndex = texIndex;
};


#shader fragment
#version 400 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Textures[32];
                               
void main()
{
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoords);
    color = texColor;
};

#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;

out vec2 v_TextureCoord;

void main()
{
   gl_Position = position;
   v_TextureCoord = textureCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextureCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_Zoom;
uniform vec2 u_PanOffset;

void main()
{
   vec2 centered = v_TextureCoord - 0.5;
   vec2 zoomed = centered / u_Zoom;
   vec2 finalCoord = zoomed +0.5 + u_PanOffset;

   vec4 textureColor = texture(u_Texture, finalCoord);
   color = textureColor;
}
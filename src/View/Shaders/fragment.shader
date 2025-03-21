#version 410 core
in vec4 vertexColor;
in float vertexSize;

out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
}
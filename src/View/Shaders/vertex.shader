#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 Matrix_Projection;
uniform mat4 Matrix_View;
uniform mat4 Matrix_Model;

void main()
{
    gl_Position = Matrix_Projection * Matrix_View * Matrix_Model * vec4(aPos, 1.0);
}

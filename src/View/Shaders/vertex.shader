#version 410 core
layout (location = 0) in vec3 aPos;    //position
layout (location = 1) in vec4 aColor;  //color
layout (location = 2) in float aSize;  //size

//matrices
uniform mat4 Matrix_Projection;
uniform mat4 Matrix_View;
uniform mat4 Matrix_Model;

//send to fragment shader
out vec4 vertexColor; 
out float vertexSize; 

void main()
{
    //matrix stack
    gl_Position = Matrix_Projection * Matrix_View * Matrix_Model * vec4(aPos, 1.0);
    
    //send to fragment shader
    vertexColor = aColor;
    vertexSize = aSize;
    
    //set point size
    gl_PointSize = aSize;
}

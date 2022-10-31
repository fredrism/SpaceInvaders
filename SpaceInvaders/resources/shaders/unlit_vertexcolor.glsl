#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 vCol;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vCol = aCol;
}

#fragment
#version 450 core
out vec4 FragColor;
in vec3 vCol;

void main()
{
	FragColor = vec4(vCol, 1);
}

#end
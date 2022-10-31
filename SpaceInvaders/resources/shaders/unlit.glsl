#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 u_transform;

void main()
{
	gl_Position = u_transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#fragment
#version 450 core
out vec4 FragColor;
uniform vec4 u_color;

void main()
{
	FragColor = u_color;
}

#end
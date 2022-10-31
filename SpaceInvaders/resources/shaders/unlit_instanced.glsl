#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4[100] u_transforms;
flat out int instanceID;

void main()
{
	gl_Position = u_transforms[gl_InstanceID] * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	instanceID = gl_InstanceID;
}

#fragment
#version 450 core
out vec4 FragColor;

flat in int instanceID;
uniform vec4[100] u_colors;

void main()
{
	FragColor = u_colors[instanceID];
}

#end
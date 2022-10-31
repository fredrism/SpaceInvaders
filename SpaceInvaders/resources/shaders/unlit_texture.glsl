#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 u_transform;

out vec2 v_texcoord;

void main()
{
	gl_Position = u_transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_texcoord = aUV;
}

#fragment
#version 450 core
out vec4 FragColor;

in vec2 v_texcoord;

uniform sampler2D u_texture;
uniform vec4 u_tint;

void main()
{
	FragColor = texture(u_texture, v_texcoord) * u_tint;
    //FragColor = vec4(v_texcoord.x, v_texcoord.y, 0, 1);
}

#end
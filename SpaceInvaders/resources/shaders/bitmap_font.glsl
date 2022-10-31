#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 u_transform;
uniform vec2[64] u_offset;
uniform vec2 u_scale;

out vec2 v_texcoord;

void main()
{
    vec2 offset = u_offset[gl_InstanceID];
	gl_Position = u_transform * vec4(aPos.x + gl_InstanceID, aPos.y, aPos.z, 1.0);
    v_texcoord = vec2(aUV.x*u_scale.x, aUV.y*u_scale.y) + offset;
}

#fragment
#version 450 core
out vec4 FragColor;

in vec2 v_texcoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
    vec4 val = texture(u_texture, v_texcoord);

    if(val.x > 0.5)
    {
        discard;
        return;
    }

	FragColor = u_color;
}

#end
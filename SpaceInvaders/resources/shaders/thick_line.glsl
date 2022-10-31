#geometry
#version 450 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 v_texcoord[];
out vec2 v_uv;

uniform float u_width;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;

    vec2 dir = normalize(p1 - p0).xy;
    vec2 offset = vec2(-dir.y, dir.x) * u_width;

    gl_Position = p0 + vec4(offset.xy * p0.w, -1, 0);
    v_uv = vec2(1, v_texcoord[0].y);
    EmitVertex();
    gl_Position = p0 - vec4(offset.xy * p0.w, -1, 0);
    v_uv = vec2(0, v_texcoord[0].y);
    EmitVertex();
    gl_Position = p1 + vec4(offset.xy * p1.w, -1, 0);
    v_uv = vec2(1, v_texcoord[1].y);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, -1, 0);
    v_uv = vec2(0, v_texcoord[1].y);
    EmitVertex();
}

#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_texcoord = aCol.xy;
}

#fragment
#version 450 core
out vec4 FragColor;
uniform vec4 u_color;
uniform sampler2D u_texture;

in vec2 v_uv;

void main()
{
	FragColor = texture(u_texture, v_uv) * u_color;
}

#end
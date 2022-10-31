#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_texcoord = aUV;
}

#fragment
#version 450 core
out vec4 FragColor;
  
in vec2 v_texcoord;

uniform sampler2D a;
uniform sampler2D b;
uniform float strength;

void main()
{             
    vec4 aCol = texture(a, v_texcoord);
    vec4 bCol = texture(b, v_texcoord);
    FragColor = aCol + bCol * 0.5;
}

#end
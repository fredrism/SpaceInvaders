#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
    v_texcoord = aUV;
}

#fragment
#version 450 core
out vec4 FragColor;
  
in vec2 v_texcoord;

uniform sampler2D image;
  
uniform int u_horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, v_texcoord).rgb * weight[0]; // current fragment's contribution
    if(u_horizontal == 1)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, v_texcoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, v_texcoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, v_texcoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, v_texcoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
#end
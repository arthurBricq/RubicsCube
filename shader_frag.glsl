#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

// texture samplers
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D textureNone;

// 

void main()
{
	float face = TexCoord.z;

	if (face == 0.0f) 
		FragColor = texture(texture0, TexCoord.xy);
	else if (face == 1.0f) 
		FragColor = texture(texture1, TexCoord.xy);
	else if (face == 2.0f) 
		FragColor = texture(texture2, TexCoord.xy);
	else if (face == 3.0f) 
		FragColor = texture(texture3, TexCoord.xy);
	else if (face == 4.0f) 
		FragColor = texture(texture4, TexCoord.xy);
	else if (face == 5.0f) 
		FragColor = texture(texture5, TexCoord.xy);
	else
		FragColor = texture(textureNone, TexCoord.xy);
}


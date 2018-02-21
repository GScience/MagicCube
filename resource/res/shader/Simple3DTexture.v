#version 330  

uniform mat4 perspective;

layout(location = 0) in  vec3 in_position;
layout(location = 1) in  vec3 in_color;
layout(location = 2) in  vec2 in_texturePos;

out vec3 color;
out vec2 texturePos;

void  main()
{
	color 		= in_color;
	texturePos  = in_texturePos;
	
	gl_Position = perspective * vec4(in_position, 1.0);
}
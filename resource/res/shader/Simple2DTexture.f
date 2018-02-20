#version 330

uniform sampler2D texture;
in vec2 textureCoord;
out vec4 color;

void main()
{
	color = texture2D(texture, textureCoord);
	if (color.a == 0.0f)
		discard;
	//color = vec4(textureCoord,0.0,1.0);
}
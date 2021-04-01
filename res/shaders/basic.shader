#shader vertex
#version 460 core

layout(location = 0) in vec2 aPosition;

uniform vec2 uSize;
uniform vec2 uPosition;

void main()
{
	vec2 position = (aPosition * uSize) + uPosition;
	gl_Position = vec4(position, 0.0, 1.0);
};


#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};
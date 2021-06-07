#shader vertex
#version 400 core

layout(location = 0) in vec2 aPosition;

uniform vec2 uSize;
uniform vec2 uPosition;
uniform float uRotate;

mat2 rotate2d(float _angle) {
	return mat2(cos(_angle), -sin(_angle),
		sin(_angle), cos(_angle));
}

void main()
{
	vec2 position = (aPosition * rotate2d(uRotate) * uSize) + uPosition;
	gl_Position = vec4(position, 0.0, 1.0);
};


#shader fragment
#version 400 core

layout(location = 0) out vec4 color;

uniform vec4 uColor;

void main()
{
	color = uColor;
};
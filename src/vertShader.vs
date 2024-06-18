#version 330

in vec3 xPos;

void main()
{
	gl_Position = vec4(xPos, 1.0);
}

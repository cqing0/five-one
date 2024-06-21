#version 330 core
out vec4 FragColor;
in vec3 InColor;

void main()
{
	FragColor = vec4(InColor, 1.0);
}

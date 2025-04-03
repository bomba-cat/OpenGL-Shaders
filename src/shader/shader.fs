#version 330 core

out vec4 FragColor;

uniform int frame;

void main()
{
  FragColor = vec4(float(frame % 255) / 255.0, 0.0, 0.0, 1.0);
  //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#version 330 core

layout(location = 0) in vec3 aPos;

uniform int frame;

void main()
{
  vec4 newPos = vec4(aPos, 1.0);
  newPos.y += float(frame % 55) / 255;
  newPos.x += float(frame % 255) / 255;
  gl_Position = newPos;
}

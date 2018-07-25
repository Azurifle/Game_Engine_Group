uniform mat4 MVP;
attribute vec3 vPos;
attribute vec3 vCol;
attribute vec2 vTex;
varying vec3 color;
varying vec2 texCoord;

void main()
{
  gl_Position = MVP * vec4(vPos, 1.0);
  color = vCol;
  texCoord = vTex;
}
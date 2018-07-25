uniform sampler2D texture;
varying vec3 color;
varying vec2 texCoord;

void main()
{
  gl_FragColor = texture2D( texture, texCoord);
}
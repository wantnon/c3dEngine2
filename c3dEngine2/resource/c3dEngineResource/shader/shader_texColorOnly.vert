
attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

//varying
varying vec2 v_texCoord;

void main(void) {
    gl_Position = projMat*viewMat*modelMat*a_position;
    v_texCoord = a_texCoord;
}
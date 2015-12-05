//-------------------------------------

//-------------------------------------
//attribute
attribute vec4 a_position;
attribute vec2 a_texCoord;

//uniform
uniform mat4 projectionModelview;

//varying
varying vec2 texCoordOut;

void main(void) {
    gl_Position = projectionModelview* a_position;
    texCoordOut = a_texCoord;
}
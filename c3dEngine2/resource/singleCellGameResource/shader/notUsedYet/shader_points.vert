
//-------------------------------------

//-------------------------------------
//attribute
attribute vec4 a_position;
attribute vec2 a_texCoord;

//uniform
uniform mat4 projectionModelview;
uniform float pointSize;

//varying
varying vec2 texCoordOut;

void main(void) {
    gl_PointSize=pointSize;//gl_PointSize is float, see: http://www.opengl.org/sdk/docs/manglsl/xhtml/gl_PointSize.xml
    gl_Position = projectionModelview* a_position;
    texCoordOut = a_texCoord;
}
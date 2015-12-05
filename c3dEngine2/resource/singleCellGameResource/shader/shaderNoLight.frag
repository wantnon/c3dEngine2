//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying mediump vec2 texCoordOut;
uniform lowp vec4 color;
uniform sampler2D Texture;
void main(void) {
    gl_FragColor = color*texture2D(Texture, texCoordOut); 
}
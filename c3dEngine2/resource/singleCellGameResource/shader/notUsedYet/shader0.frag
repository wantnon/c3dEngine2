//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying lowp vec4 mainColor;
varying lowp vec4 secondaryColor;
varying mediump vec2 texCoordOut;
uniform sampler2D Texture; 

void main(void) {
    gl_FragColor = mainColor * texture2D(Texture, texCoordOut)+secondaryColor; 
}
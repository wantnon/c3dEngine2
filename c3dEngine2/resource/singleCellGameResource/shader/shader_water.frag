//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
uniform lowp float maxAlpha;

void main(void) {
    lowp vec3 texColor=vec3(texture2D(Texture, texCoordOut));
 
    gl_FragColor = vec4(texColor,maxAlpha);
}

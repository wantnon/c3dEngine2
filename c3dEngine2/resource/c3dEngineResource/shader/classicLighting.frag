#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25


varying lowp vec4 v_mainColor;
varying lowp vec4 v_secondaryColor;
varying mediump vec2 v_texCoord;

uniform sampler2D texture;

void main(void) {
    gl_FragColor = v_mainColor * texture2D(texture, v_texCoord)+v_secondaryColor;
}

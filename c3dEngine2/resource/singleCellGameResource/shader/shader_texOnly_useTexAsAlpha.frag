#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
void main(void) {
    lowp float alpha=texture2D(Texture,texCoordOut).r;
    gl_FragColor =vec4(1.0,1.0,1.0,alpha);
    
}
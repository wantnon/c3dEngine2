#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying vec2 TexCoord;
void main(void) {
	lowp vec4 color = vec4(1.0,0.0,0.0,1.0);
    gl_FragColor = color; 
}
//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying mediump vec2 texCoordOut;
uniform sampler2D Texture;
void main() 
{ 
lowp float depth=texture2D(Texture,texCoordOut).r; 
gl_FragColor=vec4(vec3(pow(depth,100.0)),1.0);
} 
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25


varying mediump vec4 v_reflectDir_world;
uniform samplerCube texture;

void main(void) {
    gl_FragColor =textureCube(texture, v_reflectDir_world.xyz);
	//gl_FragColor =vec4(v_reflectDir_world.xyz,1.0);
}

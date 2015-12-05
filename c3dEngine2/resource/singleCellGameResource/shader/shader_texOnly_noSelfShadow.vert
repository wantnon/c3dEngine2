//-------------------------------------

//-------------------------------------
//attribute
attribute vec4 a_position;
attribute vec2 a_texCoord;

//uniform
uniform mat4 projectionModelview;
uniform mat4 modelMat;
uniform mat4 worldToLightViewportTexCoord;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;

//varying
varying vec2 texCoordOut;
varying vec4 lightViewportTexCoordDivW;

void main(void) {
    //----get pos in world space
    vec3 pos_world = vec3(modelMat*a_position);
    //----set varying

    gl_Position = projectionModelview* a_position;
    texCoordOut = a_texCoord;
    vec4 t=worldToLightViewportTexCoord*vec4(pos_world,1);//note: use pos_world
    
    lightViewportTexCoordDivW=t/t.w;
}
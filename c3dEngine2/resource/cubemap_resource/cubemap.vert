
attribute vec4 a_position;

attribute vec4 a_normal;

attribute vec4 a_color;

attribute vec2 a_texCoord;





uniform mat4 modelMat;

uniform mat4 viewMat;

uniform mat4 projMat;

uniform mat4 normMat;

uniform vec4 lightPos_world;

uniform vec4 eyePos_world;

varying vec4 v_reflectDir_world;



void main(void){


    //----get normal in world space

    vec4 norm_world = normMat*a_normal;

    //----get pos in world space

    vec4 pos_world = modelMat*a_position;

	

   

    //----set varying


    gl_Position = projMat*viewMat* pos_world;

    vec4 eyeDir_world = normalize(pos_world - eyePos_world);

    // Reflect eye direction over normal 

    v_reflectDir_world =  reflect(eyeDir_world, norm_world);


}
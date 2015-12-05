//-------------------------------------

//-------------------------------------
//attribute
attribute  vec4 a_position;

//uniform
uniform mat4 projectionModelview;


void main(void) {
    gl_Position = projectionModelview* a_position;
}
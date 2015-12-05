//-------------------------------------

//-------------------------------------
//attribute
attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texCoord;

//uniform
uniform mat4 projectionModelview;
uniform mat4 worldToLightViewportTexCoord;
uniform mat4 worldToLightViewportTexCoord2;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;
uniform vec4 diffuseML;//diffuseML=vec4(vec3(diffuse_material)*vec3(diffuse_light),diffuse_material.a)
uniform vec4 ambientML;//ambientML=vec4(vec3(ambient_material)*vec3(ambient_light),0)

//varying
varying vec4 mainColor;
varying vec2 texCoordOut;
varying vec3 lightViewportTexCoordDivW;
varying vec3 lightViewportTexCoordDivW2;

void main(void) {
    //use a_normal as world space value
    //use a_position as world space value
    //----cal diffuse color
    vec3 posToLight=normalize(lightPos_world-vec3(a_position));
    float normDotPosToLight = max(0.0, dot(vec3(a_normal), posToLight));
    vec3 diffuseColor= normDotPosToLight*vec3(diffuseML);
    //----cal ambient color
    //use vec3(ambientML) directly
    //----set varying
    //the final alpha is equal to diffuseML.a(and is equal to diffuse_material.a)
    mainColor = vec4(vec3(ambientML)+diffuseColor,diffuseML.a);
    gl_Position = projectionModelview* a_position;
    texCoordOut = a_texCoord;
    vec4 t=worldToLightViewportTexCoord*a_position;
    lightViewportTexCoordDivW=vec3(t)/t.w;
    vec4 t2=worldToLightViewportTexCoord2*a_position;
    lightViewportTexCoordDivW2=vec3(t2)/t2.w;
}

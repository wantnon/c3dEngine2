//-------------------------------------

//-------------------------------------
//attribute
attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texCoord;


//uniform
uniform mat4 modelMat;
uniform mat4 projectionModelview;
uniform mat4 normMat_toWorld;

//vectors and scalars pass from outside
uniform vec3 lightPos_world;
uniform vec3 eyePos_world;
uniform vec4 diffuseML;//diffuseML=vec4(vec3(diffuse_material)*vec3(diffuse_light),diffuse_material.a)
uniform vec4 ambientML;//ambientML=vec4(vec3(ambient_material)*vec3(ambient_light),0)
uniform vec4 specularML;//specularML=vec4(vec3(specular_material)*vec3(specular_light),0)
uniform float shininess;
uniform bool isHighlightUntransp;

//varying
varying vec4 mainColor;
varying vec4 secondaryColor;
varying vec2 texCoordOut; 


void main(void) {
    //----get normal in world space
    //suppose no scaling, then we can spare normalize
    vec3 norm_world = vec3(normMat_toWorld*a_normal);
    //----get pos in world space
    vec3 pos_world = vec3(modelMat*a_position);
    //lightPos already in world space
    //----cal diffuse color
    vec3 posToLight=normalize(lightPos_world-pos_world);
    float normDotPosToLight = max(0.0, dot(norm_world, posToLight));
    vec3 diffuseColor= normDotPosToLight*vec3(diffuseML);
    //----cal ambient color
    //vec3(ambientML);
    //----cal specular color
    vec3 posToEye=normalize(eyePos_world-pos_world);
    vec3 halfVector=(posToLight+posToEye)*0.5;//or /2.0, not /2
    float normDotHalfVector=max(0.0,dot(norm_world, halfVector));
    float pf=(normDotHalfVector==0.0?0.0:pow(normDotHalfVector,shininess));
    vec3 specularColor= pf*vec3(specularML);
    //----set varying
    //the final alpha is equal to diffuseML.a(and is equal to diffuse_material.a)
    mainColor = vec4(vec3(ambientML)+diffuseColor,diffuseML.a);
    float secondaryColorAlpha=isHighlightUntransp?pf:0.0;
    secondaryColor=vec4(specularColor,secondaryColorAlpha);
    gl_Position = projectionModelview* a_position;
    texCoordOut = a_texCoord;
}
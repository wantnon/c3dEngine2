//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying lowp vec4 mainColor;
varying mediump vec2 texCoordOut;
varying highp vec3 lightViewportTexCoordDivW;
varying highp vec3 lightViewportTexCoordDivW2;

uniform sampler2D Texture;
uniform sampler2D Texture_shadowMap;
uniform sampler2D Texture_shadowMap2;

void main(void) {
    lowp vec4 texColor=texture2D(Texture, texCoordOut);
    lowp float shadowFactor;
    highp float zLookup=texture2D(Texture_shadowMap,lightViewportTexCoordDivW.st).z;
    highp float zLookup2=texture2D(Texture_shadowMap2,lightViewportTexCoordDivW2.st).z;
    //shadow on ground no need gradual change, without gradual change is better
    if(lightViewportTexCoordDivW2.z>zLookup2){
        shadowFactor=0.55;
    }else{
        shadowFactor=(lightViewportTexCoordDivW.z>zLookup?0.65:1.0);
    }
    lowp vec4 shadowFactorRGBA=vec4(shadowFactor,shadowFactor,shadowFactor,1.0);
    gl_FragColor = shadowFactorRGBA*mainColor*texColor;
}
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
uniform lowp vec4 color;

void main(void) {
    highp float zLookup=texture2D(Texture_shadowMap,lightViewportTexCoordDivW.st).z;
    highp float zLookup2=texture2D(Texture_shadowMap2,lightViewportTexCoordDivW2.st).z;
    highp float dz=lightViewportTexCoordDivW.z-zLookup;
    highp float dz2=lightViewportTexCoordDivW2.z-zLookup2;
    lowp float shadowFactor;
    if(dz2>0.0){
        shadowFactor=0.5;
    }else{
        shadowFactor=max(0.6,1.0-max(0.0,dz)*100.0);// the shadow of scene have gradual change, makes more real and natural
    }
    lowp vec4 shadowFactorRGBA=vec4(shadowFactor,shadowFactor,shadowFactor,1.0);
    
    gl_FragColor =color*shadowFactorRGBA*mainColor *texture2D(Texture, texCoordOut);
    
}

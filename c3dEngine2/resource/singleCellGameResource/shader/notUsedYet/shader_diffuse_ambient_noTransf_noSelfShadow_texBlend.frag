//-------------------------------------

//-------------------------------------
#version 100 //see: http://www.ogre3d.org/forums/viewtopic.php?f=1&t=56121&start=25
varying lowp vec4 mainColor;
varying mediump vec2 texCoordOut;
varying mediump vec2 texCoordOut2;
varying highp vec3 lightViewportTexCoordDivW;
varying highp vec3 lightViewportTexCoordDivW2;

uniform sampler2D Texture_alpha;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture_shadowMap;
uniform sampler2D Texture_shadowMap2;

void main(void) {
    //----texBlend
    lowp vec4 k=texture2D(Texture_alpha,texCoordOut2);
    if(k.a<0.9){
        discard;//discard is too slow for iOS !
        return;
    }
    //suppose texture1 at bottom, texture2 middle, texture3 top
    lowp vec4 tex1Color=texture2D(Texture1,texCoordOut);
    lowp vec4 tex2Color=texture2D(Texture2,texCoordOut);
    lowp vec4 tex3Color=texture2D(Texture3,texCoordOut);
    lowp vec4 texBlendColor=vec4(vec3(tex1Color*k.r+tex2Color*k.g+tex3Color*k.b),1.0);//guarantee alpha is 1.0
    //----shadow
    lowp float shadowFactor;
    highp float zLookup=texture2D(Texture_shadowMap,lightViewportTexCoordDivW.st).z;
    highp float zLookup2=texture2D(Texture_shadowMap2,lightViewportTexCoordDivW2.st).z;
    if(lightViewportTexCoordDivW2.z>zLookup2){
        shadowFactor=0.55;
    }else{
        shadowFactor=(lightViewportTexCoordDivW.z>zLookup?0.65:1.0);
    }
    lowp vec4 shadowFactorRGBA=vec4(shadowFactor,shadowFactor,shadowFactor,1.0);
    //----all together
    gl_FragColor = shadowFactorRGBA*mainColor*texBlendColor;
}
varying vec2 TexCoord;
void main() 
{ 
gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;
TexCoord=gl_MultiTexCoord0.st;

} 

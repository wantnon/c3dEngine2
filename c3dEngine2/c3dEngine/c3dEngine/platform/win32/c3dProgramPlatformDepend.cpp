//
//  c3dProgramPlatformDepend.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-3-9.
//
//

#include "platform/win32/c3dProgramPlatformDepend.h"
#include "core/c3dAssert.h"
#include "core/c3dFileUtils.h"
char *textFileRead(char *fn) {

	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
			
		}
		//fclose(fp);
	}
	return content;
}

GLuint createShader_plat(const char*shaderName,const char*ext)
{
	//judage shader type: vertex shader or fragment shader
	GLuint shaderHandle=0;
	string postFix=ext;//后缀--abc
	if(postFix == "vert"){//是顶点shader 
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
	}else if(postFix == "frag"){
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	}else{
		C3DASSERT(false);
	}
	string fileName=string(shaderName)+"."+ext;
	string filePath=Cc3dFileUtils::sharedFileUtils()->getFullPath(fileName.c_str());
	const char*content = textFileRead((char*)filePath.c_str());//文件内容--abc
	glShaderSource(shaderHandle, 1, &content,NULL);
	free((char*)content);
	glCompileShader(shaderHandle);
	return shaderHandle;
}

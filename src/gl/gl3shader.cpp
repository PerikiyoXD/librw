#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../rwbase.h"
#include "../rwerror.h"
#include "../rwplg.h"
#include "../rwpipeline.h"
#include "../rwobjects.h"
#include "../rwengine.h"
#ifdef RW_OPENGL
#include <GL/glew.h>
#include "rwgl3.h"
#include "rwgl3shader.h"

namespace rw {
namespace gl3 {

UniformRegistry uniformRegistry;

int
registerUniform(const char *name)
{
	int i;
	i = findUniform(name);
	if(i >= 0) return i;
	uniformRegistry.uniformNames[uniformRegistry.numUniforms] = strdup(name);
	return uniformRegistry.numUniforms++;
}

int
findUniform(const char *name)
{
	int i;
	for(i = 0; i < uniformRegistry.numUniforms; i++)
		if(strcmp(name, uniformRegistry.uniformNames[i]) == 0)
			return i;
	return -1;
}

int
registerBlock(const char *name)
{
	int i;
	i = findBlock(name);
	if(i >= 0) return i;
	uniformRegistry.blockNames[uniformRegistry.numBlocks] = strdup(name);
	return uniformRegistry.numBlocks++;
}

int
findBlock(const char *name)
{
	int i;
	for(i = 0; i < uniformRegistry.numBlocks; i++)
		if(strcmp(name, uniformRegistry.blockNames[i]) == 0)
			return i;
	return -1;
}

Shader *currentShader;

// TODO: maybe make this public somewhere?
static char*
loadfile(const char *path)
{
	FILE *f;
	char *buf;
	long len;

	if(f = fopen(path, "rb"), f == nil){
		fprintf(stderr, "Couldn't open file %s\n", path);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	buf = (char*)malloc(len+1);
	rewind(f);
	fread(buf, 1, len, f);
	buf[len] = '\0';
	fclose(f);
	return buf;
}

static int
compileshader(GLenum type, const char *src, GLuint *shader)
{
	GLint shdr, success;
	GLint len;
	char *log;

	shdr = glCreateShader(type);
	glShaderSource(shdr, 1, &src, nil);
	glCompileShader(shdr);
	glGetShaderiv(shdr, GL_COMPILE_STATUS, &success);
	if(!success){
		fprintf(stderr, "Error in %s shader\n",
		  type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		glGetShaderiv(shdr, GL_INFO_LOG_LENGTH, &len);
		log = (char*)malloc(len);
		glGetShaderInfoLog(shdr, len, nil, log);
		fprintf(stderr, "%s\n", log);
		free(log);
		return 1;
	}
	*shader = shdr;
	return 0;
}

static int
linkprogram(GLint vs, GLint fs, GLuint *program)
{
	GLint prog, success;
	GLint len;
	char *log;

	prog = glCreateProgram();

	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if(!success){
		fprintf(stderr, "Error in program\n");
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, nil, log);
		fprintf(stderr, "%s\n", log);
		free(log);
		return 1;
	}
	*program = prog;
	return 0;
}

Shader*
Shader::fromStrings(const char *vsrc, const char *fsrc)
{
	GLuint vs, fs, program;
	int i;
	int fail;

	fail = compileshader(GL_VERTEX_SHADER, vsrc, &vs);
	if(fail)
		return nil;

	fail = compileshader(GL_FRAGMENT_SHADER, fsrc, &fs);
	if(fail)
		return nil;

	fail = linkprogram(vs, fs, &program);
	if(fail)
		return nil;
	glDeleteProgram(vs);
	glDeleteProgram(fs);

	Shader *sh = new Shader;

	// set uniform block binding
	for(i = 0; i < uniformRegistry.numBlocks; i++){
		int idx = glGetUniformBlockIndex(program,
		                                 uniformRegistry.blockNames[i]);
		if(idx >= 0)
			glUniformBlockBinding(program, idx, i);
	}

	// query uniform locations
	sh->program = program;
	sh->uniformLocations = new GLint[uniformRegistry.numUniforms];
	for(i = 0; i < uniformRegistry.numUniforms; i++)
		sh->uniformLocations[i] = glGetUniformLocation(program,
			uniformRegistry.uniformNames[i]);

	return sh;
}

Shader*
Shader::fromFiles(const char *vspath, const char *fspath)
{
	char *vsrc, *fsrc;
	vsrc = loadfile(vspath);
	fsrc = loadfile(fspath);
	Shader *s = fromStrings(vsrc, fsrc);
	free(vsrc);
	free(fsrc);
	return s;
}

void
Shader::use(void)
{
	glUseProgram(this->program);
	currentShader = this;
}

}
}

#endif

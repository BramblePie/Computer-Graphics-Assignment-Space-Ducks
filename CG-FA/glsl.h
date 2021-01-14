#pragma once

#include <GL/glew.h>
#include <fstream>

namespace glsl
{
	inline static char* readFile(const char* filename)
	{
		// Open the file
		FILE* fp;
		errno_t err;
		err = fopen_s(&fp, filename, "r");
		if (err == 0 && fp != 0)
		{
			// Move the file pointer to the end of the file and determing the length
			fseek(fp, 0, SEEK_END);
			long file_length = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			const size_t fsize = (size_t)file_length;
			char* contents = new char[fsize + 1];
			// zero out memory
			for (size_t i = 0; i < fsize + 1; i++)
			{
				contents[i] = 0;
			}
			// Here's the actual read
			fread(contents, 1, fsize, fp);
			// This is how you denote the end of a string in C
			contents[fsize] = '\0';
			fclose(fp);
			return contents;
		}
		else
			return 0;
	}

	inline static bool compiledStatus(GLint shaderID)
	{
		GLint compiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
		if (compiled)
		{
			return true;
		}
		else
		{
			GLint logLength;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
			char* msgBuffer = new char[logLength];
			glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
			printf("%s\n", msgBuffer);
			delete[] msgBuffer;
			return false;
		}
	}

	inline static GLuint makeVertexShader(const char* shaderSource)
	{
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, (const GLchar**)&shaderSource, NULL);
		glCompileShader(vertexShaderID);
		bool compiledCorrectly = compiledStatus(vertexShaderID);
		if (compiledCorrectly)
		{
			return vertexShaderID;
		}
		return static_cast<GLuint>(-1);
	}

	inline static GLuint makeFragmentShader(const char* shaderSource)
	{
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSource, NULL);
		glCompileShader(fragmentShaderID);
		//delete[] source;
		bool compiledCorrectly = compiledStatus(fragmentShaderID);
		if (compiledCorrectly)
		{
			return fragmentShaderID;
		}
		return static_cast<GLuint>(-1);
	}

	inline static GLuint makeShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
	{
		GLuint shaderID = glCreateProgram();
		glAttachShader(shaderID, vertexShaderID);
		glAttachShader(shaderID, fragmentShaderID);
		glLinkProgram(shaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return shaderID;
	}
}

/*author: r3dux
version : 0.3 - 15 / 01 / 2014
	  description : Gets GLSL source code either provided as strings or can load from filenames,
					compiles the shaders, creates a shader program which the shaders are linked
					to, then the program is validated and is ready for use via myProgram.use(),
					<draw - stuff - here> then calling myProgram.disable();
		Attributes and uniforms are stored in <string, int> maps and can be added
			via calls to addAttribute(<name - of - attribute>) and then the attribute
			index can be obtained via myProgram.attribute(<name - of - attribute>) - Uniforms
			work in the exact same way.
*/
#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include<GL/gl3w.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram
{
private:
	// static DEBUG flag - if set to false then, errors aside, we'll run completely silent
	static const bool DEBUG = true;

	// We'll use an enum to differentiate between shaders and shader programs when querying the info log
	enum class ObjectType
	{
		SHADER, PROGRAM
	};

	// Shader program and individual shader Ids
	GLuint programId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	// How many shaders are attached to the shader program
	GLuint shaderCount;

	// Map of attributes and their binding locations
	std::map<std::string, int> attributeMap;

	// Map of uniforms and their binding locations
	std::map<std::string, int> uniformMap;

	// Has this shader program been initialised?
	bool initialised;

	// ---------- PRIVATE METHODS ----------

	// Private method to compile a shader of a given type
	GLuint compileShader(std::string shaderSource, GLenum shaderType);

	// Private method to compile/attach/link/verify the shaders.
	// Note: Rather than returning a boolean as a success/fail status we'll just consider
	// a failure here to be an unrecoverable error and throw a runtime_error.
	void initialize(std::string vertexShaderSource, std::string fragmentShaderSource);

	// Private method to load the shader source code from a file
	std::string loadShaderFromFile(const std::string filename);

	// Private method to return the current shader program info log as a string
	std::string getInfoLog(ObjectType type, int id);

public:
	// Constructor
	ShaderProgram();

	// Destructor
	~ShaderProgram();

	// Method to initialise a shader program from shaders provided as files
	void initFromFiles(std::string vertexShaderFilename, std::string fragmentShaderFilename);

	// Method to initialise a shader program from shaders provided as strings
	void initFromStrings(std::string vertexShaderSource, std::string fragmentShaderSource);

	// Method to enable the shader program - we'll suggest this for inlining
	inline void use() {
		// Santity check that we're initialised and ready to go...
		if (initialised)
		{
			glUseProgram(programId);
		}
		else
		{
			std::string msg = "Shader program " + programId;
			msg += " not initialised - aborting.";
			throw std::runtime_error(msg);
		}
	}

	// Method to disable the shader - we'll also suggest this for inlining
	inline void disable() {
		glUseProgram(0);
	}

	// Method to return the bound location of a named attribute, or -1 if the attribute was not found
	GLuint attribute(const std::string attributeName);

	// Method to returns the bound location of a named uniform
	GLuint uniform(const std::string uniformName);

	// Method to add an attribute to the shader and return the bound location
	int addAttribute(const std::string attributeName);

	// Method to add a uniform to the shader and return the bound location
	int addUniform(const std::string uniformName);

}; // End of class

#endif // SHADER_PROGRAM_HPP
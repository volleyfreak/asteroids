#pragma once
#include <iostream>
#include <unordered_map>

#include "Renderer.h"
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map <std::string, int> m_uniformLocationCache;
public:
	/**
	* Shader constructor. 
	*
	* @param filepath Path to shader file. File has to be seperated into #shader vertex and #shader fragment parts
	* @param file File which made the call
	* @param line Line in where the call was made
	*
	* @returns boolean for assertion of succesfull function call
	*/
	Shader(const std::string& filepath);
	~Shader();

	//Bind Shader
	void Bind() const;
	//Unbind Shader
	void Unbind() const;

	/** Set a one dimensional float value in the shader
	*
	*
	* @param name Uniform name
	* @param value Float value of uniform
	*/
	void SetUniform1f(const std::string& name, float value);

	/** Set a two dimensional float value in the shader
	*
	*
	* @param name uniform name
	* @param v1 First float value of uniform
	* @param v2 Second float value of uniform
	*/
	void SetUniform2f(const std::string& name, float v1, float v2);

	/** Set a two dimensional float value in the shader
	*
	*
	* @param name uniform name
	* @param v0 First float value of uniform
	* @param v1 Second float value of uniform
	* @param v2 Third floatvalue of uniform
	* @param v3 Fourth float value of uniform
	*/
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& location);


};
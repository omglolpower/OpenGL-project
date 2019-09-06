#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <streambuf>
#include <conio.h>
#include <Windows.h>
#define LOG(x) std::cout << x << std::endl;
#define PI() 3.14159265
//#define ASSERT(x) if(!(x)) __debugbreak();

static void GLClearError()
{
	while (glGetError != GL_NO_ERROR);
}
static bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL error " << error << std::endl;
		return false;
	}
	return true;
}

static unsigned int CompileShader(unsigned int type, const std::string & source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//error handling:

	int result;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) //
	// this function will provede opengl with shader source code
{
	unsigned int program = glCreateProgram();


	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}



float red = 0.0f;
float increment = 0.05f;
float incrPos = 0.05f;
float positions[1000];
unsigned int indices[362];
unsigned int ibo;
float IncX = 0.0f;
float IncY = 0.0f;
float scale = 1.0f;
std::ifstream ssvs("C:\\dev\\shaders\\BasicVertexShader.txt");
std::ifstream ssfs("C:\\dev\\shaders\\UniformBasicFragmentShader.txt");
std::ostringstream ossvs, ossfs;
std::string vertexShader;

std::string fragmentShader;
//coordinates for vertices of triangles:

void circle(float centerX, float centerY, float radius)
{
	positions[0] = centerX;
	positions[1] = centerY;
	for (int i = 0; i < 361; i++)
	{
		positions[(i + 1) * 2] = cos(PI() / 180 * i) * radius * scale + centerX;
		positions[2 * (i + 1) + 1] = sin(PI() / 180 * i) * radius * scale + centerY;
	}
	unsigned int buffer; //id of buffer
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2,
		0);

	 //id of buffer
	glGenBuffers(2, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * 2, indices, GL_STATIC_DRAW);
	
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	int location = glGetUniformLocation(shader, "u_Color");

	if (red > 1.0f)
		increment = -0.05f;
	if (red < 0.0f)
		increment = 0.05f;
	red = red + increment;

	glUniform4f(location, red, 0.3f, 0.8f, 1.0f);
	glDrawElements(GL_TRIANGLE_FAN, 362, GL_UNSIGNED_INT, nullptr);
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "error" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	for (int i = 0; i < 1000; i++)
	{
		positions[i] = 0.0f;
	}
	for (int i = 0; i < 362; i++)
	{
		indices[i] = i;
	}
	ossvs << ssvs.rdbuf();
	ossfs << ssfs.rdbuf();

	vertexShader = ossvs.str();
	fragmentShader = ossfs.str();
	float CX, CY;
	CX = 0.0f;
	CY = 0.0f;
	char key_press;
	int ascii_value;
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			IncY = 0.03f;
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		    IncY = -0.03f;
		else IncY = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			IncX = -0.03f;
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			IncX = 0.03f;
		else IncX = 0.0f;
		
		CX = CX + IncX;
		CY = CY + IncY;
		glClear(GL_COLOR_BUFFER_BIT);
		
		circle(CX,CY,0.3f);
		 //specify the data
		
		
		
		
		//nullptr beacause glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		


	/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}



	glfwTerminate();
	return 0;
}

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
#include <mmsystem.h>
#define LOG(x) std::cout << x << std::endl;
#define PI() 3.14159265
#define Sound() PlaySound(TEXT("mix3.wav"), NULL, SND_ASYNC);
//this f-tion will compile shaders:
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
	// this function will provide opengl with shader source code
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


//some variables for circle positioning and color:
float red = 0.0f;
float increment = 0.05f;
float incrPos = 0.05f;
float positions[724];
unsigned int indices[362];
unsigned int buffer; //id of buffer
unsigned int ibo;
float IncX = 0.0f;
float IncY = 0.0f;
float scale = 1.0f;
short length = 1280;
short heigth = 720;
float ratio = (float)length / (float)heigth;
float R;

//reading shaders from files:
std::ifstream ssvs("C:\\dev\\shaders\\BasicVertexShader.txt");
std::ifstream ssfs("C:\\dev\\shaders\\UniformBasicFragmentShader.txt");
std::ostringstream ossvs, ossfs;
std::string vertexShader;

std::string fragmentShader;

//this function will draw a circle:
void circle(float centerX, float centerY, float radius, float red, float green, int locationVar)
{
	positions[0] = centerX;
	positions[1] = centerY;
	for (int i = 0; i < 361; i++)
	{
		positions[(i + 1) * 2] = cos(PI() / 180 * i) * radius * scale / ratio + centerX;
		positions[2 * (i + 1) + 1] = sin(PI() / 180 * i) * radius * scale + centerY;
	}
	
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


	glGenBuffers(2, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	



	glUniform4f(locationVar, red, green, 0.8f, 1.0f);
	glDrawElements(GL_TRIANGLE_FAN, 362, GL_UNSIGNED_INT, nullptr);
}

//main function. creating window, context, etc.:
int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(length, heigth, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "error" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	for (int i = 0; i < 362; i++)
	{
		indices[i] = i;
	}
	ossvs << ssvs.rdbuf();
	ossfs << ssfs.rdbuf();

	vertexShader = ossvs.str();
	fragmentShader = ossfs.str();
	float CX, CY;

	
	double valueDCX, valueDCY;
	
	CX = 0.0f;
	CY = 0.0f;
	R = 0.03f;
	const float IncX = 0.04f;
	const float IncY = 0.04f;
	bool check[3] = { true, false, false };
	float RED = 0.0f;
	float GREEN = 0.0f;
	short wait = 60;
	int intRnd;
	float flRndX, flRndY;
	// here a window will be refreshed:
	unsigned int  shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	int location = glGetUniformLocation(shader, "u_Color");
	glfwGetPrimaryMonitor();
	while (!glfwWindowShouldClose(window))
	{
		
		glfwSetInputMode(window,
			GLFW_CURSOR,
			GLFW_CURSOR_NORMAL
		);
		// 5) timer:
		if (check[0])
		{
			wait++;
		}
		if (wait > 120)
		{
			check[1] = false;
			check[0] = false;
			wait = 0;
		}

		//controls:
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && CY < 1.0f - R)
			CY = CY + IncY;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && CY > -1.0f + R)
			CY = CY - IncY;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && CX / ratio > -1.0f + R / ratio)
			CX = CX - IncX;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && CX / ratio < 1.0f - R / ratio)
			CX = CX + IncX;

		glfwGetCursorPos(window,
			&valueDCX,
			&valueDCY
			);

		CX = (float)((valueDCX - 680) / 680);
		CY = (float)(-(valueDCY-384)/384);
		std::cout << valueDCX <<' '<< valueDCY<<' ' << CX << ' ' << CY << std::endl;
		if (RED > 1.0f)
			increment = -0.05f;
		if (RED < 0.0f)
			increment = 0.05f;
		RED = RED + increment;
		glClear(GL_COLOR_BUFFER_BIT);

		// 1) draw controllable circle:
		

		// 2) generate random coordinates for "lootable" circle:
		if (!check[1])
		{
			flRndX = ((float)((rand() & 80) - (rand() & 80))) / 100.0f;
			flRndY = ((float)((rand() & 80) - (rand() & 80))) / 100.0f;
			check[1] = true;
		}

		// 3) draw "lootable" circle:
		if (!check[0])
		{
			circle(flRndX, flRndY, 0.1f, 0.0f, GREEN, location);
		}
		circle(CX, CY, R, RED, 0.1f, location);
		//check pressed mouse left button
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			check[2] = true;
		else check[2] = false;
		
		// 4) check collision of circles:
		if (1.0f > (((flRndX - CX / 1) / (0.1f / ratio)) * ((flRndX - CX / 1) / (0.1f / ratio)) +
			((flRndY - CY) / 0.1f) * ((flRndY - CY) / 0.1f))&&check[2])
		{
			check[2] = false;
			if (!check[0])
			{
				PlaySound(TEXT("mix3.wav"), NULL, SND_ASYNC);
			}
				
			check[0] = true; //llotable circle won't be drawn
		}
		

		//calling a circle function( a circle will be drawn):






		GREEN = GREEN + increment;
		glfwSwapBuffers(window);

		glfwPollEvents();
	}



	glfwTerminate();
	return 0;
}

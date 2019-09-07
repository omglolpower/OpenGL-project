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
#include "MyShaders.h"
#include "MyHexagon.h"
#define LOG(x) std::cout << x << std::endl;
#define PI() 3.14159265
#define Sound() PlaySound(TEXT("mix3.wav"), NULL, SND_ASYNC);

//some variables for circle positioning and color:

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
int location;
float RED[3] = { 0.0f , 0.0f , 0.3f };
float GREEN[3] = { 0.0f , 0.0f , 0.8f };
float BLUE[3] = { 0.6f , 0.0f , 0.2f };

void drawArrays(
	bool isHexagon, 
	int locationVar, 
	float redVar, float greenVar, float blueVar,
	int arrayType,
	unsigned int nVert
)

{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	isHexagon ?
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionsHex), positionsHex, GL_STATIC_DRAW) :
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
		
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 2,
		0);

	glUniform4f(locationVar, redVar, greenVar, blueVar, 1.0f);
	glDrawArrays(arrayType, 0, nVert);
}
//this function will draw a circle:

void circle(float centerX, float centerY,
	float radius, 
	int locationVar,
	bool isHexagon,
	unsigned short elementNumber)
{
	positions[0] = centerX;
	positions[1] = centerY;

	for (int i = 0; i < 361; i++)
	{
		positions[(i + 1) * 2] = cos(PI() / 180 * i) * radius * scale / ratio + centerX;
		positions[2 * (i + 1) + 1] = sin(PI() / 180 * i) * radius * scale + centerY;
	}

	drawArrays(
		isHexagon,
		locationVar,
		RED[elementNumber], GREEN[elementNumber], BLUE[elementNumber],
		GL_TRIANGLE_FAN,
		362
	);

}

float digitPos[2][14]
=
{
	{
	-0.5f, 1.0f,
	0.5f, 1.0f,
	-0.5f, -1.0f,
	0.5f, -1.0f

	}
	,
	{
		0.0f, 2.0f,
		0.0f, 0.0f,
		0.0f, -2.0f,
		NULL, NULL
	}
};
void hexagon1(
	float centerX, float centerY,
	short hexagonNum,
	bool vh
	)
{
	hexagon(
		centerX + 0.1f * 1.2f*digitPos[vh][2* hexagonNum],
		centerY + 0.1f * 1.1f*digitPos[vh][2 * hexagonNum + 1],
		location,
		0.75f,
		1.0f,
		ratio,
		vh,
		0.1f
	);
	drawArrays(
		positionsHex,
		location,
		RED[2], GREEN[2], BLUE[2],
		GL_TRIANGLE_STRIP,
		6
	);
}
void zero(float center)
{
	for (short i = 0; i < 4; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			0
		);
	}
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			2*i,
			1
		);
	}
}

void one(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			2*i+1,
			0
		);
	}
}

void two(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			i+1,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			1
		);
	}
}

void three(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			2*i + 1,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			1
		);
	}
}

void four(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			2 * i + 1,
			0
		);
	}

	hexagon1(
		center, 0.7f,
		0,
		0
	);
		hexagon1(
			center, 0.7f,
			1,
			1
		);
}

void five(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			i*3,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			1
		);
	}
}

void six(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			i * 3,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			1
		);
	}
	hexagon1(
		center, 0.7f,
		2,
		0
	);
}

void seven(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			2 * i + 1,
			0
		);
	}
	hexagon1(
		center, 0.7f,
		0,
		1
	);
}

void eight(float center)
{
	for (short i = 0; i < 4; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			0
		);
	}
	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			 i,
			1
		);
	}
}

void nine(float center)
{
	for (short i = 0; i < 2; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			0
		);
	}
	hexagon1(
		center, 0.7f,
		3,
		0
	);

	for (short i = 0; i < 3; i++)
	{
		hexagon1(
			center, 0.7f,
			i,
			1
		);
	}
}


void digit(short digitVar, float center)
{
	switch (digitVar)
{
case 0:zero(center);
	break;
case 1:one(center);
	break;
case 2:two(center);
	break;
case 3:three(center);
	break;
case 4:four(center);
	break;
case 5:five(center);
	break;
case 6:six(center);
	break;
case 7:seven(center);
	break;
case 8:eight(center);
	break;
case 9:nine(center);
	break;
}
}

void hits(std::string strVar)
{
	for(short i = 0; i < strVar.length(); i++)
	digit((short)strVar.at(i) - 48, 0.58f+i*0.175f);
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
	short hitCounter = 0;
	std::string hitCounterString;
	hitCounterString = std::to_string(hitCounter);
	short wait = 15;
	int intRnd;
	float flRndX, flRndY;

	// here a window will be refreshed:

	unsigned int  shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	location = glGetUniformLocation(shader, "u_Color");
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
		if (wait > 25)
		{
			check[1] = false;
			check[0] = false;
			wait = 0;
		}

		//controls:
		
		glfwGetCursorPos(window,
			&valueDCX,
			&valueDCY
			);

		CX = (float)((valueDCX - 680) / 680);
		CY = (float)(-(valueDCY-384)/384);
		//std::cout << valueDCX <<' '<< valueDCY<<' ' << CX << ' ' << CY << std::endl;
		//std::cout << (short)hitCounterString.at(0) << std::endl;
		if (RED[0] > 1.0f)
			increment = -0.05f;
		if (RED[0] < 0.0f)
			increment = 0.05f;
		RED[0] = RED[0] + increment;
		glClear(GL_COLOR_BUFFER_BIT);
		
		hits(hitCounterString);

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
			circle(flRndX, flRndY, 0.13f,location, 0, 1);
		}

		circle(CX, CY, 0.06f, location, 0,0);
		
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
				hitCounter++;
				hitCounterString = std::to_string(hitCounter);
			}
			
			check[0] = true; //llotable circle won't be drawn
		}
		
		//calling a circle function( a circle will be drawn):

		GREEN[1] = GREEN[1] + increment;
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

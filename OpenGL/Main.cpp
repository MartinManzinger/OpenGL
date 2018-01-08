
#include <stdlib.h>
#include <glfw3.h>
#include "ParticleSystem.h"
#include <Windows.h>


void DrawParticle(Particle* InParticle)
{
	// calculate helper variables
	int sides = 2;
	float temperature = InParticle->temperature;
	temperature /= 500;
	temperature = temperature >= 1.0 ? 1.0 : temperature;
	float radius = InParticle->radius+0.001;
	if (radius > 0.002) ++sides;
	if (radius > 0.005) ++sides;
	if (radius > 0.006) ++sides;
	if (radius > 0.015) ++sides;
	if (radius > 0.03) ++sides;
	if (radius > 0.03) ++sides;
	if (radius > 0.04) ++sides;
	if (radius > 0.05) ++sides;

	// render circle with open gl
	glColor3f(1, temperature, temperature);
	glBegin(GL_POLYGON);
	glShadeModel(GL_LINE_SMOOTH);
	for (double i = 0; i < 2 * PI; i += PI / sides)
		glVertex3f(cos(i) * radius + InParticle->pos.x, sin(i) * radius + InParticle->pos.y, 0.0);

	glEnd();
}

int main(void)
{
	GLFWwindow* window;

	// initialize particle engine
	ParticleEngine ParticleEngine;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1680, 1050, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		// draw particles
		ParticleEngine.GetParticle(true);
		while (1)
		{
			Particle* ParticlePtr = ParticleEngine.GetParticle();
			if (ParticlePtr != nullptr) DrawParticle(ParticlePtr);
			else break;
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		// calculate next state
		ParticleEngine.CalcNextTick();
		// align view
		ParticleEngine.ToGravCenter();

		static int Temp = 0;
		if (Temp > 2)
		{
			Temp = 0;
			ParticleEngine.AddParticle();
		}
		++Temp;

		// wait for next step
		Sleep(30);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
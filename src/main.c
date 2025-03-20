#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %s", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
	if (!glfwInit())
	{
		printf("Failed to Init GLFW\n");
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Shaderground", NULL, NULL);
	if (!window)
	{
		printf("Failed to create Window\n");
		return 2;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetKeyCallback(window, key_callback);

	// Setup done, sick!
	while (!glfwWindowShouldClose(window))
	{
		printf("On It\n");
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

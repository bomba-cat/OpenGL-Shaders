#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct
{
  float x, y;
} Vector2;

typedef struct
{
  float x, y, z;
} Vector3;

typedef struct
{
  Vector3 pos;
  Vector3 verticies[3];
} Triangle;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %s", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

char* readShaderFile(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    printf("Failed to open shader file: %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(length + 1);
  if (!buffer) {
    printf("Memory allocation failed for shader file: %s\n", filename);
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);
  return buffer;
}

char* getExecutableDir() {
  static char exePath[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
  if (len == -1) {
    return NULL;
  }
  exePath[len] = '\0';

  char* lastSlash = strrchr(exePath, '/');
  if (lastSlash) {
    *lastSlash = '\0';
  }

  return exePath;
}

char* getShaderPath(const char* filename) {
  static char fullPath[PATH_MAX];
  snprintf(fullPath, PATH_MAX, "%s/%s", getExecutableDir(), filename);
  return fullPath;
}

GLuint compileShader(GLenum type, const char* filename) {
  char* source = readShaderFile(filename);
  if (!source) return 0;
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, (const char**)&source, NULL);
  glCompileShader(shader);
  free(source);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("Shader Compilation Failed (%s):\n%s\n", filename, infoLog);
    return 0;
  }

  return shader;
}

void loadVerticies(float *verticies)
{
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  return;
}

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		printf("Failed to Init GLFW\n");
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_SAMPLES, 16);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Shaderground", NULL, NULL);
	if (!window)
	{
		printf("Failed to create Window\n");
		return 2;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  float verticies[] = 
    {
      -1.0f, -1.0f, 0.0f,
      -0.0f, -1.0f, 0.0f,
      -0.5f, 0.0f, 0.0f
    };
  printf("%f", verticies[1]);
  loadVerticies(verticies);

  GLuint shaderProgram = glCreateProgram();
  GLuint vertexProgram = compileShader(GL_VERTEX_SHADER, getShaderPath("shader/shader.vs"));
  GLuint fragmentProgram = compileShader(GL_FRAGMENT_SHADER, getShaderPath("shader/shader.fs"));

  glAttachShader(shaderProgram, vertexProgram);
  glAttachShader(shaderProgram, fragmentProgram);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  GLuint frameLoc = glGetUniformLocation(shaderProgram, "frame");

  glBindVertexArray(VBO);

  int frame = 0;
	while (!glfwWindowShouldClose(window))
	{
    int width, height;
    double time = glfwGetTime();

    frame = frame+1 % 31000;
    glUniform1i(frameLoc, frame);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSwapBuffers(window);
    glfwSwapInterval(1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
	}

	glfwDestroyWindow(window);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

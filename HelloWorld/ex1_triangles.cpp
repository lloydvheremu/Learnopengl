/*Render two triangles next to each other*/
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const char *vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";
const char *fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;
	void main() {
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}
)";

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processinput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
int main() {
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create a window object
	GLFWwindow *window = glfwCreateWindow(800, 600, "Two Triangles", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Place context of 'window' to current thread
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Set Viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Vertex Input
	float vertices[] = {
		// First triangle
		-1.0f, -0.5f, 0.0f,
		-0.625f, 0.5f, 0.0f,
		-0.25f, -0.5f, 0.0f,
		// Second triangle
		 0.625f, 0.5f, 0.0f,
		 0.25f, -0.5f, 0.0f, 
		 1.0f, -0.5f, 0.0f,

	};

	GLuint indices[] = {
		0, 1, 2,
		3, 4, 5
	};

	/* STORE VERTEX DATA INTO GPU MEMORY */
	// generate unique buffer ID for vertex buffer object, bind buffer and copy vertex data into buffer memory
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Compiling shaders*/
	// VERTEX Shader
	// Create a shader object and reference by ID
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// FRAGMENT Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check Shader Compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	// LINK SHADERS TO SHADER PROGRAM
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::Program::LINK_FAILED\n" << infoLog << std::endl;
	}

	std::cout << success;
	// render loop
	while (!glfwWindowShouldClose(window)) {
		// Process user input
		processinput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffers
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glFlush();
	glFinish();
	glfwTerminate();
	return 0;
}
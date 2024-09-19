/* Draw 2 triangles next to each other, with different colors*/
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";

const char* vertexShaderYellowSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";
const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;
	void main() {
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}
)";
const char* yellowfragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColour;
	void main() {
		FragColour = vec4(0.5f, 0.5f, 0.0f, 1.0f);
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Two Triangles", NULL, NULL);
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
	// First triangle
	float vertices1[] = {
		-1.0f, -0.5f, 0.0f,
		-0.625f, 0.5f, 0.0f,
		-0.25f, -0.5f, 0.0f,
	};

	// Second triangle
	float vertices2[] = {
		 0.625f, 0.5f, 0.0f,
		 0.25f, -0.5f, 0.0f,
		 1.0f, -0.5f, 0.0f,
	};

	/*unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5
	 };*/

	 /* STORE VERTEX DATA INTO GPU MEMORY */
	 // generate unique buffer ID for vertex buffer object, bind buffer and copy vertex data into buffer memory
	unsigned int VBO1, VBO2, VAO1, VAO2;
	// Triangle 1
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Triangle 2
	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
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

	// VERTEX Shader for second triangke
	unsigned int vertexShaderYellow = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderYellow, 1, &vertexShaderYellowSource, NULL);
	glCompileShader(vertexShaderYellow);

	// FRAGMENT Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// FRAGMENT Shader for second triangle
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &yellowfragmentShaderSource, NULL);
	glCompileShader(fragmentShaderYellow);

	
	// LINK SHADERS TO SHADER PROGRAM
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Yellow Shader program
	unsigned int shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShaderYellow);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	glGetProgramiv( shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog( shaderProgramYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}


	// render loop
	while (!glfwWindowShouldClose(window)) {
		// Process user input
		processinput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glFlush();
	glFinish();
	glfwTerminate();
	return 0;
}
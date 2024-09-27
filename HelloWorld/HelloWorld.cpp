#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	uni
	void main(){
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";

const char* fragmentShaderSource = R"(#version 330 core 
	out vec4 FragColor;
	void main(){
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}
)";


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	/* New GLFW windo*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*Create a window object*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to created GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set Viewport
	glViewport(0, 0, 800, 600);
	// When user adjust viewport height
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	// triangle coordinates
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	// Store vertex data into graphics card memory
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 0. Copy our vertices in a buffer for OpenGL to use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 1. Then set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create a vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attach shader source code to the shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create shader program
	unsigned int shaderProgram = glCreateProgram();

	// link shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// check if shader compilation is succesful
	int success;
	int vertexSuccess;
	int fragmentSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		
	if (!vertexSuccess || !fragmentSuccess || !success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::Program::LINK_FAILED\n" << infoLog << std::endl;

	}

	

	// Create render loop
	while (!glfwWindowShouldClose(window)) {
		
		// Check user input
		processInput(window);
		
		//Set background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// render an object
		glUseProgram(shaderProgram);
		// rebind VAO before drawing
		glBindVertexArray(VAO);
		// draw triagle
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;

}
#include <stdio.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLuint uniColor;
static GLuint uniModel;
static int i = 0;
void setup()
{
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	float vertices[] = 
	{
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource = 
	"#version 150 core\n"
	"in vec2 position;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 proj;\n"
	"void main(){\n"
	"	gl_Position = proj * view * model * vec4(position, 0.0, 1.0);\n"
	"}";
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if(!status)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf("%s\n", buffer);
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSource = 
	"#version 150 core\n"
	"uniform vec3 triColor;\n"
	"out vec4 outColor;\n"
	"void main(){\n"
	"	outColor = vec4(triColor, 1.0);\n"
	"}";
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint statusF;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusF);
	if(!statusF)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf("%s\n", buffer);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
		
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel,1, GL_FALSE, glm::value_ptr(model));
	
	glm::mat4 view = glm::lookAt(
		glm::vec3(1.2f, 1.2f, 1.2f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	GLuint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 1.0f, 10.0f);
	GLuint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
		
	uniColor = glGetUniformLocation(shaderProgram, "triColor");
	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(posAttrib);
	 
}

void drawnow()
{
	
	++i;
	if(i > 360) { i = 1; }
	glUniform3f(uniColor, ((float)i/180) * 0.5f, ((float)(360-i)/180) *0.5f, (float)(i*i)/(360.0f*360.0f)*1.0f);

		
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, ((float)i/180) *  glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniModel,1, GL_FALSE, glm::value_ptr(model));
	
	
	glDrawArrays(GL_LINE_LOOP,0, 3);	
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
	glfwMakeContextCurrent(window);
	
	setup();
		
	while(!glfwWindowShouldClose(window))
	{
	    	glfwSwapBuffers(window);
	    	glfwPollEvents();
	
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		    	glfwSetWindowShouldClose(window, GL_TRUE);
	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		drawnow();
	}
	
	glfwTerminate();
}

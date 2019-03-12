#include "helper.h"
#include "glm/glm.hpp" // GL Math library header
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

// WARNING: variables and functions from here
int widthTexture, heightTexture;
int winPosX, winPosY, winSizeX, winSizeY;
GLuint MVid, MVPid, heightFactorId;
float heightFactor = 10.0, speed = 0.0;
bool isFullscreen = false;
glm::vec3 up, gaze, eye;
glm::mat4 M, V, P;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void createVerticesAndTriangles(int width, int height, glm::vec3 vertices[],
	glm::vec3 triangles[]) {
	glm::vec3 vec;
	int idx = 0;
    for(int i = 0; i <= height; i++) {
        for(int j = 0; j <= width; j++) {
			vec = {j, -1, i};
			vertices[idx++] = vec;
        }
    }
	idx = 0;
	// we will have widthTexture * heightTexture * 2 triangles.
	// each triangle is represented by 3 vertices.
	for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
			triangles[idx++] = vertices[(i + 1) * (width + 1) + j];
			triangles[idx++] = vertices[i * (width + 1) + j + 1];
			triangles[idx++] = vertices[i * (width + 1) + j];
			triangles[idx++] = vertices[(i + 1) * (width + 1) + j + 1];
			triangles[idx++] = vertices[i * (width+1) + j + 1];
			triangles[idx++] = vertices[(i + 1) * (width + 1) + j];
        }
    }
}

void display(glm::mat4 M, glm::mat4 P) {
	eye += (speed * gaze);
	V = glm::lookAt(eye, gaze + eye, up);
	glUniformMatrix4fv(MVid, 1, GL_FALSE, glm::value_ptr(M * V));
	glUniformMatrix4fv(MVPid, 1, GL_FALSE, glm::value_ptr(P * V * M));
	glUniform1f(heightFactorId, heightFactor);
}

glm::vec3 handleUpDownKey(int coef, bool isUp) {
	return glm::vec3(glm::rotate(glm::mat4(1.0f), coef * 0.01f,
		glm::cross(up, gaze)) * glm::vec4(isUp ? up : gaze, 1.0f));
}

glm::vec3 handleLeftRightKey(int coef) {
	return glm::vec3(glm::rotate(glm::mat4(1.0f),
		coef * 0.01f, up) * glm::vec4(gaze,1.0f));
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mods){
	if(key == GLFW_KEY_F && action == GLFW_PRESS) {
		// TODO: change
		isFullscreen = !isFullscreen;
		if(isFullscreen) {
			glfwGetWindowPos(win, &winPosX, &winPosY);
			glfwGetWindowSize(win, &winSizeX, &winSizeY);
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(win, monitor, 0, 0,
				mode->width, mode->height, 0);
	   	} else {
	   		glfwSetWindowMonitor(win, NULL, winPosX, winPosY,
				winSizeX, winSizeY, 0);
		}
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		up = handleUpDownKey(-1, true);
		gaze = handleUpDownKey(-1, false);
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		up = handleUpDownKey(1, true);
		gaze = handleUpDownKey(1, false);
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GLFW_TRUE);
	if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
		heightFactor += 0.5;
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
		heightFactor -= 0.5;
	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
		speed += 0.1;
	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
		speed -= 0.1;
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		gaze = handleLeftRightKey(1);
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		gaze = handleLeftRightKey(-1);
}
// WARNING: variables and functions till here

static void errorCallback(int error,
	const char * description) {
	fprintf(stderr, "Error: %s\n", description);
}
int main(int argc, char * argv[]) {

	if (argc != 2) {
		printf("Only one texture image expected!\n");
		exit(-1);
	}

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);

	if (!win) {
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(win);
	glfwSetKeyCallback(win, keyCallback);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		glfwTerminate();
		exit(-1);
	}
	initShaders();
	glUseProgram(idProgramShader);
	initTexture(argv[1], & widthTexture, & heightTexture);

	// WARNING: code goes here
	glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);
	up = {0.0, 1.0, 0.0};
	gaze = {0.0, 0.0, 1.0};
	eye = {widthTexture / 2.0,
		widthTexture / 10.0, (-1.0 / 4.0) * widthTexture};
	glUniform1f(glGetUniformLocation(idProgramShader, "widthTexture"),
		widthTexture);
	glUniform1f(glGetUniformLocation(idProgramShader, "heightTexture"),
		heightTexture);
	heightFactorId = glGetUniformLocation(idProgramShader, "heightFactor");
	glUniform1f(heightFactorId, heightFactor);

	MVid = glGetUniformLocation(idProgramShader, "MV");
	MVPid = glGetUniformLocation(idProgramShader, "MVP");
	glUniformMatrix4fv(MVid, 1, GL_FALSE, glm::value_ptr(V * M));
	glUniformMatrix4fv(MVPid, 1, GL_FALSE, glm::value_ptr(P * V * M));

	int numOfVertices = (widthTexture + 1) * (heightTexture + 1);
	int bufferSize = heightTexture * widthTexture * 2 * 3;
	glm::vec3* vertices = new glm::vec3[numOfVertices]();
	glm::vec3* triangles = new glm::vec3[bufferSize]();

	createVerticesAndTriangles(widthTexture, heightTexture, vertices,
		triangles);

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * bufferSize,
		&(triangles)[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);

	M = glm::mat4(1.0f);
	P = glm::perspective(0.7853981633974483, 1.0, 0.1, 1000.0);

	while (!glfwWindowShouldClose(win)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);
		display(M, P);
		glDrawArrays(GL_TRIANGLES, 0, bufferSize);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	// WARNING: till here


	glfwDestroyWindow(win);
	glfwTerminate();

	return 0;
}

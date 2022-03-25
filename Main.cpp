/*
* @author	Marcela Estrada
* @desc		A computer animation based off of the gamecube initial animation.
* @source	I followed tutorials from https://learnopengl.com/ and used a previous 
*			project called flashing colors cubes as a boiler plate.
*/
#include <iostream>
#include <glad/glad.h>  // this needs to go before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp> 
#include <glm/glm/gtc/matrix_transform.hpp> 
#include <glm/glm/gtc/type_ptr.hpp> 

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"



// resize frame buffer to fit the window
void resizeFrameBuffer(GLFWwindow* window, int width, int height);
// process key inputs
void processInput(GLFWwindow* window, GLuint* size, int* myKeys);

// safer to use OPENGL version of floats
// give the coordinates of the vertices of a triangle to form the cube.
// Will start with very low opacity and then move up during the animation
GLfloat vertices[] =
{ //			coordinates	   /				colors				//
	-0.5f,	 0.5f,		-0.5f,	0.0f,	0.5f,	0.5f,	// lower left corner
	 0.5f,	 0.5f,		-0.5f,	0.0f,	0.5f,	0.5f,	// lower right corner
	-0.5f,	-0.5f,		-0.5f,	0.0f,	0.5f,	0.2f,	// upper corner
	 0.5f,  -0.5f,		-0.5f,	0.0f,	0.5f,	0.2f,	// inner left

	-0.5f,	 0.5f,		0.5f,	0.2f,	0.5f,	0.2f,	// lower left corner
	 0.5f,	 0.5f,		0.5f,	0.2f,	0.5f,	0.2f,	// lower right corner
	-0.5f,	-0.5f,		0.5f,	0.5f,	0.5f,	0.5f,	// upper corner
	 0.5f,  -0.5f,		0.5f,	0.5f,	0.5f,	0.5f,	// inner left
};

// indicated with vertices to use to create the square
GLuint indices[] =
{
	0, 2, 3,
	0, 3, 1,
	2, 6, 7,
	2, 7, 3,
	6, 4, 5,
	6, 5, 7,
	4, 0, 1,
	4, 1, 5,
	0, 4, 6,
	0, 6, 2,
	1, 5, 7,
	1, 7, 3,

};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Flashing Colors", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// initializing glad, setting checks in case it doesn't load
	if (gladLoadGL() != 1) {
		std::cout << "Failed to initialize Glad!!! </3" << std::endl;
		return -1;
	}


	glViewport(0, 0, 600, 600);

	// for resizing the window
	glfwSetFramebufferSizeCallback(window, resizeFrameBuffer);

	// setting up 3D 
	// setting up model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	// setting up the camera
	glm::vec3 camPos = glm::vec3(10.0f, 10.0f, 7.0f); // position of the camera
	glm::vec3 camTarget = glm::vec3(0.5f, 0.0f, -0.5f); // the target, in this case the origin
	glm::vec3 camDirection = glm::normalize(camPos - camTarget); // getting the direction of the camera
	glm::vec3 up = glm::vec3(0.0f, 10.0f, 0.0f); 
	glm::vec3 camRight = glm::normalize(glm::cross(up, camDirection));
	glm::vec3 camUp = glm::cross(camDirection, camRight);

	// setting up view matrix
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, 1.0f, 0.0f));;
	//view = glm::translate(view, glm::vec3(1.0f, 0.0f, -15.0f));

	view =	glm::lookAt(camPos,	// camera position
						camTarget,	// target position
						camUp);			// up vector
	// setting up projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// create the shader program
	Shader shaderProgram("default.vert", "default.frag");

	// generates vertex array object and binds it
	VAO VAO1;
	VAO1.Bind();

	// generates vertex buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generates element buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// links vbo to vao
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// unbind all objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// set positions for cubes 
	glm::vec3 cubePos[] = {
			glm::vec3(0.5f, 0.5f, 2.0f),
			glm::vec3(0.5f, 1.5f, 2.0f),
			glm::vec3(0.5f,  2.5f, 2.0f),

			glm::vec3(0.5f,  3.5f, 1.0f),
			glm::vec3(1.5f,  3.5f, 1.0f),
			glm::vec3(2.5f,  3.5f, 1.0f),

			glm::vec3(2.5f, 2.5f, 2.0f),
			glm::vec3(2.5f, 1.5f, 2.0f),

		    glm::vec3(3.5f, 1.5f, 1.0f),
			glm::vec3(3.5f, 2.5f, 1.0f),


			glm::vec3(2.5f,  3.5f, 1.0f),

			glm::vec3(2.5f, 3.5f, 0.0f),
			glm::vec3(2.5f, 3.5f, -1.0f),

			glm::vec3(3.5f, 2.5f, -1.0f),
			glm::vec3(3.5f, 1.5f, -1.0f),
			glm::vec3(3.5f, 0.5f, -1.0f),
	};

	GLchar dimension[16] = {
		'z','z','z','y','y','y','z','z','x','x','y','y','y','x','x','x'
	};
	GLuint turns[15] = {
		0,0,1,2,2,3,4,5,6,7,8,8,9,10,10
	};



	// uniforms for creating new squares
	GLuint mxID = glGetUniformLocation(shaderProgram.ID, "scaleX");
	GLuint myID = glGetUniformLocation(shaderProgram.ID, "scaleY");
	GLuint mzID = glGetUniformLocation(shaderProgram.ID, "scaleZ");

	// uniform for changing color
	GLuint cID = glGetUniformLocation(shaderProgram.ID, "myColor");

	// uniforms for 3D
	GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// color variables
	glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	// temp variables for uniforms
	float x = 0; // width
	float y = 0; // width
	float z = 0; // depth
	float c = 0; // color
	float time = 0;
	GLuint counter = 0;
	GLuint numFaces = 15;
	float angle = 0.0f;
	GLboolean rewind = false;
	// sizes of the squares
	GLuint size[2] = { 1,1 };
	// up,down,left,right
	int myKeys[4] = {
		false,
		false,
		false,
		false
	};

	glEnable(GL_DEPTH_TEST);

	// setting initial color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	while (!glfwWindowShouldClose(window))
	{
		if (time >=20.0f)
		{
			time = 0.1f;
			if (rewind == true) {
				counter = 15;
				/*counter--;
				if (counter == 0) {
					rewind = false;
				}*/
			}
			else {
				counter++;

			}

		}

		if (counter > 14) {
			rewind = true;
		}

		time += 0.1f;

		processInput(window, size, myKeys);

		// rows and columns to new sizes incase the user presses UP/DOWN keys
		int row = size[0];
		int col = size[1];

		// clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// start the shader program
		shaderProgram.Start();
		// bind the vao so opengl knows to use it
		VAO1.Bind();

		// reset x, y and z to 0
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		// update the color
		glUniform1f(cID, time * 0.01f);
		//c += 0.001f;
		if (counter == 15) {
			glUniform1f(cID, -glfwGetTime() + 7.0f);
		}

		// set the width and height for the squares
		// in case the sizes are new
		float w = 3 / static_cast<float>(col);
		float h = 3 / static_cast<float>(row);

		glUniform1f(mxID, x);
		glUniform1f(mxID, y);
		glUniform1f(mzID, z);

		// initial normalized cube
		glm::mat4 cube = glm::mat4(1.0f);

		// setting up the original cube
		glm::mat4 largeCube = glm::translate(cube, glm::vec3(1.5f, 1.5f, 0.0f));
		largeCube = glm::scale(largeCube, glm::vec3(3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(largeCube));


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//setting up face
		glUniform1f(cID, 0);
		if (counter == 15) {
			glUniform1f(cID, glfwGetTime() - 5.0f);
		}
		glm::vec3 axis = glm::vec3(0.0f, 0.0f, 0.0f);

		for (GLuint i = 0; i <= counter; i++) {
			GLfloat tempZ = 0.0f;

			if (dimension[i] == 'z') {
				glUniform1f(mzID, -1.0f);

				axis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else if (dimension[i] == 'x') {
				glUniform1f(mxID, -1.0f);
				axis = glm::vec3(0.0, 0.0, 1.0);
			}
			else {
				glUniform1f(myID, -1.0f);
				axis = glm::vec3(0.0, 0.0, 1.0);
			}

			glm::mat4 face = glm::translate(cube, cubePos[i]);

			face = glm::scale(face, glm::vec3(0.8f));
			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(face));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glUniform1f(mxID, x);
			glUniform1f(myID, y);
			glUniform1f(mzID, z);

			if (i >= 15){
				face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
				face = glm::rotate(face, glm::radians(-180.0f), axis);
				face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(face));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

				face = glm::translate(face, glm::vec3(3.7f, 0.0f, 3.7f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(face));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
			else if (i == counter) {
					switch (turns[i]) {
					case 0:
						face = glm::translate(face, glm::vec3(x, 0.5f, -0.5f));
						face = glm::rotate(face, -cos((time * 0.04f)) * glm::radians(-180.0f), axis);
						face = glm::translate(face, glm::vec3(x, 0.5f, -0.5f));
						break;
					case 1:
						face = glm::translate(face, glm::vec3(x, 0.5f, -0.5f));
						face = glm::rotate(face, -cos((time * 0.05f)) * glm::radians(-180.0f), axis);
						face = glm::translate(face, glm::vec3(x, 0.5f, -0.5f));
						break;
					case 2:
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						face = glm::rotate(face, -cos((time * 0.04f)) * glm::radians(-180.0f), axis);
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						break;
					case 3:
						face = glm::translate(face, glm::vec3(x, -0.5f, 0.5f));
						face = glm::rotate(face, -cos((time * 0.05f)) * glm::radians(-180.0f), glm::vec3(-1.0, 0.0, 0.0));
						face = glm::translate(face, glm::vec3(x, -0.5f, 0.5f));
						break;
					case 4:
						face = glm::translate(face, glm::vec3(x, -0.5f, -0.5f));
						face = glm::rotate(face, -cos((time * 0.04f)) * glm::radians(-180.0f), glm::vec3(-1.0, 0.0, 0.0));
						face = glm::translate(face, glm::vec3(x, -0.5f, -0.5f));
						break;
					case 5:
						face = glm::translate(face, glm::vec3(0.5f, y, -0.5f));
						face = glm::rotate(face, cos((time * 0.05f)) * glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0));
						face = glm::translate(face, glm::vec3(0.5f, y, -0.5f));
						break;
					case 6:
						face = glm::translate(face, glm::vec3(-0.5f, 0.5f, z));
						face = glm::rotate(face, cos((time * 0.04f)) * glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));
						face = glm::translate(face, glm::vec3(-0.5f, 0.5f, z));
						break;
					case 7:
						face = glm::translate(face, glm::vec3(-0.5f, 0.5f, z));
						face = glm::rotate(face, cos((time * 0.05f)) * glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));
						face = glm::translate(face, glm::vec3(-0.5f, 0.5f, z));
						break;
					case 8:
						face = glm::translate(face, glm::vec3(x, -0.5f, -0.5f));
						face = glm::rotate(face, cos((time * 0.04f)) * glm::radians(-180.0f), glm::vec3(-1.0, 0.0, 0.0));
						face = glm::translate(face, glm::vec3(x, -0.5f, -0.5f));
						break;
					case 9:
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						face = glm::rotate(face, -cos((time * 0.05f)) * glm::radians(-180.0f), axis);
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						break;
					case 10:
						face = glm::translate(face, glm::vec3(-0.5f, -0.5f, z));
						face = glm::rotate(face, -cos((time * 0.04f)) * glm::radians(-180.0f), axis);
						face = glm::translate(face, glm::vec3(-0.5f, -0.5f, z));
						break;
					default:
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						face = glm::rotate(face, cos((time * 0.04f)) * glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0));
						face = glm::translate(face, glm::vec3(0.5f, -0.5f, z));
						break;
					}		

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(face));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
				

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete all the object we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	// need to always terminate
	glfwTerminate();
	return 0;
}

void resizeFrameBuffer(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
/// <summary>
/// function for processing user input. 
/// If the user presses the escape key, the program will exit.
/// If the user presses the up key, the number of squares will go down
/// If the user presses the down key, the number of squares will go up
/// </summary>
/// <param name="window">the window object</param>
/// <param name="size">the number of squares</param>
/// <param name="myKeys">the keys</param>
void processInput(GLFWwindow* window, GLuint* size, int* myKeys)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// if the up key was pressed, it will set that key value to true
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		myKeys[0] = true;
		std::cout << size[0] << "up is being pressed" << std::endl;
	}
	// if the up key was released and it had the value from myKeys as true
	// it will then indicate to increase the number of squares
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && myKeys[0] == true) {
		myKeys[0] = false;
		if (size[0] <= 20) {
			size[0] += 1;
			size[1] += 1;
		}

	}

	// if the down key was pressed, it will set that key value to true
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		myKeys[1] = true;

	}
	// if the down key was released and it had the value from myKeys as true
	// it will then indicate to decrease the number of squares	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && myKeys[1] == true) {
		myKeys[1] = false;
		if (size[0] > 0) {
			size[0] -= 1;
			size[1] -= 1;
		}
	}

}

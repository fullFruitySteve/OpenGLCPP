#include "TestCube.h"
#include "Renderer.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace test
{
	bool showMouse = false;
	float zoom = 45.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	TestCube::TestCube()
	{
		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 5 * 36 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); //push 3 elements (x,y,z)
		layout.Push<float>(2); //push 2 elements (s,t)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/cube.shader");
		m_Shader->Bind();
		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);

		m_Texture = std::make_unique<Texture>("res/textures/box.jpg");
		m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
		m_Texture->Bind();
		m_Texture2->Bind(1);
		GLCall(glEnable(GL_DEPTH_TEST));
		m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//The name direction vector is not the best chosen name, since it is actually pointing in the reverse direction of what it is targeting.
		m_CameraDirection = glm::normalize(m_CameraPos - m_CameraTarget);
		m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRight = glm::normalize(glm::cross(m_Up, m_CameraDirection));
		m_CameraUp = glm::cross(m_CameraDirection, m_CameraRight);
		m_Radius = 10.0f;
		m_CameraSpeed = 2.5f;
		m_Zoom = 45.0f;
	}

	TestCube::~TestCube() 
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		//m_CamX = sin(glfwGetTime()) * m_Radius;
		//m_CamZ = cos(glfwGetTime()) * m_Radius;
		m_DeltaTime = deltaTime;
	}

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	void TestCube::OnRender()
	{
		GLCall(glClearColor(0.192f, 0.037f, 0.37f, 1.0f););
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		{
			// create transformations
			for (unsigned int i = 0; i < 10; i++) 
			{
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				glm::mat4 view = glm::mat4(1.0f);
				glm::mat4 projection = glm::mat4(1.0f);

				float angle = 20.0f * i;
				if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
					angle = (float)glfwGetTime() * -25.0f;

				model = glm::translate(model, cubePositions[i]);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

				//Rotate the camera in a circle
				//view = glm::translate(view, glm::vec3(m_CamX, m_CameraUp.y, m_CamZ));
				//Rotate the camera in a circle, while look at the box object
				//view = glm::lookAt(glm::vec3(m_CamX, 0.0f, m_CamZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				view = glm::lookAt(m_CameraPos, m_CameraPos + cameraFront, m_CameraUp);
				projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
				// retrieve the matrix uniform locations
				m_Shader->SetUniformMat4f("model", model);
				m_Shader->SetUniformMat4f("view", view);
				// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
				m_Shader->SetUniformMat4f("projection", projection);

				renderer.DrawArrays(0, 36);
			}
		}
	}
	void TestCube::OnImGuiRender()
	{

	}

	void TestCube::OnInput(GLFWwindow * window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_CameraPos += m_CameraSpeed * cameraFront * m_DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_CameraPos -= m_CameraSpeed * cameraFront * m_DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_CameraPos -= glm::normalize(glm::cross(cameraFront, m_CameraUp)) * m_CameraSpeed * m_DeltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_CameraPos += glm::normalize(glm::cross(cameraFront, m_CameraUp)) * m_CameraSpeed * m_DeltaTime;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			showMouse = !showMouse;
		}

		//Disable the mouse curser 
		if(!showMouse)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
	}
	bool firstMouse = true;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.5f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		zoom -= (float)yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}
}
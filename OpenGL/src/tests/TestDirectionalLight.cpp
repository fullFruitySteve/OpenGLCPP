#include "TestDirectionalLight.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	bool directionalLightShowMouse = false;
	float directionalLight_lastX = 800 / 2.0f;
	float directionalLight_lastY = 600 / 2.0f;
	bool directionalLight_firstMouse = true;
	//timing
	float directionalLightDeltaTime = 0.0f;
	Camera directionalLightCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	void directionalLightMouse_callback(GLFWwindow* window, double xpos, double ypos);
	void directionalLightProcessInput(GLFWwindow *window);

	TestDirectionalLight::TestDirectionalLight()
		: m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f)), m_LightDirection(glm::vec3(0.2f, -1.0f, -0.3f)), m_Angle(0.0f)
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};

		m_CubeVAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 36 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); //push 3 elements (x,y,z)
		layout.Push<float>(3); //push 3 elements (normals)
		layout.Push<float>(2); //push 2 elements (s,t)
		m_CubeVAO->AddBuffer(*m_VertexBuffer, layout);

		m_LightingMapsShader = std::make_unique<Shader>("res/shaders/directionalLight.shader");
		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");
		m_DiffuseMap->Bind();
		m_SpecularMap->Bind(1);
	}

	TestDirectionalLight::~TestDirectionalLight()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestDirectionalLight::OnUpdate(float deltaTime)
	{
		//m_Angle = (float)glfwGetTime() * -25.0f;
		directionalLightDeltaTime = deltaTime;
	}

	// positions all containers
	glm::vec3 DirectionalLightcubePositions[] = {
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
	void TestDirectionalLight::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Set the light
		m_LightingMapsShader->Bind();
		glm::vec3 diffuseColor = m_LightColour * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		m_LightingMapsShader->SetUniform3f("light.direction", m_LightDirection);
		m_LightingMapsShader->SetUniform3f("light.ambient", ambientColor);
		m_LightingMapsShader->SetUniform3f("light.diffuse", diffuseColor);
		m_LightingMapsShader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

		//Set the material
		m_LightingMapsShader->SetUniform1f("material.shininess", 32.0f);
		m_LightingMapsShader->SetUniform3f("viewPos", directionalLightCamera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = directionalLightCamera.GetViewMatrix();
		m_LightingMapsShader->SetUniformMat4f("projection", projection);
		m_LightingMapsShader->SetUniformMat4f("view", view);

		// world transformation
		for(unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(m_Angle * i), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, DirectionalLightcubePositions[i]);
			m_LightingMapsShader->SetUniformMat4f("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}

	void TestDirectionalLight::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Direction", &m_LightDirection.x, -360.0f, 360.0f);
	}

	void TestDirectionalLight::OnInput(GLFWwindow* window)
	{
		directionalLightProcessInput(window);
		//glfwSetCursorPosCallback(window, directionalLightMouse_callback);
		// tell GLFW to capture our mouse
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void directionalLightProcessInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			directionalLightCamera.ProcessKeyboard(FORWARD, directionalLightDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			directionalLightCamera.ProcessKeyboard(BACKWARD, directionalLightDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			directionalLightCamera.ProcessKeyboard(LEFT, directionalLightDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			directionalLightCamera.ProcessKeyboard(RIGHT, directionalLightDeltaTime);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void directionalLightMouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (directionalLight_firstMouse)
		{
			directionalLight_lastX = xpos;
			directionalLight_lastY = ypos;
			directionalLight_firstMouse = false;
		}

		float xoffset = xpos - directionalLight_lastX;
		float yoffset = directionalLight_lastY - ypos; // reversed since y-coordinates go from bottom to top

		directionalLight_lastX = xpos;
		directionalLight_lastY = ypos;

		directionalLightCamera.ProcessMouseMovement(xoffset, yoffset);
	}
}
#include "TestPointLight.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	bool pointLight_ShowMouse = false;
	float pointLight_lastX = 800 / 2.0f;
	float pointLight_lastY = 600 / 2.0f;
	bool pointLight_firstMouse = true;
	//timing
	float pointLight_DeltaTime = 0.0f;
	Camera pointLight_Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	void pointLight_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void pointLight_processInput(GLFWwindow *window);

	TestPointLight::TestPointLight()
		: m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f)), /*m_LightDirection(glm::vec3(0.2f, -1.0f, -0.3f)),*/ 
		m_LightPosition(glm::vec3(1.2f, 1.0f, 2.0f)), m_Angle(0.0f)
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

		m_PointLightShader = std::make_unique<Shader>("res/shaders/pointLightShader.shader");
		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");
		m_DiffuseMap->Bind();
		m_SpecularMap->Bind(1);
	}

	TestPointLight::~TestPointLight()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestPointLight::OnUpdate(float deltaTime)
	{
		//m_Angle = (float)glfwGetTime() * -25.0f;
		pointLight_DeltaTime = deltaTime;
	}

	// positions all containers
	glm::vec3 pointLightcubePositions[] = {
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
	void TestPointLight::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Set the light
		m_PointLightShader->Bind();
		glm::vec3 diffuseColor = m_LightColour * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		m_PointLightShader->SetUniform3f("light.position",m_LightPosition);
		m_PointLightShader->SetUniform3f("light.ambient", ambientColor);
		m_PointLightShader->SetUniform3f("light.diffuse", diffuseColor);
		m_PointLightShader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		m_PointLightShader->SetUniform1f("light.constant", 1.0f);
		m_PointLightShader->SetUniform1f("light.linear", 0.09f);
		m_PointLightShader->SetUniform1f("light.quadratic", 0.032f);

		//Set the material
		m_PointLightShader->SetUniform1f("material.shininess", 32.0f);
		m_PointLightShader->SetUniform1i("material.diffuse", 0);
		m_PointLightShader->SetUniform1i("material.specular", 1);
		m_PointLightShader->SetUniform3f("viewPos", pointLight_Camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = pointLight_Camera.GetViewMatrix();
		m_PointLightShader->SetUniformMat4f("projection", projection);
		m_PointLightShader->SetUniformMat4f("view", view);

		// world transformation
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(m_Angle * i), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, pointLightcubePositions[i]);
			m_PointLightShader->SetUniformMat4f("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}

	void TestPointLight::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Direction", &m_LightPosition.x, -2.0f, 2.0f);
	}

	void TestPointLight::OnInput(GLFWwindow* window)
	{
		pointLight_processInput(window);
		//glfwSetCursorPosCallback(window, pointLight_mouse_callback);
		// tell GLFW to capture our mouse
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void pointLight_processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pointLight_Camera.ProcessKeyboard(FORWARD, pointLight_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pointLight_Camera.ProcessKeyboard(BACKWARD, pointLight_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pointLight_Camera.ProcessKeyboard(LEFT, pointLight_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pointLight_Camera.ProcessKeyboard(RIGHT, pointLight_DeltaTime);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void pointLight_mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (pointLight_firstMouse)
		{
			pointLight_lastX = xpos;
			pointLight_lastY = ypos;
			pointLight_firstMouse = false;
		}

		float xoffset = xpos - pointLight_lastX;
		float yoffset = pointLight_lastY - ypos; // reversed since y-coordinates go from bottom to top

		pointLight_lastX = xpos;
		pointLight_lastY = ypos;

		pointLight_Camera.ProcessMouseMovement(xoffset, yoffset);
	}
}
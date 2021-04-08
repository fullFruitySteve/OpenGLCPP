#include "TestMultipleLights.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	bool miltipleLights_ShowMouse = false;
	bool multipleLights_firstMouse = true;
	float multipleLights_lastX = 800 / 2.0f;
	float multipleLights_lastY = 600 / 2.0f;
	//timing
	float multipleLights_DeltaTime = 0.0f;
	Camera multipleLights_Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	void multipleLights_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void multipleLights_processInput(GLFWwindow *window);

	TestMultipleLights::TestMultipleLights() : m_Angle(0.0f)
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

		m_MultipleLightsShader = std::make_unique<Shader>("res/shaders/multipleLights.shader");
		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");//TODO: additional?
		m_DiffuseMap->Bind();
		m_SpecularMap->Bind(1);
		m_MultipleLightsShader->Bind();
		m_MultipleLightsShader->SetUniform1f("material.shininess", 32.0f);
		m_MultipleLightsShader->SetUniform1i("material.diffuse", 0);
		m_MultipleLightsShader->SetUniform1i("material.specular", 1);

		// positions all containers
		m_CubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
		m_CubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
		m_CubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
		m_CubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
		m_CubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
		m_CubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
		m_CubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
		m_CubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
		m_CubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
		m_CubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);
		// positions of the point lights
		m_PointLightPositions[0] = glm::vec3(0.7f, 0.2f, 2.0f);
		m_PointLightPositions[1] = glm::vec3(2.3f, -3.3f, -4.0f);
		m_PointLightPositions[2] = glm::vec3(-4.0f, 2.0f, -12.0f);
		m_PointLightPositions[3] = glm::vec3(0.0f, 0.0f, -3.0f);
	}

	TestMultipleLights::~TestMultipleLights()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestMultipleLights::OnUpdate(float deltaTime)
	{
		m_Angle = (float)glfwGetTime() * -20.0f;
		multipleLights_DeltaTime = deltaTime;
	}

	void TestMultipleLights::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_MultipleLightsShader->SetUniform3f("viewPos", multipleLights_Camera.Position);
		//Directional Light
		m_MultipleLightsShader->SetUniform3f("dirLight.direction", -0.2f, 1.0f, -0.3f);
		m_MultipleLightsShader->SetUniform3f("dirLight.ambient", 0.01f, 0.01f, 0.01f);
		m_MultipleLightsShader->SetUniform3f("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
		m_MultipleLightsShader->SetUniform3f("dirLight.specular", 0.1f, 0.1f, 0.1f);
		//Point Light 1
		m_MultipleLightsShader->SetUniform3f("pointLights[0].position", m_PointLightPositions[0]);
		m_MultipleLightsShader->SetUniform3f("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
		m_MultipleLightsShader->SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		m_MultipleLightsShader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[0].constant", 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[0].linear", 0.09f);
		m_MultipleLightsShader->SetUniform1f("pointLights[0].quadratic", 0.032f);
		//Point Light 2
		m_MultipleLightsShader->SetUniform3f("pointLights[1].position", m_PointLightPositions[1]);
		m_MultipleLightsShader->SetUniform3f("pointLights[1].ambient", 0.5f, 0.5f, 0.5f);
		m_MultipleLightsShader->SetUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		m_MultipleLightsShader->SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[1].constant", 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[1].linear", 0.09f);
		m_MultipleLightsShader->SetUniform1f("pointLights[1].quadratic", 0.032f);
		//Point Light 3
		m_MultipleLightsShader->SetUniform3f("pointLights[2].position", m_PointLightPositions[2]);
		m_MultipleLightsShader->SetUniform3f("pointLights[2].ambient", 0.5f, 0.5f, 0.5f);
		m_MultipleLightsShader->SetUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		m_MultipleLightsShader->SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[2].constant", 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[2].linear", 0.09f);
		m_MultipleLightsShader->SetUniform1f("pointLights[2].quadratic", 0.032f);
		//Point Light 4
		m_MultipleLightsShader->SetUniform3f("pointLights[3].position", m_PointLightPositions[3]);
		m_MultipleLightsShader->SetUniform3f("pointLights[3].ambient", 0.5f, 0.5f, 0.5f);
		m_MultipleLightsShader->SetUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		m_MultipleLightsShader->SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[3].constant", 1.0f);
		m_MultipleLightsShader->SetUniform1f("pointLights[3].linear", 0.09f);
		m_MultipleLightsShader->SetUniform1f("pointLights[3].quadratic", 0.032f);
		//Spotlight
		m_MultipleLightsShader->SetUniform3f("spotLight.position", multipleLights_Camera.Position);
		m_MultipleLightsShader->SetUniform3f("spotLight.direction", multipleLights_Camera.Front);
		m_MultipleLightsShader->SetUniform3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_MultipleLightsShader->SetUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		m_MultipleLightsShader->SetUniform1f("spotLight.constant", 1.0f);
		m_MultipleLightsShader->SetUniform1f("spotLight.linear", 0.09f);
		m_MultipleLightsShader->SetUniform1f("spotLight.quadratic", 0.032f);
		m_MultipleLightsShader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_MultipleLightsShader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = multipleLights_Camera.GetViewMatrix();
		m_MultipleLightsShader->SetUniformMat4f("projection", projection);
		m_MultipleLightsShader->SetUniformMat4f("view", view);

		// world transformation
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(m_Angle * i), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, m_CubePositions[i]);
			m_MultipleLightsShader->SetUniformMat4f("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}

	void TestMultipleLights::OnImGuiRender()
	{
		//ImGui::SliderFloat3("Light Direction", &m_PointLightPositions[0].x, -3, 3);
	}

	void TestMultipleLights::OnInput(GLFWwindow* window)
	{
		multipleLights_processInput(window);
		glfwSetCursorPosCallback(window, multipleLights_mouse_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void multipleLights_processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			multipleLights_Camera.ProcessKeyboard(FORWARD, multipleLights_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			multipleLights_Camera.ProcessKeyboard(BACKWARD, multipleLights_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			multipleLights_Camera.ProcessKeyboard(LEFT, multipleLights_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			multipleLights_Camera.ProcessKeyboard(RIGHT, multipleLights_DeltaTime);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void multipleLights_mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (multipleLights_firstMouse)
		{
			multipleLights_lastX = xpos;
			multipleLights_lastY = ypos;
			multipleLights_firstMouse = false;
		}

		float xoffset = xpos - multipleLights_lastX;
		float yoffset = multipleLights_lastY - ypos; // reversed since y-coordinates go from bottom to top

		multipleLights_lastX = xpos;
		multipleLights_lastY = ypos;

		multipleLights_Camera.ProcessMouseMovement(xoffset, yoffset);
	}
}
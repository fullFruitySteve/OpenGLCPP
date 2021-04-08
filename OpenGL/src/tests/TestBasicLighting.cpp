#include "TestBasicLighting.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "camera.h"
namespace test
{
	void basicLighting_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void basicLighting_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void basicLightingProcessInput(GLFWwindow *window);
	// camera
	Camera basicLightingCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	float basicLighting_lastX = 800 / 2.0f;
	float basicLighting_lastY = 600 / 2.0f;
	bool basicLighting_firstMouse = true;
	//timing
	float basicLightingDeltaTime = 0.0f;

	TestBasicLighting::TestBasicLighting()
		: m_LightPos(glm::vec3(0.2f, 1.0f, 2.0f)), m_ObjectColour(glm::vec3(1.0f, 0.5f, 0.31f)), m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f)), m_Radius(3.0f)
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		m_CubeVAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 36 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); //push 3 elements (x,y,z)
		layout.Push<float>(3); //push 3 elements (normals)
		m_CubeVAO->AddBuffer(*m_VertexBuffer, layout);

		m_LightCubeVAO = std::make_unique<VertexArray>();
		m_LightCubeVAO->AddBuffer(*m_VertexBuffer, layout);
		m_ColourShader = std::make_unique<Shader>("res/shaders/basicLighting.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shaders/lightCube.shader");
	}

	TestBasicLighting::~TestBasicLighting()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestBasicLighting::OnUpdate(float deltaTime)
	{
		basicLightingDeltaTime = deltaTime;
		m_LightPos.x = sin(glfwGetTime()) * m_Radius;
		m_LightPos.z = cos(glfwGetTime()) * m_Radius;
	}

	void TestBasicLighting::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_ColourShader->Bind();
		m_ColourShader->SetUniform3f("objectColor", m_ObjectColour);
		m_ColourShader->SetUniform3f("lightColor", m_LightColour);
		m_ColourShader->SetUniform3f("lightPos", m_LightPos);
		m_ColourShader->SetUniform3f("viewPos", basicLightingCamera.Position);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(basicLightingCamera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = basicLightingCamera.GetViewMatrix();
		m_ColourShader->SetUniformMat4f("projection", projection);
		m_ColourShader->SetUniformMat4f("view", view);
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		m_ColourShader->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// also draw the lamp object
		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMat4f("projection", projection);
		m_LightCubeShader->SetUniformMat4f("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		m_LightCubeShader->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestBasicLighting::OnImGuiRender()
	{

	}

	void TestBasicLighting::OnInput(GLFWwindow* window)
	{
		basicLightingProcessInput(window);
		glfwSetCursorPosCallback(window, basicLighting_mouse_callback);
		glfwSetScrollCallback(window, basicLighting_scroll_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void basicLightingProcessInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			basicLightingCamera.ProcessKeyboard(FORWARD, basicLightingDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			basicLightingCamera.ProcessKeyboard(BACKWARD, basicLightingDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			basicLightingCamera.ProcessKeyboard(LEFT, basicLightingDeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			basicLightingCamera.ProcessKeyboard(RIGHT, basicLightingDeltaTime);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void basicLighting_mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (basicLighting_firstMouse)
		{
			basicLighting_lastX = xpos;
			basicLighting_lastY = ypos;
			basicLighting_firstMouse = false;
		}

		float xoffset = xpos - basicLighting_lastX;
		float yoffset = basicLighting_lastY - ypos; // reversed since y-coordinates go from bottom to top

		basicLighting_lastX = xpos;
		basicLighting_lastY = ypos;

		basicLightingCamera.ProcessMouseMovement(xoffset, yoffset);
	}
	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
	void basicLighting_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		basicLightingCamera.ProcessMouseScroll(yoffset);
	}
}
#include "TestColour.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "camera.h"

namespace test
{
	// camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.5f));

	TestColour::TestColour() 
		: m_LightPos(glm::vec3(0.2f, 0.5f, 2.0f)), m_ObjectColour(glm::vec3(1.0f, 0.5f, 0.31f)), m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f))
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
		};

		m_CubeVAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 3 * 36 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); //push 3 elements (x,y,z)
		m_CubeVAO->AddBuffer(*m_VertexBuffer, layout);

		m_LightCubeVAO = std::make_unique<VertexArray>();
		m_LightCubeVAO->AddBuffer(*m_VertexBuffer, layout);
		m_ColourShader = std::make_unique<Shader>("res/shaders/colours.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shaders/lightCube.shader");
	}

	TestColour::~TestColour()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestColour::OnUpdate(float deltaTime)
	{
	}

	void TestColour::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_ColourShader->Bind();
		m_ColourShader->SetUniform3f("objectColor", m_ObjectColour);
		m_ColourShader->SetUniform3f("lightColor", m_LightColour);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
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

	void TestColour::OnImGuiRender()
	{
		ImGui::ColorEdit3("Object color", &m_ObjectColour.x);
		ImGui::ColorEdit3("Light color", &m_LightColour.x);
	}

	void TestColour::OnInput(GLFWwindow * window)
	{
	}
}
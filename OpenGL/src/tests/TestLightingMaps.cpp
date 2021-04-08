#include "TestLightingMaps.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	TestLightingMaps::TestLightingMaps()
		: m_LightPos(glm::vec3(1.2f, 1.0f, 2.0f)), m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f)), m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)), m_Angle(0.0f)
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

		m_LightCubeVAO = std::make_unique<VertexArray>();
		m_LightCubeVAO->AddBuffer(*m_VertexBuffer, layout);
		m_LightingMapsShader = std::make_unique<Shader>("res/shaders/lightingMaps.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shaders/lightCube.shader");
		m_DiffuseMap = std::make_unique<Texture>("res/textures/container2.png");
		m_SpecularMap = std::make_unique<Texture>("res/textures/container2_specular.png");
		m_EmissionMap = std::make_unique<Texture>("res/textures/matrix.jpg");
		m_DiffuseMap->Bind();
		m_SpecularMap->Bind(1);
		m_EmissionMap->Bind(2);
	}

	TestLightingMaps::~TestLightingMaps()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestLightingMaps::OnUpdate(float deltaTime)
	{
		m_Angle = (float)glfwGetTime() * -25.0f;
	}

	void TestLightingMaps::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Set the light
		m_LightingMapsShader->Bind();
		m_LightingMapsShader->SetUniform3f("light.position", m_LightPos);
		glm::vec3 diffuseColor = m_LightColour * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		m_LightingMapsShader->SetUniform3f("light.ambient", ambientColor);
		m_LightingMapsShader->SetUniform3f("light.diffuse", diffuseColor);
		m_LightingMapsShader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

		//Set the material
		m_LightingMapsShader->SetUniform1f("material.shininess", 64.0f);
		m_LightingMapsShader->SetUniform1i("material.diffuse", 0);
		m_LightingMapsShader->SetUniform1i("material.specular", 1);
		m_LightingMapsShader->SetUniform1i("material.emission", 2);
		m_LightingMapsShader->SetUniform3f("viewPos", m_Camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = m_Camera.GetViewMatrix();
		m_LightingMapsShader->SetUniformMat4f("projection", projection);
		m_LightingMapsShader->SetUniformMat4f("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_LightingMapsShader->SetUniformMat4f("model", model);
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

	void TestLightingMaps::OnImGuiRender()
	{

	}

	void TestLightingMaps::OnInput(GLFWwindow* window)
	{

	}
}
#include "TestMaterial.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	//Playing with materials http://devernay.free.fr/cours/opengl/materials.html

	TestMaterial::TestMaterial()
		: m_LightPos(glm::vec3(0.2f, 1.0f, 2.0f)), m_LightColour(glm::vec3(1.0f, 1.0f, 1.0f)), m_Radius(3.0f), m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_Ambient(glm::vec3(1.0f, 0.5f, 0.31f)), m_Diffuse(glm::vec3(1.0f, 0.5f, 0.31f)), m_Specular(glm::vec3(1.0f, 1.0f, 1.0f)), m_Shininess(32.0f),
		m_Animate(false)
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
		m_MaterialShader = std::make_unique<Shader>("res/shaders/material.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shaders/lightCube.shader");
	}

	TestMaterial::~TestMaterial()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestMaterial::OnUpdate(float deltaTime)
	{
		if (m_Animate)
		{
			m_LightColour.x = sin(glfwGetTime() * 2.0f);
			m_LightColour.y = sin(glfwGetTime() * 0.7f);
			m_LightColour.z = sin(glfwGetTime() * 1.3f);
			m_LightPos.x = sin(glfwGetTime()) * m_Radius;
			m_LightPos.z = cos(glfwGetTime()) * m_Radius;
		}
		else
		{
			m_LightColour = glm::vec3(1.0f, 1.0f, 1.0f);
			m_LightPos = glm::vec3(0.2f, 1.0f, 2.0f);
		}
	}

	void TestMaterial::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Set the light
		m_MaterialShader->Bind();
		m_MaterialShader->SetUniform3f("light.position", m_LightPos);
		glm::vec3 diffuseColor = m_LightColour * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		m_MaterialShader->SetUniform3f("light.ambient", ambientColor);
		m_MaterialShader->SetUniform3f("light.diffuse", diffuseColor);
		m_MaterialShader->SetUniform3f("light.specular", m_Specular);

		//Set the material
		m_MaterialShader->SetUniform3f("material.ambient", m_Ambient);
		m_MaterialShader->SetUniform3f("material.diffuse", m_Diffuse);
		m_MaterialShader->SetUniform3f("material.specular", m_Specular);
		m_MaterialShader->SetUniform1f("material.shininess", m_Shininess);
		m_MaterialShader->SetUniform3f("viewPos", m_Camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = m_Camera.GetViewMatrix();
		m_MaterialShader->SetUniformMat4f("projection", projection);
		m_MaterialShader->SetUniformMat4f("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, cubePositions[i]);
		m_MaterialShader->SetUniformMat4f("model", model);
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

	void TestMaterial::OnImGuiRender()
	{
		ImGui::Checkbox("Animate", &m_Animate);
		ImGui::ColorEdit4("Light Colour", &m_LightColour.x);
		ImGui::SliderFloat3("Ambient", &m_Ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse", &m_Diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", &m_Specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Shinines", &m_Shininess, 0.0f, 32.0f);
	}

	void TestMaterial::OnInput(GLFWwindow* window)
	{

	}
}
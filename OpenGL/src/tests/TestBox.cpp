#include "TestBox.h"
#include "Renderer.h"
#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	TestBox::TestBox()
		: m_TranslationA(200, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float vertices[] = {
			// positions					 // colors				// texture coords
			 200.0f,  200.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 200.0f, -200.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-200.0f, -200.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-200.0f,  200.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); //push 3 elements (x,y,z)
		layout.Push<float>(3); //push 3 elements (r,g,b)
		layout.Push<float>(2); //push 2 elements (s,t)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		m_Shader = std::make_unique<Shader>("res/shaders/box.shader");
		m_Shader->Bind();
		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);

		m_Texture = std::make_unique<Texture>("res/textures/box.jpg");
		m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
		rotationAngle = 0;
		scale = 1;
	}

	TestBox::~TestBox(){}

	void TestBox::OnUpdate(float deltaTime) 
	{
		if (animate)
		{
			if (rotationAngle > 360.0f)
				rotationAngle = 0;
			rotationAngle += 1.5f;

			if (mixValue > 1.0f)
				mixValue = 0;
			if (mixValue < 0.0f)
				mixValue = 1.0f;
			mixValue += 0.01f;
		}
	}

	void TestBox::OnRender()
	{
		m_Shader->SetUniform1f("mixValue", mixValue);

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f););
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();
		m_Texture2->Bind(1);

		//update matracies each frame
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 MVP = m_Proj * m_View * model;
			MVP = glm::rotate(MVP, glm::radians(rotationAngle), glm::vec3(0.0, 0.0, 1.0));
			MVP = glm::scale(MVP, glm::vec3(scale, scale, scale));
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", MVP);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestBox::OnImGuiRender()
	{
		ImGui::Checkbox("Animate", &animate);
		ImGui::SliderFloat3("translation", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat("Mix Value", &mixValue, 0.0f, 1.0f);
		ImGui::SliderFloat("Rotation", &rotationAngle, 0.0f, 360.0f);
		ImGui::SliderFloat("Scale", &scale, -1.0f, 2.0f);
	}
	void TestBox::OnInput(GLFWwindow * window)
	{
	}
}
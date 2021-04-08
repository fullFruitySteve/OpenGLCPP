#include "TestTexture2D.h"
#include "Renderer.h"
#include "imgui\imgui.h"

//#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"
//#include "Texture.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0), 
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positions[] =
		{
			-100.0f, -100.0f, 0.0f, 0.0f,
			 100.0f, -100.0f, 1.0f, 0.0f,
			 100.0f, 100.0f, 1.0f, 1.0f,
			 -100.0f, 100.0f, 0.0f, 1.0f
		};

		unsigned int indicies[] =
		{
			0,1,2,
			2,3,0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); //push 2 elements (x,y)
		layout.Push<float>(2); //push 2 elements (s,t)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6);
		m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);

		m_Texture = std::make_unique<Texture>("res/textures/meme");
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		//update matracies each frame
		//Rendering obj1
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 MVP = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", MVP);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		//rendering obj2
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 MVP = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", MVP);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("translation", &m_TranslationA.x, 0.0f, 960.0f);      
		ImGui::SliderFloat3("translation2", &m_TranslationB.x, 0.0f, 960.0f);// Edit 3 floats using a slider from 0.0f to 960.0f
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	void TestTexture2D::OnInput(GLFWwindow * window)
	{
	}
}
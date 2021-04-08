#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include <memory>

namespace test
{
	class TestBox : public Test
	{
	public:
		TestBox();
		~TestBox();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture>m_Texture2;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		glm::vec3 m_TranslationA;
		glm::mat4 m_Proj, m_View;

		float mixValue;
		float rotationAngle;
		float scale;
		bool animate;
	};
}
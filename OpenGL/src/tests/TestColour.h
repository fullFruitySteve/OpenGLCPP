#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <memory>

namespace test
{
	class TestColour : public Test
	{
	public:
		TestColour();
		~TestColour();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

	private:
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexArray> m_LightCubeVAO;
		std::unique_ptr<Shader> m_ColourShader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		glm::vec3 m_LightPos;
		glm::vec3 m_ObjectColour;
		glm::vec3 m_LightColour;
	};
}
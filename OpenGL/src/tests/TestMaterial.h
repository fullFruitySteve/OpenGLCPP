#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <memory>
#include "camera.h"

namespace test
{
	class TestMaterial : public Test
	{
	public:
		TestMaterial();
		~TestMaterial();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

	private:
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexArray> m_LightCubeVAO;
		std::unique_ptr<Shader> m_MaterialShader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		Camera m_Camera;
		glm::vec3 m_LightPos;
		glm::vec3 m_LightColour;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;
		float m_Radius;
		bool m_Animate;
	};
}
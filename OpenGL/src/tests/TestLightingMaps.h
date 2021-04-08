#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <memory>
#include "Texture.h"
#include "camera.h"

namespace test
{
	class TestLightingMaps : public Test
	{
	public:
		TestLightingMaps();
		~TestLightingMaps();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

	private:
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexArray> m_LightCubeVAO;
		std::unique_ptr<Shader> m_LightingMapsShader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Texture> m_DiffuseMap, m_SpecularMap, m_EmissionMap;

		Camera m_Camera;
		glm::vec3 m_LightPos;
		glm::vec3 m_LightColour;
		float m_Angle;
	};
}
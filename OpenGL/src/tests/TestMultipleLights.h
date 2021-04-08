#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <memory>
#include "Texture.h"
#include "camera.h"

namespace test
{
	class TestMultipleLights : public Test
	{
	public:
		TestMultipleLights();
		~TestMultipleLights();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

	private:
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<Shader> m_MultipleLightsShader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Texture> m_DiffuseMap, m_SpecularMap;

		glm::vec3 m_PointLightPositions[4];
		glm::vec3 m_CubePositions[10];
		float m_Angle;
	};
}
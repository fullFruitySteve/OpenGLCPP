#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include <memory>

namespace test
{
	class TestCube : public Test
	{
	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInput(GLFWwindow* window) override;

		float m_Zoom;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture, m_Texture2;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		Renderer renderer;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraTarget;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_Up;
		glm::vec3 m_CameraRight;
		glm::vec3 m_CameraUp;
		float m_Radius;
		float m_CamX;
		float m_CamZ;
		float m_CameraSpeed;
		float m_DeltaTime;
	};
}
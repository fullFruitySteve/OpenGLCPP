#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "tests\TestClearColor.h"
#include "tests\TestTexture2D.h"
#include "tests\TestBox.h"
#include "tests\TestCube.h"
#include "tests\TestColour.h"
#include "tests\TestBasicLighting.h"
#include "tests\TestMaterial.h"
#include "tests\TestLightingMaps.h"
#include "tests\TestDirectionalLight.h"
#include "tests\TestPointLight.h"
#include "tests\TestSpotLight.h"
#include"tests/TestMultipleLights.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	 GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	//Setup for ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "error initialising GLEW" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("Texture2D");
		testMenu->RegisterTest<test::TestBox>("Box Test");
		testMenu->RegisterTest<test::TestCube>("3D Cube Test");
		testMenu->RegisterTest<test::TestColour>("Colour Test");
		testMenu->RegisterTest<test::TestBasicLighting>("Basic Lighting");
		testMenu->RegisterTest<test::TestMaterial>("Material Test");
		testMenu->RegisterTest<test::TestLightingMaps>("Lighting Map");
		testMenu->RegisterTest<test::TestDirectionalLight>("Directional Light");
		testMenu->RegisterTest<test::TestPointLight>("Point Light");
		testMenu->RegisterTest<test::TestSpotLight>("Spot Light");
		testMenu->RegisterTest<test::TestMultipleLights>("Multiple Lights");

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(deltaTime);
				currentTest->OnRender();
				currentTest->OnInput(window);

				ImGui::Begin("Test");

				if (currentTest != testMenu && ImGui::Button("<--"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			/* Poll for and process events */
			glfwPollEvents();
		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
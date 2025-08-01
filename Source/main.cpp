#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Swapchain.h"
#include "Window/Window.h"
#define VOLK_IMPLEMENTATION
#include "volk.h"

// Main code
int main(int argc, const char** argv)
{
	gran::DeviceCreationData creationData { .m_AppName = "Granulite", .m_UseValidationLayer = true, };
	gran::Device device;
	gran::DeviceQueue deviceQueue;
	gran::Swapchain swapchain;

	GLFWwindow* window = gran::Window::CreateWindowGLFW(creationData);
	assert(window);

	gran::Window::InitializeWindowRHI(*window, creationData, device, deviceQueue);

	double prevTime = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		glfwPollEvents();

		gran::Window::UpdateWindowSwapchain(*window, creationData, device, deviceQueue, swapchain);

	}

	gran::Window::CleanupWindowRHI(*window, device, swapchain);
	gran::Window::DestroyWindow(*window);
	

	return 0;
}

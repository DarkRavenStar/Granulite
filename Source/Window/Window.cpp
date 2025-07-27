#include "Window/Window.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Swapchain.h"
#include "VkBootstrap.h"
#include "glfw/glfw3.h"
#include "volk.h"

namespace
{
	static void GLFW_ErrorCallback(int error, const char* description)
	{
		printf("GLFW Error %d: %s\n", error, description);
	}
} // namespace

GLFWwindow* gran::Window::CreateWindowGLFW(const gran::DeviceCreationData& creationData)
{
	glfwSetErrorCallback(GLFW_ErrorCallback);
	if (!glfwInit())
	{
		return nullptr;
	}

	if (!glfwVulkanSupported())
	{
		printf("GLFW: Vulkan Not Supported\n");
		return nullptr;
	}

	// Create window with Vulkan context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1280, 720, creationData.m_AppName.c_str(), nullptr, nullptr);

	return window;
}

void gran::Window::InitializeWindowRHI(
    GLFWwindow& window,
    const gran::DeviceCreationData& creationData,
    gran::Device& out_Device,
    gran::DeviceQueue& out_DeviceQueue)
{
	VK_CHECK(volkInitialize());

	vkb::Instance instance = gran::RHI::SetupInstance(creationData, out_Device, out_DeviceQueue);

	volkLoadInstanceOnly(out_Device.m_Instance);

	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(out_Device.m_Instance, &window, nullptr, &out_Device.m_Surface);
	VK_CHECK(err);

	gran::RHI::SetupDevice(instance, out_Device, out_DeviceQueue);

	volkLoadDevice(out_Device.m_Device);

	/*
	std::vector<const char*> extensions;
	uint32_t extensions_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
	for (uint32_t i = 0; i < extensions_count; i++)
	{
	    extensions.push_back(glfw_extensions[i]);
	}
	*/
	// SetupVulkan(extensions);

	// Create Window Surface

	// Create Framebuffers
	// int w, h;
	// glfwGetFramebufferSize(window, &w, &h);
	// ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
	// SetupVulkanWindow(wd, surface, w, h);
}

void gran::Window::CleanupWindowRHI(GLFWwindow& window, gran::Device& device, gran::Swapchain& swapchain)
{
	VK_CHECK(vkDeviceWaitIdle(device.m_Device));

	gran::RHI::DestroySwapchain(device.m_Device, swapchain.m_Swapchain, swapchain.m_SwapchainImageViews);

	vkDestroySurfaceKHR(device.m_Instance, device.m_Surface, nullptr);

	vkDestroyDevice(device.m_Device, nullptr);

	if (device.m_DebugMessenger)
	{
		vkDestroyDebugUtilsMessengerEXT(device.m_Instance, device.m_DebugMessenger, nullptr);
	}

	vkDestroyInstance(device.m_Instance, nullptr);

	volkFinalize();
}

gran::SwapchainWindowState gran::Window::UpdateWindowSwapchain(
    GLFWwindow& window,
    const gran::DeviceCreationData& deviceCreationData,
    const gran::Device& device,
    const gran::DeviceQueue& deviceQueue,
    gran::Swapchain& swapchain)
{
	// Instead of hooks, we using polling method instead
	int width = 0, height = 0;
	glfwGetFramebufferSize(&window, &width, &height);

	if (width == 0 || height == 0)
	{
		return gran::SwapchainWindowState::Minimized;
	}

	VkExtent2D windowSize { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	if (swapchain.m_SwapchainExtent.width == windowSize.width &&
	    swapchain.m_SwapchainExtent.height == windowSize.height)
	{
		return gran::SwapchainWindowState::NoChange;
	}

	vkDeviceWaitIdle(device.m_Device);

	VkSwapchainKHR oldSwapchain = swapchain.m_Swapchain;
	std::vector<VkImageView> oldSwapchainImageViews = std::move(swapchain.m_SwapchainImageViews);

	gran::RHI::CreateSwapchain(deviceCreationData, device, deviceQueue, oldSwapchain, windowSize, swapchain);

	gran::RHI::DestroySwapchain(device.m_Device, oldSwapchain, oldSwapchainImageViews);

	return gran::SwapchainWindowState::Resized;
}

void gran::Window::DestroyWindow(GLFWwindow& window)
{
	glfwDestroyWindow(&window);
	glfwTerminate();
}

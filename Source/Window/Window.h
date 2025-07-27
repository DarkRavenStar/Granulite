#pragma once

// Forward declaration
typedef struct GLFWwindow GLFWwindow;

namespace gran
{
	struct DeviceCreationData;
	struct Device;
	struct DeviceQueue;
	struct Swapchain;
} // namespace gran

namespace gran
{
	enum SwapchainWindowState
	{
		NoChange,
		Minimized,
		Resized,
	};
} // namespace gran

namespace gran::Window
{
	// Order of initialization
	// - CreateWindowGLFW
	// - InitializeWindowRHI
	// - UpdateWindowSwapchain - at start and update loop
	// - CleanupWindowRHI
	// - DestroyWindow

	GLFWwindow* CreateWindowGLFW(const gran::DeviceCreationData& creationData);

	void InitializeWindowRHI(
	    GLFWwindow& window,
	    const gran::DeviceCreationData& creationData,
	    gran::Device& out_Device,
	    gran::DeviceQueue& out_DeviceQueue);

	void CleanupWindowRHI(GLFWwindow& window, gran::Device& device, gran::Swapchain& swapchain);

	gran::SwapchainWindowState UpdateWindowSwapchain(
	    GLFWwindow& window,
	    const gran::DeviceCreationData& deviceCreationData,
	    const gran::Device& device,
	    const gran::DeviceQueue& deviceQueue,
	    gran::Swapchain& swapchain);

	void DestroyWindow(GLFWwindow& window);

} // namespace gran::Window

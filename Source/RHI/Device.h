#pragma once

#include <string>
#include "vulkan/vulkan.h"
#include "VkBootstrap.h"
#include "glfw/glfw3.h"

struct DeviceCreationData
{
	std::string m_AppName;
	bool m_UseValidationLayer;
};

struct Device
{
	VkInstance m_Instance = {};                     // Vulkan library handle
	VkDebugUtilsMessengerEXT m_DebugMessenger = {}; // Vulkan debug output handle
	VkPhysicalDevice m_PhysicalDevice = {};         // GPU chosen as the default device
	VkDevice m_Device = {};                         // Vulkan device for commands
	VkSurfaceKHR m_Surface = {};                    // Vulkan window surface
};

// Replicated and modified from vkbootstrap
enum class QueueType
{
	present,
	graphics,
	compute,
	transfer,
	max
};

struct QueueData
{
	VkQueue m_QueueHandle = {};
	uint32_t m_QueueFamily = {};
};

struct DeviceQueue
{
	QueueData m_Queue[static_cast<uint32_t>(QueueType::max)];
};


namespace gran::RHI
{
	//Order of initialization
	//SetupInstance
	//Create Window/Initialize Surface first
	//SetupDevice

	vkb::Instance SetupInstance(const DeviceCreationData& creationData, Device& out_Device, DeviceQueue& out_DeviceQueue);

	void SetupDevice(const vkb::Instance& instance, Device& out_Device, DeviceQueue& out_DeviceQueue);

} // namespace gran::RHI

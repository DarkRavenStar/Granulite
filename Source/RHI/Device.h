#pragma once

#include "vulkan/vulkan.h"
#include <string>

// Forward declaration
namespace vkb
{
	struct Instance;
}

namespace gran
{
	struct DeviceCreationData
	{
		std::string m_AppName;
		bool m_UseValidationLayer;
		bool m_UseVSync;
		uint32_t m_MaxFramesInFlight = 2;
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
	// Instead of enum class, we are using this
	// for enumeration instead. So not strongly typed
	// or need long static_cast declaration
	enum QueueType
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
		QueueData m_Queue[QueueType::max];
	};
} // namespace gran

namespace gran::RHI::Device
{
	// Order of initialization
	// - SetupInstance
	// - Create Window/Initialize Surface first
	// - SetupDevice

	vkb::Instance SetupInstance(const gran::DeviceCreationData& creationData, gran::Device& out_Device, gran::DeviceQueue& out_DeviceQueue);

	void SetupDevice(const vkb::Instance& instance, gran::Device& out_Device, gran::DeviceQueue& out_DeviceQueue);

} // namespace gran::RHI::Device

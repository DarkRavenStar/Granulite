#include "Device.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define API_VERSION VK_API_VERSION_1_4

vkb::Instance gran::RHI::SetupInstance(const DeviceCreationData& creationData, Device& out_Device, DeviceQueue& out_DeviceQueue)
{
	vkb::InstanceBuilder builder;

	builder.set_app_name(creationData.m_AppName.c_str());
	builder.request_validation_layers(creationData.m_UseValidationLayer);
	//"VK_LAYER_KHRONOS_validation"
	// builder.add_validation_feature_enable(VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT)
	//.add_validation_feature_enable(VkValidationFeatureEnableEXT enable)
	//.add_validation_feature_disable(VkValidationFeatureDisableEXT disable)
	//enable_extension_features_if_present
	builder.use_default_debug_messenger();
	builder.require_api_version(1, 3, 0);

	auto createInstanceResult = builder.build();
	vkb::Instance instance = createInstanceResult.value();

	out_Device.m_Instance = instance.instance;
	out_Device.m_DebugMessenger = instance.debug_messenger;

	return instance;
}

void gran::RHI::SetupDevice(const vkb::Instance& instance, Device& out_Device, DeviceQueue& out_DeviceQueue)
{
	// Teena - RESEARCH WHAT EACH DEVICE FEATURE IS IF YOU HAVE TIME
	// Currently we are targetting GPU Driven rendering stuff only
	// and minimal features to achieve it

	static std::vector<const char*> requiredExtensions {
		// VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, //Useful for raytracing
		// VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME, //Useful for raytracing
		// VK_KHR_RAY_QUERY_EXTENSION_NAME, //Useful for raytracing
		// VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME, //Available in core 1.2 and above
		VK_EXT_MESH_SHADER_EXTENSION_NAME, // Assuming mesh shader is available, if not fallback to compute
	};

	VkPhysicalDeviceFeatures features{};
	features.independentBlend = true; // Can do custom blending?
	features.geometryShader = true; // for mesh shaders?
	features.multiDrawIndirect = true;
	features.drawIndirectFirstInstance = true; // Not sure why this is here
	features.samplerAnisotropy = true;
	features.pipelineStatisticsQuery = true;
	features.shaderStorageImageWriteWithoutFormat = true; //???
	features.shaderFloat64 = true; //???
	features.shaderInt64 = true;   //???
	features.shaderInt16 = true;   //???
	// Allow for dynamic/ unordered access???
	features.shaderUniformBufferArrayDynamicIndexing = true;
	features.shaderSampledImageArrayDynamicIndexing = true;
	features.shaderStorageBufferArrayDynamicIndexing = true;
	features.shaderStorageImageArrayDynamicIndexing = true;
	features.shaderStorageImageArrayDynamicIndexing = true;
	
		
	// vulkan 1.1 features
	VkPhysicalDeviceVulkan11Features features11{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
	features11.storageBuffer16BitAccess = true;
	features11.shaderDrawParameters = true;

	// vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;
	//features12.hostQueryReset = true, // Seems like needed for profilling
	// Enabled for experimentation
	features12.descriptorBindingPartiallyBound = true;
	features12.descriptorBindingPartiallyBound = true;
	features12.descriptorBindingSampledImageUpdateAfterBind = true;
	features12.descriptorBindingUpdateUnusedWhilePending = true;
	features12.descriptorBindingVariableDescriptorCount = true;

	features12.drawIndirectCount = true;
	features12.runtimeDescriptorArray = true;
	features12.samplerFilterMinmax = true;
	features12.scalarBlockLayout = true;

	features12.shaderBufferInt64Atomics = true;
	features12.shaderFloat16 = true;
	features12.shaderInt8 = true;
	features12.shaderInt8 = true;

	features12.shaderSampledImageArrayNonUniformIndexing = true;
	features12.shaderSampledImageArrayNonUniformIndexing = true;
	features12.shaderStorageBufferArrayNonUniformIndexing = true;

	features12.storageBuffer8BitAccess = true;
	features12.storagePushConstant8 = true;
	features12.uniformAndStorageBuffer8BitAccess = true;

	features12.timelineSemaphore = true;

	// vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features13 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features13.dynamicRendering = true; // Not sure if this is needed if GPU Driven
	features13.synchronization2 = true;
	features13.maintenance4 = true; // Not sure if needed

	// Not sure if needed
	VkPhysicalDeviceVulkan14Features features14 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES };
	features14.maintenance5 = true;
	features14.maintenance6 = true;
	features14.pushDescriptor = true;

	VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT };
	meshShaderFeatures.taskShader = true;
	meshShaderFeatures.meshShader = true;

	VkPhysicalDeviceRobustness2FeaturesEXT deviceRobustFeatures2 {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,
		.nullDescriptor = true,
	};

	VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shaderAtomicsFeatures {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT,
		.shaderImageInt64Atomics = true,
	};

	vkb::PhysicalDeviceSelector selector { instance };
	selector.set_minimum_version(1, 3);
	selector.add_required_extensions(requiredExtensions);
	selector.set_required_features(features);
	selector.add_required_extension_features(meshShaderFeatures);
	selector.add_required_extension_features(deviceRobustFeatures2);
	selector.add_required_extension_features(shaderAtomicsFeatures);
	selector.set_required_features_11(features11);
	selector.set_required_features_12(features12);
	selector.set_required_features_13(features13);
	selector.set_required_features_14(features14);
	selector.set_surface(out_Device.m_Surface);
	//selector.defer_surface_initialization();

	vkb::PhysicalDevice physicalDevice = selector.select().value();

	printf("Selected GPU %s\n", physicalDevice.name.c_str());

	vkb::DeviceBuilder deviceBuilder { physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	out_Device.m_Device = vkbDevice.device;
	out_Device.m_PhysicalDevice = physicalDevice.physical_device;

	// get_queue is used because overlapping queue is fine at the moment
	// unless we need a dedicated queue to help with async and parallelization
	for (uint32_t i = 0; i < static_cast<uint32_t>(QueueType::max); i++)
	{
		QueueData& queue = out_DeviceQueue.m_Queue[i];
		auto queueType = static_cast<vkb::QueueType>(i);
		queue.m_QueueHandle = vkbDevice.get_queue(queueType).value();
		queue.m_QueueFamily = vkbDevice.get_queue_index(queueType).value();
	}
}

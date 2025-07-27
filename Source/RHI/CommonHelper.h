#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//Please include these 2 files if needed before using CommonHelper.h
//#include "vulkan/vulkan.h"
//#include "volk.h"

#define API_VERSION VK_API_VERSION_1_4

#define VK_CHECK(call) \
	do \
	{ \
		VkResult result_ = call; \
		assert(result_ == VK_SUCCESS); \
	} while (0)

#define VK_CHECK_FORCE(call) \
	do \
	{ \
		VkResult result_ = call; \
		if (result_ != VK_SUCCESS) \
		{ \
			fprintf(stderr, "%s:%d: %s failed with error %d\n", __FILE__, __LINE__, #call, result_); \
			abort(); \
		} \
	} while (0)

#define VK_CHECK_SWAPCHAIN(call) \
	do \
	{ \
		VkResult result_ = call; \
		assert(result_ == VK_SUCCESS || result_ == VK_SUBOPTIMAL_KHR || result_ == VK_ERROR_OUT_OF_DATE_KHR); \
	} while (0)

#define VK_CHECK_QUERY(call) \
	do \
	{ \
		VkResult result_ = call; \
		assert(result_ == VK_SUCCESS || result_ == VK_NOT_READY); \
	} while (0)

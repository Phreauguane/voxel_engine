#pragma once
#include "config.h"

namespace vkInit
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	bool checkDeviceExtensionSupport(
		const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions,
		const bool& debug)
	{
		std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

		if (debug)
		{
			std::cout << "Device can support extensions:\n";
		}

		for (vk::ExtensionProperties& ext : device.enumerateDeviceExtensionProperties())
		{
			if (debug)
			{
				std::cout << "\t\"" << ext.extensionName << "\"\n";
			}

			requiredExtensions.erase(ext.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool isSuitable(vk::PhysicalDevice& device, bool debug)
	{
		if (debug)
		{
			std::cout << "Checking if device is suitable\n";
		}

		const std::vector<const char*> requestedExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		if (debug)
		{
			std::cout << "Requested device extensions:\n";

			for (const char* ext : requestedExtensions)
			{
				std::cout << "\t\"" << ext << "\"\n";
			}
		}

		if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug))
		{
			if (debug)
			{
				std::cout << "Device can support the requested extensions\n";
			}
		}
		else
		{
			if (debug)
			{
				std::cout << "Device can't support the requested extensions\n";
			}
			return false;
		}
		return true;
	}

	vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debug)
	{
		if (debug)
		{
			std::cout << "Choosing physical device...\n";
		}

		std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();

		if (debug)
		{
			std::cout << "There are " << availableDevices.size() << " available devices\n";
		}

		for (vk::PhysicalDevice device : availableDevices)
		{
			if (debug)
			{
				log_device_properties(device);
			}
			if (isSuitable(device, debug))
			{
				return device;
			}
		}

		return  nullptr;
	}

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR surface, bool debug)
	{
		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

		if (debug)
		{
			std::cout << "System can support " << queueFamilies.size() << " queue families\n";
		}

		for (int i = 0; i < queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;

				if (debug)
				{
					std::cout << "Queue family " << i << " is suitable for graphics\n";
				}
			}

			if (device.getSurfaceSupportKHR(i, surface))
			{
				indices.presentFamily = i;

				if (debug)
				{
					std::cout << "Queue family " << i << " is suitable for presenting\n";
				}
			}

			if (indices.isComplete())
			{
				break;
			}
		}

		return indices;
	}

	vk::Device create_logical_device(vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR surface, bool debug)
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, debug);
		std::vector<uint32_t> uniqueIndices;
		uniqueIndices.push_back(indices.graphicsFamily.value());
		if (indices.graphicsFamily.value() != indices.presentFamily.value())
		{
			uniqueIndices.push_back(indices.presentFamily.value());
		}
		float queuePriority{ 1.0f };

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
		for (uint32_t queueIndex : uniqueIndices)
		{
			queueCreateInfo.push_back(
				vk::DeviceQueueCreateInfo(
					vk::DeviceQueueCreateFlags(), queueIndex,
					1, &queuePriority
				)
			);
		}

		std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

		std::vector<const char*> enabledLayers;

		if (debug)
		{
			enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
		}

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(),
			queueCreateInfo.size(), queueCreateInfo.data(),
			enabledLayers.size(), enabledLayers.data(),
			deviceExtensions.size(), deviceExtensions.data(),
			&deviceFeatures
		);

		try
		{
			vk::Device device = physicalDevice.createDevice(deviceInfo);
			if (debug)
			{
				std::cout << "GPU Successfully abstracted\n";
			}
			return device;
		}
		catch (vk::SystemError err)
		{
			if (debug)
			{
				std::cout << "Device creation failed\n";
			}
			return nullptr;
		}
	}

	std::array<vk::Queue, 2> get_queues(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, bool debug)
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, debug);

		return { {
				device.getQueue(indices.graphicsFamily.value(), 0),
				device.getQueue(indices.presentFamily.value(), 0)
			} };
	}

	SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug)
	{
		SwapChainSupportDetails support;

		support.capabilities = device.getSurfaceCapabilitiesKHR(surface);

		if (debug)
		{
			std::cout << "Swapchain can support the following surface capabilities:\n"
				<< "\tmin image count: " << support.capabilities.minImageCount << '\n'
				<< "\tmax image count: " << support.capabilities.maxImageCount << '\n'
				<< "\tcurrent extent: " << support.capabilities.currentExtent.width << 'x' << support.capabilities.currentExtent.height << '\n'
				<< "\tmin extent: " << support.capabilities.minImageExtent.width << 'x' << support.capabilities.minImageExtent.height << '\n'
				<< "\tmax extent: " << support.capabilities.maxImageExtent.width << 'x' << support.capabilities.maxImageExtent.height << '\n'
				<< "\tmax image array layers: " << support.capabilities.maxImageArrayLayers << '\n';
			
			std::cout << "\tsupported transforms:\n";
			std::vector<std::string> stringList = log_transform_bits(support.capabilities.supportedTransforms);
			for (std::string line : stringList)
			{
				std::cout << "\t\t" << line << '\n';
			}
			
			std::cout << "\tcurrent transform:\n";
			stringList = log_transform_bits(support.capabilities.currentTransform);
			for (std::string line : stringList)
			{
				std::cout << "\t\t" << line << '\n';
			}

			std::cout << "\tsupported alpha operations:\n";
			stringList = log_alpha_composite_bits(support.capabilities.supportedCompositeAlpha);
			for (std::string line : stringList)
			{
				std::cout << "\t\t" << line << '\n';
			}

			std::cout << "\tsupported image usage:\n";
			stringList = log_image_usage_bits(support.capabilities.supportedUsageFlags);
			for (std::string line : stringList)
			{
				std::cout << "\t\t" << line << '\n';
			}
		}

		support.formats = device.getSurfaceFormatsKHR(surface);

		if (debug)
		{
			for (vk::SurfaceFormatKHR supportedFormat : support.formats)
			{
				std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
				std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
			}
		}

		return support;
	}
}

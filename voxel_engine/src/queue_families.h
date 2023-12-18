#pragma once
#include "config.h"

namespace vkUtil
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
	
	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR surface, bool debug)
	{
		vkUtil::QueueFamilyIndices indices;

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
}
#pragma once
#include "config.h"
#include "queue_families.h"
#include "logging.h"
#include "frame.h"

namespace vkInit
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	struct SwapChainBundle
	{
		vk::SwapchainKHR swapchain;
		std::vector<vkUtil::SwapChainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

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
			stringList = log_image_usage_bits_small(support.capabilities.supportedUsageFlags);
			for (std::string line : stringList)
			{
				std::cout << "\t\t" << line << '\n';
			}
		}

		support.formats = device.getSurfaceFormatsKHR(surface);

		std::cout << "\tsupported surface formats:\n";

		if (debug)
		{
			for (vk::SurfaceFormatKHR supportedFormat : support.formats)
			{
				std::cout << "\t\tpixel format: " << vk::to_string(supportedFormat.format) << '\n';
				std::cout << "\t\tcolor space : " << vk::to_string(supportedFormat.colorSpace) << '\n';
			}
		}

		support.presentModes = device.getSurfacePresentModesKHR(surface);

		std::cout << "\tpresent modes:\n";

		for (vk::PresentModeKHR presentMode : support.presentModes)
		{
			std::cout << "\t\t" << log_present_mode_small(presentMode) << '\n';
		}

		return support;
	}

	vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats)
	{
		for (vk::SurfaceFormatKHR format : formats)
		{
			if (format.format == vk::Format::eB8G8R8A8Unorm
				&& format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			{
				return format;
			}
		}

		return formats[0];
	}

	vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes)
	{
		for (vk::PresentModeKHR mode : presentModes)
		{
			if (mode == vk::PresentModeKHR::eMailbox)
			{
				return mode;
			}
		}

		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D choose_swapchain_extent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else
		{
			vk::Extent2D extent = { width, height };

			extent.width = std::min(
				capabilities.maxImageExtent.width,
				std::max(capabilities.minImageExtent.width, width)
			);

			extent.height = std::min(
				capabilities.maxImageExtent.height,
				std::max(capabilities.minImageExtent.height, height)
			);

			return extent;
		}
	}

	SwapChainBundle create_swapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, int width, int height, bool debug)
	{
		SwapChainSupportDetails support = query_swapchain_support(physicalDevice, surface, debug);

		vk::SurfaceFormatKHR format = choose_swapchain_surface_format(support.formats);

		vk::PresentModeKHR presentMode = choose_swapchain_present_mode(support.presentModes);

		vk::Extent2D extent = choose_swapchain_extent(width, height, support.capabilities);

		uint32_t imageCount = std::min(
			support.capabilities.maxImageCount,
			support.capabilities.minImageCount + 1
		);

		vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(), surface, imageCount,
			format.format, format.colorSpace, extent,
			1, vk::ImageUsageFlagBits::eColorAttachment
		);

		vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, false);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		if (queueFamilyIndices[0] != queueFamilyIndices[1])
		{
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		createInfo.preTransform = support.capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

		SwapChainBundle bundle{};
		try
		{
			bundle.swapchain = logicalDevice.createSwapchainKHR(createInfo);
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("failed to create swapchain");
		}

		std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
		bundle.frames.resize(images.size());

		for (int i = 0; i < images.size(); i++)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.image = images[i];
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.format = format.format;

			bundle.frames[i].image = images[i];
			bundle.frames[i].imageView = logicalDevice.createImageView(imageViewCreateInfo);
		}

		bundle.format = format.format;
		bundle.extent = extent;

		return bundle;
	}
}
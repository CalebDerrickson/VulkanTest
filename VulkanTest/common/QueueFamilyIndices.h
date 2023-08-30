#pragma once
#include <optional>

struct QueueFamilyIndices {

	// Keep track of how many instances there 
	// are for the moment
	static uint32_t InstanceCount;

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }

	QueueFamilyIndices()
	{
		InstanceCount++;
	};
};


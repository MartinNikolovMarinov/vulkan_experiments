#pragma once

// IMPORTANT:
// This file initializes default settings for core and std core. Anywhere the application wants to use somthing from
// core or std core, it must include this file instead of directly including them.
// This is because defining default macros can be done only here.

#undef CORE_DEFAULT_ALLOCATOR
#define CORE_DEFAULT_ALLOCATOR() core::StdAllocator

#include <core.h>

using namespace coretypes;

#include <fmt/core.h>
#include <fmt/color.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Vulkan uses depth range 0 to 1
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.h>

void initCore();

using Sb = core::StrBuilder<>;

template<> addr_size core::hash(const core::StrView& key);
template<> addr_size core::hash(const i32& key);
template<> addr_size core::hash(const u32& key);

template<> bool core::eq(const core::StrView& a, const core::StrView& b);
template<> bool core::eq(const i32& a, const i32& b);
template<> bool core::eq(const u32& a, const u32& b);


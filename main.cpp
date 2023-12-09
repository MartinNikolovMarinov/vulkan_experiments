#include <init_core.h>

#include <cstdlib>
#include <string> // I am forced by tinyobjloader to use std::string.
#include <chrono>

enum ErrorType : i32 {
    None,

    GLFWInitFailed,
    GLFWWindowCreationFailed,
    GLFWSetEventHandlerCallbackFailed,

    VulkanInstanceCreationFailed,
    VulkanListExtensionsFailed,
    VulkanListValidationLayersFailed,
    VulkanNoSupportedDevicesErr,
    VulkanDeviceCreationFailed,
    VulkanDebugMessengerCreationFailed,
    VulkanExtensionNotSupported,
    VulkanValidationLayerNotSupported,
    VulkanListDeviceExtensionsFailed,
    VulkanSwapChainSupportQueryFailed,
    VulkanSwapChainCreationFailed,
    VulkanSwapImageViewCreationFailed,
    VulkanCreateShaderModuleFailed,
    VulkanPipelineCreationFailed,
    VulkanRenderPassCreationFailed,
    VulkanFramebufferCreationFailed,
    VulkanCommandPoolCreationFailed,
    VulkanCommandBufferCreationFailed,
    VulkanBeginCommandBufferFailed,
    VulkanEndCommandBufferFailed,
    VulkanSemaphoreCreationFailed,
    VulkanFenceCreationFailed,
    VulkanVertexBufferCreationFailed,
    VulkanFailedToFindMemoryType,
    VulkanVertexBufferMemoryAllocationFailed,
    VulkanVertexBufferMemoryBindingFailed,
    VulkanDescriptorSetLayoutCreationFailed,
    VulkanMapMemoryFailed,
    VulkanDescriptorPoolCreationFailed,
    VulkanDescriptorSetAllocationFailed,
    VulkanTextureImageCreationFailed,
    VulkanTextureImageMemoryAllocationFailed,
    VulkanTextureImageMemoryBindingFailed,
    VulkanImageViewCreationFailed,
    VulkanTextureSamplerCreationFailed,

    FailedToLoadShader,
    FailedToLoadModel,
    FailedToLoadImage,

    SENTINEL
};

const char* errorTypeToCptr(ErrorType t) {
    switch (t) {
        case None:                                     return "None";

        case GLFWInitFailed:                           return "GLFWInitFailed";
        case GLFWWindowCreationFailed:                 return "GLFWWindowCreationFailed";
        case GLFWSetEventHandlerCallbackFailed:        return "GLFWSetEventHandlerCallbackFailed";

        case VulkanInstanceCreationFailed:             return "VulkanInstanceCreationFailed";
        case VulkanListExtensionsFailed:               return "VulkanListExtensionsFailed";
        case VulkanListValidationLayersFailed:         return "VulkanListValidationLayersFailed";
        case VulkanNoSupportedDevicesErr:              return "VulkanNoSupportedDevicesErr";
        case VulkanDeviceCreationFailed:               return "VulkanDeviceCreationFailed";
        case VulkanDebugMessengerCreationFailed:       return "VulkanDebugMessengerCreationFailed";
        case VulkanExtensionNotSupported:              return "VulkanExtensionNotSupported";
        case VulkanValidationLayerNotSupported:        return "VulkanValidationLayerNotSupported";
        case VulkanListDeviceExtensionsFailed:         return "VulkanListDeviceExtensionsFailed";
        case VulkanSwapChainSupportQueryFailed:        return "VulkanSwapChainSupportQueryFailed";
        case VulkanSwapChainCreationFailed:            return "VulkanSwapChainCreationFailed";
        case VulkanSwapImageViewCreationFailed:        return "VulkanSwapImageViewCreationFailed";
        case VulkanCreateShaderModuleFailed:           return "VulkanCreateShaderModuleFailed";
        case VulkanPipelineCreationFailed:             return "VulkanPipelineCreationFailed";
        case VulkanRenderPassCreationFailed:           return "VulkanRenderPassCreationFailed";
        case VulkanFramebufferCreationFailed:          return "VulkanFramebufferCreationFailed";
        case VulkanCommandPoolCreationFailed:          return "VulkanCommandPoolCreationFailed";
        case VulkanCommandBufferCreationFailed:        return "VulkanCommandBufferCreationFailed";
        case VulkanBeginCommandBufferFailed:           return "VulkanBeginCommandBufferFailed";
        case VulkanEndCommandBufferFailed:             return "VulkanEndCommandBufferFailed";
        case VulkanSemaphoreCreationFailed:            return "VulkanSemaphoreCreationFailed";
        case VulkanFenceCreationFailed:                return "VulkanFenceCreationFailed";
        case VulkanVertexBufferCreationFailed:         return "VulkanVertexBufferCreationFailed";
        case VulkanFailedToFindMemoryType:             return "VulkanFailedToFindMemoryType";
        case VulkanVertexBufferMemoryAllocationFailed: return "VulkanVertexBufferMemoryAllocationFailed";
        case VulkanVertexBufferMemoryBindingFailed:    return "VulkanVertexBufferMemoryBindingFailed";
        case VulkanDescriptorSetLayoutCreationFailed:  return "VulkanDescriptorSetLayoutCreationFailed";
        case VulkanMapMemoryFailed:                    return "VulkanMapMemoryFailed";
        case VulkanDescriptorPoolCreationFailed:       return "VulkanDescriptorPoolCreationFailed";
        case VulkanDescriptorSetAllocationFailed:      return "VulkanDescriptorSetAllocationFailed";
        case VulkanTextureImageCreationFailed:         return "VulkanTextureImageCreationFailed";
        case VulkanTextureImageMemoryAllocationFailed: return "VulkanTextureImageMemoryAllocationFailed";
        case VulkanTextureImageMemoryBindingFailed:    return "VulkanTextureImageMemoryBindingFailed";
        case VulkanImageViewCreationFailed:            return "VulkanImageViewCreationFailed";
        case VulkanTextureSamplerCreationFailed:       return "VulkanTextureSamplerCreationFailed";

        case FailedToLoadShader:                       return "FailedToLoadShader";
        case FailedToLoadModel:                        return "FailedToLoadModel";
        case FailedToLoadImage:                        return "FailedToLoadImage";

        case SENTINEL: return "SENTINEL";
    }

    return "Unknown";
}

struct Vertex {
    core::vec3f pos;
    core::vec3f color;
    core::vec2f texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static core::SArr<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        core::SArr<VkVertexInputAttributeDescription, 3> attributeDescriptions (3);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT; // vec2
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

template <> addr_size core::hash(const Vertex& key) {
    addr_size h = addr_size(core::simpleHash_32(reinterpret_cast<const void*>(&key), sizeof(key)));
    return h;
}

template <> bool core::eq(const Vertex& a, const Vertex& b) {
    bool ret = a.pos.equals(b.pos) &&
               a.color.equals(b.color) &&
               a.texCoord.equals(b.texCoord);
    return ret;
}

// Alignment requiremnets are provided in the Vulkan Specification here -
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap15.html#interfaces-resources-layout
struct UniformBufferObject {
    alignas(16) core::mat4f model;
    alignas(16) core::mat4f view;
    alignas(16) core::mat4f proj;
};

constexpr static core::vec3f X_AXIS = core::v(1.f, 0.f, 0.f);
constexpr static core::vec3f Y_AXIS = core::v(0.f, 1.f, 0.f);
constexpr static core::vec3f Z_AXIS = core::v(0.f, 0.f, 1.f);

struct Error {
    Sb description = {};
    ErrorType type = None;
};

core::expected<core::Arr<VkExtensionProperties>, Error> getAllSupportedVkExtensions() {
    u32 extCount = 0;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
    }

    core::Arr<VkExtensionProperties> exts (extCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
    }

    return exts;
}

core::expected<core::Arr<VkLayerProperties>, Error> getAllSupportedVkValidationLayers() {
    u32 layerCount = 0;
    if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
    }

    core::Arr<VkLayerProperties> layers (layerCount);
    if (vkEnumerateInstanceLayerProperties(&layerCount, layers.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
    }

    return layers;
}

core::expected<core::Arr<VkExtensionProperties>, Error> getAllSupportedVkDeviceExtensions(VkPhysicalDevice device) {
    u32 extCount = 0;
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan device extension enumeration failed", VulkanListDeviceExtensionsFailed });
    }

    core::Arr<VkExtensionProperties> exts (extCount);
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, exts.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan device extension enumeration failed", VulkanListDeviceExtensionsFailed });
    }

    return exts;
}

bool checkExtensionSupport(
    const core::Arr<const char*>& extensions,
    const core::Arr<VkExtensionProperties>& supported
) {
    for (addr_size i = 0; i < extensions.len(); i++) {
        const char* ext = extensions[i];
        addr_size extLen = core::cptrLen(ext);
        bool found = false;
        for (addr_size j = 0; j < supported.len(); j++) {
            const char* sup = supported[j].extensionName;
            if (core::cptrEq(ext, sup, extLen)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

bool checkValidationLayerSupport(
    const core::Arr<const char*>& validationLayers,
    const core::Arr<VkLayerProperties>& supported
) {
    for (addr_size i = 0; i < validationLayers.len(); i++) {
        const char* vlayer = validationLayers[i];
        addr_size vlen = core::cptrLen(vlayer);
        bool found = false;
        for (addr_size j = 0; j < supported.len(); j++) {
            const char* sup = supported[j].layerName;
            if (core::cptrEq(vlayer, sup, vlen)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                                //  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                    [[maybe_unused]] void* pUserData) -> VkBool32 {
        fmt::print("Validation layer: {} ", pCallbackData->pMessage);
        fmt::print("Severity: {} ", u64(messageSeverity));
        fmt::print("Type: {} ", messageType);
        fmt::print("\n");
        return VK_FALSE;
    };

    return createInfo;
}

VkResult wrap_vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
) {
    static PFN_vkCreateDebugUtilsMessengerEXT func = nullptr;

    if (func == nullptr) {
        func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func == nullptr) {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void wrap_vkDestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
) {
    static PFN_vkDestroyDebugUtilsMessengerEXT func = nullptr;

    if (func == nullptr) {
        func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func == nullptr) {
            return;
        }
    }

    func(instance, debugMessenger, pAllocator);
}

struct QueueFamilyIndices {
    i64 graphicsFamily = -1; // queue family index for graphics commands
    i64 presentFamily = -1; // queue family index for presentation commands

    bool isComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    // Get the number of queue families:
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    // Get the queue families:
    core::Arr<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Iterate over the queue families and find the ones that support graphics and presentation:
    for (addr_size i = 0; i < queueFamilies.len(); i++) {
        VkQueueFamilyProperties queueFamily = queueFamilies[i];
        VkBool32 graphicsSupport = (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? VK_TRUE : VK_FALSE;
        if (graphicsSupport) {
            indices.graphicsFamily = i64(i);
        }

        // Check if the physical device supports presentation to the provided platform surface:
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i64(i);
        }

        if (indices.isComplete()) {
            // Stop iterating once support for the required queue families is found.
            break;
        }
    }

    return indices;
}

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    core::Arr<VkSurfaceFormatKHR> formats;
    core::Arr<VkPresentModeKHR> presentModes;
};

core::expected<SwapChainSupportDetails, Error> querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    // Get the surface capabilities:
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan surface capabilities query failed", VulkanSwapChainSupportQueryFailed });
    }

    u32 formatCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan surface formats query failed", VulkanSwapChainSupportQueryFailed });
    }

    if (formatCount != 0) {
        details.formats = core::Arr<VkSurfaceFormatKHR> (formatCount);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface formats query failed", VulkanSwapChainSupportQueryFailed });
        }
    }

    u32 presentModeCount = 0;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan surface present modes query failed", VulkanSwapChainSupportQueryFailed });
    }

    if (presentModeCount != 0) {
        details.presentModes = core::Arr<VkPresentModeKHR> (presentModeCount);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface present modes query failed", VulkanSwapChainSupportQueryFailed });
        }
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const core::Arr<VkSurfaceFormatKHR>& availableFormats) {
    for (addr_size i = 0; i < availableFormats.len(); i++) {
        VkSurfaceFormatKHR format = availableFormats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    if (availableFormats.len() > 0) {
        return availableFormats[0];
    }

    return {};
}

VkPresentModeKHR chooseSwapPresentMode(const core::Arr<VkPresentModeKHR>& availablePresentModes) {
    for (addr_size i = 0; i < availablePresentModes.len(); i++) {
        VkPresentModeKHR mode = availablePresentModes[i];
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(GLFWwindow* glfwWindow , const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != core::MAX_U32) {
        return capabilities.currentExtent;
    }

    i32 width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);

    VkExtent2D actualExtent = { static_cast<u32>(width), static_cast<u32>(height) };

    actualExtent.width = core::clamp<u32>(
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width,
        actualExtent.width
    );
    actualExtent.height = core::clamp<u32>(
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height,
        actualExtent.height
    );

    return actualExtent;
}

core::expected<u32, Error> findMemoryType(VkPhysicalDevice pdevice, u32 typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pdevice, &memProperties);

    for (u32 i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return core::unexpected<Error>({ "Vulkan memory type not found", VulkanFailedToFindMemoryType });
}

core::expected<Error> createBuffer(VkPhysicalDevice pdevice, VkDevice device, VkDeviceSize size,
                                   VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                   VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan buffer creation failed", VulkanVertexBufferCreationFailed });
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    auto memTypeRes = findMemoryType(pdevice, memRequirements.memoryTypeBits, properties);
    if (memTypeRes.hasErr()) {
        return core::unexpected<Error>(core::move(memTypeRes.err()));
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = core::move(memTypeRes.value());

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan buffer memory allocation failed", VulkanVertexBufferMemoryAllocationFailed });
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);

    return {};
}

struct Application {

#ifndef NDEBUG
    #define USE_VALIDATORS true
#else
    #define USE_VALIDATORS false
#endif

    static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2; // NOTE: should be a power of 2 to avoid modulo operations.

    struct AppProps {
        i32 width;
        i32 height;
        const char* title;
    };

    core::expected<Error> run() {
        initCore();

        if (auto ret = initWindow(); ret.hasErr()) {
            return core::unexpected<Error>(core::move(ret.err()));
        }

        if (auto ret = initVulkan(); ret.hasErr()) {
            return core::unexpected<Error>(core::move(ret.err()));
        }

        mainLoop();

        cleanup();

        return {};
    }

    Application create(AppProps&& props) {
        Application ret;
        ret.m_width = props.width;
        ret.m_height = props.height;
        ret.m_title = props.title;

        return ret;
    }

private:

    core::expected<Error> initWindow() {
        if (i32 ret = glfwInit(); ret != GLFW_TRUE) {
            return core::unexpected<Error>({ "GLFW initialization failed", GLFWInitFailed });
        }

        // Hint to GLFW that we don't want to use OpenGL:
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Create the GLFW window:
        m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
        if (!m_glfwWindow) {
            return core::unexpected<Error>({ "GLFW window creation failed", GLFWWindowCreationFailed });
        }

        glfwSetWindowUserPointer(m_glfwWindow, this);

        // Set event handler callbacks:

        const char* errDesc = nullptr;

        glfwSetErrorCallback([](i32 error, const char* description) {
            fmt::print(stderr, "GLFW error: {} - {}\n", error, description);
        });
        if (i32 errCode = glfwGetError(&errDesc); errCode != GLFW_NO_ERROR) {
            Error err;
            err.type = GLFWSetEventHandlerCallbackFailed;
            err.description = "Failed to set GLFW ErrorCallback, reason: ";
            err.description.append(errDesc ? errDesc : "Unknown");
            err.description.append("\n");
            return core::unexpected(core::move(err));
        }

        return {};
    }

#pragma region Initialize Vulkan

    core::expected<Error> initVulkan() {
        if (auto res = createInstance(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        #if USE_VALIDATORS
            if (auto res = createDebugMessenger(); res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        #endif

        if (auto res = createSurface(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = pickPhysicalDevice(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createLogicalDevice(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createSwapChain(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createImageViews(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createRenderPass(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createDescriptorSetLayout(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createGraphicsPipeline(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createCommandPool(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createDepthResources(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createFramebuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createTextureImage(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createTextureImageView(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createTextureSampler(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = loadModels(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createVertexBuffer(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createIndexBuffer(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createUniformBuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createDescriptorPool(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createDescriptorSets(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createCommandBuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createSyncObjects(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        return {};
    }

    core::expected<Error> createInstance() {
        // [STEP 1] Create Vulkan application info:

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_title;
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

        // [STEP 2] Create Vulkan instance info:

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // [STEP 3] Extensions. Identify required Vulkan extensions and check if the system supports them:

        {
            // Get all supported Vulkan extensions:
            {
                auto res = getAllSupportedVkExtensions();
                if (res.hasErr()) {
                    return core::unexpected<Error>(core::move(res.err()));
                }
                m_vkSupportedExtensions = core::move(res.value());
            }

            // Get all required Vulkan extensions:
            {
                // Required for GLFW:
                u32 glfwExtensionsCount = 0;
                const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
                if (!glfwExtensions) {
                    // This would be highly unusual. Perhaps Vulkan is not available on this system?
                    return core::unexpected<Error>({ "GLFW required extensions returned nullptr", VulkanInstanceCreationFailed });
                }
                m_vkActiveExtensions.append(glfwExtensions, glfwExtensionsCount);
            }

            // Check if the required extensions are supported:
            if (!checkExtensionSupport(m_vkActiveExtensions, m_vkSupportedExtensions)) {
                return core::unexpected<Error>({ "Vulkan required extensions not supported", VulkanExtensionNotSupported });
            }

            // Optional Extensions:

            #if USE_VALIDATORS
                // Add Vulkan SDK standard debug extensions:
                m_vkActiveExtensions.append(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            #endif

            // Set the extensions in the createInfo:

            createInfo.enabledExtensionCount = u32(m_vkActiveExtensions.len());
            createInfo.ppEnabledExtensionNames = m_vkActiveExtensions.data();
        }

        // [STEP 4] Validation layers. Request the required validation layers if the program is build in debug mode:

        #if USE_VALIDATORS
            // Get all supported Vulkan validation layers:
            {
                auto res = getAllSupportedVkValidationLayers();
                if (res.hasErr()) {
                    return core::unexpected<Error>(core::move(res.err()));
                }
                m_vkSupportedValidationLayers = core::move(res.value());
            }

            // Add Vulkan SDK standard validation layers:
            m_vkActiveValidationLayers.append("VK_LAYER_KHRONOS_validation");

            // Check if the required validation layers are supported:
            if (!checkValidationLayerSupport(m_vkActiveValidationLayers, m_vkSupportedValidationLayers)) {
                return core::unexpected<Error>({ "Vulkan required validation layers not supported", VulkanValidationLayerNotSupported });
            }

            // Set the required validation layers in the createInfo:
            createInfo.enabledLayerCount = m_vkActiveValidationLayers.len();
            createInfo.ppEnabledLayerNames = m_vkActiveValidationLayers.data();

            // The following code is required to enable the validation layers during instance creation:
            VkDebugUtilsMessengerCreateInfoEXT dmcinfo = createDebugMessengerInfo();
            createInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&dmcinfo);
        #else
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        #endif

        // [STEP 5] Finally. Create the Vulkan instance:

        if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance creation failed", VulkanInstanceCreationFailed });
        }

        // TODO: Needs additional code to work on MACOS!

        return {};
    }

    core::expected<Error> createDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = createDebugMessengerInfo();
        if (wrap_vkCreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_vkDebugMessenger) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan debug messenger creation failed", VulkanDebugMessengerCreationFailed });
        }
        return {};
    }

    core::expected<Error> pickPhysicalDevice() {
        // Query the number of devices that support Vulkan;
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            return core::unexpected<Error>({ "No vulkan compatible devices found", VulkanNoSupportedDevicesErr });
        }

        // Enumberate the physical devices and store info about them:
        core::Arr<VkPhysicalDevice> phyDevices (deviceCount);
        vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, phyDevices.data());

        // Append the required device extensions:
        m_vkActiveDeviceExtensions.append(VK_KHR_SWAPCHAIN_EXTENSION_NAME); // This application needs swapchain support.

        // Pick the first suitable device:
        for (addr_size i = 0; i < phyDevices.len(); i++) {
            VkPhysicalDevice device = phyDevices[i];
            auto res = isDeviceSutable(device, m_vkSurface);
            if (res.hasErr()) return core::unexpected<Error>(core::move(res.err()));
            if (res.value() == true) {
                m_vkPhysicalDevice = device;
                break;
            }
        }

        // Fail if no suitable device was found:
        if (m_vkPhysicalDevice == VK_NULL_HANDLE) {
            return core::unexpected<Error>({ "No vulkan suitable devices found", VulkanNoSupportedDevicesErr });
        }

        return {};
    }

    core::expected<bool, Error> isDeviceSutable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        // Get all supported extensions for the device:
        auto supportedDeviceExt = getAllSupportedVkDeviceExtensions(device);
        if (supportedDeviceExt.hasErr()) {
            return core::unexpected<Error>(core::move(supportedDeviceExt.err()));
        }

        // Check if the required extensions are supported:
        if (!checkExtensionSupport(m_vkActiveDeviceExtensions, supportedDeviceExt.value())) {
            return false;
        }

        // Check if the swapchain is adaquite:
        {
            auto res = querySwapChainSupport(device, surface);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
            SwapChainSupportDetails swapChainSupport = core::move(res.value());
            bool swapChainAdequate = !swapChainSupport.formats.empty() &&
                                     !swapChainSupport.presentModes.empty();
            if (!swapChainAdequate) {
                return false;
            }
        }

        // Check if feature support is adaquite:
        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        if (!supportedFeatures.samplerAnisotropy) {
            // No anisotropic filtering support.
            return false;
        }

        // The most important check for device suitability is the type of queue families supported by the device.
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        bool ret = indices.isComplete();
        return ret;
    }

    core::expected<Error> createLogicalDevice() {

        // [STEP 1] Create a queue family info.
        QueueFamilyIndices queueIndices = findQueueFamilies(m_vkPhysicalDevice, m_vkSurface);
        if (!queueIndices.isComplete()) {
            return core::unexpected<Error>({ "Vulkan queue family indices not complete", VulkanDeviceCreationFailed });
        }

        core::Arr<VkDeviceQueueCreateInfo> queueCreateInfos;
        core::HashSet<u32> uniqueQueueFamilies;
        {
            auto lval = u32(queueIndices.graphicsFamily);
            uniqueQueueFamilies.put(lval);
        }
        {
            auto lval = u32(queueIndices.presentFamily);
            uniqueQueueFamilies.put(lval);
        }

        f32 queuePriority = 1.0f; // be careful where you place this.
        uniqueQueueFamilies.keys([&](u32 key) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = key;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.append(core::move(queueCreateInfo));
            return true;
        });

        // [STEP 2] Specify used device features.
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        // [STEP 3] Create the logical device info.
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = u32(queueCreateInfos.len());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = u32(m_vkActiveDeviceExtensions.len());
        createInfo.ppEnabledExtensionNames = m_vkActiveDeviceExtensions.data();

        /* NOTE:
            Previous implementations of Vulkan made a distinction between instance and device specific validation
            layers, but this is no longer the case. That means that the enabledLayerCount and ppEnabledLayerNames fields
            of VkDeviceCreateInfo are ignored by up-to-date implementations. However, it is still a good idea to set
            them anyway to be compatible with older implementations
        */
        createInfo.enabledLayerCount = 0;
        #if USE_VALIDATORS
            createInfo.enabledLayerCount = u32(m_vkActiveValidationLayers.len());
            createInfo.ppEnabledLayerNames = m_vkActiveValidationLayers.data();
        #else
            createInfo.enabledLayerCount = 0;
        #endif

        // [STEP 4] Create the device.
        if (vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkDevice) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan logical device creation failed", VulkanDeviceCreationFailed });
        }

        // [STEP 5] Get the graphics queue.
        vkGetDeviceQueue(m_vkDevice, queueIndices.graphicsFamily, 0, &m_vkGraphicsQueue);
        vkGetDeviceQueue(m_vkDevice, queueIndices.presentFamily, 0, &m_vkPresetQueue);

        return {};
    }

    core::expected<Error> createSurface() {
        // Vulkan can't interface with the windowing system directly to present images to the screen.
        // It needs an intermediary object called a Surface, which is platform dependent and here it's handled by GLFW.
        if (glfwCreateWindowSurface(m_vkInstance, m_glfwWindow, nullptr, &m_vkSurface) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface creation failed", VulkanDeviceCreationFailed });
        }

        return {};
    }

    core::expected<Error> createSwapChain() {
        SwapChainSupportDetails swapChainSupport;
        {
            auto ret = querySwapChainSupport(m_vkPhysicalDevice, m_vkSurface);
            if (ret.hasErr()) {
                return core::unexpected<Error>(core::move(ret.err()));
            }
            swapChainSupport = core::move(ret.value());
        }

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(m_glfwWindow, swapChainSupport.capabilities);

        // Adding 1 to the minimum image count guarantees that we won't have to wait for internal driver operations to
        // complete before acquiring another image to render to.
        u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            // Don't exceed the maximum image capability.
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_vkSurface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(m_vkPhysicalDevice, m_vkSurface);
        u32 queueFamilyIndices[] = { u32(indices.graphicsFamily), u32(indices.presentFamily) };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        // To specify no transformation:
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_vkDevice, &createInfo, nullptr, &m_vkSwapChain) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan swapchain creation failed", VulkanSwapChainCreationFailed });
        }

        vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &imageCount, nullptr);
        m_vkSwapChainImages = core::Arr<VkImage> (imageCount);
        vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &imageCount, m_vkSwapChainImages.data());
        m_vkSwapChainExtent = extent;
        m_vkSwapChainImageFormat = surfaceFormat.format;

        return {};
    }

    core::expected<Error> createImageViews() {
        m_vkSwapChainImageViews = core::Arr<VkImageView> (m_vkSwapChainImages.len());

        for (addr_size i = 0; i < m_vkSwapChainImages.len(); i++) {
            auto res = createImageView(m_vkSwapChainImages[i], m_vkSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
            m_vkSwapChainImageViews[i] = core::move(res.value());
        }

        return {};
    }

    core::expected<Error> createGraphicsPipeline() {
        static constexpr const char* VERT_SHADER_PATH = ASSETS_PATH "/shaders/04_with_texture.vert.spv";

        core::Arr<u8> vertShaderCode;
        {
            auto res = core::fileReadEntire(VERT_SHADER_PATH, vertShaderCode);
            if (res.hasErr()) {
                Error err;
                err.type = FailedToLoadShader;
                err.description = "Failed to load vertex shader code: ";
                err.description.append(VERT_SHADER_PATH);
                err.description.append(", reason: ");
                {
                    char out[core::MAX_SYSTEM_ERR_MSG_SIZE] = {};
                    core::pltErrorDescribe(res.err(), out);
                    err.description.append(out);
                }
                return core::unexpected(core::move(err));
            }
        }

        static constexpr const char* FRAG_SHADER_PATH = ASSETS_PATH "/shaders/04_with_texture.frag.spv";

        core::Arr<u8> fragShaderCode;
        {
            auto res = core::fileReadEntire(FRAG_SHADER_PATH, fragShaderCode);
            if (res.hasErr()) {
                Error err;
                err.type = FailedToLoadShader;
                err.description = "Failed to load fragment shader code: ";
                err.description.append(FRAG_SHADER_PATH);
                err.description.append(", reason: ");
                {
                    char out[core::MAX_SYSTEM_ERR_MSG_SIZE] = {};
                    core::pltErrorDescribe(res.err(), out);
                    err.description.append(out);
                }
                return core::unexpected(core::move(err));
            }
        }

        VkShaderModule vertShaderModule;
        {
            auto ret = createShaderModule(vertShaderCode);
            if (ret.hasErr()) {
                return core::unexpected<Error>(core::move(ret.err()));
            }
            vertShaderModule = core::move(ret.value());
        }
        defer { vkDestroyShaderModule(m_vkDevice, vertShaderModule, nullptr); };

        VkShaderModule fragShaderModule;
        {
            auto ret = createShaderModule(fragShaderCode);
            if (ret.hasErr()) {
                return core::unexpected<Error>(core::move(ret.err()));
            }
            fragShaderModule = core::move(ret.value());
        }
        defer { vkDestroyShaderModule(m_vkDevice, fragShaderModule, nullptr); };

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main"; // Entry point for the shader.

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main"; // Entry point for the shader.

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        static constexpr u32 DYNAMIC_STATES_COUNT = 2;
        VkDynamicState dynamicStates[DYNAMIC_STATES_COUNT] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = DYNAMIC_STATES_COUNT;
        dynamicState.pDynamicStates = dynamicStates;

        VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = u32(attributeDescriptions.len());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        // rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE; // TODO: Enable this for anti-aliasing.
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                              VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT |
                                              VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_vkDescriptorSetLayout;

        if (vkCreatePipelineLayout(m_vkDevice, &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan pipeline layout creation failed", VulkanPipelineCreationFailed });
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_vkPipelineLayout;
        pipelineInfo.renderPass = m_vkRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.pDepthStencilState = &depthStencil;

        if (vkCreateGraphicsPipelines(m_vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_vkGraphicsPipeline) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan graphics pipeline creation failed", VulkanPipelineCreationFailed });
        }

        return {};
    }

    core::expected<VkShaderModule, Error> createShaderModule(const core::Arr<u8>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.len();
        createInfo.pCode = reinterpret_cast<const u32*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan shader module creation failed", VulkanCreateShaderModuleFailed });
        }

        return shaderModule;
    }

    core::expected<Error> createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_vkSwapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkAttachmentDescription attachments[] = { colorAttachment, depthAttachment };
        constexpr addr_size attachmentsCount = sizeof(attachments) / sizeof(attachments[0]);

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachmentsCount;
        renderPassInfo.pAttachments = attachments;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_vkDevice, &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan render pass creation failed", VulkanRenderPassCreationFailed });
        }

        return {};
    }

    core::expected<Error> createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding bindings[2] = { uboLayoutBinding, samplerLayoutBinding };
        constexpr addr_size bindingCount = sizeof(bindings) / sizeof(bindings[0]);

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = u32(bindingCount);
        layoutInfo.pBindings = bindings;

        if (vkCreateDescriptorSetLayout(m_vkDevice, &layoutInfo, nullptr, &m_vkDescriptorSetLayout) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan descriptor set layout creation failed", VulkanDescriptorSetLayoutCreationFailed });
        }

        return {};
    }

    core::expected<Error> createFramebuffers() {
        m_vkSwapChainFrameBuffers = core::Arr<VkFramebuffer> (m_vkSwapChainImageViews.len());

        for (addr_size i = 0; i < m_vkSwapChainImageViews.len(); i++) {
            VkImageView attachments[] = {
                m_vkSwapChainImageViews[i],
                m_vkDepthImageView
            };
            constexpr addr_size attachmentsCount = sizeof(attachments) / sizeof(attachments[0]);

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_vkRenderPass;
            framebufferInfo.attachmentCount = attachmentsCount;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_vkSwapChainExtent.width;
            framebufferInfo.height = m_vkSwapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_vkDevice, &framebufferInfo, nullptr, &m_vkSwapChainFrameBuffers[i]) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan framebuffer creation failed", VulkanFramebufferCreationFailed });
            }
        }

        return {};
    }

    core::expected<Error> createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_vkPhysicalDevice, m_vkSurface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = u32(queueFamilyIndices.graphicsFamily);

        if (vkCreateCommandPool(m_vkDevice, &poolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan command pool creation failed", VulkanCommandPoolCreationFailed });
        }

        return {};
    }

    VkFormat findSupportedFormat(const core::Arr<VkFormat>& candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features) {

        for (addr_size i = 0; i < candidates.len(); i++) {
            VkFormat format = candidates[i];

            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        return VK_FORMAT_UNDEFINED;
    }

    VkFormat findDepthFormat() {
        core::Arr<VkFormat> candidates (3);
        candidates[0] = VK_FORMAT_D32_SFLOAT;
        candidates[1] = VK_FORMAT_D32_SFLOAT_S8_UINT;
        candidates[2] = VK_FORMAT_D24_UNORM_S8_UINT;

        VkFormat ret = findSupportedFormat(candidates,
                                           VK_IMAGE_TILING_OPTIMAL,
                                           VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        return ret;
    }

    bool hasStencilComponent(VkFormat format) {
        bool ret = format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
                   format == VK_FORMAT_D24_UNORM_S8_UINT;
        return ret;
    }

    core::expected<Error> createDepthResources() {
        VkFormat depthFormat = findDepthFormat();

        {
            VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
            VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            auto res = createImage(m_vkSwapChainExtent.width, m_vkSwapChainExtent.height, 1, depthFormat,
                                   tiling, usage, props, m_vkDepthImage, m_vkDepthImageMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        {
            auto res = createImageView(m_vkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }

            m_vkDepthImageView = core::move(res.value());
        }

        return {};
    }

    core::expected<Error> createTextureImage() {
        constexpr const char* TEXTURE_PATH = ASSETS_PATH "/textures/viking_room.png";
        i32 texW, texH, texChannels;

        stbi_uc* pixels = stbi_load(TEXTURE_PATH, &texW, &texH, &texChannels, STBI_rgb_alpha);
        if (!pixels) {
            return core::unexpected<Error>({ "Failed to load texture image", FailedToLoadImage });
        }
        defer { stbi_image_free(pixels); };

        VkDeviceSize imageSize = texW * texH * 4;
        m_mipLevels = u32(core::floor(core::log2(core::max(f32(texW), f32(texH))))) + 1;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        {
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, imageSize,
                                    usage, props, stagingBuffer, stagingBufferMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        defer {
            vkDestroyBuffer(m_vkDevice, stagingBuffer, nullptr);
            vkFreeMemory(m_vkDevice, stagingBufferMemory, nullptr);
        };

        void* data;
        if (vkMapMemory(m_vkDevice, stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture image mapping failed", VulkanMapMemoryFailed });
        }
            core::memcopy(data, pixels, imageSize);
        vkUnmapMemory(m_vkDevice, stagingBufferMemory);

        {
            auto res = createImage(texW, texH, m_mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                                   VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vkTextureImage, m_vkTextureImageMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        transitionImageLayout(m_vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels);
        // Transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

        copyBufferToImage(stagingBuffer, m_vkTextureImage, u32(texW), u32(texH));

        generateMipmaps(m_vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, texW, texH, m_mipLevels);

        return {};
    }

    void generateMipmaps(VkImage image, VkFormat format, u32 texW, u32 texH, u32 mipLevels) {
        // Check if image format supports linear blitting:
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, format, &formatProps);

        if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            // Mip levels should not be generated at runtime. It makes more sense for perfromance to generate them
            // in advance and load them from the texture file.
            Assert(false, "Texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        i32 mipW = texW;
        i32 mipH = texH;

        for (u32 i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipW, mipH, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipW > 1 ? mipW / 2 : 1, mipH > 1 ? mipH / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                           image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1, &blit,
                           VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &barrier);

            if (mipW > 1) mipW /= 2;
            if (mipH > 1) mipH /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        endSingleTimeCommands(commandBuffer);
    }

    core::expected<Error> createTextureImageView() {
        auto res = createImageView(m_vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels);
        if (res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }
        m_vkTextureImageView = core::move(res.value());
        return {};
    }

    core::expected<Error> createTextureSampler() {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;

        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE; // false means [0, 1], true means [0, texW] or [0, texH]

        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = f32(m_mipLevels);

        if (vkCreateSampler(m_vkDevice, &samplerInfo, nullptr, &m_vkTextureSampler) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture sampler creation failed", VulkanTextureSamplerCreationFailed });
        }

        return {};
    }

    core::expected<Error> createImage(u32 width, u32 height, u32 mipLevels, VkFormat format, VkImageTiling tiling,
                                      VkImageUsageFlags usage, VkMemoryPropertyFlags props, VkImage& image,
                                      VkDeviceMemory& imageMemory) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0;

        if (vkCreateImage(m_vkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture image creation failed", VulkanTextureImageCreationFailed });
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_vkDevice, image, &memRequirements);

        u32 memTypeIndex;
        {
            auto res = findMemoryType(m_vkPhysicalDevice, memRequirements.memoryTypeBits, props);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
            memTypeIndex = core::move(res.value());
        }

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = memTypeIndex;

        if (vkAllocateMemory(m_vkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture image memory allocation failed", VulkanTextureImageMemoryAllocationFailed });
        }

        if (vkBindImageMemory(m_vkDevice, image, imageMemory, 0) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture image memory binding failed", VulkanTextureImageMemoryBindingFailed });
        }

        return {};
    }

    core::expected<VkImageView, Error> createImageView(VkImage image,
                                                       VkFormat format,
                                                       VkImageAspectFlags aspectFlag,
                                                       u32 mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;

        viewInfo.subresourceRange.aspectMask = aspectFlag;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(m_vkDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan texture image view creation failed", VulkanImageViewCreationFailed });
        }

        return imageView;
    }

    VkCommandBuffer beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_vkCommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_vkDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_vkGraphicsQueue);

        vkFreeCommandBuffers(m_vkDevice, m_vkCommandPool, 1, &commandBuffer);
    }

    core::expected<Error> loadModels() {
        using namespace tinyobj;
        using namespace std;

        constexpr const char* MODEL_PATH = ASSETS_PATH "/models/viking_room.obj";

        attrib_t attrib;
        vector<shape_t> shapes;
        vector<material_t> materials;
        string warn, err;

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH)) {
            Error ret;
            ret.type = FailedToLoadModel;
            ret.description = "Failed to load model: ";
            ret.description.append(MODEL_PATH);
            ret.description.append(", reason: ");
            ret.description.append(err.c_str());
            return core::unexpected(core::move(ret));
        }

        if (!warn.empty()) {
            fmt::print("WARN: {}\n", warn.c_str());
        }

        core::HashMap<Vertex, u32> uniqueVertices;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = core::v(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );

                vertex.texCoord = core::v(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                );

                vertex.color = core::v(1.0f, 1.0f, 1.0f);

                u32* indicesCount = uniqueVertices.get(vertex);
                if (!indicesCount) {
                    uniqueVertices.put(vertex, u32(m_vertices.len()));
                    m_vertices.append(vertex);
                    indicesCount = uniqueVertices.get(vertex);
                }
                m_indices.append(*indicesCount);
            }
        }

        fmt::print("Loaded model: {}\n", MODEL_PATH);
        fmt::print("Vertices: {}\n", m_vertices.len());
        fmt::print("Indices: {}\n", m_indices.len());

        return {};
    }

    core::expected<Error> createVertexBuffer() {
        VkDeviceSize bufferSize = m_vertices.byteLen();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        {
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, bufferSize,
                                    usage, props, stagingBuffer, stagingBufferMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        defer {
            vkDestroyBuffer(m_vkDevice, stagingBuffer, nullptr);
            vkFreeMemory(m_vkDevice, stagingBufferMemory, nullptr);
        };

        void* data;
        if (vkMapMemory(m_vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan vertex buffer mapping failed", VulkanMapMemoryFailed });
        }
            core::memcopy(data, m_vertices.data(), bufferSize);
        vkUnmapMemory(m_vkDevice, stagingBufferMemory);

        {
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, bufferSize,
                                    usage, props, m_vkVertexBuffer, m_vkVertexBufferMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        {
            auto res = copyBuffer(stagingBuffer, m_vkVertexBuffer, bufferSize);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        return {};
    }

    core::expected<Error> createIndexBuffer() {
        VkDeviceSize bufferSize = m_indices.byteLen();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        {
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, bufferSize,
                                    usage, props, stagingBuffer, stagingBufferMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        defer {
            vkDestroyBuffer(m_vkDevice, stagingBuffer, nullptr);
            vkFreeMemory(m_vkDevice, stagingBufferMemory, nullptr);
        };

        void* data;
        if (vkMapMemory(m_vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan index buffer mapping failed", VulkanMapMemoryFailed });
        }
            core::memcopy(data, m_indices.data(), bufferSize);
        vkUnmapMemory(m_vkDevice, stagingBufferMemory);

        {
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                       VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, bufferSize,
                                    usage, props, m_vkIndexBuffer, m_vkIndexBufferMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        {
            auto res = copyBuffer(stagingBuffer, m_vkIndexBuffer, bufferSize);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        }

        return {};
    }

    core::expected<Error> createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        m_vkUniformBuffers.fill(0, 0, MAX_FRAMES_IN_FLIGHT);
        m_vkUniformBuffersMemory.fill(0, 0, MAX_FRAMES_IN_FLIGHT);
        m_vkUniformBuffersMapped.fill(0, 0, MAX_FRAMES_IN_FLIGHT);

        for (addr_size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            auto& ubo = m_vkUniformBuffers[i];
            auto& uboMemory = m_vkUniformBuffersMemory[i];
            VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            auto res = createBuffer(m_vkPhysicalDevice, m_vkDevice, bufferSize, usage, props, ubo, uboMemory);
            if (res.hasErr()) {
                return core::unexpected<Error>(core::move(res.err()));
            }

            void** uboMapped = &m_vkUniformBuffersMapped[i];
            if (vkMapMemory(m_vkDevice, uboMemory, 0, bufferSize, 0, uboMapped) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan uniform buffer mapping failed", VulkanMapMemoryFailed });
            }
        }

        return {};
    }

    core::expected<Error> createDescriptorPool() {
        VkDescriptorPoolSize poolSizes[2] = {};
        constexpr addr_size poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]);
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = u32(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = u32(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSizeCount;
        poolInfo.pPoolSizes = poolSizes;
        poolInfo.maxSets = u32(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(m_vkDevice, &poolInfo, nullptr, &m_vkDescriptorPool) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan descriptor pool creation failed", VulkanDescriptorPoolCreationFailed });
        }

        return {};
    }

    core::expected<Error> createDescriptorSets() {
        core::Arr<VkDescriptorSetLayout> layouts (MAX_FRAMES_IN_FLIGHT);
        layouts.fill(m_vkDescriptorSetLayout, 0, MAX_FRAMES_IN_FLIGHT);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_vkDescriptorPool;
        allocInfo.descriptorSetCount = u32(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        m_vkDescriptorSets.fill(VK_NULL_HANDLE, 0, MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(m_vkDevice, &allocInfo, m_vkDescriptorSets.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan descriptor set allocation failed", VulkanDescriptorSetAllocationFailed });
        }

         for (addr_size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_vkUniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = m_vkTextureImageView;
            imageInfo.sampler = m_vkTextureSampler;

            VkWriteDescriptorSet descriptorWrites[2] = {};
            constexpr addr_size descriptorWriteCount = sizeof(descriptorWrites) / sizeof(descriptorWrites[0]);

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = m_vkDescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = m_vkDescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(m_vkDevice, descriptorWriteCount, descriptorWrites, 0, nullptr);
        }

        return {};
    }

    core::expected<Error> createCommandBuffers() {
        m_vkCommandBuffers = core::Arr<VkCommandBuffer> (MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_vkCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = m_vkCommandBuffers.len();

        if (vkAllocateCommandBuffers(m_vkDevice, &allocInfo, m_vkCommandBuffers.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan command buffer creation failed", VulkanCommandBufferCreationFailed });
        }

        return {};
    }

    core::expected<Error> createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Start the fence in the signaled state.

        m_vkImageAvailableSemaphores = core::Arr<VkSemaphore> (MAX_FRAMES_IN_FLIGHT);
        m_vkRenderFinishedSemaphores = core::Arr<VkSemaphore> (MAX_FRAMES_IN_FLIGHT);
        m_vkInFlightFences = core::Arr<VkFence> (MAX_FRAMES_IN_FLIGHT);

        for (addr_size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkImageAvailableSemaphores[i]) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan semaphore creation failed", VulkanSemaphoreCreationFailed });
            }
            if (vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &m_vkRenderFinishedSemaphores[i]) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan semaphore creation failed", VulkanSemaphoreCreationFailed });
            }
            if (vkCreateFence(m_vkDevice, &fenceInfo, nullptr, &m_vkInFlightFences[i]) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan fence creation failed", VulkanFenceCreationFailed });
            }
        }

        return {};
    }

    core::expected<Error> recreateSwapChain() {
        glfwGetFramebufferSize(m_glfwWindow, &m_width, &m_height);
        while (m_width == 0 || m_height == 0) {
            glfwGetFramebufferSize(m_glfwWindow, &m_width, &m_height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_vkDevice);

        cleanupSwapChain();

        if (auto res = createSwapChain(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createImageViews(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createDepthResources(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createFramebuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        return {};
    }

    core::expected<Error> copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);

        return {};
    }

    void transitionImageLayout(VkImage image,
                               VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout,
                               u32 mipLevels) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;

        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (hasStencilComponent(format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else {
            Assert(false, "unsupported layout transition!");
        }

        vkCmdPipelineBarrier(commandBuffer,
                             sourceStage, destinationStage,
                             0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        endSingleTimeCommands(commandBuffer);
    }

    void copyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        endSingleTimeCommands(commandBuffer);
    }

    void cleanupSwapChain() {
        vkDestroyImageView(m_vkDevice, m_vkDepthImageView, nullptr);
        vkDestroyImage(m_vkDevice, m_vkDepthImage, nullptr);
        vkFreeMemory(m_vkDevice, m_vkDepthImageMemory, nullptr);

        for (addr_size i = 0; i < m_vkSwapChainFrameBuffers.len(); i++) {
            vkDestroyFramebuffer(m_vkDevice, m_vkSwapChainFrameBuffers[i], nullptr);
        }

        for (addr_size i = 0; i < m_vkSwapChainImageViews.len(); i++) {
            vkDestroyImageView(m_vkDevice, m_vkSwapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(m_vkDevice, m_vkSwapChain, nullptr);
    }

#pragma endregion

    void mainLoop() {
        while (!glfwWindowShouldClose(m_glfwWindow)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(m_vkDevice);
    }

    core::expected<Error> recordCommandBuffer(VkCommandBuffer commandBuffer, u32 idx) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan command buffer recording failed", VulkanBeginCommandBufferFailed });
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_vkRenderPass;
        renderPassInfo.framebuffer = m_vkSwapChainFrameBuffers[idx];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_vkSwapChainExtent;

        VkClearValue clearValues[2] = {};
        constexpr addr_size clearValuesCount = sizeof(clearValues) / sizeof(clearValues[0]);
        clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = clearValuesCount;
        renderPassInfo.pClearValues = clearValues;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkGraphicsPipeline);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = f32(m_vkSwapChainExtent.width);
            viewport.height = f32(m_vkSwapChainExtent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = { 0, 0 };
            scissor.extent = m_vkSwapChainExtent;
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { m_vkVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, m_vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkPipelineLayout, 0, 1,
                                    &m_vkDescriptorSets[m_currentFrame], 0, nullptr);

            // vkCmdDraw(commandBuffer, u32(m_vertices.len()), 1, 0, 0);
            vkCmdDrawIndexed(commandBuffer, u32(m_indices.len()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan command buffer recording failed", VulkanEndCommandBufferFailed });
        }

        return {};
    }

    void updateUniformBuffer(u64 currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        f32 time = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = core::rotateRight(core::mat4f::identity(), Z_AXIS, core::degToRad(time * 40.0f));
        ubo.view = core::lookAtRH(core::v(2.0f, 2.0f, 2.0f), core::v(0.0f, 0.0f, 0.0f), Z_AXIS);
        core::radians fovy = core::degToRad(45.0f);
        f32 aspectRatio = f32(m_vkSwapChainExtent.width) / f32(m_vkSwapChainExtent.height);
        f32 nearPlane = 0.1f;
        f32 farPlane = 10.0f;
        ubo.proj = core::perspectiveRH_NO(fovy, aspectRatio, nearPlane, farPlane);
        ubo.proj[1][1] *= -1; // Flip the Y coordinate. Vulklan uses a different coordinate system than OpenGL.

        core::memcopy(m_vkUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void drawFrame() {
        // 1. Wait for the previous frame to finish

        if (auto res = vkWaitForFences(m_vkDevice, 1, &m_vkInFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX); res != VK_SUCCESS) {
            Panic("Failed to wait for fence.");
        }

        // 2. Acquire an image from the swapchain

        u32 imageIndex;
        if (auto res = vkAcquireNextImageKHR(m_vkDevice, m_vkSwapChain, UINT64_MAX, m_vkImageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex); res != VK_SUCCESS) {
            if (res == VK_ERROR_OUT_OF_DATE_KHR) {
                if (auto res = recreateSwapChain(); res.hasErr()) {
                    Panic("Failed to recreate swapchain.");
                }
                return;
            }
            else if (res == VK_SUBOPTIMAL_KHR) {
                // TODO: Might want to re-create the swapchain in this case.
                fmt::print("[WARN] Swapchain is suboptimal.\n");
            }
            else {
                Panic("Failed to acquire swapchain image.");
            }
        }

        // 2. Update descriptors

        updateUniformBuffer(m_currentFrame);

        // 3. Reset the fence before using it again.

        if (auto res = vkResetFences(m_vkDevice, 1, &m_vkInFlightFences[m_currentFrame]); res != VK_SUCCESS) {
            Panic("Failed to reset fence.");
        }

        // 4. Record a command buffer which draws the scene onto the image

        if (auto res = vkResetCommandBuffer(m_vkCommandBuffers[m_currentFrame], 0); res != VK_SUCCESS) {
            Panic("Failed to reset command buffer.");
        }
        if (auto res = recordCommandBuffer(m_vkCommandBuffers[m_currentFrame], imageIndex); res.hasErr()) {
            Panic("Failed to record command buffer.");
        }

        // 5. Submit the recorded command buffer

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_vkImageAvailableSemaphores[m_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_vkCommandBuffers[m_currentFrame];

        VkSemaphore signalSemaphores[] = { m_vkRenderFinishedSemaphores[m_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (auto res = vkQueueSubmit(m_vkGraphicsQueue, 1, &submitInfo, m_vkInFlightFences[m_currentFrame]); res != VK_SUCCESS) {
            Panic("Failed to submit draw command buffer.");
        }

        // 6. Present the swapchain image

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_vkSwapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        {
            auto res = vkQueuePresentKHR(m_vkPresetQueue, &presentInfo);
            if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
                if (auto res = recreateSwapChain(); res.hasErr()) {
                    Panic("Failed to recreate swapchain.");
                }
            }
            else if (res != VK_SUCCESS) {
                Panic("Failed to present swapchain image.");
            }
        }

        m_currentFrame = (m_currentFrame + 1) & (MAX_FRAMES_IN_FLIGHT - 1);
    }

    void cleanup() {
        cleanupSwapChain();

        vkDestroySampler(m_vkDevice, m_vkTextureSampler, nullptr);
        vkDestroyImageView(m_vkDevice, m_vkTextureImageView, nullptr);

        vkDestroyImage(m_vkDevice, m_vkTextureImage, nullptr);
        vkFreeMemory(m_vkDevice, m_vkTextureImageMemory, nullptr);

        for (addr_size i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(m_vkDevice, m_vkUniformBuffers[i], nullptr);
            vkFreeMemory(m_vkDevice, m_vkUniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(m_vkDevice, m_vkDescriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(m_vkDevice, m_vkDescriptorSetLayout, nullptr);

        vkDestroyBuffer(m_vkDevice, m_vkVertexBuffer, nullptr);
        vkFreeMemory(m_vkDevice, m_vkVertexBufferMemory, nullptr);

        vkDestroyBuffer(m_vkDevice, m_vkIndexBuffer, nullptr);
        vkFreeMemory(m_vkDevice, m_vkIndexBufferMemory, nullptr);

        vkDestroyPipeline(m_vkDevice, m_vkGraphicsPipeline, nullptr);
        vkDestroyPipelineLayout(m_vkDevice, m_vkPipelineLayout, nullptr);

        vkDestroyRenderPass(m_vkDevice, m_vkRenderPass, nullptr);

        for (addr_size i = 0; i < m_vkRenderFinishedSemaphores.len(); i++) {
            vkDestroySemaphore(m_vkDevice, m_vkRenderFinishedSemaphores[i], nullptr);
        }
        for (addr_size i = 0; i < m_vkImageAvailableSemaphores.len(); i++) {
            vkDestroySemaphore(m_vkDevice, m_vkImageAvailableSemaphores[i], nullptr);
        }
        for (addr_size i = 0; i < m_vkInFlightFences.len(); i++) {
            vkDestroyFence(m_vkDevice, m_vkInFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(m_vkDevice, m_vkCommandPool, nullptr);

        vkDestroyDevice(m_vkDevice, nullptr);

        #if USE_VALIDATORS
            wrap_vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr);
        #endif

        vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
        vkDestroyInstance(m_vkInstance, nullptr);

        glfwDestroyWindow(m_glfwWindow);
        glfwTerminate();
    }

    // GLFW statekeeping:
    GLFWwindow* m_glfwWindow = nullptr;
    i32 m_width = 0;
    i32 m_height = 0;
    const char* m_title = nullptr;

    // Application statekeeping:
    u64 m_currentFrame = 0;

    // Vulkan statekeeping:
    VkInstance m_vkInstance = VK_NULL_HANDLE;
    core::Arr<const char*> m_vkActiveExtensions;
    core::Arr<VkExtensionProperties> m_vkSupportedExtensions;
    core::Arr<const char*> m_vkActiveValidationLayers;
    core::Arr<VkLayerProperties> m_vkSupportedValidationLayers;
    core::Arr<const char*> m_vkActiveDeviceExtensions;
    VkDebugUtilsMessengerEXT m_vkDebugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_vkDevice = VK_NULL_HANDLE;
    VkQueue m_vkGraphicsQueue = VK_NULL_HANDLE;
    VkSurfaceKHR m_vkSurface = VK_NULL_HANDLE;
    VkQueue m_vkPresetQueue = VK_NULL_HANDLE;
    VkSwapchainKHR m_vkSwapChain = VK_NULL_HANDLE;
    core::Arr<VkImage> m_vkSwapChainImages;
    VkExtent2D m_vkSwapChainExtent = {};
    core::Arr<VkImageView> m_vkSwapChainImageViews;
    VkFormat m_vkSwapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkDescriptorSetLayout m_vkDescriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout m_vkPipelineLayout = VK_NULL_HANDLE;
    VkRenderPass m_vkRenderPass = VK_NULL_HANDLE;
    VkPipeline m_vkGraphicsPipeline = VK_NULL_HANDLE;
    core::Arr<VkFramebuffer> m_vkSwapChainFrameBuffers;

    // Command Pools and Buffers
    VkCommandPool m_vkCommandPool = VK_NULL_HANDLE;
    core::Arr<VkCommandBuffer> m_vkCommandBuffers;

    // Sync Objects
    core::Arr<VkSemaphore> m_vkImageAvailableSemaphores;
    core::Arr<VkSemaphore> m_vkRenderFinishedSemaphores;
    core::Arr<VkFence> m_vkInFlightFences;

    // Vertices
    core::Arr<Vertex> m_vertices;
    VkBuffer m_vkVertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_vkVertexBufferMemory = VK_NULL_HANDLE;

    // Indices
    core::Arr<u32> m_indices;
    VkBuffer m_vkIndexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_vkIndexBufferMemory = VK_NULL_HANDLE;

    // Uniform Buffers
    core::Arr<VkBuffer> m_vkUniformBuffers;
    core::Arr<VkDeviceMemory> m_vkUniformBuffersMemory;
    core::Arr<void*> m_vkUniformBuffersMapped;

    // Descriptor Pools and Sets
    VkDescriptorPool m_vkDescriptorPool = VK_NULL_HANDLE;
    core::Arr<VkDescriptorSet> m_vkDescriptorSets;

    // Textures
    u32 m_mipLevels = 0;
    VkImage m_vkTextureImage;
    VkDeviceMemory m_vkTextureImageMemory;
    VkImageView m_vkTextureImageView;
    VkSampler m_vkTextureSampler;

    // Depth Buffer Image
    VkImage m_vkDepthImage;
    VkDeviceMemory m_vkDepthImageMemory;
    VkImageView m_vkDepthImageView;
};

// NEXT: Start from here -> https://vulkan-tutorial.com/Multisampling

i32 main() {
    constexpr const char* APP_TITLE = "Vulkan Example App";
    Application app = app.create({ 800, 600, APP_TITLE });
    if (auto res = app.run(); res.hasErr()) {
        fmt::print(stderr, "Error: {}\n", res.err().description.view().data());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

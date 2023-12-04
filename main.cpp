#include <init_core.h>

#include <cstdlib>

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

    FailedToLoadShader,

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

        case FailedToLoadShader:                       return "FailedToLoadShader";

        case SENTINEL: return "SENTINEL";
    }

    return "Unknown";
}

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
                                //  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT; // TODO: Do I want this?
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
        fmt::print("GLFW window initialization\n");

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
        fmt::print("Vulkan initialization\n");

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

        if (auto res = createGraphicsPipeline(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createFramebuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createCommandPool(); res.hasErr()) {
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
        fmt::print("Vulkan instance creation\n");

        // [STEP 1] Create Vulkan application info:
        fmt::print("  [STEP 1] Create Vulkan application info\n");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_title;
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

        // [STEP 2] Create Vulkan instance info:
        fmt::print("  [STEP 2] Create Vulkan instance info\n");

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // [STEP 3] Extensions. Identify required Vulkan extensions and check if the system supports them:
        fmt::print("  [STEP 3] Extensions\n");

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
        fmt::print("  [STEP 4] Validation layers\n");

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
        fmt::print("  [STEP 5] Create the Vulkan instance\n");

        if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance creation failed", VulkanInstanceCreationFailed });
        }

        // FIXME: Needs additional code to work on MACOS!

        return {};
    }

    core::expected<Error> createDebugMessenger() {
        fmt::print("Creating Debug Messenger\n");

        VkDebugUtilsMessengerCreateInfoEXT createInfo = createDebugMessengerInfo();
        if (wrap_vkCreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &m_vkDebugMessenger) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan debug messenger creation failed", VulkanDebugMessengerCreationFailed });
        }
        return {};
    }

    core::expected<Error> pickPhysicalDevice() {
        fmt::print("Picking a physical device\n");

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

        // The most important check for device suitability is the type of queue families supported by the device.
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        bool ret = indices.isComplete();
        return ret;
    }

    core::expected<Error> createLogicalDevice() {
        fmt::print("Creating logical device\n");

        // [STEP 1] Create a queue family info.
        fmt::print("  [STEP 1] Create a queue family info\n");
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
        fmt::print("  [STEP 2] Specify used device features\n");
        VkPhysicalDeviceFeatures deviceFeatures{};

        // [STEP 3] Create the logical device info.
        fmt::print("  [STEP 3] Create the logical device info\n");
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
        fmt::print("  [STEP 4] Create the device\n");
        if (vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkDevice) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan logical device creation failed", VulkanDeviceCreationFailed });
        }

        // [STEP 5] Get the graphics queue.
        fmt::print("  [STEP 5] Get the graphics queues\n");
        vkGetDeviceQueue(m_vkDevice, queueIndices.graphicsFamily, 0, &m_vkGraphicsQueue);
        vkGetDeviceQueue(m_vkDevice, queueIndices.presentFamily, 0, &m_vkPresetQueue);

        return {};
    }

    core::expected<Error> createSurface() {
        fmt::print("Creating a Surface\n");

        // Vulkan can't interface with the windowing system directly to present images to the screen.
        // It needs an intermediary object called a Surface, which is platform dependent and here it's handled by GLFW.
        if (glfwCreateWindowSurface(m_vkInstance, m_glfwWindow, nullptr, &m_vkSurface) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface creation failed", VulkanDeviceCreationFailed });
        }

        return {};
    }

    core::expected<Error> createSwapChain() {
        fmt::print("Creating a Swap Chain\n");

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
        m_vkChainImageFormat = surfaceFormat.format;

        return {};
    }

    core::expected<Error> createImageViews() {
        fmt::print("Creating image views\n");

        m_vkSwapChainImageViews = core::Arr<VkImageView> (m_vkSwapChainImages.len());

        for (addr_size i = 0; i < m_vkSwapChainImages.len(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_vkSwapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_vkChainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_vkSwapChainImageViews[i]) != VK_SUCCESS) {
                return core::unexpected<Error>({ "Vulkan image view creation failed",  });
            }
        }

        return {};
    }

    core::expected<Error> createGraphicsPipeline() {
        fmt::print("Creating graphics pipeline\n");

        static constexpr const char* VERT_SHADER_PATH = ASSETS_PATH "/shaders/01_hardcoded_triangle.vert.spv";

        fmt::print("  [STEP 1] Load vertex shader code\n");
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

        static constexpr const char* FRAG_SHADER_PATH = ASSETS_PATH "/shaders/01_hardcoded_triangle.frag.spv";

        fmt::print("  [STEP 2] Load fragment shader code\n");
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

        fmt::print("  [STEP 3] Create shader modules\n");
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

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

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
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        fmt::print("  [STEP 4] Create pipeline layout\n");

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

        fmt::print("  [STEP 5] Create graphics pipeline\n");

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
        fmt::print("Creating render pass\n");

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_vkChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_vkDevice, &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan render pass creation failed", VulkanRenderPassCreationFailed });
        }

        return {};
    }

    core::expected<Error> createFramebuffers() {
        fmt::print("Creating framebuffers\n");

        m_vkSwapChainFrameBuffers = core::Arr<VkFramebuffer> (m_vkSwapChainImageViews.len());

        for (addr_size i = 0; i < m_vkSwapChainImageViews.len(); i++) {
            VkImageView attachments[] = {
                m_vkSwapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_vkRenderPass;
            framebufferInfo.attachmentCount = 1;
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
        fmt::print("Creating command pool\n");

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

    core::expected<Error> createCommandBuffers() {
        fmt::print("Creating command buffers\n");

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
        fmt::print("Creating sync objects\n");

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
        fmt::println("Recreating swapchain");

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

        if (auto res = createFramebuffers(); res.hasErr()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        return {};
    }

    void cleanupSwapChain() {
        for (addr_size i = 0; i < m_vkSwapChainFrameBuffers.len(); i++) {
            vkDestroyFramebuffer(m_vkDevice, m_vkSwapChainFrameBuffers[i], nullptr);
        }

        for (addr_size i = 0; i < m_vkSwapChainImageViews.len(); i++) {
            vkDestroyImageView(m_vkDevice, m_vkSwapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(m_vkDevice, m_vkSwapChain, nullptr);
    }

#pragma endregion

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

        VkClearValue clearColor{};
        clearColor.color = { 0.0f, 0.0f, 0.0f, 1.0f };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

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

            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan command buffer recording failed", VulkanEndCommandBufferFailed });
        }

        return {};
    }

    void mainLoop() {
        fmt::print("Starting Main loop\n");

        while (!glfwWindowShouldClose(m_glfwWindow)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(m_vkDevice);
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

        if (auto res = vkResetFences(m_vkDevice, 1, &m_vkInFlightFences[m_currentFrame]); res != VK_SUCCESS) {
            Panic("Failed to reset fence.");
        }

        // 3. Record a command buffer which draws the scene onto the image

        if (auto res = vkResetCommandBuffer(m_vkCommandBuffers[m_currentFrame], 0); res != VK_SUCCESS) {
            Panic("Failed to reset command buffer.");
        }
        if (auto res = recordCommandBuffer(m_vkCommandBuffers[m_currentFrame], imageIndex); res.hasErr()) {
            Panic("Failed to record command buffer.");
        }

        // 4. Submit the recorded command buffer

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

        // 5. Present the swapchain image

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
        fmt::print("Running cleaning up code\n");

        cleanupSwapChain();

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
    VkFormat m_vkChainImageFormat = VK_FORMAT_UNDEFINED;
    VkPipelineLayout m_vkPipelineLayout = VK_NULL_HANDLE;
    VkRenderPass m_vkRenderPass = VK_NULL_HANDLE;
    VkPipeline m_vkGraphicsPipeline = VK_NULL_HANDLE;
    core::Arr<VkFramebuffer> m_vkSwapChainFrameBuffers;
    VkCommandPool m_vkCommandPool;
    core::Arr<VkCommandBuffer> m_vkCommandBuffers;
    core::Arr<VkSemaphore> m_vkImageAvailableSemaphores;
    core::Arr<VkSemaphore> m_vkRenderFinishedSemaphores;
    core::Arr<VkFence> m_vkInFlightFences;
    u64 m_currentFrame = 0;
};

// NEXT: Start from here -> https://vulkan-tutorial.com/Drawing_a_triangle/Swap_chain_recreation
// After that go back and re-read the previous chapters.

i32 main() {
    constexpr const char* APP_TITLE = "Vulkan Example App";
    Application app = app.create({ 800, 600, APP_TITLE });
    if (auto res = app.run(); res.hasErr()) {
        fmt::print(stderr, "Error: {}\n", res.err().description.view().data());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

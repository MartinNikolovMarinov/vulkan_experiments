#include <init_core.h>

#include <cstdlib>

enum ErrorType : i32 {
    None,

    GLFWInitFailed,
    GLFWWindowCreationFailed,

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

    SENTINEL
};

const char* errorTypeToCptr(ErrorType t) {
    switch (t) {
        case None:                                     return "None";

        case GLFWInitFailed:                           return "GLFWInitFailed";
        case GLFWWindowCreationFailed:                 return "GLFWWindowCreationFailed";

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

        case SENTINEL: return "SENTINEL";
    }

    return "Unknown";
}

struct Error {
    sb description = {};
    ErrorType type = None;
};

core::expected<core::arr<VkExtensionProperties>, Error> getAllSupportedVkExtensions() {
    u32 extCount = 0;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
    }

    core::arr<VkExtensionProperties> exts (extCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
    }

    return exts;
}

core::expected<core::arr<VkLayerProperties>, Error> getAllSupportedVkValidationLayers() {
    u32 layerCount = 0;
    if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
    }

    core::arr<VkLayerProperties> layers (layerCount);
    if (vkEnumerateInstanceLayerProperties(&layerCount, layers.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
    }

    return layers;
}

core::expected<core::arr<VkExtensionProperties>, Error> getAllSupportedVkDeviceExtensions(VkPhysicalDevice device) {
    u32 extCount = 0;
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan device extension enumeration failed", VulkanListDeviceExtensionsFailed });
    }

    core::arr<VkExtensionProperties> exts (extCount);
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, exts.data()) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan device extension enumeration failed", VulkanListDeviceExtensionsFailed });
    }

    return exts;
}

bool checkExtensionSupport(
    const core::arr<const char*>& extensions,
    const core::arr<VkExtensionProperties>& supported
) {
    for (addr_size i = 0; i < extensions.len(); i++) {
        const char* ext = extensions[i];
        addr_size extLen = core::cptr_len(ext);
        bool found = false;
        for (addr_size j = 0; j < supported.len(); j++) {
            const char* sup = supported[j].extensionName;
            if (core::cptr_eq(ext, sup, extLen)) {
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
    const core::arr<const char*>& validationLayers,
    const core::arr<VkLayerProperties>& supported
) {
    for (addr_size i = 0; i < validationLayers.len(); i++) {
        const char* vlayer = validationLayers[i];
        addr_size vlen = core::cptr_len(vlayer);
        bool found = false;
        for (addr_size j = 0; j < supported.len(); j++) {
            const char* sup = supported[j].layerName;
            if (core::cptr_eq(vlayer, sup, vlen)) {
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
    createInfo.pNext = nullptr;
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

static VkResult wrap_vkCreateDebugUtilsMessengerEXT(
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

static void wrap_vkDestroyDebugUtilsMessengerEXT(
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

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    // Get the number of queue families:
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    // Get the queue families:
    core::arr<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
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
    core::arr<VkSurfaceFormatKHR> formats;
    core::arr<VkPresentModeKHR> presentModes;
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
        details.formats = core::arr<VkSurfaceFormatKHR> (formatCount);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface formats query failed", VulkanSwapChainSupportQueryFailed });
        }
    }

    u32 presentModeCount = 0;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS) {
        return core::unexpected<Error>({ "Vulkan surface present modes query failed", VulkanSwapChainSupportQueryFailed });
    }

    if (presentModeCount != 0) {
        details.presentModes = core::arr<VkPresentModeKHR> (presentModeCount);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan surface present modes query failed", VulkanSwapChainSupportQueryFailed });
        }
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const core::arr<VkSurfaceFormatKHR>& availableFormats) {
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

VkPresentModeKHR chooseSwapPresentMode(const core::arr<VkPresentModeKHR>& availablePresentModes) {
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

    struct AppProps {
        i32 width;
        i32 height;
        const char* title;
    };

    core::expected<Error> run() {
        initCore();

        if (auto ret = initWindow(); ret.has_err()) {
            return core::unexpected<Error>(core::move(ret.err()));
        }

        if (auto ret = initVulkan(); ret.has_err()) {
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

        // Set glfw error callback:
        glfwSetErrorCallback([](i32 error, const char* description) {
            fmt::print(stderr, "GLFW error: {} - {}\n", error, description);
        });

        // Hint to GLFW that we don't want to use OpenGL:
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Disable window resizing:
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create the GLFW window:
        m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
        if (!m_glfwWindow) {
            return core::unexpected<Error>({ "GLFW window creation failed", GLFWWindowCreationFailed });
        }

        return {};
    }

#pragma region Initialize Vulkan

    core::expected<Error> initVulkan() {
        fmt::print("Vulkan initialization\n");

        if (auto res = createInstance(); res.has_err()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        #if USE_VALIDATORS
            if (auto res = createDebugMessenger(); res.has_err()) {
                return core::unexpected<Error>(core::move(res.err()));
            }
        #endif

        if (auto res = createSurface(); res.has_err()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = pickPhysicalDevice(); res.has_err()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createLogicalDevice(); res.has_err()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createSwapChain(); res.has_err()) {
            return core::unexpected<Error>(core::move(res.err()));
        }

        if (auto res = createImageViews(); res.has_err()) {
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
        appInfo.pNext = nullptr;
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
                if (res.has_err()) {
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
                if (res.has_err()) {
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
        core::arr<VkPhysicalDevice> phyDevices (deviceCount);
        vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, phyDevices.data());

        // Append the required device extensions:
        m_vkActiveDeviceExtensions.append(VK_KHR_SWAPCHAIN_EXTENSION_NAME); // This application needs swapchain support.

        // Pick the first suitable device:
        for (addr_size i = 0; i < phyDevices.len(); i++) {
            VkPhysicalDevice device = phyDevices[i];
            auto res = isDeviceSutable(device, m_vkSurface);
            if (res.has_err()) return core::unexpected<Error>(core::move(res.err()));
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
        if (supportedDeviceExt.has_err()) {
            return core::unexpected<Error>(core::move(supportedDeviceExt.err()));
        }

        // Check if the required extensions are supported:
        if (!checkExtensionSupport(m_vkActiveDeviceExtensions, supportedDeviceExt.value())) {
            return false;
        }

        // Check if the swapchain is adaquite:
        {
            auto res = querySwapChainSupport(device, surface);
            if (res.has_err()) {
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

        core::arr<VkDeviceQueueCreateInfo> queueCreateInfos;
        core::hash_set<u32> uniqueQueueFamilies;
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
            queueCreateInfo.pNext = nullptr;
            queueCreateInfo.queueFamilyIndex = key;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.append(core::move(queueCreateInfo));
        });

        // [STEP 2] Specify used device features.
        fmt::print("  [STEP 2] Specify used device features\n");
        VkPhysicalDeviceFeatures deviceFeatures{};

        // [STEP 3] Create the logical device info.
        fmt::print("  [STEP 3] Create the logical device info\n");
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = nullptr;
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
        SwapChainSupportDetails swapChainSupport;
        {
            auto ret = querySwapChainSupport(m_vkPhysicalDevice, m_vkSurface);
            if (ret.has_err()) {
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
        createInfo.pNext = nullptr;
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
        m_vkSwapChainImages = core::arr<VkImage> (imageCount);
        vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &imageCount, m_vkSwapChainImages.data());
        m_vkSwapChainExtent = extent;
        m_vkChainImageFormat = surfaceFormat.format;

        return {};
    }

    core::expected<Error> createImageViews() {
        m_vkSwapChainImageViews = core::arr<VkImageView> (m_vkSwapChainImages.len());

        for (addr_size i = 0; i < m_vkSwapChainImages.len(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.pNext = nullptr;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
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

#pragma endregion

    void mainLoop() {
        fmt::print("Starting Main loop\n");

        while (!glfwWindowShouldClose(m_glfwWindow)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        fmt::print("Running cleaning up code\n");

        #if USE_VALIDATORS
            wrap_vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr);
        #endif

        for (addr_size i = 0; i < m_vkSwapChainImageViews.len(); i++) {
            vkDestroyImageView(m_vkDevice, m_vkSwapChainImageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(m_vkDevice, m_vkSwapChain, nullptr);
        vkDestroyDevice(m_vkDevice, nullptr);
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
    core::arr<const char*> m_vkActiveExtensions;
    core::arr<VkExtensionProperties> m_vkSupportedExtensions;
    core::arr<const char*> m_vkActiveValidationLayers;
    core::arr<VkLayerProperties> m_vkSupportedValidationLayers;
    core::arr<const char*> m_vkActiveDeviceExtensions;
    VkDebugUtilsMessengerEXT m_vkDebugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_vkDevice = VK_NULL_HANDLE;
    VkQueue m_vkGraphicsQueue = VK_NULL_HANDLE;
    VkSurfaceKHR m_vkSurface = VK_NULL_HANDLE;
    VkQueue m_vkPresetQueue = VK_NULL_HANDLE;
    VkSwapchainKHR m_vkSwapChain = VK_NULL_HANDLE;
    core::arr<VkImage> m_vkSwapChainImages;
    VkExtent2D m_vkSwapChainExtent = {};
    core::arr<VkImageView> m_vkSwapChainImageViews;
    VkFormat m_vkChainImageFormat = VK_FORMAT_UNDEFINED;
};

i32 main() {
    constexpr const char* APP_TITLE = "Vulkan Example App";
    Application app = app.create({ 800, 600, APP_TITLE });
    if (auto res = app.run(); res.has_err()) {
        fmt::print(stderr, "Error: {}\n", res.err().description.view().data());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

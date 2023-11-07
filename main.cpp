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

core::expected<Error> checkExtensionSupport(
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
            Error err;
            err.type = VulkanListExtensionsFailed;
            err.description.append("Vulkan extension not supported: ");
            err.description.append(ext);
            return core::unexpected<Error>(core::move(err));
        }
    }

    return {};
}

core::expected<Error> checkValidationLayerSupport(
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
            Error err;
            err.type = VulkanListValidationLayersFailed;
            err.description.append("Vulkan validation layer not supported: ");
            err.description.append(vlayer);
            return core::unexpected<Error>(core::move(err));
        }
    }

    return {};
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
    i64 graphicsFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0;
    }
};

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    core::arr<VkQueueFamilyProperties> queueFamilies (queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (addr_size i = 0; i < queueFamilies.len(); i++) {
        VkQueueFamilyProperties queueFamily = queueFamilies[i];
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i64(i);
        }

        if (indices.isComplete()) {
            break;
        }
    }

    return indices;
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

        // Create a window:
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

        if (auto res = pickPhysicalDevice(); res.has_err()) {
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
            if (auto err = checkExtensionSupport(m_vkActiveExtensions, m_vkSupportedExtensions); err.has_err()) {
                return core::unexpected<Error>(core::move(err.err()));
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
            if (auto err = checkValidationLayerSupport(m_vkActiveValidationLayers, m_vkSupportedValidationLayers); err.has_err()) {
                return core::unexpected<Error>(core::move(err.err()));
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
            return core::unexpected<Error>({ "Vulkan debug messenger creation failed", VulkanInstanceCreationFailed });
        }
        return {};
    }

    core::expected<Error> pickPhysicalDevice() {
        fmt::print("Picking a physical device\n");

        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            return core::unexpected<Error>({ "No vulkan compatible devices found", VulkanNoSupportedDevicesErr });
        }

        core::arr<VkPhysicalDevice> devices (deviceCount);
        vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

        auto isDeviceSutable = [](VkPhysicalDevice device) -> bool {
            QueueFamilyIndices indices = findQueueFamilies(device);
            bool ret = indices.isComplete();
            return ret;
        };

        for (addr_size i = 0; i < devices.len(); i++) {
            VkPhysicalDevice device = devices[i];
            if (isDeviceSutable(device)) {
                m_vkPhysicalDevice = device;
                break;
            }
        }

        if (m_vkPhysicalDevice == VK_NULL_HANDLE) {
            return core::unexpected<Error>({ "No vulkan suitable devices found", VulkanNoSupportedDevicesErr });
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
    VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
    core::arr<const char*> m_vkActiveExtensions;
    core::arr<VkExtensionProperties> m_vkSupportedExtensions;
    core::arr<const char*> m_vkActiveValidationLayers;
    core::arr<VkLayerProperties> m_vkSupportedValidationLayers;
    VkDebugUtilsMessengerEXT m_vkDebugMessenger = VK_NULL_HANDLE;
    QueueFamilyIndices m_vkQueueFamilyIndices = {};
};

i32 main() {
    // Vulkan initialization steps:
    // Create an instance (VkInstance)
    // Select a supported graphics card (VkPhysicalDevice)
    // Create a VkDevice and VkQueue for drawing and presentation
    // Create a window, window surface and swap chain
    // Wrap the swap chain images into VkImageView
    // Create a render pass that specifies the render targets and usage
    // Create framebuffers for the render pass
    // Set up the graphics pipeline
    // Allocate and record a command buffer with the draw commands for every possible swap chain image
    // Draw frames by acquiring images, submitting the right draw command buffer and returning the images back to the swap chain

    constexpr const char* APP_TITLE = "Vulkan Example App";
    Application app = app.create({ 800, 600, APP_TITLE });
    Expect(app.run(), "Run failed.");
    return EXIT_SUCCESS;
}

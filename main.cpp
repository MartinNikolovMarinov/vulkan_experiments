#include <init_core.h>

#include <cstdlib>

enum ErrorType : i32 {
    None,

    GLFWInitFailed,
    GLFWWindowCreationFailed,

    VulkanInstanceCreationFailed,
    VulkanListExtensionsFailed,
    VulkanListValidationLayersFailed,

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

        case SENTINEL: return "SENTINEL";
    }

    return "Unknown";
}

struct Error {
    sb description = {};
    ErrorType type = None;
};

struct Application {
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
    template <typename CB>
    static core::expected<Error> walkAllVkExtensions(CB cb) {
        u32 extCount = 0;
        if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
        }

        VkExtensionProperties exts[extCount];
        if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance extension enumeration failed", VulkanListExtensionsFailed });
        }

        for (u32 i = 0; i < extCount; ++i) {
            if (!cb(exts[i])) return {};
        }

        return {};
    }

    template <typename CB>
    static core::expected<Error> walkAllVkValidationLayers(CB cb) {
        u32 layerCount = 0;
        if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
        }

        VkLayerProperties layers[layerCount];
        if (vkEnumerateInstanceLayerProperties(&layerCount, layers) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance layer enumeration failed", VulkanListValidationLayersFailed });
        }

        for (u32 i = 0; i < layerCount; ++i) {
            if (!cb(layers[i])) return {};
        }

        return {};
    }

    static core::expected<Error> checkExtensionsSupport(const core::arr<const char*>& extNames) {
        for (u32 i = 0; i < extNames.len(); ++i) {
            const char* extName = extNames[i];
            addr_size extNameLen = core::cptr_len(extName);

            bool found = false;
            auto res = walkAllVkExtensions([&found, &extName, &extNameLen] (const VkExtensionProperties& e) {
                bool shouldContinueWalk = true;
                if (core::cptr_eq(e.extensionName, extName, extNameLen)) {
                    found = true;
                    shouldContinueWalk = false;
                }
                return shouldContinueWalk;
            });
            if (res.has_err()) {
                // Walk failed.
                return core::unexpected<Error>(core::move(res.err()));
            }

            if (!found) {
                Error err;
                err.type = VulkanInstanceCreationFailed;
                err.description.append("Required Vulkan extension ");
                err.description.append(extName, extNameLen);
                err.description.append(" is not supported.");
                return core::unexpected<Error>(core::move(err));
            }
        }

        return {};
    }

    static core::expected<Error> checkValidationLayerSupport(const core::arr<const char*>& validationLayers) {
        for (u32 i = 0; i < validationLayers.len(); ++i) {
            const char* layerName = validationLayers[i];
            addr_size layerNameLen = core::cptr_len(layerName);

            bool found = false;
            auto res = walkAllVkValidationLayers([&found, &layerName, &layerNameLen] (const VkLayerProperties& l) {
                bool shouldContinueWalk = true;
                if (core::cptr_eq(l.layerName, layerName, layerNameLen)) {
                    found = true;
                    shouldContinueWalk = false;
                }
                return shouldContinueWalk;
            });
            if (res.has_err()) {
                // Walk failed.
                return core::unexpected<Error>(core::move(res.err()));
            }

            if (!found) {
                Error err;
                err.type = VulkanInstanceCreationFailed;
                err.description.append("Required Vulkan validation layer ");
                err.description.append(layerName, layerNameLen);
                err.description.append(" is not supported.");
                return core::unexpected<Error>(core::move(err));
            }
        }

        return {};
    }

    static VkResult createDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    ) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void destroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator
    ) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    core::expected<Error> initWindow() {
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
        m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
        if (!m_window) {
            return core::unexpected<Error>({ "GLFW window creation failed", GLFWWindowCreationFailed });
        }

        return {};
    }

    core::expected<Error> createInstance() {
        // Create Vulkan application info:

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = m_title;
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

        // Create Vulkan instance info:

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Extensions
        {
            // Required Extensions:

            // Add Vulkan extensions required by GLFW:
            u32 glfwExtensionsCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
            m_vkExtNames.append(glfwExtensions, glfwExtensionsCount);

            // Check if the required extensions are supported:
            if (auto err = checkExtensionsSupport(m_vkExtNames); err.has_err()) {
                return core::unexpected<Error>(core::move(err.err()));
            }

            // Optional Extensions:

#ifndef NDEBUG
            // Add Vulkan SDK standard debug extensions:
            m_vkExtNames.append(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            // Set the extensions in the createInfo:

            createInfo.enabledExtensionCount = u32(m_vkExtNames.len());
            createInfo.ppEnabledExtensionNames = m_vkExtNames.data();
        }

#ifndef NDEBUG
        // Validation layers:
        {
            // Add Vulkan SDK standard validation layers:
            m_vkValidationLayers.append("VK_LAYER_KHRONOS_validation");

            // Check if the required validation layers are supported:
            if (auto err = checkValidationLayerSupport(m_vkValidationLayers); err.has_err()) {
                return core::unexpected<Error>(core::move(err.err()));
            }

            // Set the required validation layers in the createInfo:
            createInfo.enabledLayerCount = m_vkValidationLayers.len();
            createInfo.ppEnabledLayerNames = m_vkValidationLayers.data();

            VkDebugUtilsMessengerCreateInfoEXT dmcinfo = createDebugMessengerInfo();
            createInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&dmcinfo);
        }
#else
        createInfo.enabledLayerCount = 0;
#endif

        // Create the Vulkan instance:
        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan instance creation failed", VulkanInstanceCreationFailed });
        }

        // FIXME: Needs additional code to work on MACOS!

        return {};
    }

    VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.pNext = nullptr;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
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

    core::expected<Error> createDebugMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = createDebugMessengerInfo();
        if (createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
            return core::unexpected<Error>({ "Vulkan debug messenger creation failed", VulkanInstanceCreationFailed });
        }
        return {};
    }

    core::expected<Error> initVulkan() {
        if (auto err = createInstance(); err.has_err()) {
            return core::unexpected<Error>(core::move(err.err()));
        }
#ifndef NDEBUG
        if (auto err = createDebugMessenger(); err.has_err()) {
            return core::unexpected<Error>(core::move(err.err()));
        }
#endif
        return {};
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
#ifndef NDEBUG
        destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
#endif
        vkDestroyInstance(m_instance, nullptr);
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    GLFWwindow* m_window = nullptr;
    i32 m_width = 0;
    i32 m_height = 0;
    const char* m_title = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;
    core::arr<const char*> m_vkExtNames;
    core::arr<const char*> m_vkValidationLayers;
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT m_debugMessenger;
#endif
};

i32 main() {
    // Create a VkInstance
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

#include <init_core.h>

struct Application {
    static constexpr u32 width = 800;
    static constexpr u32 height = 600;
    static constexpr const char* title = "Vulkan example";

    void run() {
        initCore();
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow() {
        glfwInit();

        // Hint to GLFW that we don't want to use OpenGL:
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Disable window resizing:
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create a window:
        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    void initVulkan() {

    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    GLFWwindow* m_window;
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

    Application app;
    app.run();
    return EXIT_SUCCESS;
}

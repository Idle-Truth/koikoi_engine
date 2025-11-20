#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>


class KoiKoi {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

//region References and Memory
    GLFWwindow* window;
    VkInstance instance;
//endregion

//region Helper Functions
    bool isDeviceSuitable(VkPhysicalDevice device) {
        return true;
    }
//endregion


    void initWindow() {

        //constants to set dimensions for window
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;

        //glfw specific function that creates a glfw instance
        glfwInit();

        // Tells glfw to not create OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Tells glfw not to be resizeable
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Window creation call
        window = glfwCreateWindow(WIDTH, HEIGHT, "KoiKoi", nullptr, nullptr);


    }

    void initVulkan() {
        createInstance();
        pickPhysicalDevice();

    }

    void createInstance() {

        // Informs driver of engine information
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "KoiKoi";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Tells Vulkan which global extensions and validation layers are being used
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Setups glfw extensions through glfw function glfwGetRequiredInstanceExtensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //region Available Extensions Check
        // Uncomment code to list all available extensions to CLI
        /*
        std::cout << "available extensions:\n";

        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
        */
    //endregion


        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void pickPhysicalDevice() {

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        // Lists all physical devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //region Available Devices Check
        // Uncomment code to list all available devices to CLI
        /*
        std::cout << "available physical devices:\n";

        for (const auto& device : devices) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            std::cout << '\t' << deviceProperties.deviceName << '\n';
        }
        */
        //endregion

        // Edge Case if no devices are found
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }

    void mainLoop() {

        // Loop that keeps glfw window running
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // Destroys instance and frees it's memory
        vkDestroyInstance(instance, nullptr);

        // Destroys window instance and frees it's memory
        glfwDestroyWindow(window);

        // Destroys glfw instance and frees it's memory
        glfwTerminate();

    }
};



int main() {
    KoiKoi app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
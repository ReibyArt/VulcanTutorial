// СЛОИ ВАЛИДАЦИИ ВУЛКАН //
// Message callback //


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Validation layers

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

// if not DEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else 
const bool enableValidationLayers = true;
#endif 



class HelloTriangleApplication {

public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    GLFWwindow* window;
    VkInstance instance;

    // Создание экземпляра, который соединяет наше приложение и библиотеку Vulkan + информация о нашем приложении
    void createInstance() {

        // Проверка слоёв валидации на доступность // 
       /* if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation Layers requested, but not avaliable!");
        }*/

        


        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        

        // Необязательный параметр, сообщающий драйверу информацию о глобальных расширениях и уровнях проверки (?)
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Extendions For GLFW
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Check if validation layers is enabled
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);       


        // Count extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties>extensionss(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
            extensionss.data());

        std::cout << "My Extensions -> " << extensionCount << std::endl;
        std::cout << "Avalilable Extensions: ->\n";
        for (const auto& extensionsss : extensionss)
        {
            std::cout << '\t' << extensionsss.extensionName << '\n';
        }


        // Подсчёт расширений загружаемых vulkan
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
        
        std::cout <<"Validation Layers -> "<< enableValidationLayers << std::endl;
    }

        // Layers of Support Validation //
        bool checkValidationLayerSupport() {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
            return false;

            for (const char* layerName : validationLayers)
            {
                bool layerFound = false;
                for (const auto& layerProperties : availableLayers )
                {
                    if (strcmp(layerName, layerProperties.layerName) == 0) {
                        layerFound = true;
                        break;
                    }
                }
                if (!layerFound) {
                    return false;
                }
            }
                return true;
        }


        // List of extension whether validation layers are enable or not //
        std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;

            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            if (enableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
            return extensions;
        }


        // Callback Function // 
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {
            std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
            return false;
        }






    // для инициализации GLFW и создания окна
    void initWindow() {    
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //
        window = glfwCreateWindow(WIDTH, HEIGHT, "VulcanReiby", nullptr, nullptr);
    }
    // Инициализация Vulkan
    void initVulkan() {
        std::cout << "Init The Vulkan......" << std::endl;
        createInstance();
        setupDebugMessenger();
    }

    void setupDebugMessenger() {
        std::cout << "DebuggerMessenger in true!\n";
        if (!enableValidationLayers) {
        std::cout << "DebuggerMessenger in false!\n";
            return;
        }
    }

    // Опишем главный цикл
    void mainLoop() {  
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

        }
    }
    // Очистка ресурсов
    void cleanup() {

        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};


 // MAIN FUNCTION // 
int main() {
    
    HelloTriangleApplication app;


    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

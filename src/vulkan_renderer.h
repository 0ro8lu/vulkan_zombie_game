#ifndef _VULKAN_RENDERER_H_
#define _VULKAN_RENDERER_H_

#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

#include <vk_mem_alloc.h>

#include <optional>
#include <vector>

#include "utils.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

struct InstanceData {
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
    uint32_t textureIndex;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(InstanceData);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 1;
        attributeDescriptions[0].location = 2;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(InstanceData, position);

        attributeDescriptions[1].binding = 1;
        attributeDescriptions[1].location = 3;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(InstanceData, rotation);

        attributeDescriptions[2].binding = 1;
        attributeDescriptions[2].location = 4;
        attributeDescriptions[2].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(InstanceData, scale);

        attributeDescriptions[3].binding = 1;
        attributeDescriptions[3].location = 5;
        attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[3].offset = offsetof(InstanceData, textureIndex);

        return attributeDescriptions;
    }
};

enum BufferType {
    STAGING_BUFFER,
    GPU_BUFFER, 
};

class VulkanRenderer {
public:
    VulkanRenderer(GLFWwindow* window, uint32_t max_layers, uint32_t max_num_objects, uint32_t max_frames_fligth);
    ~VulkanRenderer();

    void wait();
    void drawFrame(const std::vector<DynamicUBO> gameObjectData);
    void draw(const std::vector<std::pair<VkBuffer, size_t>>& renderData);
    
    bool framebufferResized = false;
private:

    void initVulkan();   
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createVMAAllocator();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createTextureArray();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();

    void allocateInstanceBuffer(size_t bufferLen, VkBuffer& instanceBuffer, VmaAllocation& instanceBufferAllocation);
    void uploadToInstanceBuffer(VkBuffer& instanceBuffer, const std::vector<InstanceData>& instanceData);
    void deallocateInstanceBuffer(VkBuffer& instanceBuffer, VmaAllocation& instanceBufferAllocation);

    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkValidationLayerSupport();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    // void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void recordCommandBuffer(VkCommandBuffer commandBuffer, std::vector<std::pair<VkBuffer, size_t>> renderData, uint32_t imageIndex);

    void recreateSwapChain();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkImageView createImageView(VkImage image, VkFormat format);

    void updateUniformBuffer(uint32_t currentImage);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void* createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, BufferType bufferType, VkBuffer& buffer, VmaAllocation& allocation);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaAllocationCreateFlagBits flags, VkImage& image, VmaAllocation& allocation, uint32_t layerCount);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, std::vector<VkBufferImageCopy> &bufferCopyRegions);

    std::vector<const char*> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void cleanupSwapChain();

    GLFWwindow* window;

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

#ifdef __APPLE__
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"
    };
#else
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
#endif

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<Vertex> vertices = {
        {{0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f}, {0.0f, 1.0f}},
        {{0.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    const uint32_t max_layers;
    const uint32_t max_num_objects;
    const uint32_t max_frames_flight;

    uint32_t numObjects;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    std::vector<VkBuffer> staticUniformBuffers;
    // std::vector<VkBuffer> dynamicUniformBuffers;
    std::vector<VmaAllocation> staticUniformBuffersAllocation;
    std::vector<VmaAllocation> dynamicDynamicBuffersAllocation;
    std::vector<void*> staticUniformBuffersMapped;
    std::vector<void*> dynamicUniformBuffersMapped;
    size_t dynamicAlignment = 0;

    VkImage textureImage;
    VmaAllocation textureAllocation;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferAllocation;
    VkBuffer indexBuffer;
    VmaAllocation indexBufferAllocation;

    VkCommandPool commandPool;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VmaAllocator allocator;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    friend class GameGraphicsManager;
};

#endif

#pragma once
#include "constants.h"
#include "Utils.h"

class SyncManager
{
public:

    SyncManager();
    ~SyncManager();

    void createSyncObjects(VkDevice device);
    void destroySyncObjects(VkDevice device);

    inline std::vector<VkSemaphore> getAvailableSemaphores() const noexcept { return _imageAvailableSemaphores; }
    inline std::vector<VkSemaphore> getFinishedSemaphores() const noexcept { return _renderFinishedSemaphores; }
    inline std::vector<VkFence> getInFlightFences() const noexcept { return _inFlightFences; }

private:

    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;

};


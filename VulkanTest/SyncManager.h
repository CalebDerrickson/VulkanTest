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

    std::vector<VkSemaphore> getAvailableSemaphores() { return _imageAvailableSemaphores; }
    std::vector<VkSemaphore> getFinishedSemaphores() { return _renderFinishedSemaphores; }
    std::vector<VkFence> getInFlightFences() { return _inFlightFences; }

private:

    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;

};


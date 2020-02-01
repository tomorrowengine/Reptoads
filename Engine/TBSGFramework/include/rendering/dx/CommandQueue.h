#pragma once





#ifdef PLATFORM_WINDOWS
#include <d3d12.h>              
#include <wrl.h>                

#include <atomic>               
#include <cstdint>              
#include <condition_variable>   

#include "ThreadSafeQueue.h"

class CommandList;

class CommandQueue
{
public:
    CommandQueue(D3D12_COMMAND_LIST_TYPE type);
    virtual ~CommandQueue();

    
    std::shared_ptr<CommandList> GetCommandList();

    
    
    uint64_t ExecuteCommandList(std::shared_ptr<CommandList> commandList);
    uint64_t ExecuteCommandLists( const std::vector<std::shared_ptr<CommandList> >& commandLists );

    uint64_t Signal();
    bool IsFenceComplete(uint64_t fenceValue);
    void WaitForFenceValue(uint64_t fenceValue);
    void Flush();

    
    void Wait( const CommandQueue& other );

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;

private:
    
    void ProccessInFlightCommandLists();

    
    
    
    using CommandListEntry = std::tuple<uint64_t, std::shared_ptr<CommandList> >;

    D3D12_COMMAND_LIST_TYPE                         m_CommandListType;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>      m_d3d12CommandQueue;
    Microsoft::WRL::ComPtr<ID3D12Fence>             m_d3d12Fence;
    std::atomic_uint64_t                            m_FenceValue;

    ThreadSafeQueue<CommandListEntry>               m_InFlightCommandLists;
    ThreadSafeQueue<std::shared_ptr<CommandList> >  m_AvailableCommandLists;

    
    std::thread m_ProcessInFlightCommandListsThread;
    std::atomic_bool m_bProcessInFlightCommandLists;
    std::mutex m_ProcessInFlightCommandListsThreadMutex;
    std::condition_variable m_ProcessInFlightCommandListsThreadCV;
};
#endif

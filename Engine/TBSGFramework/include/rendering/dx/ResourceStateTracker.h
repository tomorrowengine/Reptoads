#pragma once




#ifdef PLATFORM_WINDOWS

#include <d3d12.h>

#include <mutex>
#include <map>
#include <unordered_map>
#include <vector>

class CommandList;
class Resource;

class ResourceStateTracker
{
public:
    ResourceStateTracker();
    virtual ~ResourceStateTracker();

    
    void ResourceBarrier(const D3D12_RESOURCE_BARRIER& barrier);

    
    void TransitionResource( ID3D12Resource* resource, D3D12_RESOURCE_STATES stateAfter, UINT subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES );
    void TransitionResource(const Resource& resource, D3D12_RESOURCE_STATES stateAfter, UINT subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

    
    void UAVBarrier(const Resource* resource = nullptr);

    
    void AliasBarrier(const Resource* resourceBefore = nullptr, const Resource* resourceAfter = nullptr);

    
    uint32_t FlushPendingResourceBarriers(CommandList& commandList);

    
    void FlushResourceBarriers(CommandList& commandList);

    
    void CommitFinalResourceStates();

    
    void Reset();

    
    static void Lock();

    
    static void Unlock();

    
    static void AddGlobalResourceState(ID3D12Resource* resource, D3D12_RESOURCE_STATES state);

    
    static void RemoveGlobalResourceState(ID3D12Resource* resource);

protected:

private:
    
    using ResourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>;

    
    
    
    ResourceBarriers m_PendingResourceBarriers;

    
    ResourceBarriers m_ResourceBarriers;

    
    struct ResourceState
    {
        
        explicit ResourceState(D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON)
            : State(state)
        {}

        
        void SetSubresourceState(UINT subresource, D3D12_RESOURCE_STATES state)
        {
            if (subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
            {
                State = state;
                SubresourceState.clear();
            }
            else
            {
                SubresourceState[subresource] = state;
            }
        }

        
        
        
        
        D3D12_RESOURCE_STATES GetSubresourceState(UINT subresource) const
        {
            D3D12_RESOURCE_STATES state = State;
            const auto iter = SubresourceState.find(subresource);
            if (iter != SubresourceState.end())
            {
                state = iter->second;
            }
            return state;
        }

        
        
        D3D12_RESOURCE_STATES State;
        std::map<UINT, D3D12_RESOURCE_STATES> SubresourceState;
    };

    using ResourceStateMap = std::unordered_map<ID3D12Resource*, ResourceState>;

    
    
    
    ResourceStateMap m_FinalResourceState;

public:
    
    
    static ResourceStateMap ms_GlobalResourceState;

private:
    
    static std::mutex ms_GlobalMutex;
    static bool ms_IsLocked;
};
#endif

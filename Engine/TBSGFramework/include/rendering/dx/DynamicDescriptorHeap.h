#pragma once




#ifdef PLATFORM_WINDOWS
#include "d3dx12.h"

#include <wrl.h>

#include <cstdint>
#include <memory>
#include <queue>

class CommandList;
class RootSignature;

class DynamicDescriptorHeap
{
public:
    DynamicDescriptorHeap(
        D3D12_DESCRIPTOR_HEAP_TYPE heapType,
        uint32_t numDescriptorsPerHeap = 1024);

    virtual ~DynamicDescriptorHeap();

    
    void StageDescriptors(uint32_t rootParameterIndex, uint32_t offset, uint32_t numDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptors);

    
    void CommitStagedDescriptors( CommandList& commandList, std::function<void(ID3D12GraphicsCommandList*, UINT, D3D12_GPU_DESCRIPTOR_HANDLE)> setFunc );
    void CommitStagedDescriptorsForDraw(CommandList& commandList);
    void CommitStagedDescriptorsForDispatch(CommandList& commandList);

    
    D3D12_GPU_DESCRIPTOR_HANDLE CopyDescriptor( CommandList& comandList, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor);

    
    void ParseRootSignature( const RootSignature& rootSignature);

    
    void Reset();

protected:

private:
    
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RequestDescriptorHeap();
    
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap();

    
    
    uint32_t ComputeStaleDescriptorCount() const;

    
    static const uint32_t MaxDescriptorTables = 32;

    
    struct DescriptorTableCache
    {
        DescriptorTableCache()
            : NumDescriptors(0)
            , BaseDescriptor(nullptr)
        {}

        
        void Reset()
        {
            NumDescriptors = 0;
            BaseDescriptor = nullptr;
        }

        
        uint32_t NumDescriptors;
        
        D3D12_CPU_DESCRIPTOR_HANDLE* BaseDescriptor;
    };

    
    
    
    
    
    
    
    D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorHeapType;

    
    uint32_t m_NumDescriptorsPerHeap;

    
    uint32_t m_DescriptorHandleIncrementSize;

    
    std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> m_DescriptorHandleCache;

    
    DescriptorTableCache m_DescriptorTableCache[MaxDescriptorTables];

    
    
    uint32_t m_DescriptorTableBitMask;
    
    
    
    uint32_t m_StaleDescriptorTableBitMask;

    using DescriptorHeapPool = std::queue< Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> >;

    DescriptorHeapPool m_DescriptorHeapPool;
    DescriptorHeapPool m_AvailableDescriptorHeaps;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CurrentDescriptorHeap;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_CurrentGPUDescriptorHandle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_CurrentCPUDescriptorHandle;

    uint32_t m_NumFreeHandles;
    
};
#endif

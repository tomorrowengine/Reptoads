#include <rendering/dx/DX12LibPCH.h>
#include "core/Assertion.h"
#ifdef PLATFORM_WINDOWS

#include <rendering/dx/DynamicDescriptorHeap.h>

#include <rendering/dx/DX12Application.h>
#include <rendering/dx/CommandList.h>
#include <rendering/dx/RootSignature.h>

DynamicDescriptorHeap::DynamicDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t numDescriptorsPerHeap)
    : m_DescriptorHeapType(heapType)
    , m_NumDescriptorsPerHeap(numDescriptorsPerHeap)
    , m_DescriptorTableBitMask(0)
    , m_StaleDescriptorTableBitMask(0)
    , m_CurrentCPUDescriptorHandle(D3D12_DEFAULT)
    , m_CurrentGPUDescriptorHandle(D3D12_DEFAULT)
    , m_NumFreeHandles(0)
{
    m_DescriptorHandleIncrementSize = DX12Application::Get().GetDescriptorHandleIncrementSize(heapType);

    
    m_DescriptorHandleCache = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE[]>(m_NumDescriptorsPerHeap);
}

DynamicDescriptorHeap::~DynamicDescriptorHeap()
{}

void DynamicDescriptorHeap::ParseRootSignature(const RootSignature& rootSignature)
{
    
    
    m_StaleDescriptorTableBitMask = 0;

    const auto& rootSignatureDesc = rootSignature.GetRootSignatureDesc();

    
    
    m_DescriptorTableBitMask = rootSignature.GetDescriptorTableBitMask(m_DescriptorHeapType);
    uint32_t descriptorTableBitMask = m_DescriptorTableBitMask;

    uint32_t currentOffset = 0;
    DWORD rootIndex;
    while (_BitScanForward(&rootIndex, descriptorTableBitMask) && rootIndex < rootSignatureDesc.NumParameters)
    {
        uint32_t numDescriptors = rootSignature.GetNumDescriptors(rootIndex);

        DescriptorTableCache& descriptorTableCache = m_DescriptorTableCache[rootIndex];
        descriptorTableCache.NumDescriptors = numDescriptors;
        descriptorTableCache.BaseDescriptor = m_DescriptorHandleCache.get() + currentOffset;

        currentOffset += numDescriptors;

        
        descriptorTableBitMask ^= (1 << rootIndex);
    }

    
    ASSERT(currentOffset <= m_NumDescriptorsPerHeap && "The root signature requires more than the maximum number of descriptors per descriptor heap. Consider increasing the maximum number of descriptors per descriptor heap.");
}

void DynamicDescriptorHeap::StageDescriptors(uint32_t rootParameterIndex, uint32_t offset, uint32_t numDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptor)
{
    
    
    if (numDescriptors > m_NumDescriptorsPerHeap || rootParameterIndex >= MaxDescriptorTables )
    {
        throw std::bad_alloc();
    }

    DescriptorTableCache& descriptorTableCache = m_DescriptorTableCache[rootParameterIndex];

    
    
    if ( (offset + numDescriptors) > descriptorTableCache.NumDescriptors)
    {
        throw std::length_error("Number of descriptors exceeds the number of descriptors in the descriptor table.");
    }

    D3D12_CPU_DESCRIPTOR_HANDLE* dstDescriptor = (descriptorTableCache.BaseDescriptor + offset);
    for (uint32_t i = 0; i < numDescriptors; ++i)
    {
        dstDescriptor[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(srcDescriptor, i, m_DescriptorHandleIncrementSize);
    }

    
    
    m_StaleDescriptorTableBitMask |= (1 << rootParameterIndex);
}

uint32_t DynamicDescriptorHeap::ComputeStaleDescriptorCount() const
{
    uint32_t numStaleDescriptors = 0;
    DWORD i;
    DWORD staleDescriptorsBitMask = m_StaleDescriptorTableBitMask;

    while ( _BitScanForward( &i, staleDescriptorsBitMask ) )
    {
        numStaleDescriptors += m_DescriptorTableCache[i].NumDescriptors;
        staleDescriptorsBitMask ^= ( 1 << i );
    }

    return numStaleDescriptors;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DynamicDescriptorHeap::RequestDescriptorHeap()
{
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    if (!m_AvailableDescriptorHeaps.empty())
    {
        descriptorHeap = m_AvailableDescriptorHeaps.front();
        m_AvailableDescriptorHeaps.pop();
    }
    else
    {
        descriptorHeap = CreateDescriptorHeap();
        m_DescriptorHeapPool.push(descriptorHeap);
    }

    return descriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DynamicDescriptorHeap::CreateDescriptorHeap()
{
    auto device = DX12Application::Get().GetDevice();

    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
    descriptorHeapDesc.Type = m_DescriptorHeapType;
    descriptorHeapDesc.NumDescriptors = m_NumDescriptorsPerHeap;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    ThrowIfFailed(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap)));

    return descriptorHeap;
}

void DynamicDescriptorHeap::CommitStagedDescriptors(CommandList& commandList, std::function<void(ID3D12GraphicsCommandList*, UINT, D3D12_GPU_DESCRIPTOR_HANDLE)> setFunc)
{
    
    uint32_t numDescriptorsToCommit = ComputeStaleDescriptorCount();

    if ( numDescriptorsToCommit > 0 )
    {
        auto device = DX12Application::Get().GetDevice();
        auto d3d12GraphicsCommandList = commandList.GetGraphicsCommandList().Get();
        assert(d3d12GraphicsCommandList != nullptr);

        if ( !m_CurrentDescriptorHeap || m_NumFreeHandles < numDescriptorsToCommit )
        {
            m_CurrentDescriptorHeap = RequestDescriptorHeap();
            m_CurrentCPUDescriptorHandle = m_CurrentDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            m_CurrentGPUDescriptorHandle = m_CurrentDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
            m_NumFreeHandles = m_NumDescriptorsPerHeap;

            commandList.SetDescriptorHeap(m_DescriptorHeapType, m_CurrentDescriptorHeap.Get());

            
            
            
            m_StaleDescriptorTableBitMask = m_DescriptorTableBitMask;
        }

        DWORD rootIndex;
        
        while ( _BitScanForward( &rootIndex, m_StaleDescriptorTableBitMask ) )
        {
            UINT numSrcDescriptors = m_DescriptorTableCache[rootIndex].NumDescriptors;
            D3D12_CPU_DESCRIPTOR_HANDLE* pSrcDescriptorHandles = m_DescriptorTableCache[rootIndex].BaseDescriptor;

            D3D12_CPU_DESCRIPTOR_HANDLE pDestDescriptorRangeStarts[] =
            {
                m_CurrentCPUDescriptorHandle
            };
            UINT pDestDescriptorRangeSizes[] =
            {
                numSrcDescriptors
            };

            
            device->CopyDescriptors(1, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
                numSrcDescriptors, pSrcDescriptorHandles, nullptr, m_DescriptorHeapType);

            
            setFunc(d3d12GraphicsCommandList, rootIndex, m_CurrentGPUDescriptorHandle);

            
            m_CurrentCPUDescriptorHandle.Offset(numSrcDescriptors, m_DescriptorHandleIncrementSize);
            m_CurrentGPUDescriptorHandle.Offset(numSrcDescriptors, m_DescriptorHandleIncrementSize);
            m_NumFreeHandles -= numSrcDescriptors;

            
            m_StaleDescriptorTableBitMask ^= (1 << rootIndex);
        }
    }
}

void DynamicDescriptorHeap::CommitStagedDescriptorsForDraw(CommandList& commandList)
{
    CommitStagedDescriptors(commandList, &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable);
}

void DynamicDescriptorHeap::CommitStagedDescriptorsForDispatch(CommandList& commandList)
{
    CommitStagedDescriptors(commandList, &ID3D12GraphicsCommandList::SetComputeRootDescriptorTable);
}

D3D12_GPU_DESCRIPTOR_HANDLE DynamicDescriptorHeap::CopyDescriptor(CommandList& comandList, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor)
{
    if (!m_CurrentDescriptorHeap || m_NumFreeHandles < 1)
    {
        m_CurrentDescriptorHeap = RequestDescriptorHeap();
        m_CurrentCPUDescriptorHandle = m_CurrentDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_CurrentGPUDescriptorHandle = m_CurrentDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_NumFreeHandles = m_NumDescriptorsPerHeap;

        comandList.SetDescriptorHeap(m_DescriptorHeapType, m_CurrentDescriptorHeap.Get());

        
        
        
        m_StaleDescriptorTableBitMask = m_DescriptorTableBitMask;
    }

    auto device = DX12Application::Get().GetDevice();

    D3D12_GPU_DESCRIPTOR_HANDLE hGPU = m_CurrentGPUDescriptorHandle;
    device->CopyDescriptorsSimple(1, m_CurrentCPUDescriptorHandle, cpuDescriptor, m_DescriptorHeapType);

    m_CurrentCPUDescriptorHandle.Offset(1, m_DescriptorHandleIncrementSize);
    m_CurrentGPUDescriptorHandle.Offset(1, m_DescriptorHandleIncrementSize);
    m_NumFreeHandles -= 1;

    return hGPU;
}

void DynamicDescriptorHeap::Reset()
{
    m_AvailableDescriptorHeaps = m_DescriptorHeapPool;
    m_CurrentDescriptorHeap.Reset();
    m_CurrentCPUDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
    m_CurrentGPUDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
    m_NumFreeHandles = 0;
    m_DescriptorTableBitMask = 0;
    m_StaleDescriptorTableBitMask = 0;

    
    for (int i = 0; i < MaxDescriptorTables; ++i)
    {
        m_DescriptorTableCache[i].Reset();
    }
}
#endif

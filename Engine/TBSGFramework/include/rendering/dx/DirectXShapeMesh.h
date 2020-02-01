#pragma once



#ifdef PLATFORM_WINDOWS
#include <rendering/dx/CommandList.h>
#include <rendering/dx/VertexBuffer.h>
#include <rendering/dx/IndexBuffer.h>

#include <DirectXMath.h>
#include <d3d12.h>

#include <wrl.h>

#include <memory> 
#include <vector>

 
struct VertexPositionNormalTexture
{
    VertexPositionNormalTexture()
    { }

    VertexPositionNormalTexture(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT2& textureCoordinate)
        : position(position),
        normal(normal),
        textureCoordinate(textureCoordinate)
    { }

    VertexPositionNormalTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR textureCoordinate)
    {
        XMStoreFloat3(&this->position, position);
        XMStoreFloat3(&this->normal, normal);
        XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
    }

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 textureCoordinate;

    static const int InputElementCount = 3;
    static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

using VertexCollection = std::vector<VertexPositionNormalTexture>;
using IndexCollection = std::vector<uint32_t>;

class DirectXShapeMesh
{
public:

    void Draw(CommandList& commandList);

    static std::unique_ptr<DirectXShapeMesh> CreateCube(CommandList& commandList, float size = 1, bool rhcoords = false);
    static std::unique_ptr<DirectXShapeMesh> CreateSphere(CommandList& commandList, float diameter = 1, size_t tessellation = 16, bool rhcoords = false);
    static std::unique_ptr<DirectXShapeMesh> CreateCone(CommandList& commandList, float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = false);
    static std::unique_ptr<DirectXShapeMesh> CreateTorus(CommandList& commandList, float diameter = 1, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = false);
    static std::unique_ptr<DirectXShapeMesh> CreatePlane(CommandList& commandList, float width = 1, float height = 1, bool rhcoords = false);

protected:

private:
    friend struct std::default_delete<DirectXShapeMesh>;

    DirectXShapeMesh();
    DirectXShapeMesh(const DirectXShapeMesh& copy) = delete;
    virtual ~DirectXShapeMesh();

    void Initialize(CommandList& commandList, VertexCollection& vertices, IndexCollection& indices, bool rhcoords);

    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    UINT m_IndexCount;
};
#endif

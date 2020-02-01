#include <rendering/dx/DX12LibPCH.h>
#ifdef PLATFORM_WINDOWS
#include <rendering/dx/DirectXShapeMesh.h>

#include <rendering/dx/DX12Application.h>


using namespace DirectX;
using namespace Microsoft::WRL;

const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTexture::InputElements[] =
{
    { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 6*4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

DirectXShapeMesh::DirectXShapeMesh()
    : m_IndexCount(0)
{}

DirectXShapeMesh::~DirectXShapeMesh()
{
    
}

void DirectXShapeMesh::Draw(CommandList& commandList)
{
    commandList.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList.SetVertexBuffer(0, m_VertexBuffer);
    commandList.SetIndexBuffer(m_IndexBuffer);
    commandList.DrawIndexed(m_IndexCount);
}

std::unique_ptr<DirectXShapeMesh> DirectXShapeMesh::CreateSphere(CommandList& commandList, float diameter, size_t tessellation, bool rhcoords)
{
    VertexCollection vertices;
    IndexCollection indices;

    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    float radius = diameter / 2.0f;
    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - (float)i / verticalSegments;

        float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
        float dy, dxz;

        XMScalarSinCos(&dy, &dxz, latitude);

        
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = (float)j / horizontalSegments;

            float longitude = j * XM_2PI / horizontalSegments;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            vertices.push_back(VertexPositionNormalTexture(normal * radius, normal, textureCoordinate));
        }
    }

    
    size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = (j + 1) % stride;

            indices.push_back(static_cast<uint32_t>( i * stride + j ));
            indices.push_back(static_cast<uint32_t>(nextI * stride + j));
            indices.push_back(static_cast<uint32_t>(i * stride + nextJ));

            indices.push_back(static_cast<uint32_t>(i * stride + nextJ));
            indices.push_back(static_cast<uint32_t>(nextI * stride + j));
            indices.push_back(static_cast<uint32_t>(nextI * stride + nextJ));
        }
    }

    
    std::unique_ptr<DirectXShapeMesh> mesh(new DirectXShapeMesh());

    mesh->Initialize(commandList, vertices, indices, rhcoords);

    return mesh;
}

std::unique_ptr<DirectXShapeMesh> DirectXShapeMesh::CreateCube(CommandList& commandList, float size, bool rhcoords)
{
    
    const int FaceCount = 6;

    static const XMVECTORF32 faceNormals[FaceCount] =
    {
        { 0,  0,  1 },
        { 0,  0, -1 },
        { 1,  0,  0 },
        { -1,  0,  0 },
        { 0,  1,  0 },
        { 0, -1,  0 },
    };

    static const XMVECTORF32 textureCoordinates[4] =
    {
        { 1, 0 },
        { 1, 1 },
        { 0, 1 },
        { 0, 0 },
    };

    VertexCollection vertices;
    IndexCollection indices;

    size /= 2;

    
    for (int i = 0; i < FaceCount; i++)
    {
        XMVECTOR normal = faceNormals[i];

        
        XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

        XMVECTOR side1 = XMVector3Cross(normal, basis);
        XMVECTOR side2 = XMVector3Cross(normal, side1);

        
        size_t vbase = vertices.size();
        indices.push_back(static_cast<uint32_t>(vbase + 0));
        indices.push_back(static_cast<uint32_t>(vbase + 1));
        indices.push_back(static_cast<uint32_t>(vbase + 2));

        indices.push_back(static_cast<uint32_t>(vbase + 0));
        indices.push_back(static_cast<uint32_t>(vbase + 2));
        indices.push_back(static_cast<uint32_t>(vbase + 3));

        
        vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
        vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
        vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
        vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
    }

    
    std::unique_ptr<DirectXShapeMesh> mesh(new DirectXShapeMesh());

    mesh->Initialize(commandList, vertices, indices, rhcoords);

    return mesh;
}


static inline XMVECTOR GetCircleVector(size_t i, size_t tessellation)
{
    float angle = i * XM_2PI / tessellation;
    float dx, dz;

    XMScalarSinCos(&dx, &dz, angle);

    XMVECTORF32 v = { dx, 0, dz, 0 };
    return v;
}

static inline XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
{
    float angle = (i * XM_2PI / tessellation) + XM_PIDIV2;
    float dx, dz;

    XMScalarSinCos(&dx, &dz, angle);

    XMVECTORF32 v = { dx, 0, dz, 0 };
    return v;
}


static void CreateCylinderCap(VertexCollection& vertices, IndexCollection& indices, size_t tessellation, float height, float radius, bool isTop)
{
    
    for (size_t i = 0; i < tessellation - 2; i++)
    {
        size_t i1 = (i + 1) % tessellation;
        size_t i2 = (i + 2) % tessellation;

        if (isTop)
        {
            std::swap(i1, i2);
        }

        size_t vbase = vertices.size();
        indices.push_back(static_cast<uint32_t>(vbase));
        indices.push_back(static_cast<uint32_t>(vbase + i1));
        indices.push_back(static_cast<uint32_t>(vbase + i2));
    }

    
    XMVECTOR normal = g_XMIdentityR1;
    XMVECTOR textureScale = g_XMNegativeOneHalf;

    if (!isTop)
    {
        normal = -normal;
        textureScale *= g_XMNegateX;
    }

    
    for (size_t i = 0; i < tessellation; i++)
    {
        XMVECTOR circleVector = GetCircleVector(i, tessellation);

        XMVECTOR position = (circleVector * radius) + (normal * height);

        XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

        vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));
    }
}

std::unique_ptr<DirectXShapeMesh> DirectXShapeMesh::CreateCone(CommandList& commandList, float diameter, float height, size_t tessellation, bool rhcoords)
{
    VertexCollection vertices;
    IndexCollection indices;

    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    height /= 2;

    XMVECTOR topOffset = g_XMIdentityR1 * height;

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    
    for (size_t i = 0; i <= tessellation; i++)
    {
        XMVECTOR circlevec = GetCircleVector(i, tessellation);

        XMVECTOR sideOffset = circlevec * radius;

        float u = (float)i / tessellation;

        XMVECTOR textureCoordinate = XMLoadFloat(&u);

        XMVECTOR pt = sideOffset - topOffset;

        XMVECTOR normal = XMVector3Cross(GetCircleTangent(i, tessellation), topOffset - pt);
        normal = XMVector3Normalize(normal);

        
        vertices.push_back(VertexPositionNormalTexture(topOffset, normal, g_XMZero));
        vertices.push_back(VertexPositionNormalTexture(pt, normal, textureCoordinate + g_XMIdentityR1));

        indices.push_back(static_cast<uint32_t>(i * 2));
        indices.push_back(static_cast<uint32_t>((i * 2 + 3) % (stride * 2)));
        indices.push_back(static_cast<uint32_t>((i * 2 + 1) % (stride * 2)));
    }

    
    CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

    
    std::unique_ptr<DirectXShapeMesh> mesh(new DirectXShapeMesh());

    mesh->Initialize(commandList, vertices, indices, rhcoords);

    return mesh;
}

std::unique_ptr<DirectXShapeMesh> DirectXShapeMesh::CreateTorus(CommandList& commandList, float diameter, float thickness, size_t tessellation, bool rhcoords)
{
    VertexCollection vertices;
    IndexCollection indices;

    if (tessellation < 3)
        throw std::out_of_range("tessellation parameter out of range");

    size_t stride = tessellation + 1;

    
    for (size_t i = 0; i <= tessellation; i++)
    {
        float u = (float)i / tessellation;

        float outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

        
        
        XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

        
        for (size_t j = 0; j <= tessellation; j++)
        {
            float v = 1 - (float)j / tessellation;

            float innerAngle = j * XM_2PI / tessellation + XM_PI;
            float dx, dy;

            XMScalarSinCos(&dy, &dx, innerAngle);

            
            XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
            XMVECTOR position = normal * thickness / 2;
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            position = XMVector3Transform(position, transform);
            normal = XMVector3TransformNormal(normal, transform);

            vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));

            
            size_t nextI = (i + 1) % stride;
            size_t nextJ = (j + 1) % stride;

            indices.push_back(static_cast<uint32_t>(i * stride + j));
            indices.push_back(static_cast<uint32_t>(i * stride + nextJ));
            indices.push_back(static_cast<uint32_t>(nextI * stride + j));

            indices.push_back(static_cast<uint32_t>(i * stride + nextJ));
            indices.push_back(static_cast<uint32_t>(nextI * stride + nextJ));
            indices.push_back(static_cast<uint32_t>(nextI * stride + j));
        }
    }

    
    std::unique_ptr<DirectXShapeMesh> mesh(new DirectXShapeMesh());

    mesh->Initialize(commandList, vertices, indices, rhcoords);

    return mesh;
}

std::unique_ptr<DirectXShapeMesh> DirectXShapeMesh::CreatePlane(CommandList& commandList, float width, float height, bool rhcoords)
{
    VertexCollection vertices = 
    {
        { XMFLOAT3(-0.5f * width, 0.0f,  0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, 
        { XMFLOAT3( 0.5f * width, 0.0f,  0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }, 
        { XMFLOAT3( 0.5f * width, 0.0f, -0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, 
        { XMFLOAT3(-0.5f * width, 0.0f, -0.5f * height), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }  
    };
    
    IndexCollection indices = 
    {
        0, 3, 1, 1, 3, 2
    };

    std::unique_ptr<DirectXShapeMesh> mesh(new DirectXShapeMesh());

    mesh->Initialize(commandList, vertices, indices, rhcoords);

    return mesh;
}



static void ReverseWinding(IndexCollection& indices, VertexCollection& vertices)
{
    assert((indices.size() % 3) == 0);
    for (auto it = indices.begin(); it != indices.end(); it += 3)
    {
        std::swap(*it, *(it + 2));
    }

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->textureCoordinate.x = (1.f - it->textureCoordinate.x);
    }
}

void DirectXShapeMesh::Initialize(CommandList& commandList, VertexCollection& vertices, IndexCollection& indices, bool rhcoords)
{
    if (vertices.size() >= USHRT_MAX)
        throw std::exception("Too many vertices for 16-bit index buffer");

    if (!rhcoords)
        ReverseWinding(indices, vertices);

    commandList.CopyVertexBuffer(m_VertexBuffer, vertices);
    commandList.CopyIndexBuffer(m_IndexBuffer, indices);

    m_IndexCount = static_cast<UINT>(indices.size());
}
#endif

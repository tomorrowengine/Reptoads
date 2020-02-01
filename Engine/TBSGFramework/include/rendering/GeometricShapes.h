#pragma once
#include <rendering/Mesh.h>
#include "memory/Containers.h"

namespace gfx
{
	void CreateCube( float size, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, bool rhcoords = false);
	void CreateSphere( float diameter, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, size_t tessellation = 16, bool rhcoords = false);
	void CreateCone( float diameter, float height, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, size_t tessellation = 32, bool rhcoords = false);
	void CreateTorus( float diameter, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = false);
	void CreatePlane( float width, float height, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, bool rhcoords = false);

}

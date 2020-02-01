#include <rendering/GeometricShapes.h>
#include "core/Assertion.h"
#include <vector>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace gfx
{
	static const glm::vec3 colorWhite3{ 1.0f, 1.0f, 1.0f };

	
	static void ReverseWinding(ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices)
	{
		ASSERT((indices.size() % 3) == 0);
		for (auto it = indices.begin(); it != indices.end(); it += 3) {
			std::swap(*it, *(it + 2));
		}

		for (auto it = vertices.begin(); it != vertices.end(); ++it) {
			it->uv.x = (1.f - it->uv.x);
		}
	}

	void CreateSphere( float diameter, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, size_t tessellation, bool rhcoords)
	{
		
		if (tessellation < 3)
			throw std::out_of_range("tessellation parameter out of range");

		float radius = diameter / 2.0f;
		size_t verticalSegments = tessellation;
		size_t horizontalSegments = tessellation * 2;

		
		for (size_t i = 0; i <= verticalSegments; i++) {
			float v = 1 - (float)i / verticalSegments;

			float latitude = (i * glm::pi<float>() / verticalSegments) - glm::half_pi<float>();
			float dy, dxz;

			dy = glm::sin(latitude);
			dxz = glm::cos(latitude);
			

			
			for (size_t j = 0; j <= horizontalSegments; j++) {
				float u = (float)j / horizontalSegments;

				float longitude = j * glm::two_pi<float>() / horizontalSegments;
				float dx, dz;

				dx = glm::sin(longitude);
				dz = glm::cos(longitude);
				

				dx *= dxz;
				dz *= dxz;

				glm::vec3 normal(dx, dy, dz);
				glm::vec2 textureCoordinate(u, v);

				vertices.push_back(Vertex{ normal * radius, {1.0f, 1.0f, 1.0f},normal,textureCoordinate });
			}
		}

		
		size_t stride = horizontalSegments + 1;

		for (size_t i = 0; i < verticalSegments; i++) {
			for (size_t j = 0; j <= horizontalSegments; j++) {
				size_t nextI = i + 1;
				size_t nextJ = (j + 1) % stride;

				indices.push_back(static_cast<uint32_t>(i * stride + j));
				indices.push_back(static_cast<uint32_t>(nextI * stride + j));
				indices.push_back(static_cast<uint32_t>(i * stride + nextJ));

				indices.push_back(static_cast<uint32_t>(i * stride + nextJ));
				indices.push_back(static_cast<uint32_t>(nextI * stride + j));
				indices.push_back(static_cast<uint32_t>(nextI * stride + nextJ));
			}
		}

		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}

	void CreateCube( float size, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, bool rhcoords)
	{
		
		const int FaceCount = 6;

		static const glm::vec3 faceNormals[FaceCount] =
		{
			{ 0,  0,  1 },
			{ 0,  0, -1 },
			{ 1,  0,  0 },
			{ -1,  0,  0 },
			{ 0,  1,  0 },
			{ 0, -1,  0 },
		};

		static const glm::vec2 textureCoordinates[4] =
		{
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			{ 0, 0 },
		};

		size /= 2;

		
		for (int i = 0; i < FaceCount; i++) {
			glm::vec3 normal = faceNormals[i];

			
			glm::vec3 basis = (i >= 4) ? glm::vec3{ 0.0f, 0.0f, 1.0f } : glm::vec3{ 0.0f, 1.0f, 0.0f};

			glm::vec3 side1 = glm::cross(normal, basis);
			glm::vec3 side2 = glm::cross(normal, side1);

			
			size_t vbase = vertices.size();
			indices.push_back(static_cast<uint32_t>(vbase + 0));
			indices.push_back(static_cast<uint32_t>(vbase + 1));
			indices.push_back(static_cast<uint32_t>(vbase + 2));

			indices.push_back(static_cast<uint32_t>(vbase + 0));
			indices.push_back(static_cast<uint32_t>(vbase + 2));
			indices.push_back(static_cast<uint32_t>(vbase + 3));

			
			vertices.push_back(Vertex{(normal - side1 - side2) * size, colorWhite3, normal, textureCoordinates[0]});
			vertices.push_back(Vertex{(normal - side1 + side2) * size, colorWhite3, normal, textureCoordinates[1]});
			vertices.push_back(Vertex{(normal + side1 + side2) * size, colorWhite3, normal, textureCoordinates[2]});
			vertices.push_back(Vertex{(normal + side1 - side2) * size, colorWhite3, normal, textureCoordinates[3]});
		}

		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}

	
	static inline glm::vec3 GetCircleVector(size_t i, size_t tessellation)
	{
		float angle = i * glm::two_pi<float>() / tessellation;
		float dx, dz;

		dx = glm::sin(angle);
		dz = glm::cos(angle);
		

		
		return {dx, 0, dz};
	}

	static inline glm::vec3 GetCircleTangent(size_t i, size_t tessellation)
	{
		float angle = (i * glm::two_pi<float>() / tessellation) + glm::half_pi<float>();
		float dx, dz;

		dx = std::sin(angle);
		dz = std::cos(angle);
		

		
		return {dx, 0, dz};
	}

	
	static void CreateCylinderCap(ptl::vector<Vertex>& vertices, ptl::vector<uint32_t>& indices, size_t tessellation, float height, float radius, bool isTop)
	{
		
		for (size_t i = 0; i < tessellation - 2; i++) {
			size_t i1 = (i + 1) % tessellation;
			size_t i2 = (i + 2) % tessellation;

			if (isTop) {
				std::swap(i1, i2);
			}

			size_t vbase = vertices.size();
			indices.push_back(static_cast<uint32_t>(vbase));
			indices.push_back(static_cast<uint32_t>(vbase + i1));
			indices.push_back(static_cast<uint32_t>(vbase + i2));
		}

		
		glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
		glm::vec2 textureScale = { -0.5f, -0.5f };

		if (!isTop) {
			normal = -normal;
			textureScale *= glm::vec2{ -1.0f, 1.0f};
		}

		
		for (size_t i = 0; i < tessellation; i++) {
			glm::vec3 circleVector = GetCircleVector(i, tessellation);

			glm::vec3 position = (circleVector * radius) + (normal * height);
			glm::vec2 circleXZ{ circleVector.x, circleVector.z };
			glm::vec2 textureCoordinate = circleXZ * textureScale + glm::vec2{ 0.5f, 0.5f};

			vertices.push_back(Vertex{ position, colorWhite3, normal, textureCoordinate });
		}
	}

	void CreateCone( float diameter, float height, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, size_t tessellation, bool rhcoords)
	{

		if (tessellation < 3)
			throw std::out_of_range("tessellation parameter out of range");

		height /= 2;

		glm::vec3 topOffset = glm::vec3{ 0.0f, 1.0f, 0.0f } * height;

		float radius = diameter / 2;
		size_t stride = tessellation + 1;

		
		for (size_t i = 0; i <= tessellation; i++) {
			glm::vec3 circlevec = GetCircleVector(i, tessellation);

			glm::vec3 sideOffset = circlevec * radius;

			float u = (float)i / tessellation;

			glm::vec2 textureCoordinate{ u, 0.0f };

			glm::vec3 pt = sideOffset - topOffset;

			glm::vec3 normal = glm::cross(GetCircleTangent(i, tessellation), topOffset - pt);
			normal = glm::normalize(normal);

			
			vertices.push_back(Vertex{ topOffset, colorWhite3, normal, glm::vec2{0,0} });
			vertices.push_back(Vertex{ pt, colorWhite3, normal, textureCoordinate + glm::vec2{ 0.0f, 1.0f} });

			indices.push_back(static_cast<uint32_t>(i * 2));
			indices.push_back(static_cast<uint32_t>((i * 2 + 3) % (stride * 2)));
			indices.push_back(static_cast<uint32_t>((i * 2 + 1) % (stride * 2)));
		}

		
		CreateCylinderCap(vertices, indices, tessellation, height, radius, false);

		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}

	void CreateTorus( float diameter, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, float thickness, size_t tessellation, bool rhcoords)
	{

		if (tessellation < 3)
			throw std::out_of_range("tessellation parameter out of range");

		size_t stride = tessellation + 1;

		
		for (size_t i = 0; i <= tessellation; i++) {
			float u = (float)i / tessellation;

			float outerAngle = i * glm::two_pi<float>() / tessellation - glm::half_pi<float>();

			
			
			
			glm::mat4 transform = glm::rotate(glm::mat4{ 1 }, outerAngle, glm::vec3{ 0, 1, 0 }) * glm::translate(glm::mat4{ 1 }, glm::vec3{ diameter / 2, 0, 0 });

			
			for (size_t j = 0; j <= tessellation; j++) {
				float v = 1 - (float)j / tessellation;

				float innerAngle = j * glm::two_pi<float>() / tessellation + glm::pi<float>();
				float dx, dy;

				dx = glm::sin(innerAngle);
				dy = glm::cos(innerAngle);
				

				
				glm::vec3 normal(dx, dy, 0);
				glm::vec3 position = normal * thickness / 2.0f;
				glm::vec2 textureCoordinate(u, v);

				glm::vec4 pos4(position, 1.0f);
				position = glm::vec3{ transform * pos4 };
				glm::mat4 transformNoTranslation{ transform };
				transformNoTranslation[3] = glm::vec4{ 0, 0, 0, 1 };
				normal = glm::vec3{ transformNoTranslation * glm::vec4(normal, 1.0f) };

				vertices.push_back(Vertex{ position, colorWhite3, normal, textureCoordinate });

				
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

		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}

	void CreatePlane( float width, float height, ptl::vector<uint32_t>& indices, ptl::vector<Vertex>& vertices, bool rhcoords)
	{
		vertices =
		{
			{ glm::vec3(-0.5f * width, 0.0f,  0.5f * height), colorWhite3, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) }, 
			{ glm::vec3(0.5f * width, 0.0f,  0.5f * height),  colorWhite3, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }, 
			{ glm::vec3(0.5f * width, 0.0f, -0.5f * height),  colorWhite3, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, 
			{ glm::vec3(-0.5f * width, 0.0f, -0.5f * height), colorWhite3, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }  
		};

		indices =
		{
			0, 3, 1, 1, 3, 2
		};

		if (!rhcoords)
			ReverseWinding(indices, vertices);
	}
}

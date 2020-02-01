#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "resource_loading/GLTFLoader.h"
#include "core/Assertion.h"
#include "rendering/IRenderer.h"
#include "scene/SceneGraph.h"
#include <iostream>

template<typename T, typename TCharTraits, typename TAllocator>
static bool StringEndsWith(const std::basic_string<T, TCharTraits, TAllocator>& fullString, const std::basic_string<T, TCharTraits, TAllocator>& ending)
{
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}


GLTFLoader::GLTFLoader(gfx::IRenderer& render) :renderer_(render), gpuResourceManager_(&render.GetGfxResourceManager())
{

}

bool GLTFLoader::LoadModelFromGltf(const ptl::string& gltfFile, scene::SceneNode* rootSceneNode)
{
	this->gltfFile_ = gltfFile;
	this->rootSceneNode_ = rootSceneNode;

	if (!LoadModelFromGltfOrGlb(gltfFile, &model_))
		return false;

	if (model_.scenes.empty()) {
		std::cout << "The model: '" << gltfFile << "' does not contain any scenes!\n";
		return false;
	}

	ASSERT_MSG(model_.defaultScene >= 0 && static_cast<size_t>(model_.defaultScene) < model_.scenes.size(), "default scene not in range");

	CreateLights();

	int defaultScene = model_.defaultScene != -1 ? model_.defaultScene : 0;

	auto& scene = model_.scenes[defaultScene];

	for (int nodeIndex : scene.nodes) {
		AddNodeToScene(nodeIndex, rootSceneNode);
	}

    for(auto& node : model_.nodes)
    {
        if(node.camera != -1)
        {
            AddCameraToScene(node);
        }
    }

	this->gltfFile_ = "";
	this->model_ = tinygltf::Model{};
	this->rootSceneNode_ = nullptr;
	this->materialMap_.clear();
	this->renderableMeshMap_.clear();

	return true;
}

bool GLTFLoader::LoadModelFromGltfOrGlb(const ptl::string& filename, tinygltf::Model* model)
{
	tinygltf::TinyGLTF loader;
	ptl::string err;
	ptl::string warn;

	if (StringEndsWith(filename, ptl::string{ ".gltf" })) {
		bool ret = loader.LoadASCIIFromFile(model, &err, &warn, filename);
		if (!ret) {
			std::cout << "Failed to parse ASCII file: " << filename << "\n";
			if (!err.empty())
				std::cerr << "Error: " << err << '\n';
			else
				std::cerr << "No error message available...\n";

			if (!warn.empty())
				std::cout << "Warning: " << warn << '\n';
			else
				std::cout << "No warning message available...\n";

			BREAK_DEBUGGER();
			return false;
		}
	} else if (StringEndsWith(filename, ptl::string{ ".glb" })) {
		bool ret = loader.LoadBinaryFromFile(model, &err, &warn, filename);
		if (!ret) {
			std::cout << "Failed to parse binary: " << filename << "\n";
			if (!err.empty())
				std::cerr << "Error: " << err << '\n';
			else
				std::cerr << "No error message available...\n";

			if (!warn.empty())
				std::cout << "Warning: " << warn << '\n';
			else
				std::cout << "No warning message available...\n";

			BREAK_DEBUGGER();
			return false;
		}
	} else {
		ASSERT_MSG(false, "gltfFile does not end with .gltf or .glb");
		return false;
	}

	return true;
}

void GLTFLoader::AddNodeToScene(int nodeIndex, scene::SceneNode* parentSceneNode)
{
	auto& node = model_.nodes[nodeIndex];

	core::Transform transform = GetLocalTransformFromNode(node, parentSceneNode);
	scene::SceneNode* sceneNode = parentSceneNode->AddSceneNode(std::move(transform));
	sceneNode->name = node.name;

	int meshIndex = node.mesh;
	if (meshIndex != -1) {
		CreateMeshMaterialTexture(meshIndex, sceneNode);
	}

	for (auto childNodeIndex : node.children) {
		AddNodeToScene(childNodeIndex, sceneNode);
	}
}

void GLTFLoader::AddCameraToScene(tinygltf::Node& camera)
{
    core::Transform transform = GetLocalTransformFromNode(camera, rootSceneNode_);
    scene::SceneNode* sceneNode = rootSceneNode_->AddSceneNode(std::move(transform));
    sceneNode->name = camera.name;

    const int meshIndex = camera.mesh;
    if (meshIndex != -1) {
        CreateMeshMaterialTexture(meshIndex, sceneNode);
    }
}

void GLTFLoader::CreateMeshMaterialTexture(int meshIndex, scene::SceneNode* sceneNode)
{
	auto& mesh = model_.meshes[meshIndex];

	auto it = renderableMeshMap_.find(meshIndex);
	if (it == renderableMeshMap_.end()) {
		renderableMeshMap_.insert({ meshIndex, {} });

		for (auto primitiveIndex = 0; primitiveIndex < mesh.primitives.size(); ++primitiveIndex) {
			auto& primitive = mesh.primitives[primitiveIndex];
			auto materialIndex = primitive.material;

			ptl::vector<uint32_t> indices;
			ptl::vector<gfx::Vertex > vertices;
			
			{
				auto& indexAccessor = model_.accessors[primitive.indices];
				if (indexAccessor.bufferView != -1) {
					auto& indexBufferView = model_.bufferViews[indexAccessor.bufferView];
					auto& indexBuffer = model_.buffers[indexBufferView.buffer];

					auto indexByteStride = indexAccessor.ByteStride(indexBufferView);
					auto indexBufferOffset = indexAccessor.byteOffset + indexBufferView.byteOffset;

					auto* byte = &indexBuffer.data[0];
					for (size_t i = 0; i < indexAccessor.count; ++i) {
						auto* bytePos = byte + indexBufferOffset + indexByteStride * i;

						ASSERT(indexAccessor.type == TINYGLTF_TYPE_SCALAR);

						switch (indexAccessor.componentType) {
						case TINYGLTF_COMPONENT_TYPE_FLOAT: {
							ASSERT_MSG(false, "The index buffer has a type: 'float'! How does that work?");
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_DOUBLE: {
							ASSERT_MSG(false, "The index buffer has a type: 'double'! How does that work?");
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
							uint8_t* uint8s = reinterpret_cast<uint8_t*>(bytePos);
							indices.push_back(*uint8s);
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_BYTE: {
							ASSERT_MSG(false, "The index buffer has a type: 'byte'!"
								"I thought it could only be a unsigned type?");
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
							uint16_t* uint16s = reinterpret_cast<uint16_t*>(bytePos);
							indices.push_back(*uint16s);
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_SHORT: {
							ASSERT_MSG(false, "The index buffer has a type: 'short'!"
								"I thought it could only be a unsigned type?");
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
							uint32_t* uint32s = reinterpret_cast<uint32_t*>(bytePos);
							indices.push_back(*uint32s);
							break;
						}
						case TINYGLTF_COMPONENT_TYPE_INT: {
							ASSERT_MSG(false, "The index buffer has a type: 'int'!"
								"I thought it could only be a unsigned type?");
							break;
						}
						default:
							break;
						}
					}
				}
			}

			
			{
				auto& positionAccesor = model_.accessors[primitive.attributes["POSITION"]];
				auto& normalAccesor = model_.accessors[primitive.attributes["NORMAL"]];
				tinygltf::Accessor* uvAccesor = nullptr;
				if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
					uvAccesor = &model_.accessors[primitive.attributes["TEXCOORD_0"]];

				ASSERT(positionAccesor.count == normalAccesor.count && (uvAccesor == nullptr || positionAccesor.count == uvAccesor->count));
				size_t vertexCount = std::min(positionAccesor.count, normalAccesor.count);
				if (uvAccesor)
					vertexCount = std::min(vertexCount, uvAccesor->count);

				ASSERT(positionAccesor.type == TINYGLTF_TYPE_VEC3 && positionAccesor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
				ASSERT(normalAccesor.type == TINYGLTF_TYPE_VEC3 && normalAccesor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
				ASSERT(uvAccesor == nullptr || uvAccesor->type == TINYGLTF_TYPE_VEC2 && uvAccesor->componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

				
				vertices.resize(vertexCount);

				auto& positionBufferView = model_.bufferViews[positionAccesor.bufferView];
				auto& positionBuffer = model_.buffers[positionBufferView.buffer];

				const auto positionByteStride = positionAccesor.ByteStride(positionBufferView);
				const auto positionBufferOffset = positionAccesor.byteOffset + positionBufferView.byteOffset;

				auto& normalBufferView = model_.bufferViews[normalAccesor.bufferView];
				auto& normalBuffer = model_.buffers[normalBufferView.buffer];

				const auto normalByteStride = normalAccesor.ByteStride(normalBufferView);
				const auto normalBufferOffset = normalAccesor.byteOffset + normalBufferView.byteOffset;

				tinygltf::BufferView* uvBufferView = nullptr;
				tinygltf::Buffer* uvBuffer = nullptr;
				int uvByteStride = 0;
				size_t uvBufferOffset = 0;
				if (uvAccesor) {
					uvBufferView = &model_.bufferViews[uvAccesor->bufferView];
					uvBuffer = &model_.buffers[uvBufferView->buffer];

					uvByteStride = uvAccesor->ByteStride(*uvBufferView);
					uvBufferOffset = uvAccesor->byteOffset + uvBufferView->byteOffset;
				}

				for (int i = 0; i < vertexCount; ++i) {
					auto* positionPtr = positionBuffer.data.data() + (positionBufferOffset + positionByteStride * i);
					auto* normalPtr = normalBuffer.data.data() + (normalBufferOffset + normalByteStride * i);

					vertices[i].pos = *reinterpret_cast<glm::vec3*>(positionPtr);
					vertices[i].normal = *reinterpret_cast<glm::vec3*>(normalPtr);
					if (uvAccesor) {
						auto* uvPtr = uvBuffer->data.data() + (uvBufferOffset + uvByteStride * i);
						vertices[i].uv = *reinterpret_cast<glm::vec2*>(uvPtr);
					} else {
						vertices[i].uv = { 0.0f, 0.0f };
					}

					vertices[i].color = { 0.5f, 0.5f, 1.0f };
				}
			}

			gfx::Material* mat = nullptr;
			ptl::string materialName = GetNameForMaterial(materialIndex);
			MaterialId materialId{0}; 
			auto materialMapFindIt = materialMap_.find(materialIndex);
			if (materialMapFindIt == materialMap_.end()) {
				materialId = CreateMaterial(materialIndex);
				mat = &gpuResourceManager_->GetMaterial(materialId);
				materialMap_.emplace(materialIndex, materialId);
			} else {
				materialId = materialMapFindIt->second;
				mat = &gpuResourceManager_->GetMaterial(materialId);
			}

			ptl::string meshName = GetNameForMesh(meshIndex, primitiveIndex);
			MeshId meshId = gpuResourceManager_->CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), meshName);
			auto* meshResource = &gpuResourceManager_->GetMesh(meshId);
			gfx::RenderableMesh rMesh{ meshId , meshResource, materialId, mat };
			renderableMeshMap_.at(meshIndex).push_back(rMesh);
		}
	}

	for (auto& renderableMesh : renderableMeshMap_[meshIndex]) {
		ptl::string renderableName = gltfFile_ + " [Renderable] id: " + ptl::to_string(meshIndex) + "_" + ptl::to_string(renderableMesh.materialId._id);

		Renderable renderable{renderableMesh.meshId, renderableMesh.materialId};
		sceneNode->renderables.push_back(renderable);
	}
}

void GLTFLoader::CreateLights()
{
	auto& nodes = model_.nodes;
	tinygltf::Node* node_{ nullptr };
	auto findLightMesh = [&nodes](ptl::string& name_, tinygltf::Node*& node)
	{
		
		const auto itr = std::find_if(nodes.begin(), nodes.end(), [&name_](auto& node)
		{
			return (!node.extensions.empty() && node.name == name_);
		});
		if (itr != nodes.end())
			node = &*itr;
	};
	
	for (auto& light : model_.lights) {
		findLightMesh(light.name, node_);
		if (node_ == nullptr) continue;

		glm::dmat4& nodeMatrix = *reinterpret_cast<glm::dmat4*>(node_->matrix.data());

		core::Transform transform{ nodeMatrix,nullptr };
	}
}

static tinygltf::ColorValue gltfColorGetOrDefault(tinygltf::Material& material, const ptl::string& name, const tinygltf::ColorValue& defaultVal)
{
	return material.values.find(name) != material.values.end() ?
		material.values.at(name).ColorFactor() :
		defaultVal;
}

static tinygltf::ColorValue gltfAdditionalColorGetOrDefault(tinygltf::Material& material, const ptl::string& name, const tinygltf::ColorValue& defaultVal)
{
	return material.additionalValues.find(name) != material.additionalValues.end() ?
		material.additionalValues.at(name).ColorFactor() :
		defaultVal;
}

static int gltfTextureGetOrDefault(tinygltf::Material& material, const ptl::string& name, int defaultVal)
{
	return material.values.find(name) != material.values.end() ?
		material.values.at(name).TextureIndex() :
		defaultVal;
}

static int gltfAdditionalTextureGetOrDefault(tinygltf::Material& material, const ptl::string& name, int defaultVal)
{
	return material.additionalValues.find(name) != material.additionalValues.end() ?
		material.additionalValues.at(name).TextureIndex() :
		defaultVal;
}

static float gltfAdditionalGetFloatOrDefault(tinygltf::Material& material, const ptl::string& keyName, float defaultVal)
{
	return material.additionalValues.find(keyName) != material.additionalValues.end() ?
		static_cast<float>(material.additionalValues.at(keyName).number_value) :
		defaultVal;
}

static BlendMode GetBlendMode(tinygltf::Material& material)
{
	if (material.additionalValues.find("alphaMode") != material.additionalValues.end()) {
		ptl::string stringBlendMode = material.additionalValues.at("alphaMode").string_value;
		if(stringBlendMode == "OPAQUE") {
			return BlendMode::Opaque;
		} if(stringBlendMode == "MASK") {
			return BlendMode::Masked;
		} if(stringBlendMode == "BLEND") {
			return BlendMode::Blending;
		}
	}

	
	return BlendMode::Opaque;
}

MaterialId GLTFLoader::CreateMaterial(int materialIndex)
{
    if(materialIndex == -1)
    {
        static bool initialized = false;
        static MaterialId materialId;
        if (!initialized) {
            materialId = gpuResourceManager_->CreateMaterial(BlendMode::Opaque, glm::vec4{ 1, 1, 1, 1 }, gpuResourceManager_->GetDefaultTextureId(),
                glm::vec3{ 0, 0, 0 }, gpuResourceManager_->GetDefaultTextureId());
        }
        return materialId;
    }

	tinygltf::Material& tinygltfMaterial = model_.materials[materialIndex];

	tinygltf::ColorValue emissiveFactorColor = gltfAdditionalColorGetOrDefault(tinygltfMaterial, "emissiveFactor", tinygltf::ColorValue{1.0, 1.0, 1, 1} );
	tinygltf::ColorValue diffuseFactorColor = gltfColorGetOrDefault(tinygltfMaterial, "baseColorFactor", tinygltf::ColorValue{1, 1, 1, 1});

	glm::vec3 emissiveFactor{ static_cast<float>(emissiveFactorColor[0]), static_cast<float>(emissiveFactorColor[1]), static_cast<float>(emissiveFactorColor[2]) };
	glm::vec4 diffuseFactor{ static_cast<float>(diffuseFactorColor[0]), static_cast<float>(diffuseFactorColor[1]), static_cast<float>(diffuseFactorColor[2]), static_cast<float>(diffuseFactorColor[3]) };

	int diffuseTextureIndex = gltfTextureGetOrDefault(tinygltfMaterial, "baseColorTexture", -1);
	int emissiveTextureIndex = gltfAdditionalTextureGetOrDefault(tinygltfMaterial, "emissiveTexture", -1);

	BlendMode blendMode = GetBlendMode(tinygltfMaterial);
	float alphaCutoff = gltfAdditionalGetFloatOrDefault(tinygltfMaterial, "alphaCutoff", 0.5f);
	if (blendMode == BlendMode::Opaque) {
		alphaCutoff = 0.0f;
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	TextureId diffuseTextureId;
	if (diffuseTextureIndex != -1) { 
		ptl::string textureName = GetNameForTexture(diffuseTextureIndex);
		if (!gpuResourceManager_->ContainsTexture(textureName)) {
			auto& baseTexture = model_.textures[diffuseTextureIndex];
			auto& baseImage = model_.images[baseTexture.source];
			auto* ptr = baseImage.image.data();

			diffuseTextureId = gpuResourceManager_->CreateTextureFromMemory(ptr, baseImage.width, baseImage.height, baseImage.bits, baseImage.component, glm::vec2(0.0f,0.0f), textureName);
		} else {
			diffuseTextureId = gpuResourceManager_->GetTextureId(textureName);
		}
	} else {
		diffuseTextureId = gpuResourceManager_->GetDefaultTextureId();
	}

	TextureId emissiveTextureId;
	if (emissiveTextureIndex != -1) { 
		ptl::string textureName = GetNameForTexture(emissiveTextureIndex);
		if (!gpuResourceManager_->ContainsTexture(textureName)) {
			auto& baseTexture = model_.textures[emissiveTextureIndex];
			auto& baseImage = model_.images[baseTexture.source];
			auto* ptr = baseImage.image.data();

			emissiveTextureId = gpuResourceManager_->CreateTextureFromMemory(ptr, baseImage.width, baseImage.height, baseImage.bits, baseImage.component, glm::vec2(0.0f, 0.0f), textureName);
		} else {
			emissiveTextureId = gpuResourceManager_->GetTextureId(textureName);
		}
	} else {
		emissiveTextureId = gpuResourceManager_->GetDefaultTextureId();
	}

	MaterialId materialId;
	if (materialMap_.find(materialIndex) != materialMap_.end()) {
		materialId = materialMap_.at(materialIndex);
		ASSERT_MSG(gpuResourceManager_->ContainsMaterial(materialId), "GLTF loader has thinks it created a material but the resource manager doesn't contain it!");
	} else { 
		ptl::string materialName = GetNameForMaterial(materialIndex);
		materialId = gpuResourceManager_->CreateMaterial(BlendMode::Opaque, diffuseFactor, diffuseTextureId, emissiveFactor, emissiveTextureId, alphaCutoff, materialName);
	}

	return materialId;
}

ptl::string GLTFLoader::GetNameForMaterial(int materialIndex)
{
    if(materialIndex == -1)
    {
        return "Default";
    }
	auto& material = model_.materials[materialIndex];

	ptl::string materialType = material.values["baseColorTexture"].TextureIndex() != -1 ?
		"[Textured Material]" :
		"[Colored Material]";
	ptl::string name = gltfFile_ + " " + materialType + " id:" + ptl::to_string(materialIndex);

	if (!material.name.empty())
		name += " named: '" + material.name + "'";

	return name;
}

ptl::string GLTFLoader::GetNameForTexture(int textureIndex)
{
	auto& texture = model_.textures[textureIndex];
	auto& image = model_.images[texture.source];
	if (!image.uri.empty()) {
		return gltfFile_ + " [Texture] id: " + ptl::to_string(textureIndex) + " name: '" + image.uri + "'";
	} else {
		return gltfFile_ + " [Texture] id: " + ptl::to_string(textureIndex) + " unnamed";
	}
}

ptl::string GLTFLoader::GetNameForMesh(int meshIndex, int primitiveIndex)
{
	auto& mesh = model_.meshes[meshIndex];

	ptl::string name = gltfFile_ + " [Mesh] id:" + ptl::to_string(meshIndex) + "_" + ptl::to_string(primitiveIndex);
	if (!mesh.name.empty())
		name += " named: '" + mesh.name + "'";
	else
		name += " unnamed";

	return name;
}


core::Transform GLTFLoader::GetLocalTransformFromNode(tinygltf::Node& node, scene::SceneNode* parentNode)
{
	core::Transform transform;
	if (node.matrix.size() == 16) {
		glm::dmat4& nodeMatrix = *reinterpret_cast<glm::dmat4*>(node.matrix.data());
		transform = core::Transform{ nodeMatrix, &parentNode->transform };
	} else {
		glm::dvec3 pos = node.translation.size() == 3 ?
			*reinterpret_cast<glm::dvec3*>(node.translation.data()) :
			glm::dvec3{};
		glm::dquat rot = node.rotation.size() == 4 ?
			*reinterpret_cast<glm::dquat*>(node.rotation.data()) :
			glm::identity<glm::dquat>();
		glm::dvec3 scale = node.scale.size() == 3 ?
			*reinterpret_cast<glm::dvec3*>(node.scale.data()) :
			glm::dvec3{ 1, 1, 1 };

		transform = core::Transform{ static_cast<glm::vec3>(pos), static_cast<glm::quat>(rot), static_cast<glm::vec3>(scale), &parentNode->transform };
	}

	return transform;
}

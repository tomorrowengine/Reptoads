#include "SceneHandler.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include "core/Config.h"
#include "GameDatabase.h"
#include "core/StrHash.h"
#include "rendering/IRenderer.h"
#include "gameplay/Transform.h"
#include "scene/SceneGraph.h"
#include "resource_loading/GLTFLoader.h"
#include "rendering/GeometricShapes.h"

#include <sstream>
#include "resource_loading/ParticleParser.h"

namespace
{
    
    ptl::string MakeRenderableID(unsigned int sceneIndex, const ptl::string& renderableName, const ptl::string& nodeName)
    {
        return ptl::to_string(sceneIndex) + "_" + ptl::to_string(tbsg::HashString(renderableName)) + "_" + ptl::to_string(tbsg::HashString(nodeName));
    }
}



void tbsg::SceneHandler::Initialize()
{
    rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> > mapDoc;
    char readBuffer[512];
#ifdef PLATFORM_WINDOWS
    FILE* fp;
    errno_t err = fopen_s(&fp, tbsg::Config::Get().mapsFile.c_str(), "r");
    ASSERT_MSG(!err, "Could not load config");
#else
    FILE* fp = fopen(tbsg::Config::Get().mapsFile.c_str(), "r"); 
    if (!fp)
        ASSERT_MSG(false, "Could not load config");
#endif
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    mapDoc.ParseStream(is);
    if(mapDoc.IsArray())
    {
        auto mapArray = mapDoc.GetArray();
        m_database.InitMaps(mapArray.Size());
        for(auto& maps : mapArray)
        {
            ptl::vector<ptl::string> mapsVector;
            unsigned int mapID = tbsg::HashString(maps["name"].GetString());
            ASSERT_MSG(mapID != 0, "Map ID cannot be 0!");
            auto mapNames = maps["maps"].GetArray();
            mapsVector.reserve(mapNames.Size());
            for (auto& map : mapNames)
            {
                auto chapterGltf = ptl::to_string(map.GetString());
                mapsVector.push_back(tbsg::Config::Get().MakeModelPath(chapterGltf.c_str()));
            }
            m_database.AddMaps(mapID, std::move(mapsVector));
        }
    }
    
    
    m_cachedCardUnitsOfPlayers.push_back({});
    m_cachedCardUnitsOfPlayers.push_back({});
}

void tbsg::SceneHandler::DisableScene(unsigned sceneIndex)
{
    auto& rm = m_view->GetGfxResourceManager();
    for (auto& sceneNode : *m_scenes[sceneIndex]) {
        for (auto& renderable : sceneNode.renderables) {
            
            
            
			m_view->RemoveRenderable(renderable.renderableRegisteredId);
            
        }
    }

    playerWaypoints.waypoints.clear();
    opponentWaypoints.waypoints.clear();
    cameraWaypoints.waypoints.clear();
}

void tbsg::SceneHandler::EnableScene(unsigned sceneIndex)
{
    auto& rm = m_view->GetGfxResourceManager();
    
    for (auto& sceneNode : *m_scenes[sceneIndex]) 
    {
        for (auto& renderable : sceneNode.renderables) 
        {
            
            renderable.renderableRegisteredId = m_view->AddRenderable(&sceneNode.transform, renderable.meshId, renderable.materialId);
        }

        std::istringstream ss(sceneNode.name.c_str());
        std::string token;
        std::getline(ss, token, '_');
        if (token == "WP")
        {
            std::getline(ss, token, '_');
            if (token == "C")
            {
                const waypoint::WaypointIndex waypointIndex = waypoint::GetWaypointIndexFromString(ss, token);
                if (waypointIndex.x >= cameraWaypoints.waypoints.size())
                {
                    cameraWaypoints.waypoints.resize(waypointIndex.x + 1);
                    cameraWaypoints.waypoints[waypointIndex.x] = ptl::vector<core::Transform>{ sceneNode.transform };
                }
                else
                {
                    cameraWaypoints.waypoints[waypointIndex.x].push_back(std::move(sceneNode.transform));
                }
            }
            else if (token == "P1")
            {
                const waypoint::WaypointIndex waypointIndex = waypoint::GetWaypointIndexFromString(ss, token);
                if (waypointIndex.x >= playerWaypoints.waypoints.size())
                {
                    playerWaypoints.waypoints.resize(waypointIndex.x + 1);
                    playerWaypoints.waypoints[waypointIndex.x] = ptl::vector<core::Transform>{ sceneNode.transform };
                }
                else
                {
                    playerWaypoints.waypoints[waypointIndex.x].push_back(std::move(sceneNode.transform));
                }
            }
            else if (token == "P2")
            {
                const waypoint::WaypointIndex waypointIndex = waypoint::GetWaypointIndexFromString(ss, token);
                if (waypointIndex.x >= opponentWaypoints.waypoints.size())
                {
                    opponentWaypoints.waypoints.resize(waypointIndex.x + 1);
                    opponentWaypoints.waypoints[waypointIndex.x] = ptl::vector<core::Transform>{ sceneNode.transform };
                }
                else
                {
                    opponentWaypoints.waypoints[waypointIndex.x].push_back(std::move(sceneNode.transform));
                }
            }
        }
        else if (token == "SP")
        {
            std::getline(ss, token, '_');
            if (token == "card1")
            {
                std::getline(ss, token, '_');
                const unsigned int index = stoi(token);

                if (index >= cardSpawnPoints[0].size())
                {
                    cardSpawnPoints[0].resize(index + 1);
                    cardSpawnPoints[0][index] = &sceneNode;
                }
                else
                {
                    cardSpawnPoints[0][index] = &sceneNode;
                }

            }
            else if (token == "card2")
            {
                std::getline(ss, token, '_');
                const unsigned int index = stoi(token);

                if (index >= cardSpawnPoints[1].size())
                {
                    cardSpawnPoints[1].resize(index + 1);
                    cardSpawnPoints[1][index] = &sceneNode;
                }
                else
                {
                    cardSpawnPoints[1][index] = &sceneNode;
                }
            }
            else if (token == "M")
            {
                std::getline(ss, token, '_');
                const unsigned int index = stoi(token);

                if (index >= monsterSpawnPoints.size())
                {
                    monsterSpawnPoints.resize(index + 1);
                    monsterSpawnPoints[index] = &sceneNode;
                }
                else
                {
                    monsterSpawnPoints[index] = &sceneNode;
                }
            }
        }
    }
}

void tbsg::SceneHandler::LoadScene(ptl::string scene)
{
    GLTFLoader gltfLoader{ *m_view };
    auto& sceneNode = m_scenes[m_currentScene];
    gltfLoader.LoadModelFromGltf(scene,sceneNode.get());
    ++m_currentScene;
}

void tbsg::SceneHandler::UnloadScene(unsigned int sceneID)
{
    DisableScene(sceneID);
    int counter = 0;
    for (auto& sceneNode : *m_scenes[sceneID]) {
        for (auto& renderable : sceneNode.renderables) {
			
            
        }
    }
}

void tbsg::SceneHandler::LoadMap(unsigned int mapId)
{
    auto map = m_database.GetMap(mapId);
    printf("[SceneModel] loading map %i\n", mapId);

    for(auto& mapName : map)
    {
        printf("[SceneModel] start loading scene %s\n", mapName.c_str());

        m_scenes.push_back(ptl::make_shared<scene::SceneNode>(scene::SceneNode{}));
        LoadScene(mapName);

        printf("[SceneModel] done loading scene %s\n", mapName.c_str());
    }
    m_currentScene = 0;
    
    printf("[SceneModel] done loading map %i\n", mapId);
}

void tbsg::SceneHandler::UnloadMap()
{
	std::cout << "UnloadMap\n";
    int idx = 0;
    for (auto& scenes : m_scenes)
    {
      UnloadScene(idx);
      ++idx;
    }
    m_currentScene = 0;
	std::cout << "Done UnloadMap\n";
    
}

void tbsg::SceneHandler::LoadParticles()
{
    UnloadParticles();
    ParticleParser particleParser{ m_view->GetGfxResourceManager() };

    
    auto particleListPath = tbsg::Config::Get().MakeParticlePath("particlelist.json");
    rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> > particles;
    FILE* fp;
    char readBuffer[512];
#ifdef PLATFORM_WINDOWS
    errno_t err = fopen_s(&fp, particleListPath.c_str(), "r");
    ASSERT_MSG(!err, "Could not load config");
#else
    fp = fopen(particleListPath.c_str(), "r"); 
    if (!fp)
        ASSERT_MSG(false, "Could not load config");
#endif
    rapidjson::FileReadStream particleReader(fp, readBuffer, sizeof(readBuffer));
    particles.ParseStream(particleReader);
    if (particles.IsArray())
    {
        particleEmitters.reserve(particles.GetArray().Size());
        auto particleNames = particles.GetArray();
        for (auto& particleName : particleNames)
        {
            auto particleNameStr = ptl::to_string(particleName.GetString());
#ifdef PLATFORM_WINDOWS
            bool succesfullyLoaded = false;
            ParticleEmitter emitter{};
            std::tie(emitter, succesfullyLoaded) = particleParser.DeserializeParticleFromFile(tbsg::Config::Get().MakeParticlePath(particleNameStr));
            particleEmitters.emplace(particleNameStr,emitter);
#endif
        }
    }
}

void tbsg::SceneHandler::UnloadParticles()
{
    const auto size = particleEmitters.size();
    particleEmitters.clear();
    particleEmitters.reserve(size);
}

ParticleEmitter tbsg::SceneHandler::GetParticle(const ptl::string& name) const
{
    return particleEmitters.at(name);
}

void tbsg::SceneHandler::FreeUnitCache()
{
	m_cachedCardUnitsOfPlayers.clear();
	m_cachedCardUnitsOfPlayers.push_back({});
	m_cachedCardUnitsOfPlayers.push_back({});
}

scene::SceneNode* tbsg::SceneHandler::AddModel(ptl::string name, core::Transform& transform, scene::SceneNode* parent)
{
    parent = (parent == nullptr) ? m_scenes[m_currentScene].get() : parent;
    
    
	
    auto it = loadedModels.find(name);
    if(it != loadedModels.end() )
    {
        scene::SceneNode* copyModel = new scene::SceneNode{ *it->second };
        copyModel->name = name;
        copyModel->transform = transform;
        copyModel->SetParent(parent);

        for (auto& subNode : *copyModel) {
            for (auto& renderable : subNode.renderables) {
                renderable.renderableRegisteredId = m_view->AddRenderable(&copyModel->transform, renderable.meshId, renderable.materialId);
            }
        }
        
		
        return copyModel;
    }

    
    GLTFLoader gltfLoader{ *m_view };
    scene::SceneNode* node = new scene::SceneNode;
    const bool succesfullyLoaded = gltfLoader.LoadModelFromGltf(tbsg::Config::Get().MakeModelPath(name + ptl::string{".glb"}), node);
    ASSERT(succesfullyLoaded);
    node->name = name;
    loadedModels.insert(std::make_pair(name, ptl::shared_ptr<scene::SceneNode>{ node }));

    scene::SceneNode* copyModel = new scene::SceneNode{ *node };

    copyModel->name = name;
    copyModel->transform = transform;
    copyModel->SetParent(parent);

    for (auto& subNode : *copyModel) {
	    for (auto& renderable : subNode.renderables) {
		    renderable.renderableRegisteredId = m_view->AddRenderable(&copyModel->transform, renderable.meshId, renderable.materialId);
	    }
    }

    return copyModel;
}

scene::SceneNode* tbsg::SceneHandler::AddModel(ptl::unique_ptr<scene::SceneNode>&& node, core::Transform& transform, scene::SceneNode* parent)
{
    
    parent = (parent == nullptr) ? m_scenes[m_currentScene].get() : parent;
    node->transform = transform;
    auto ptr = node.get();
    parent->AddSceneNode(std::move(node));
    
    return ptr;
}

void tbsg::SceneHandler::DisableModel(scene::SceneNode* model)
{
	std::cout  << "Model to be removed: " << model->name << '\n';
    for (auto& childs : model->GetChildren())
        DisableModel(childs.get());

    for (auto& renderable : model->renderables){
		std::cout << "Remove: " << renderable.renderableRegisteredId._id << '\n';
        if (renderable.renderableRegisteredId.IsNotNull()) {
            m_view->RemoveRenderable(renderable.renderableRegisteredId);
			std::cout << "Removed renderable \n";
            renderable.renderableRegisteredId = RenderableId{ 0 };
        }else
        {
			std::cout << "Remove was not possible \n";
        }
	}
        
}

void tbsg::SceneHandler::EnableModel(scene::SceneNode* model)
{
	if (IsModelEnabled(model)) return;
    auto& rm = m_view->GetGfxResourceManager();
    for (auto& childs : model->GetChildren())
        EnableModel(childs.get());
    for (auto& renderable : model->renderables){
		ASSERT_MSG(!renderable.renderableRegisteredId.IsNotNull(), 
			"The renderable id should be null (0)."
			" Reason: By overriding the last id, that id is no longer accesisable, which means it cant be removed from the renderer anymore!");
        renderable.renderableRegisteredId = m_view->AddRenderable(&model->transform, renderable.meshId, renderable.materialId);
    }
}

bool tbsg::SceneHandler::IsModelEnabled(const ptl::string& name) const
{
    auto* nodeInScene = m_scenes[m_currentScene]->FindNodeByName(name);
    if (nodeInScene == nullptr) return false;
    return IsModelEnabled(nodeInScene);
}

bool tbsg::SceneHandler::IsModelEnabled(scene::SceneNode* model) const
{
    if (model == nullptr) return false;
	for (auto& renderable : model->renderables){
		if (renderable.renderableRegisteredId._id == 0) return false;
        if(!m_view->ContainsRenderable(renderable.renderableRegisteredId)) return false;
	}

    return true;
}

ptl::shared_ptr<scene::SceneNode> tbsg::SceneHandler::ChangeScene()
{
    DisableScene(m_currentScene);
    ++m_currentScene;
    if (m_currentScene >= m_scenes.size()) m_currentScene = 0;
    EnableScene(m_currentScene);
    return m_scenes[m_currentScene];
}

void tbsg::SceneHandler::DeleteRenderables(scene::SceneNode* sceneNode)
{
    for (auto& renderable : sceneNode->renderables)
    {
        m_view->RemoveRenderable(renderable.renderableRegisteredId);
    }

    for (auto& child : sceneNode->GetChildren())
    {
        for (auto& renderable : child->renderables)
        {
            m_view->RemoveRenderable(renderable.renderableRegisteredId);
        }
    }
    sceneNode->Clear();
}

ptl::shared_ptr <scene::SceneNode> tbsg::SceneHandler::GetCurrentScene()
{
    return m_scenes[m_currentScene];
}

scene::SceneNode* tbsg::SceneHandler::AddWorldspaceText(const gfx::UITextOptionsForTexture& options,
    float worldspacePanelWidth, float worldspacePanelHeight, core::Transform localTransform, scene::SceneNode* parent,
    bool reverseWindingOrder, bool flipXAxisUVs)
{

    parent = parent != nullptr ? parent : GetCurrentScene().get();
	auto& uiRenderer = *m_view->GetUIRenderer();
	auto& resourceManager = m_view->GetGfxResourceManager();

	static uint32_t counter = 0;
	uint32_t currentCount = ++counter;

	auto* node = parent->AddSceneNode(std::move(localTransform));

	TextureId textureId = uiRenderer.CreateTextTexture(options);
	ptl::vector<uint32_t> indices;
	ptl::vector<gfx::Vertex> vertices;
	gfx::CreatePlane(worldspacePanelWidth, worldspacePanelHeight, indices, vertices, reverseWindingOrder);
	if(flipXAxisUVs) {
		for (auto& vertex : vertices) {
			vertex.uv.x = 1.0f - vertex.uv.x;
		}
	}
	ptl::string meshName = ptl::string("Worldspace Text Plane Mesh #") + ptl::to_string(currentCount);
	MeshId planeMeshId = resourceManager.CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), 
		meshName);

	auto& planeMesh = resourceManager.GetMesh(planeMeshId);

	ptl::string materialName = ptl::string("Worldspace text material #") + ptl::to_string(currentCount);
	MaterialId materialId = resourceManager.CreateMaterial(BlendMode::Masked, { 1, 1, 1, 1 }, textureId, 
		{0, 0, 0}, resourceManager.GetDefaultTextureId());

	auto& material = resourceManager.GetMaterial(materialId);

	ptl::string renderableName = ptl::string{ "Worldspace text renderable #" } + ptl::to_string(currentCount);
	Renderable renderable{ planeMeshId, materialId };

	renderable.renderableRegisteredId = m_view->AddRenderable(&node->transform, planeMeshId, materialId);

	node->renderables.push_back(renderable);

	return node;
}

scene::SceneNode* tbsg::SceneHandler::AddWorldspacePlane(float worldspacePanelWidth, float worldspacePanelHeight, const ptl::string& textureName,
    core::Transform localTransform, scene::SceneNode* parent, bool reverseWindingOrder, bool flipXAxisUVs)
{
    parent = parent != nullptr ? parent : GetCurrentScene().get();

    auto* node = parent->AddSceneNode(localTransform);

    static uint32_t counter = 0;
    uint32_t currentCount = ++counter;

    ptl::vector<uint32_t> indices;
    ptl::vector<gfx::Vertex> vertices;
    gfx::CreatePlane(worldspacePanelWidth, worldspacePanelHeight, indices, vertices, reverseWindingOrder);
    if (flipXAxisUVs) {
        for (auto& vertex : vertices) {
            vertex.uv.x = 1.0f - vertex.uv.x;
        }
    }

    auto& rm = m_view->GetGfxResourceManager();

    ptl::string meshName = "Worldspace plane #" + ptl::to_string(currentCount);
    MeshId planeMeshId = rm.CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), meshName);
    auto& mesh = rm.GetMesh(planeMeshId);

    gfx::Texture* texture;
    if (rm.ContainsTexture(textureName)) {
        texture = &rm.GetTexture(textureName);
    }
    else {
        rm.CreateTexture(textureName);
        texture = &rm.GetTexture(textureName);
    }
	TextureId textureId = rm.GetTextureId(textureName);

    gfx::Material* material;
    ptl::string materialName = "Worldspace plane w/texture: '" + textureName + "' id: " + ptl::to_string(currentCount);
	
    
	
        MaterialId materialId = rm.CreateMaterial(BlendMode::Blending, {1, 1, 1, 1}, textureId, 
			{0, 0, 0}, rm.GetDefaultTextureId(), 0.5f, materialName);
        material = &rm.GetMaterial(materialId);
    

    ptl::string renderableName = "Worldspace plane renderable: #" + ptl::to_string(currentCount);

	Renderable renderable{ planeMeshId, materialId };
	renderable.renderableRegisteredId = m_view->AddRenderable(&node->transform, planeMeshId, materialId);

    
    

    node->renderables.push_back(renderable);

    return node;
}

scene::SceneNode* tbsg::SceneHandler::AddWorldspacePlane(float worldspacePanelWidth, float worldspacePanelHeight,
	const TextureId textureId, core::Transform localTransform, scene::SceneNode* parent, bool reverseWindingOrder,
	bool flipXAxisUVs)
{
	parent = parent != nullptr ? parent : GetCurrentScene().get();

	auto* node = parent->AddSceneNode(localTransform);

	static uint32_t counter = 0;
	uint32_t currentCount = ++counter;

	ptl::vector<uint32_t> indices;
	ptl::vector<gfx::Vertex> vertices;
	gfx::CreatePlane(worldspacePanelWidth, worldspacePanelHeight, indices, vertices, reverseWindingOrder);
	if (flipXAxisUVs) {
		for (auto& vertex : vertices) {
			vertex.uv.x = 1.0f - vertex.uv.x;
		}
	}

	auto& rm = m_view->GetGfxResourceManager();

	ptl::string meshName = "Worldspace plane #" + ptl::to_string(currentCount);
	MeshId planeMeshId = rm.CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), meshName);
	auto& mesh = rm.GetMesh(planeMeshId);

	gfx::Material* material;
	ptl::string materialName = "Worldspace plane w/texture:  id: " + ptl::to_string(currentCount);
	
	
	
	MaterialId materialId = rm.CreateMaterial(BlendMode::Blending, { 1, 1, 1, 1 }, textureId,
		{ 0, 0, 0 }, rm.GetDefaultTextureId(), 0.5f, materialName);
	material = &rm.GetMaterial(materialId);
	

	ptl::string renderableName = "Worldspace plane renderable: #" + ptl::to_string(currentCount);

	Renderable renderable{ planeMeshId, materialId };
	renderable.renderableRegisteredId = m_view->AddRenderable(&node->transform, planeMeshId, materialId);

	
	

	node->renderables.push_back(renderable);

	return node;
}

void tbsg::SceneHandler::UpdateWorldspaceText(const gfx::UITextOptionsForTexture& options, scene::SceneNode* node)
{
	ASSERT(node->renderables.size() == 1, 
		"We expect the node parameter to have 1 child, which is created with AddWorldspaceText");

	Renderable& renderable = node->renderables[0];

	m_view->RemoveRenderable(renderable.renderableRegisteredId);

	auto& rm = m_view->GetGfxResourceManager();
	auto* uiRenderer = m_view->GetUIRenderer();

	auto& material = rm.GetMaterial(renderable.materialId);
	
	auto textureId = material.diffuseTextureId;
	uiRenderer->RemoveTextTexture(textureId);
	rm.DeleteTexture(textureId);

	rm.DeleteMaterial(renderable.materialId);

	static uint32_t counter = 0;
	ptl::string newMatName = "UpdatedMaterial #" + ptl::to_string(++counter);
	auto newTextTextureId = uiRenderer->CreateTextTexture(options);
	auto newMaterialId = rm.CreateMaterial(BlendMode::Masked, {1, 1, 1, 1}, newTextTextureId, {0, 0, 0}, rm.GetDefaultTextureId(), 0.5f, newMatName);

	renderable.materialId = newMaterialId;
	renderable.renderableRegisteredId = m_view->AddRenderable(&node->transform, renderable.meshId, renderable.materialId);
}

scene::SceneNode* tbsg::SceneHandler::AddCubeToScene(const ptl::string& nameOfModel, TextureId textureId, size_t sizeOfCube,
                                                     core::Transform transform, scene::SceneNode* parent)
{
    parent = (parent != nullptr) ? parent : m_scenes[m_currentScene].get();

    auto& resourceManager = m_view->GetGfxResourceManager();

    ptl::vector<uint32_t> indices;
    ptl::vector<gfx::Vertex> vertices;
    gfx::CreateCube(sizeOfCube, indices, vertices);

    MeshId cubeMeshId =  resourceManager.CreateMesh(vertices.data(), vertices.size(), indices.data(), indices.size(), nameOfModel);
	MaterialId cubeMaterialId = resourceManager.CreateMaterial(BlendMode::Blending, { 1, 1, 1, 1 }, textureId,
		{0, 0, 0}, resourceManager.GetDefaultTextureId(), 0.5f, nameOfModel);

	auto* node = parent->AddSceneNode(transform);

	Renderable cubeRenderable{ cubeMeshId, cubeMaterialId };
	

    node->renderables.push_back(cubeRenderable);
    node->name = nameOfModel;
    EnableModel(node);
    return node;
}

scene::SceneNode* tbsg::SceneHandler::GetHeroModel(unsigned heroIndex)
{
    return nullptr;
}

scene::SceneNode* tbsg::SceneHandler::GetCardModelFromCache(unsigned cardID, unsigned slotIndex, unsigned playerIndex)
{
    
    auto& cardUnitCacheOfPlayer = m_cachedCardUnitsOfPlayers[playerIndex];
    const auto result = std::find_if(cardUnitCacheOfPlayer.begin(), cardUnitCacheOfPlayer.end(), [cardID](const auto& card) {return card.first == cardID; });
	if(result != cardUnitCacheOfPlayer.end())
		return result->second.get();
    
    return nullptr;
}

scene::SceneNode* tbsg::SceneHandler::CreateCardModelForPlayer(unsigned cardID, unsigned playerIndex)
{
	

    GLTFLoader gltfLoader{ *m_view };
    const ptl::string sceneNodeName = "card" + ptl::to_string(playerIndex);
	std::cout << "SceneNodeName: " << sceneNodeName << '\n';
	auto node = AddWorldspacePlane(6.4f, 10.1f, GetCardTexture(cardID));
    node->transform = core::MakeDefaultTransform();
    node->name = sceneNodeName;
    return node;
}

gfx::TextureId tbsg::SceneHandler::GetCardTexture(unsigned int cardID)
{
    gfx::TextureId textureID{};
    if (cardTextures.find(cardID) != cardTextures.end())
    {
        textureID = cardTextures.find(cardID)->second;
    }

    return textureID;
}

void tbsg::SceneHandler::CreateCardTexture(unsigned int cardID)
{
    auto& resourceManager = m_view->GetGfxResourceManager();
	if (cardTextures.find(cardID) == cardTextures.end())
	{
	    auto card = m_database.GetCard(cardID);
		auto base = "Card_" + card->meta.name + ".png";
	    auto path =  "/"+Config::Get().MakeTexturePath("Card_" + card->meta.name + ".png");
		path.erase(std::remove_if(path.begin(), path.end(), isspace), path.end());
		base.erase(std::remove_if(base.begin(), base.end(), isspace), base.end());
	    if (!resourceManager.ContainsTexture(path))
	    {
	        gfx::TextureId textureID = resourceManager.CreateTexture(base);
	        cardTextures.insert(std::make_pair(cardID, textureID));
	    }
	}
}

scene::SceneNode* tbsg::SceneHandler::ChangeMaterialOf(scene::SceneNode* model, const ptl::string& materialName)
{
    ASSERT_MSG(false, "Not implemented yet")
    return nullptr;
}

scene::SceneNode* tbsg::SceneHandler::ChangeMaterialOf(const ptl::string& modelName, const ptl::string& materialName)
{
    ASSERT_MSG(false, "Not implemented yet")
    return nullptr;
}


void tbsg::SceneHandler::SetRenderer(gfx::IRenderer* const view)
{
    m_view = view;
}

void tbsg::SceneHandler::SetParticleRenderer(gfx::IParticleRenderer* const particleRenderer)
{
	this->particleRenderer = particleRenderer;
}

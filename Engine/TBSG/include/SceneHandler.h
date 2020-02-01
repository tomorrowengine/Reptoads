#pragma once
#define WIN32_LEAN_AND_MEAN
#include "memory/String.h"
#include "memory/Containers.h"
#include "memory/smart_ptr.h"
#include <gameplay/Transform.h>
#include "scene/SceneGraph.h"
#include "gameplay/WaypointSystem.h"
#include "rendering/IRenderer.h"
#include "rendering/IParticleRenderer.h"
#include "core/SparseSet.h"

class GameDatabase;
namespace gfx
{
	struct TextureId;
	struct UITextOptionsForTexture;
	class IUIRenderer;
}

namespace tbsg
{
	class SceneHandler
	{
	public:
		GameDatabase& m_database;
		gfx::IRenderer* m_view{ nullptr };
		gfx::IParticleRenderer* particleRenderer{ nullptr };
		ptl::vector<ptl::shared_ptr<scene::SceneNode>> m_scenes{};
		ptl::vector<ptl::vector<std::pair<unsigned int, ptl::unique_ptr<scene::SceneNode>>>> m_cachedCardUnitsOfPlayers{};
		unsigned int m_currentScene{};
		ptl::unordered_map<ptl::string, ptl::shared_ptr<scene::SceneNode>> loadedModels{};
        waypoint::WaypointData playerWaypoints{};
        waypoint::WaypointData opponentWaypoints{};
        waypoint::WaypointData cameraWaypoints{};

        waypoint::WaypointData cardWaypoints{};

        ptl::vector<scene::SceneNode*> monsterSpawnPoints{};
        ptl::vector<ptl::vector<scene::SceneNode*>> cardSpawnPoints{2};

        float timer;

        ptl::map<unsigned int, gfx::TextureId> cardTextures{};

        ptl::unordered_map<ptl::string, gfx::ParticleEmitter> particleEmitters{};

    public:
        SceneHandler(GameDatabase& database, gfx::IRenderer* view = nullptr)
            : m_database(database),
              m_view(view)
        {
        }
        
        void Initialize();
        
        void DisableScene(unsigned int scene);
        
        void EnableScene(unsigned int scene);
        
        void LoadScene(ptl::string scene);

        void UnloadScene(unsigned int  id);
        
        void LoadMap(unsigned int  id);

        
        void UnloadMap();

        void LoadParticles();
        void UnloadParticles();

        ParticleEmitter GetParticle(const ptl::string& name) const;

        ptl::shared_ptr<scene::SceneNode> ChangeScene();


        void DeleteRenderables(scene::SceneNode* sceneNode);
        
        void FreeUnitCache();

        
        scene::SceneNode* AddModel(ptl::string name, core::Transform& transform, scene::SceneNode* parent = nullptr);

        scene::SceneNode* AddModel(ptl::unique_ptr<scene::SceneNode>&&, core::Transform& transform, scene::SceneNode* parent = nullptr);
        
        
        

        
        void DisableModel(scene::SceneNode* model);
        
        void EnableModel(scene::SceneNode* model);

        
        bool IsModelEnabled(const ptl::string& name) const;
        bool IsModelEnabled(scene::SceneNode* model) const;



        
        ptl::shared_ptr <scene::SceneNode> GetCurrentScene();

		
		
		
		
		
		
		
		
		
		scene::SceneNode* AddWorldspaceText(const gfx::UITextOptionsForTexture& options, float worldspacePanelWidth, float worldspacePanelHeight, 
			core::Transform localTransform = core::Transform{}, scene::SceneNode* parent = nullptr, bool reverseWindingOrder = false, bool flipXAxisUVs = true);

		
		
		
		
		
		
		
		
		
		scene::SceneNode* AddWorldspacePlane(float worldspacePanelWidth, float worldspacePanelHeight, const ptl::string& textureName, 
			core::Transform localTransform = core::Transform{}, scene::SceneNode* parent = nullptr, bool reverseWindingOrder = false, bool flipXAxisUVs = true);

		scene::SceneNode* AddWorldspacePlane(float worldspacePanelWidth, float worldspacePanelHeight, const gfx::TextureId textureId, 
			core::Transform localTransform = core::Transform{}, scene::SceneNode* parent = nullptr, bool reverseWindingOrder = false, bool flipXAxisUVs = true);

		void UpdateWorldspaceText(const gfx::UITextOptionsForTexture& options, scene::SceneNode* node);

        
        scene::SceneNode* AddCubeToScene(const ptl::string& name, TextureId textureId, size_t size, core::Transform transform,scene::SceneNode* parent = nullptr); 

		scene::SceneNode* GetHeroModel(unsigned int heroIndex);

        
        scene::SceneNode* GetCardModelFromCache(unsigned int cardID, unsigned int slotIndex, unsigned int playerIndex);

        scene::SceneNode* CreateCardModelForPlayer(unsigned int cardID, unsigned int playerIndex);


        gfx::TextureId GetCardTexture(unsigned int cardID);

        void CreateCardTexture(unsigned int cardID);

        scene::SceneNode* ChangeMaterialOf(scene::SceneNode* model, const ptl::string& materialName);

        scene::SceneNode* ChangeMaterialOf(const ptl::string& modelName, const ptl::string& materialName);

        void SetRenderer(gfx::IRenderer* const view);
		void SetParticleRenderer(gfx::IParticleRenderer* const particleRenderer);
	};
}




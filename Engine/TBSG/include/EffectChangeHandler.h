#pragma once
#include "Payloads.h"
#include "utils/EventQueues.h"
#include "memory/smart_ptr.h"
#include "rendering/Camera.h"
#include "rendering/unified_shader_passes/ParticlePass.h"

namespace tbsg
{
    class SceneHandler;
}

namespace audio
{
    class AudioSystem;
}

class GameDatabase;
namespace EffectChangeHandler
{

    void ProcessChanges(ptl::vector<tbsg::Change> changes, ptl::queue<tbsg::EffectEvent*>& effectEvent, ptl::shared_ptr<UIEventQueue> userInterface, tbsg::SceneHandler& sceneHandler, tbsg::ClientMatchState& clientMatchState, audio::AudioSystem& audioSystem, GameDatabase& database, gfx::Camera& camera, double& deltaTime, unsigned
                        playerIndex);

    bool UpdateUIText(UIInteractionEvent uiType, std::array<int, 2> value, ptl::shared_ptr<UIEventQueue> userInterface);
	bool UpdateUITextPacket(UIInteractionEvent uiType, Packet value, ptl::shared_ptr<UIEventQueue> userInterface);
    bool UpdateUICardBox(UIInteractionEvent uiType, int cardID, ptl::string path, ptl::shared_ptr<UIEventQueue> userInterface);
    bool UpdateUIString(UIInteractionEvent uiType, ptl::string string, ptl::shared_ptr<UIEventQueue> userInterface);
	bool EnqueueUIEvent(UIInteractionEvent uiType, ptl::shared_ptr<UIEventQueue> userInterface);
	bool EnqueueUIEventBool(UIInteractionEvent uiType, bool value, ptl::shared_ptr<UIEventQueue> userInterface);

    bool PlaySoundClipID(unsigned int clipID, audio::AudioSystem* audioSystem);
    bool PlaySoundClipName(ptl::string audioClipName, audio::AudioSystem* audioSystem);

    bool PlayParticleEffect(tbsg::SceneHandler* sceneHandler, glm::vec3 position, ptl::string particleName);

    bool AddDelay(float* timer, double* deltaTime, float delay);

    bool MoveToNextPosition(tbsg::SceneHandler* sceneHandler, gfx::Camera* camera, double* deltaTime);
    bool GoToNextPosition(tbsg::SceneHandler* sceneHandler, gfx::Camera* camera, double* deltaTime);
    bool RevealCard(tbsg::SceneHandler* sceneHandler, unsigned int playerID, unsigned int encounterID, float speed, double* deltaTime);

    bool RemoveMonster(tbsg::SceneHandler* sceneHandler, tbsg::ClientMatchState* clientMatchState);
    bool RemoveCard(tbsg::SceneHandler* sceneHandler, int playerIndex);

    int GetIndexInVector(ptl::vector<unsigned int>& inVector, unsigned int ID);

    bool SpawnCard(unsigned playerID, unsigned card, tbsg::SceneHandler& sceneHandler, tbsg::ClientMatchState& clientMatchState, bool clearBeforeSpawn = false);

    template<typename F, typename ...Args>
    tbsg::EffectEvent* RegisterAPIEvent(F listener, ptl::string name, Args...args);
};

template<typename F, typename ...Args>
inline tbsg::EffectEvent* EffectChangeHandler::RegisterAPIEvent(F listener, ptl::string name, Args...args)
{
    
    tbsg::EffectEvent* ev = new tbsg::EffectEvent();
    ev->customEventName = name;
    ev->eventDelegate = [listener, args...]() 
    {
        auto func = [listener, args...]()
        {
            return (listener)(args...);
        };
        return func();
    };
    return ev;
};

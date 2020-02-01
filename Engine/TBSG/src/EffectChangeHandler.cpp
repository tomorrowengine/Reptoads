#include "..\include\EffectChangeHandler.h"
#include "SceneHandler.h"
#include "audio/AudioSystem.h"
#include "GameDatabase.h"

void EffectChangeHandler::ProcessChanges(ptl::vector<tbsg::Change> changes, ptl::queue<tbsg::EffectEvent*>& effectEvent, ptl::shared_ptr<UIEventQueue> userInterface, tbsg::SceneHandler& sceneHandler, tbsg::ClientMatchState& clientMatchState, audio::AudioSystem& audioSystem, GameDatabase& database, gfx::Camera& camera, double& deltaTime,unsigned int playerIndex)
{

	
	for (auto change : changes)
	{
		
		switch (change.changeType)
		{
			case tbsg::EffectChange::Card_Health:
			{
				
				clientMatchState.monsterCards[0].data.health += change.change;
				
				ptl::array<int, 2> healthValues{ static_cast<int>(clientMatchState.monsterCards[0].data.health), static_cast<int>(clientMatchState.monsterCards[0].data.maxHealth) };
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText,"UpdateMonsterHealthUI",UIInteractionEvent::OnUpdateHealthMonster, healthValues, userInterface));
				
				
				if (change.change > 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Monster1_utility-V1.wav", &audioSystem));
                    glm::vec3 position = sceneHandler.monsterSpawnPoints[sceneHandler.monsterSpawnPoints.size() - clientMatchState.monsterCards.size()]->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "HealingParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
					
				}
				else
				{
					
                    glm::vec3 position = sceneHandler.monsterSpawnPoints[sceneHandler.monsterSpawnPoints.size() - clientMatchState.monsterCards.size()]->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                    

				}
				break;
			}
			case tbsg::EffectChange::Card_Attack:
			{
				break;
			}
			case tbsg::EffectChange::Card_Armor:
			{
				
				clientMatchState.monsterCards[0].data.armor += change.change;
				
				
				ptl::array<int, 2>  armorValues{ static_cast<int>(clientMatchState.monsterCards[0].data.armor), 0 };
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateMonsterArmorUI", UIInteractionEvent::OnUpdateArmorMonster, armorValues, userInterface));
				
				
				if (change.change > 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Monster1_utility-V1.wav", &audioSystem));
                    
                    glm::vec3 position = sceneHandler.monsterSpawnPoints[sceneHandler.monsterSpawnPoints.size() - clientMatchState.monsterCards.size()]->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "ArmorParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
				}
				else
				{
					
                    glm::vec3 position = sceneHandler.monsterSpawnPoints[sceneHandler.monsterSpawnPoints.size() - clientMatchState.monsterCards.size()]->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                    

				}
				break;
			}
			case tbsg::EffectChange::Card_Particle:
			{
				break;
			}
			case tbsg::EffectChange::Card_Animation:
			{
				break;
			}
			case tbsg::EffectChange::Card_Death:
			{
				
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Monster1_death-V1.wav", &audioSystem));

                glm::vec3 position = sceneHandler.monsterSpawnPoints[sceneHandler.monsterSpawnPoints.size() - clientMatchState.monsterCards.size()]->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DeathParticle.json"));
                effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::RemoveMonster, "RemoveMonster", &sceneHandler, &clientMatchState));
                effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                
				break;
			}
			case tbsg::EffectChange::Hero_Weapon_Attack:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Weapon_Durability:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Health:
			{
				
				clientMatchState.heroes[change.index].health += change.change;

				
				ptl::array<int, 2>  healthValues{ static_cast<int>(clientMatchState.heroes[change.index].health), static_cast<int>(clientMatchState.heroes[change.index].maxHealth) };
				if (change.index == 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroHealthUI", UIInteractionEvent::OnUpdateHealthPlayer, healthValues, userInterface));
				}
				else
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroHealthUI", UIInteractionEvent::OnUpdateHealthOpponent, healthValues, userInterface));
				}

				
				if (change.change > 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Player1_utility-V1.wav", &audioSystem));
                    if (change.index == 0)
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("player")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "HealingParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
                    else
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "HealingParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
				}
				else
				{
					
                    if (change.index == 0)
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("player")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
                    else
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
				}
				break;
			}
			case tbsg::EffectChange::Hero_Resource:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Armor:
			{
				
				clientMatchState.heroes[change.index].armor += change.change;

				
				ptl::array<int, 2> armorValues{ static_cast<int>(clientMatchState.heroes[change.index].armor), 0 };
				if (change.index == 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroArmorhUI", UIInteractionEvent::OnUpdateArmorPlayer, armorValues, userInterface));
				}
				else
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroArmorhUI", UIInteractionEvent::OnUpdateArmorOpponent, armorValues, userInterface));
				}

				
				if (change.change > 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Player1_utility-V1.wav", &audioSystem));
                    if (change.index == 0)
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("player")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "ArmorParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
                    else
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "ArmorParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
				}
				else
				{
					
                    if (change.index == 0)
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("player")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
                    else
                    {
                        glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DamageParticle.json"));
                        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                        
                    }
				}
				break;
			}
			case tbsg::EffectChange::Hero_Attack:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Particle:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Animation:
			{
				break;
			}
			case tbsg::EffectChange::Hero_Death:
			{
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Player1_death-V1.wav", &audioSystem));
				bool enable = false;
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::EnqueueUIEventBool, "DisableCardSubmitForDeadPlayer", UIInteractionEvent::OnSetConfirmEnabled, enable, userInterface));
                if (change.index == 0)
                {
                    glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("player")->transform.GetPos() + glm::vec3{ 0, 1, 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DeathParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                }
                else
                {
                    glm::vec3 position = sceneHandler.GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos() + glm::vec3{0, 1, 0};
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlayParticleEffect, "PlayParticleEffect", &sceneHandler, position, "DeathParticle.json"));
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.5f));
                }
				break;
			}
			case tbsg::EffectChange::Hero_WinLose:
			{
				if (change.index == 0)
				{
					ptl::array<int, 2> valueWinLoss{ change.change, 0 };
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroHealthUI", UIInteractionEvent::OnUpdateHealthPlayer, valueWinLoss, userInterface));
					if (change.change == 0)
					{
						effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::EnqueueUIEvent, "EnqueueOnWinScreen", UIInteractionEvent::OnWinScreen, userInterface));
						effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "victory_sound-V1.wav", &audioSystem));
					}
					else
					{
						effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::EnqueueUIEvent, "EnqueueOnLoseScreen", UIInteractionEvent::OnLoseScreen, userInterface));
						effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "lose_sound-V2.wav", &audioSystem));
					}
				}
				break;
			}
			case tbsg::EffectChange::Reward_Attack:
			{
				break;
			}
			case tbsg::EffectChange::Reward_Health:
			{
				clientMatchState.heroes[change.index].health += change.change;
				ptl::array<int, 2>  healthValues{ static_cast<int>(clientMatchState.heroes[change.index].health), static_cast<int>(clientMatchState.heroes[change.index].maxHealth) };
				if (change.index == 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroHealthUI", UIInteractionEvent::OnUpdateHealthPlayer, healthValues, userInterface));
				}
				else
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroHealthUI", UIInteractionEvent::OnUpdateHealthOpponent, healthValues, userInterface));
				}
				break;
			}
			case tbsg::EffectChange::Reward_Armor:
			{
				clientMatchState.heroes[change.index].armor += change.change;
				ptl::array<int, 2> armorValues{ static_cast<int>(clientMatchState.heroes[change.index].armor), 0 };
				if (change.index == 0)
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroArmorhUI", UIInteractionEvent::OnUpdateArmorPlayer, armorValues, userInterface));
				}
				else
				{
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateHeroArmorhUI", UIInteractionEvent::OnUpdateArmorOpponent, armorValues, userInterface));
				}
				break;
			}
			case tbsg::EffectChange::Reward_Resource:
			{
				break;
			}
			case tbsg::EffectChange::Reward_Weapon_Attack:
			{
				break;
			}
			case tbsg::EffectChange::Reward_Weapon_Durability:
			{
				break;
			}
			case tbsg::EffectChange::Deck_Shuffle:
			{
				break;
			}
			case tbsg::EffectChange::Deck_DiscardToDeck:
			{
				if (change.index == 0)
				{
					clientMatchState.playerDecks[change.index].push_back(change.change);
					clientMatchState.playerDiscards[change.index].erase(clientMatchState.playerDiscards[change.index].begin() + GetIndexInVector(clientMatchState.playerDiscards[change.index],change.change));

					ptl::array<int, 2> vectorSizeDeck{ static_cast<int>(clientMatchState.playerDecks[change.index].size()), 0 };
					ptl::array<int, 2> vectorSizeDiscard{ static_cast<int>(clientMatchState.playerDiscards[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDeckUI", UIInteractionEvent::OnUpdateDeckPlayer, vectorSizeDeck, userInterface));
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDiscardUI", UIInteractionEvent::OnUpdateDiscardPlayer, vectorSizeDiscard, userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Deck_AddCard:
			{
				if (change.index == 0)
				{
					clientMatchState.playerDecks[change.index].push_back(change.change);

					ptl::array<int, 2>vectorSize{static_cast<int>(clientMatchState.playerDecks[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDeckUI", UIInteractionEvent::OnUpdateDeckPlayer, vectorSize, userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Deck_RemoveCard:
			{
				if (change.index == 0)
				{
					clientMatchState.playerDecks[change.index].erase(clientMatchState.playerDecks[change.index].begin() + GetIndexInVector(clientMatchState.playerDecks[change.index], change.change));

					ptl::array<int, 2>vectorSize{ static_cast<int>(clientMatchState.playerDecks[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDeckUI", UIInteractionEvent::OnUpdateDeckPlayer, vectorSize, userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Deck_DrawCard:
			{
				if (change.index == 0)
				{
					if (!clientMatchState.playerDecks[change.index].empty())
					{
						clientMatchState.playerDecks[change.index].erase(
							clientMatchState.playerDecks[change.index].begin() + GetIndexInVector(clientMatchState.playerDecks[change.index], change.change)
						);
					}
					clientMatchState.playerHands[change.index].push_back(change.change);

					ptl::array<int, 2>vectorSize{ static_cast<int>(clientMatchState.playerDecks[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDeckUI", UIInteractionEvent::OnUpdateDeckPlayer, vectorSize, userInterface));
					auto card = database.GetCard(change.change);
					if (card != nullptr)
					{
						ptl::string filename = "Card_" + card->meta.name + ".png";
						effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUICardBox, "UpdateHandUI", UIInteractionEvent::OnPushCard, change.change, filename, userInterface));
					}

					
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Card_draw-V1.wav", &audioSystem));

				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Hand_AddCard:
			{
				if (change.index == 0)
				{
					clientMatchState.playerHands[change.index].push_back(change.change);
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUICardBox, "UpdateHandUI", UIInteractionEvent::OnPushCard, change.change, "Card_" + database.GetCard(change.change)->meta.name + ".png", userInterface));
					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipName, "PlayAudioClip", "Card_draw-V1.wav", &audioSystem));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Hand_RemoveCard:
			{
				if (change.index == 0)
				{
					int index = GetIndexInVector(clientMatchState.playerHands[change.index], change.change);
					clientMatchState.playerHands[change.index].erase(clientMatchState.playerHands[change.index].begin() + index);

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUICardBox, "UpdateHandUI", UIInteractionEvent::OnPopCard, change.change, "Card_" + database.GetCard(change.change)->meta.name + ".png", userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Discard_AddCard:
			{
				if (change.index == 0)
				{
					clientMatchState.playerDiscards[change.index].push_back(change.change);

					ptl::array<int, 2>vectorSize{ static_cast<int>(clientMatchState.playerDiscards[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDiscardUI", UIInteractionEvent::OnUpdateDiscardPlayer, vectorSize, userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::Discard_RemoveCard:
			{
				if (change.index == 0)
				{
					clientMatchState.playerDiscards[change.index].erase(clientMatchState.playerDiscards[change.index].begin() + GetIndexInVector(clientMatchState.playerDiscards[change.index], change.change));

					ptl::array<int, 2>vectorSize{ static_cast<int>(clientMatchState.playerDiscards[change.index].size()), 0 };

					effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateDiscardUI", UIInteractionEvent::OnUpdateDiscardPlayer, vectorSize, userInterface));
				}
				else
				{
					
				}
				break;
			}
			case tbsg::EffectChange::NextMonster:
			{
                clientMatchState.monsterCards.erase(clientMatchState.monsterCards.begin());
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::GoToNextPosition, "MoveToNextWaypoint", &sceneHandler, &camera, &deltaTime));

                if (!clientMatchState.monsterCards.empty())
                {
					ptl::array<int, 2> monsterHealth{ static_cast<int>(clientMatchState.monsterCards[0].data.health), static_cast<int>(clientMatchState.monsterCards[0].data.maxHealth) };

                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateMonsterHealthhUI", UIInteractionEvent::OnUpdateHealthMonster, monsterHealth, userInterface));

					ptl::array<int, 2> monsterArmor{ static_cast<int>(clientMatchState.monsterCards[0].data.armor), 0 };
                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateMonsterArmorhUI", UIInteractionEvent::OnUpdateArmorMonster, monsterArmor, userInterface));

                    effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIString, "UpdateMonsterNameUI", UIInteractionEvent::OnUpdateNameMonster, clientMatchState.monsterCards[0].meta.name, userInterface));
                }
				else
				{
					audioSystem.StopMusic();
					audioSystem.StartMusic(1);
				}
				break;
			}
			case tbsg::EffectChange::NextStartingPlayer:
			{
				ptl::array<int, 2> nextPlayerValues{ static_cast<int>(change.index), 0 };
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::UpdateUIText, "UpdateFristPlayerTagUI", UIInteractionEvent::OnUpdateFirstPlayerTag, nextPlayerValues, userInterface));
				break;
			}
			case tbsg::EffectChange::Play_Sound:
			{
				effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::PlaySoundClipID, "PlayAudioClip", change.change, &audioSystem));
				break;
			}
		    case tbsg::EffectChange::None: 
            {
                break;
            }
		    default:
			{
                break;
			};
		}
        effectEvent.push(RegisterAPIEvent(&EffectChangeHandler::AddDelay, "AddDelay", &sceneHandler.timer, &deltaTime, 0.25f));
		
	}
}

bool EffectChangeHandler::UpdateUIText(UIInteractionEvent uiType, ptl::array<int, 2> value, ptl::shared_ptr<UIEventQueue> userInterface)
{
	ptl::array<int, 2> values[2]{ value[0], value[1]};
	userInterface->enqueue(uiType, [values]() mutable
	{
		return static_cast<void*>(&values);
	});
	return true;
}

bool EffectChangeHandler::UpdateUITextPacket(UIInteractionEvent uiType, Packet value,
	ptl::shared_ptr<UIEventQueue> userInterface)
{
	return true;
}

bool EffectChangeHandler::UpdateUICardBox(UIInteractionEvent uiType, int cardID, ptl::string path, ptl::shared_ptr<UIEventQueue> userInterface)
{
	std::pair<int, ptl::string> pair = std::make_pair(cardID, path);
	userInterface->enqueue(uiType, [pair]() mutable
	{
		return static_cast<void*>(&pair);
	});
	return true;
}

bool EffectChangeHandler::UpdateUIString(UIInteractionEvent uiType, ptl::string string, ptl::shared_ptr<UIEventQueue> userInterface)
{
	userInterface->enqueue(uiType, [string]() mutable
	{
		return static_cast<void*>(&string);
	});
	return true;
}

bool EffectChangeHandler::EnqueueUIEvent(UIInteractionEvent uiType, ptl::shared_ptr<UIEventQueue> userInterface)
{
	userInterface->enqueue(uiType, nullptr);
	return true;
}

bool EffectChangeHandler::EnqueueUIEventBool(UIInteractionEvent uiType, bool value, ptl::shared_ptr<UIEventQueue> userInterface)
{
	userInterface->enqueue(uiType, [value]() mutable
		{
			return static_cast<void*>(&value);
		});
	return true;
}

bool EffectChangeHandler::PlaySoundClipID(unsigned int clipID, audio::AudioSystem* audioSystem)
{
    audioSystem->PlaySoundEffect(clipID);
	return true;
}

bool EffectChangeHandler::PlaySoundClipName(ptl::string audioClipName, audio::AudioSystem * audioSystem)
{
	audioSystem->PlaySoundEffect(audioSystem->GetSoundIDFromName(audioClipName));
	return true;
}

bool EffectChangeHandler::PlayParticleEffect(tbsg::SceneHandler* sceneHandler, glm::vec3 position, ptl::string particleName)
{
    sceneHandler->particleRenderer->AddEmitter(core::Transform{ position, glm::quat{}, glm::vec3{0.15,0.15,0.15} }, std::move(sceneHandler->GetParticle(particleName)));
    return true;
}


bool EffectChangeHandler::AddDelay(float* timer, double* deltaTime, float delay)
{
    *timer += static_cast<float>(*deltaTime);

    if(*timer >= delay)
    {
        *timer = 0.f;
        return true;
    }
    return false;
}

bool EffectChangeHandler::MoveToNextPosition(tbsg::SceneHandler* sceneHandler, gfx::Camera* camera, double* deltaTime)
{
    float lerpTime              = 0.f;
    float lerpSpeedCamera       = 0.f;
    float lerpSpeedOpponent     = 0.f;
    const float lerpSpeedPlayer = 75.f;

    if(sceneHandler->playerWaypoints.distanceToNextPoint == 0.f)
    {
        
        sceneHandler->playerWaypoints.distanceToNextPoint = waypoint::GetWaypointDistance(sceneHandler->playerWaypoints);
    }
    lerpTime = sceneHandler->playerWaypoints.distanceToNextPoint / lerpSpeedPlayer;

    if(sceneHandler->cameraWaypoints.distanceToNextPoint == 0.f)
    {
        
        sceneHandler->cameraWaypoints.distanceToNextPoint = waypoint::GetWaypointDistance(sceneHandler->cameraWaypoints);
    }
    lerpSpeedCamera = sceneHandler->cameraWaypoints.distanceToNextPoint / lerpTime;

    if (sceneHandler->opponentWaypoints.distanceToNextPoint == 0.f)
    {
        
        sceneHandler->opponentWaypoints.distanceToNextPoint = waypoint::GetWaypointDistance(sceneHandler->opponentWaypoints);
    }
    lerpSpeedOpponent = sceneHandler->opponentWaypoints.distanceToNextPoint / lerpTime;

    waypoint::TransformAlongSpline(sceneHandler->cameraWaypoints, *deltaTime, camera->m_Translation, camera->m_Rotation, lerpSpeedCamera);
    waypoint::TransformAlongSpline(sceneHandler->playerWaypoints, *deltaTime, sceneHandler->GetCurrentScene()->FindNodeByName("player")->transform, lerpSpeedPlayer);
    waypoint::TransformAlongSpline(sceneHandler->opponentWaypoints, *deltaTime, sceneHandler->GetCurrentScene()->FindNodeByName("opponent")->transform, lerpSpeedOpponent);
    
    
    
    

    if(sceneHandler->cameraWaypoints.isDone && sceneHandler->playerWaypoints.isDone && sceneHandler->opponentWaypoints.isDone)
    {
        sceneHandler->cameraWaypoints.isDone     = false;
        sceneHandler->playerWaypoints.isDone     = false;
        sceneHandler->opponentWaypoints.isDone   = false;
                    
        sceneHandler->playerWaypoints.distanceToNextPoint    = 0.f;
        sceneHandler->cameraWaypoints.distanceToNextPoint    = 0.f;
        sceneHandler->opponentWaypoints.distanceToNextPoint  = 0.f;

        return true;
    }
    return false;
}

bool EffectChangeHandler::GoToNextPosition(tbsg::SceneHandler* sceneHandler, gfx::Camera* camera, double* deltaTime)
{
    waypoint::TransformAlongSpline(sceneHandler->cameraWaypoints, *deltaTime, camera->m_Translation, camera->m_Rotation, 5.f);
    const bool shouldPlayOpponentLandParticle = waypoint::MoveCharacter(sceneHandler->opponentWaypoints, sceneHandler->cameraWaypoints, 0.15, 0.8, *deltaTime, sceneHandler->GetCurrentScene()->FindNodeByName("opponent")->transform, 20.f, 10.f);
    const bool shouldPlayPlayerLandParticle = waypoint::MoveCharacter(sceneHandler->playerWaypoints, sceneHandler->cameraWaypoints, 0.1, 0.75, *deltaTime, sceneHandler->GetCurrentScene()->FindNodeByName("player")->transform, 20.f, 10.f);

    if(shouldPlayOpponentLandParticle)
    {
        PlayParticleEffect(sceneHandler, sceneHandler->GetCurrentScene()->FindNodeByName("opponent")->transform.GetPos(), "LandParticle.json");
    }
    if(shouldPlayPlayerLandParticle)
    {
        PlayParticleEffect(sceneHandler, sceneHandler->GetCurrentScene()->FindNodeByName("player")->transform.GetPos(), "LandParticle.json");
    }
    if (sceneHandler->cameraWaypoints.isDone && sceneHandler->playerWaypoints.isDone && sceneHandler->opponentWaypoints.isDone)
    {
        sceneHandler->cameraWaypoints.isDone        = false;
        

        sceneHandler->cameraWaypoints.alpha     = 0.f;
        sceneHandler->playerWaypoints.alpha     = 0.f;
        sceneHandler->opponentWaypoints.alpha   = 0.f;

        return true;
    }
    return false;
}

bool EffectChangeHandler::RevealCard(tbsg::SceneHandler* sceneHandler, unsigned int playerID, unsigned int encounterID, float speed, double* deltaTime)
{
	
   
    
    return true;
}

bool EffectChangeHandler::RemoveMonster(tbsg::SceneHandler* sceneHandler, tbsg::ClientMatchState* clientMatchState)
{
    const int monsterID = sceneHandler->monsterSpawnPoints.size() - clientMatchState->monsterCards.size() - 1;
    if(monsterID >= 0 && monsterID < sceneHandler->monsterSpawnPoints.size())
    {
        sceneHandler->DisableModel(sceneHandler->GetCurrentScene()->FindNodeByName("monster" + ptl::to_string(monsterID)));
    }
    return true;
}

bool EffectChangeHandler::RemoveCard(tbsg::SceneHandler* sceneHandler, int playerIndex)
{
    scene::SceneNode* card = sceneHandler->GetCurrentScene()->FindNodeByName("card" + ptl::to_string(playerIndex));
    if(card)
    {
		sceneHandler->DisableModel(card);
    }
    return true;
}

int EffectChangeHandler::GetIndexInVector(ptl::vector<unsigned int>& inVector, unsigned int ID)
{

	for (int i = 0; i < inVector.size(); ++i)
	{
		if (inVector[i] == ID)
		{
			return i;
		}
	}
	return 0;
}

bool EffectChangeHandler::SpawnCard(unsigned int playerID, unsigned int cardId, tbsg::SceneHandler& sceneHandler, tbsg::ClientMatchState& clientMatchState, bool clearBeforeSpawn)
{
	

	if (clearBeforeSpawn) EffectChangeHandler::RemoveCard(&sceneHandler, playerID);

	int cardIndex = sceneHandler.cardSpawnPoints[playerID].size() - clientMatchState.monsterCards.size();
	cardIndex = (sceneHandler.cardSpawnPoints[playerID].size() == cardIndex) ? sceneHandler.cardSpawnPoints[playerID].size()-1 : cardIndex;
	std::cout << cardIndex << '\n';

	scene::SceneNode* cardModel = sceneHandler.CreateCardModelForPlayer(cardId, playerID);
	if (cardModel != nullptr)
	{
		sceneHandler.EnableModel(cardModel);
		cardModel->SetParent(sceneHandler.cardSpawnPoints[playerID][cardIndex]);
		cardModel->transform.SetPos(glm::vec3(0, 1, 0));
		cardModel->transform.SetRot(glm::quat{ {glm::radians(-90.0f),0,0} });
		cardModel->transform.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	}
    return true;
}

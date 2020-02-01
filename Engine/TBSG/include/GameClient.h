#pragma once
#include "Net/Client.h"
#include "memory/smart_ptr.h"
#include "utils/EventQueues.h"
#include "memory/Containers.h"

class GameClient : public net::Client
{
public:
	GameClient() = default;
	~GameClient();

	void Initialize(ptl::shared_ptr<GameEventQueue> gameEventQueue);
	void Update();

	
	void OnConnect() override;
	void OnDisconnect() override;
	void OnIdentificationSuccess() override;
	void OnIdentificationFailure(net::IdentifyResponse response) override;

	
	const ptl::string& GetNetworkAccessToken() const { return accessToken; }
	void SetNetworkAccessToken(const ptl::string& newToken) { accessToken = newToken; }

private:
	void GetIdentity(Packet& packet) override;
	void HandleCustomPacket(unsigned customCommand, Packet& packet) override;

	
	void ClientIsReady(Packet&& packet);
	void OnCardHover(Packet&& packet);
	void OnCardSelected(Packet&& packet);
	void OnCardsSubmitted(Packet&& packet);

	ptl::string accessToken{};

	ptl::shared_ptr<GameEventQueue> gameEventQueue;
	ptl::unordered_map<GameEvent, GameEventQueue::Handle> listenerHandles{};
};

#include "DatabaseAPI.h"
#include <utility>
#include "LoggingFunction.h"
#include <windows.h>

inline tbsg::Card* GetCard(unsigned int cardId, ptl::sparse_set<unsigned int, tbsg::Card>& cards)
{
	return &cards.at(cardId);
}

db::DatabaseAPI::DatabaseAPI(DatabaseCredentials credentials, unsigned int projectId) : projectId(projectId), credentials(std::move(credentials))
{
}

void db::DatabaseAPI::OpenDatabaseConnection()
{
	this->account = mariadb::account::create(
		this->credentials.host,
		this->credentials.username,
		this->credentials.password,
		this->credentials.database,
		this->credentials.port
	);
	this->connection = mariadb::connection::create(this->account);
	try{
		if(!this->connection->connect())
		{
			cof::Error("{} Failed to connect to database!", databasePrefix);
			assert(false);
		}
	}catch(std::exception e)
	{
		cof::Error("{} Failed to connect to database!", databasePrefix);
		assert(false);
	}
	cof::Info("{} Connected to database!", databasePrefix);
}

unsigned db::DatabaseAPI::GetProfileId(const ptl::string& token) const
{
	mariadb::statement_ref tokenStatement = connection->create_statement("SELECT * FROM `sessions` WHERE `token`=?;");
	tokenStatement->set_string(0, std::string(token));
	mariadb::result_set_ref tokenResult = tokenStatement->query();

	if (tokenResult->row_count() <= 0)
	{
		return 0;
	}

	tokenResult->next();

	return tokenResult->get_unsigned32("profileId");
}

tbsg::Profile db::DatabaseAPI::GetProfile(unsigned profileId) const
{
	mariadb::statement_ref profileStatement = connection->create_statement("SELECT * FROM `profiles` WHERE `id`=?;");
	profileStatement->set_unsigned32(0, profileId);
	mariadb::result_set_ref profileResult = profileStatement->query();

	tbsg::Profile profile{};

	if (profileResult->row_count() <= 0)
	{
		profile.id = 0;
		return profile;
	}

	profileResult->next();

	profile.id = profileResult->get_unsigned32("id");
	profile.username = profileResult->get_string("username");

	return profile;
}

ptl::sparse_set<unsigned int, tbsg::Card> db::DatabaseAPI::GetCards() const
{
	mariadb::statement_ref cardsStatement = connection->create_statement("SELECT * FROM `cards` WHERE `projectId`=?;");
	cardsStatement->set_unsigned32(0, this->projectId);
	mariadb::result_set_ref cardsResult = cardsStatement->query();

	ptl::sparse_set<unsigned int, tbsg::Card> cards;
	cards.reserve(cardsResult->row_count());

	auto handleCardEffects = [](tbsg::Card& card, tbsg::BaseEffect effect, int value)
	{
		if (value != 0)
		{
			card.data.baseCardEffects.emplace_back(tbsg::BaseCardEffects{ effect, value });
		}
	};

	while (cardsResult->next())
	{
		tbsg::Card card;

		card.id = cardsResult->get_unsigned32("id");
		card.meta.name = cardsResult->get_string("name");
		card.meta.description = cardsResult->get_string("description");
		card.meta.rarity = cardsResult->get_unsigned32("cardRarityId");
		card.meta.type = cardsResult->get_unsigned32("cardTypeId");

		handleCardEffects(card, tbsg::BaseEffect::MonsterDamage, cardsResult->get_signed32("monsterDamage"));
		handleCardEffects(card, tbsg::BaseEffect::OpponentDamage, cardsResult->get_signed32("opponentDamage"));
		handleCardEffects(card, tbsg::BaseEffect::SelfDamage, cardsResult->get_signed32("selfDamage"));

		handleCardEffects(card, tbsg::BaseEffect::MonsterHealth, cardsResult->get_signed32("monsterHealth"));
		handleCardEffects(card, tbsg::BaseEffect::OpponentHealth, cardsResult->get_signed32("opponentHealth"));
		handleCardEffects(card, tbsg::BaseEffect::SelfHealth, cardsResult->get_signed32("selfHealth"));

		handleCardEffects(card, tbsg::BaseEffect::MonsterArmor, cardsResult->get_signed32("monsterArmor"));
		handleCardEffects(card, tbsg::BaseEffect::OpponentArmor, cardsResult->get_signed32("opponentArmor"));
		handleCardEffects(card, tbsg::BaseEffect::SelfArmor, cardsResult->get_signed32("selfArmor"));

		handleCardEffects(card, tbsg::BaseEffect::OpponentDrawCard, cardsResult->get_signed32("opponentDraw"));
		handleCardEffects(card, tbsg::BaseEffect::SelfDrawCard, cardsResult->get_signed32("selfDraw"));

		handleCardEffects(card, tbsg::BaseEffect::OpponentDiscardCard, cardsResult->get_signed32("opponentDiscard"));
		handleCardEffects(card, tbsg::BaseEffect::SelfDiscardCard, cardsResult->get_signed32("selfDiscard"));

		cards.insert(card.id, card);
	}

	return cards;
}

ptl::sparse_set<unsigned, tbsg::MonsterCard> db::DatabaseAPI::GetMonsterCards() const
{
	mariadb::statement_ref cardsStatement = connection->create_statement("SELECT * FROM `monsters` WHERE `projectId`=?;");
	cardsStatement->set_unsigned32(0, this->projectId);
	mariadb::result_set_ref cardsResult = cardsStatement->query();

	ptl::sparse_set<unsigned int, tbsg::MonsterCard> cards;
	cards.reserve(cardsResult->row_count());

	while (cardsResult->next())
	{
		tbsg::MonsterCard card;

		card.id = cardsResult->get_unsigned32("id");
		card.meta.name = cardsResult->get_string("name");
		card.meta.description = cardsResult->get_string("description");
		card.data.maxHealth = cardsResult->get_unsigned32("maxHealth");
		card.data.health = card.data.maxHealth;
		card.data.monsterTrait = cardsResult->get_unsigned32("trait");

		cards.insert(card.id, card);
	}

	return cards;
}

ptl::sparse_set<unsigned, tbsg::CardType> db::DatabaseAPI::GetCardTypes() const
{
	mariadb::result_set_ref cardTypesResult = connection->query("SELECT * FROM `cardTypes`;");

	ptl::sparse_set<unsigned, tbsg::CardType> cardTypes;
	cardTypes.reserve(cardTypesResult->row_count());

	while (cardTypesResult->next())
	{
		tbsg::CardType type{};

		type.id = cardTypesResult->get_unsigned32("id");
		type.name = cardTypesResult->get_string("name");

		cardTypes.insert(type.id, type);
	}

	return cardTypes;
}

ptl::sparse_set<unsigned, tbsg::CardRarity> db::DatabaseAPI::GetCardRarities() const
{
	mariadb::result_set_ref cardRarityResult = connection->query("SELECT * FROM `cardRarity`;");

	ptl::sparse_set<unsigned, tbsg::CardRarity> cardRarities;
	cardRarities.reserve(cardRarityResult->row_count());

	while (cardRarityResult->next())
	{
		tbsg::CardRarity rarity{};

		rarity.id = cardRarityResult->get_unsigned32("id");
		rarity.name = cardRarityResult->get_string("name");

		cardRarities.insert(rarity.id, rarity);
	}

	return cardRarities;
}

ptl::vector<tbsg::Deck> db::DatabaseAPI::GetDecksOfProfile(unsigned int profileId, ptl::sparse_set<unsigned int, tbsg::Card>& cards) const
{
	mariadb::statement_ref deckStatement = connection->create_statement("SELECT * FROM `decks` WHERE `profileId`=? AND `projectId`=?;");
	deckStatement->set_unsigned32(0, profileId);
	deckStatement->set_unsigned32(1, this->projectId);
	mariadb::result_set_ref deckResult = deckStatement->query();

	ptl::vector<tbsg::Deck> decks{};
	decks.reserve(deckResult->row_count());

	while (deckResult->next())
	{
		tbsg::Deck deck;

		deck.id = deckResult->get_unsigned32("id");
		deck.name = deckResult->get_string("name");

		const mariadb::result_set_ref cardsResult = connection->query(std::string("SELECT * FROM `deckCardRelation` WHERE `deckId`=" + ptl::to_string(deck.id) + ";" ));
		while (cardsResult->next())
		{
			deck.cards.push_back(GetCard(cardsResult->get_unsigned32("cardId"), cards));
		}

		decks.push_back(deck);
	}

	return decks;
}

tbsg::MonsterDeck db::DatabaseAPI::GetMonsterDeck(unsigned id, ptl::sparse_set<unsigned, tbsg::MonsterCard>& cards) const
{
	mariadb::statement_ref deckStatement = connection->create_statement("SELECT * FROM `monsterDecks` WHERE `id`=? AND `projectId`=?;");
	deckStatement->set_unsigned32(0, id);
	deckStatement->set_unsigned32(1, this->projectId);
	mariadb::result_set_ref deckResult = deckStatement->query();

	tbsg::MonsterDeck deck{};

	if (deckResult->row_count() <= 0)
	{
		deck.id = 0;
		return deck;
	}

	deck.id = deckResult->get_unsigned32("id");
	deck.name = deckResult->get_string("name");

	const mariadb::result_set_ref cardsResult = connection->query(std::string("SELECT * FROM `monsterDeckRelation` WHERE `deckId`=" + ptl::to_string(deck.id) + ";"));
	while (cardsResult->next())
	{
		deck.cards.push_back(&cards.at(cardsResult->get_unsigned32("id")));
	}

	return deck;
}

tbsg::MonsterDeck db::DatabaseAPI::GetMonsterDeck(const ptl::string& name, ptl::sparse_set<unsigned, tbsg::MonsterCard>& cards) const
{
	mariadb::statement_ref deckStatement = connection->create_statement("SELECT * FROM `monsterDecks` WHERE `name`=? AND `projectId`=?;");
	deckStatement->set_string(0, std::string(name));
	deckStatement->set_unsigned32(1, this->projectId);
	mariadb::result_set_ref deckResult = deckStatement->query();

	tbsg::MonsterDeck deck{};

	if (deckResult->row_count() <= 0)
	{
		deck.id = 0;
		return deck;
	}

	deckResult->next();

	deck.id = deckResult->get_unsigned32("id");
	deck.name = deckResult->get_string("name");

	const mariadb::result_set_ref cardsResult = connection->query(std::string("SELECT * FROM `monsterDeckRelation` WHERE `deckId`=" + ptl::to_string(deck.id) + ";"));
	while (cardsResult->next())
	{
		deck.cards.push_back(&cards.at(cardsResult->get_unsigned32("monsterId")));
	}

	return deck;
}

void db::DatabaseAPI::GetMonsterDecks(ptl::sparse_set<unsigned int, tbsg::MonsterDeck>& monsterDecks,ptl::sparse_set<unsigned int, tbsg::MonsterCard>& cards) const
{
	mariadb::statement_ref deckStatement = connection->create_statement("SELECT monsterDecks.*,monsterDeckRelation.monsterId as monsterId FROM monsterDecks INNER JOIN monsterDeckRelation ON monsterDeckRelation.deckId = monsterDecks.id WHERE projectId = ?");
	deckStatement->set_unsigned32(0, this->projectId);
	mariadb::result_set_ref deckResults = deckStatement->query();

	if (deckResults->row_count() <= 0)
	{
		return;
	}
	monsterDecks.reserve(deckResults->row_count());
	while (deckResults->next())
	{
		if(!monsterDecks.has(deckResults->get_unsigned32("id"))){
			tbsg::MonsterDeck deck{ };
			deck.id = deckResults->get_unsigned32("id");
			deck.name = deckResults->get_string("name");
			deck.cards.push_back(&cards.at(deckResults->get_unsigned32("monsterId")));
			monsterDecks.insert(deck.id, deck);
		}else
		{
			auto& deckRef = monsterDecks.at(deckResults->get_unsigned32("id"));
			deckRef.cards.push_back(&cards.at(deckResults->get_unsigned32("monsterId")));
		}
	}
	cof::Debug("Found {} monster decks", deckResults->row_count());
}

void db::DatabaseAPI::GetScriptsForCards(
	ptl::sparse_set<unsigned, tbsg::Script>& scripts) const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.*, cards.name as scriptName,cards.id as cardId FROM `scripts` \
		INNER JOIN scriptCardRelation ON scriptCardRelation.scriptId = scripts.id\
		INNER JOIN cards ON scriptCardRelation.cardId = cards.id\
		WHERE cards.projectId = 1 AND(scripts.id, scripts.revision) IN(SELECT scripts.id, MAX(scripts.revision) revision FROM scripts GROUP BY scripts.id)"
	);
	const mariadb::result_set_ref scriptResult = scriptStatement->query();
	if (scriptResult->row_count() <= 0)
	{
		return;
	}
	while (scriptResult->next())
	{
		scripts.insert(scriptResult->get_unsigned32("cardId"), { scriptResult->get_unsigned32("id"),scriptResult->get_unsigned32("cardId"),0 ,scriptResult->get_string("scriptName"),scriptResult->get_string("content") });
	}
}

void db::DatabaseAPI::GetScriptsForMonsterCards(ptl::sparse_set<unsigned int, tbsg::Script>& scripts) const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.*, monsters.name as scriptName,monsters.id as monsterId FROM `scripts` \
		INNER JOIN scriptMonsterRelation ON scriptMonsterRelation.scriptId = scripts.id \
		INNER JOIN monsters ON scriptMonsterRelation.monsterId = monsters.id \
		WHERE monsters.projectId = 1 AND(scripts.id, scripts.revision) IN(SELECT scripts.id, MAX(scripts.revision) revision FROM scripts GROUP BY scripts.id)"
	);
	const mariadb::result_set_ref scriptResult = scriptStatement->query();
	if (scriptResult->row_count() <= 0)
	{
		return;
	}
	while(scriptResult->next())
	{
		scripts.insert(scriptResult->get_unsigned32("monsterId"), { scriptResult->get_unsigned32("id"),0,scriptResult->get_unsigned32("monsterId") ,scriptResult->get_string("scriptName"),scriptResult->get_string("content") });
	}
}

tbsg::Script db::DatabaseAPI::GetScriptForCard(unsigned cardId) const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.id,scripts.content,scripts.revision FROM scripts			\
		INNER JOIN scriptCardRelation ON scripts.id = scriptCardRelation.scriptId	\
		WHERE scriptCardRelation.cardId = ?											\
		ORDER BY revision DESC														\
		LIMIT 1;"
	);
	scriptStatement->set_unsigned32(0, cardId);
	const mariadb::result_set_ref scriptResult = scriptStatement->query();

	tbsg::Script script{};

	if (scriptResult->row_count() <= 0)
	{
		script.id = 0;
		return script;
	}

	scriptResult->next();

	script.id = scriptResult->get_unsigned32("id");
	script.cardId = cardId;
	script.code = scriptResult->get_string("content");

	return script;
}

tbsg::Script db::DatabaseAPI::GetScriptForMonsterCard(unsigned cardId) const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.id,scripts.content,scripts.revision FROM scripts				\
		INNER JOIN scriptMonsterRelation ON scripts.id = scriptMonsterRelation.scriptId	\
		WHERE scriptMonsterRelation.monsterId = ?										\
		ORDER BY revision DESC															\
		LIMIT 1;"
	);
	scriptStatement->set_unsigned32(0, cardId);
	const mariadb::result_set_ref scriptResult = scriptStatement->query();

	tbsg::Script script{};

	if (scriptResult->row_count() <= 0)
	{
		script.id = 0;
		return script;
	}

	scriptResult->next();

	script.id = scriptResult->get_unsigned32("id");
	script.monsterCardId = cardId;
	script.code = scriptResult->get_string("content");

	return script;
}

ptl::vector<tbsg::Script> db::DatabaseAPI::GetOtherScripts() const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.*, scriptOtherRelation.name FROM scripts							\
		INNER JOIN scriptOtherRelation ON scripts.id = scriptOtherRelation.scriptId			\
		WHERE scriptOtherRelation.projectId = ?												\
		AND(id, revision) IN(SELECT id, MAX(revision) revision FROM scripts GROUP BY id);"
	);
	scriptStatement->set_unsigned32(0, this->projectId);
	const mariadb::result_set_ref scriptResult = scriptStatement->query();

	ptl::vector<tbsg::Script> scripts{};

	while (scriptResult->next())
	{
		tbsg::Script script;

		script.id = scriptResult->get_unsigned32("id");
		script.name = scriptResult->get_string("name");
		script.code = scriptResult->get_string("content");

		scripts.push_back(script);
	}

	return scripts;
}

tbsg::Script db::DatabaseAPI::GetOtherScript(const ptl::string& scriptName) const
{
	mariadb::statement_ref scriptStatement = connection->create_statement(
		"SELECT scripts.id,scripts.content,scripts.revision FROM scripts				\
		INNER JOIN scriptOtherRelation ON scripts.id = scriptOtherRelation.scriptId		\
		WHERE scriptOtherRelation.name = ?												\
		ORDER BY revision DESC															\
		LIMIT 1;"
	);
	scriptStatement->set_string(0, std::string(scriptName));
	const mariadb::result_set_ref scriptResult = scriptStatement->query();

	tbsg::Script script{};

	if (scriptResult->row_count() <= 0)
	{
		script.id = 0;
		return script;
	}

	scriptResult->next();

	script.id = scriptResult->get_unsigned32("id");
	script.name = scriptName;
	script.code = scriptResult->get_string("content");

	return script;
}

ptl::vector<tbsg::Server> db::DatabaseAPI::GetAvailableServers(bool production) const
{
	mariadb::statement_ref serverStatement = connection->create_statement("SELECT * FROM `servers` WHERE `occupied`=0 AND `production`=? AND `lastOnline` > NOW() - INTERVAL 15 SECOND;");
	serverStatement->set_unsigned32(0, production ? 1 : 0);
	mariadb::result_set_ref serverResult = serverStatement->query();

	ptl::vector<tbsg::Server> servers{};

	while(serverResult->next())
	{
		tbsg::Server server{};
		server.id = serverResult->get_unsigned32("id");
		server.hostname = serverResult->get_string("hostname");
		server.ip = serverResult->get_string("ip");
		server.port = serverResult->get_unsigned32("port");
		server.occupied = false; 
		servers.push_back(server);
	}

	return servers;
}

unsigned db::DatabaseAPI::RegisterServer(const std::string& ip, unsigned port, ptl::string hostname, bool production) const
{
	mariadb::statement_ref serverStatement = connection->create_statement("INSERT INTO `servers` (`hostname`, `ip`, `port`, `production`, `lastOnline`) VALUES (?, ?, ?, ?, NOW());");
	if(hostname.empty())
	{
		serverStatement->set_null(0);
	}
	else
	{
		serverStatement->set_string(0, std::string(hostname));
	}
	serverStatement->set_string(1, std::string(ip));
	serverStatement->set_unsigned32(2, port);
	serverStatement->set_unsigned32(3, production ? 1 : 0);
	return static_cast<unsigned int>(serverStatement->insert());
}

void db::DatabaseAPI::PingServer(unsigned serverId) const
{
	mariadb::statement_ref serverStatement = connection->create_statement("UPDATE `servers` SET `lastOnline`=NOW() WHERE `id`=?;");
	serverStatement->set_unsigned32(0, serverId);
	serverStatement->execute();
}

void db::DatabaseAPI::SetServerOccupied(unsigned serverId, bool isOccupied) const
{
	mariadb::statement_ref serverStatement = connection->create_statement("UPDATE `servers` SET `occupied`=?, `lastOnline`=NOW() WHERE `id`=?;");
	serverStatement->set_unsigned32(0, isOccupied ? 1 : 0);
	serverStatement->set_unsigned32(1, serverId);
	serverStatement->execute();
}

void db::DatabaseAPI::RegisterIp(unsigned profileId, const ptl::string& ip) const
{
	this->RegisterIp(profileId, 0, ip);
}

void db::DatabaseAPI::RegisterIp(unsigned profileId, unsigned serverId, const ptl::string& ip) const
{
	mariadb::statement_ref ipStatement = connection->create_statement("INSERT INTO `ipLog` (`profileId`, `serverId`, `ip`, `date`) VALUES (?, ?, ?, NOW());");
	ipStatement->set_unsigned32(0, profileId);
	if(serverId == 0)
	{
		ipStatement->set_null(1);
	}
	else
	{
		ipStatement->set_unsigned32(1, serverId);
	}
	ipStatement->set_string(2, ip);
	ipStatement->execute();
}


const tbsg::Match db::DatabaseAPI::RegisterMatch(tbsg::Match match) const
{
	mariadb::statement_ref matchStatement = connection->create_statement("INSERT INTO `matches` (`ownerProfileId`, `ownerDeckId`, `opponentProfileId`, `opponentDeckId`, `serverId`, `createdDate`) VALUES (?, ?, ?, ?, ?, NOW());");
	matchStatement->set_unsigned32(0, match.ownerProfileId);
	matchStatement->set_unsigned32(1, match.ownerDeckId);
	if(match.opponentProfileId == 0)
	{
		matchStatement->set_null(2);
	}
	else
	{
		matchStatement->set_unsigned32(2, match.opponentProfileId);
	}
	if(match.opponentDeckId == 0)
	{
		matchStatement->set_null(3);
	}
	else
	{
		matchStatement->set_unsigned32(3, match.opponentDeckId);
	}
	matchStatement->set_unsigned32(4, match.serverId);
	match.id = static_cast<unsigned int>(matchStatement->insert());
	return match;
}

void db::DatabaseAPI::StartMatch(unsigned matchId) const
{
	mariadb::statement_ref matchStatement = connection->create_statement("UPDATE `matches` SET `startDate`=NOW() WHERE `id`=?;");
	matchStatement->set_unsigned32(0, matchId);
	matchStatement->execute();
}

void db::DatabaseAPI::EndMatch(unsigned matchId) const
{
	mariadb::statement_ref matchStatement = connection->create_statement("UPDATE `matches` SET `endDate`=NOW() WHERE `id`=?;");
	matchStatement->set_unsigned32(0, matchId);
	matchStatement->execute();
}


const tbsg::Match db::DatabaseAPI::GetNextMatchForServer(unsigned serverId) const
{
	mariadb::statement_ref matchStatement = connection->create_statement("SELECT * FROM `matches` WHERE `matches`.`startDate` IS NULL  AND `matches`.`endDate` IS NULL AND `matches`.`serverId`=? LIMIT 1");
	matchStatement->set_unsigned32(0, serverId);
	mariadb::result_set_ref matchResult = matchStatement->query();

	tbsg::Match match{};

	if (matchResult->row_count() <= 0)
	{
		match.id = 0;
		return match;
	}

	matchResult->next();

	match.id = matchResult->get_unsigned32("id");
	match.ownerProfileId = matchResult->get_unsigned32("ownerProfileId");
	match.ownerDeckId = matchResult->get_unsigned32("ownerDeckId");
	if(matchResult->get_is_null("opponentProfileId"))
	{
		match.opponentProfileId = 0;
	}
	else
	{
		match.opponentProfileId = matchResult->get_unsigned32("opponentProfileId");
	}
	if(matchResult->get_is_null("opponentDeckId"))
	{
		match.opponentDeckId = 0;
	}
	else
	{
		match.opponentDeckId = matchResult->get_unsigned32("opponentDeckId");
	}
	match.serverId = matchResult->get_unsigned32("serverId");

	return match;
}

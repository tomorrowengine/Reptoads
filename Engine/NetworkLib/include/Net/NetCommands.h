#pragma once


enum class NetCommands
{
	
	Identify,
	
	IdentifySuccessful,
	
	IdentifyFailure,
	
	NotIdentified,

	
	ConnectedWithoutEncryption,

	HandshakeServerKey,
	HandshakeDataKey,
	HandshakeSuccess,
	HandshakeFailed,

	CryptoPacket,
    
	CustomCommand
};

inline std::string GetName(NetCommands command)
{
	switch(command)
	{
		case NetCommands::Identify: return "Identify";
		case NetCommands::IdentifySuccessful: return "IdentifySuccessful";
		case NetCommands::IdentifyFailure: return "IdentifyFailure";
		case NetCommands::NotIdentified: return "NotIdentified";
		case NetCommands::ConnectedWithoutEncryption: return "ConnectedWithoutEncryption";
		case NetCommands::HandshakeServerKey: return "HandshakeServerKey";
		case NetCommands::HandshakeDataKey: return "HandshakeDataKey";
		case NetCommands::HandshakeSuccess: return "HandshakeSuccess";
		case NetCommands::HandshakeFailed: return "HandshakeFailed";
		case NetCommands::CryptoPacket: return "CryptoPacket";
		case NetCommands::CustomCommand: return "CustomCommand";
	}
	return "Unknown";
}

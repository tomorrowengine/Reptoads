#pragma once

#include "Crypto/NetRSA.h"
#include "Crypto/NetAES.h"

namespace net
{
	struct KeyChain
	{
		NetRSA handshakeKey;
		NetAES dataKey;
	};

	inline KeyChain EmptyKeyChain()
	{
		return KeyChain{
				{
					{}, 
					{}  
				},
				{
					{ {}, {} } 
				},
		};
	}

}

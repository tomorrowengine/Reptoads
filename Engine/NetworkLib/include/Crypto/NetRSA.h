#pragma once

#include <string>
#include <stdio.h>
#include <cstring>
#include <stdlib.h> 
#include <memory>

namespace net
{

	struct NetRSAKey
	{
		size_t bitSize;
		size_t modBitSize;
		size_t crtBitSize;

		std::shared_ptr<unsigned char> modulus;
		std::shared_ptr<unsigned char> exponent;
		std::shared_ptr<unsigned char> dmp1;
		std::shared_ptr<unsigned char> dmq1;
		std::shared_ptr<unsigned char> iqmp;
	};

	class NetRSA
	{
	public:
		NetRSA();
		NetRSA(NetRSAKey privateKey, NetRSAKey publicKey);

		const NetRSAKey& GetPublicKey() const noexcept { return this->publicKey; };
		const NetRSAKey& GetPrivateKey() const noexcept { return this->privateKey; };

		static void GenerateKeyPair(const int keySize, NetRSAKey& publicKey, NetRSAKey& privateKey);

		void Encrypt(const unsigned char* data, const size_t dataSize, std::unique_ptr<unsigned char[]>& encryptedData, size_t& encryptedDataSize);
		void Decrypt(const unsigned char* data, const size_t dataSize, std::unique_ptr<unsigned char[]>& decryptedData, size_t& decryptedDataSize);

	private:
		void Equalize(NetRSAKey& key);

		NetRSAKey privateKey{};
		NetRSAKey publicKey{};

		static constexpr int keyLength = 1024; 
	};
}

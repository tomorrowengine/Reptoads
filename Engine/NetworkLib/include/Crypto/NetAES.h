#pragma once

#include <memory>

namespace net
{
	struct NetAESKey
	{
		size_t bitSize;
		std::shared_ptr<unsigned char> key;
	};

	class NetAES
	{
	public:
		NetAES();
		NetAES(NetAESKey key);

		void Encrypt(const unsigned char* data, const size_t dataSize, std::unique_ptr<unsigned char[]>& ivOut, std::unique_ptr<unsigned char[]>& encryptedData, size_t& encryptedDataSize, size_t& paddingSize);
		void Decrypt(const unsigned char* data, const size_t dataSize, unsigned char* ivIn, std::unique_ptr<unsigned char[]>& decryptedData, size_t& decryptedDataSize, size_t paddingSize);

		const NetAESKey& GetKey() const noexcept { return this->key; }

		static NetAESKey GenerateKey(const int keySize);
		static std::unique_ptr<unsigned char[]> GenerateIV(const int ivSize);
		static std::unique_ptr<unsigned char[]> GenerateBytes(const int size);

		static constexpr int keyLength = 256; 
		static constexpr int ivLength = 128; 
		static constexpr int blockSize = 16; 
	private:
		unsigned char* AddPadding(unsigned char* data, const size_t dataSize, size_t& paddingSize);
		unsigned char* RemovePadding(unsigned char* data, const size_t dataSize, size_t paddingSize);

		NetAESKey key{};
	};
}

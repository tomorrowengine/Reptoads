#include "Crypto/NetAES.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <memory>
#include <random>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <climits>
#include <stdio.h>
#include <cstring>

namespace net
{
	NetAES::NetAES()
	{
		this->key = GenerateKey(keyLength);
	}

	NetAES::NetAES(NetAESKey key)
	{
		this->key = key;
	}

	void NetAES::Encrypt(const unsigned char * data, const size_t dataSize, std::unique_ptr<unsigned char[]>& ivOut, std::unique_ptr<unsigned char[]>& encryptedData, size_t & encryptedDataSize, size_t& paddingSize)
	{
		unsigned char* paddedData = new unsigned char[dataSize];
		memcpy(paddedData, data, dataSize);

		paddedData = AddPadding(paddedData, dataSize, paddingSize);
		size_t paddedDataSize = dataSize + paddingSize;

		EVP_CIPHER_CTX *ctx;

		int len;
		int ciphertextLen;

		ctx = EVP_CIPHER_CTX_new();
		if (!ctx)
		{
			ERR_print_errors_fp(stderr);
		}

		ivOut = std::unique_ptr<unsigned char[]>{ GenerateIV(ivLength).release() };

		if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, this->key.key.get(), ivOut.get()) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		EVP_CIPHER_CTX_set_padding(ctx, 0);

		unsigned char* cipherData = new unsigned char[paddedDataSize + 256];

		if (EVP_EncryptUpdate(ctx, cipherData, &len, paddedData, static_cast<int>(paddedDataSize)) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		ciphertextLen = len;

		if (EVP_EncryptFinal_ex(ctx, cipherData + len, &len) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		ciphertextLen += len;

		unsigned char* trimmedCipherData = new unsigned char[ciphertextLen];

		memcpy(trimmedCipherData, cipherData, ciphertextLen);

		delete[] cipherData;

		EVP_CIPHER_CTX_free(ctx);

		encryptedData = std::unique_ptr<unsigned char[]>{ trimmedCipherData };
		encryptedDataSize = ciphertextLen;

		delete[] paddedData;
	}

	void NetAES::Decrypt(const unsigned char * data, const size_t dataSize, unsigned char* ivIn, std::unique_ptr<unsigned char[]>& decryptedData, size_t & decryptedDataSize, size_t paddingSize)
	{
		EVP_CIPHER_CTX *ctx;

		int len;
		int decryptedLen;

		ctx = EVP_CIPHER_CTX_new();
		if (!ctx)
		{
			ERR_print_errors_fp(stderr);
		}

		if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, this->key.key.get(), ivIn) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		EVP_CIPHER_CTX_set_padding(ctx, 0);

		unsigned char* decrypted = new unsigned char[dataSize + 256];

		if (EVP_DecryptUpdate(ctx, decrypted, &len, data, static_cast<int>(dataSize)) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		decryptedLen = len;

		if (EVP_DecryptFinal_ex(ctx, decrypted + len, &len) != 1)
		{
			ERR_print_errors_fp(stderr);
		}

		decryptedLen += len;

		unsigned char* trimmedDecryptedData = new unsigned char[decryptedLen];

		memcpy(trimmedDecryptedData, decrypted, decryptedLen);

		delete[] decrypted;

		EVP_CIPHER_CTX_free(ctx);

		trimmedDecryptedData = RemovePadding(trimmedDecryptedData, decryptedLen, paddingSize);
		decryptedData = std::unique_ptr<unsigned char[]>{ trimmedDecryptedData };
		decryptedDataSize = decryptedLen - paddingSize;
	}

	NetAESKey NetAES::GenerateKey(const int keySize)
	{
		return NetAESKey{ static_cast<size_t>(keySize), std::shared_ptr<unsigned char>{ GenerateBytes(keySize >> 3).release(), [](unsigned char *p) { delete[] p; } } };
	}

	std::unique_ptr<unsigned char[]> NetAES::GenerateIV(const int ivSize)
	{
		return GenerateBytes(ivSize >> 3);
	}

	std::unique_ptr<unsigned char[]> NetAES::GenerateBytes(const int size)
	{
		std::random_device r;
		std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		std::mt19937 gen(seed);
		std::uniform_int_distribution<> dis(0, UCHAR_MAX);

		auto rand = [&gen, &dis]() { return static_cast<unsigned char>(dis(gen)); };

		std::unique_ptr<unsigned char[]> key = std::unique_ptr<unsigned char[]>{ new unsigned char[size] };

		std::generate_n(key.get(), static_cast<size_t>(size), rand);

		return key;
	}

	unsigned char* NetAES::AddPadding(unsigned char* data, const size_t dataSize, size_t& paddingSize)
	{
		int remaining = blockSize - (dataSize % blockSize);

		if (remaining < blockSize)
		{
			unsigned char* newData = new unsigned char[dataSize + remaining];
			memcpy(newData, data, dataSize);

			auto remainderBytes = GenerateBytes(remaining);
			memcpy(&newData[dataSize], remainderBytes.get(), remaining);

			paddingSize = static_cast<size_t>(remaining);
			delete[] data;
			data = newData;
		}
		else
		{
			paddingSize = 0;
		}

		return data;
	}

	unsigned char* NetAES::RemovePadding(unsigned char * data, const size_t dataSize, size_t paddingSize)
	{
		if (paddingSize > 0)
		{
			unsigned char* newData = new unsigned char[dataSize - paddingSize];
			memcpy(newData, data, dataSize - paddingSize);
			delete[] data;
			data = newData;
		}

		return data;
	}
}

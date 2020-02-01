#include "Crypto/NetRSA.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/evperr.h>

#include <assert.h>

#include <vector>

namespace net
{
	NetRSA::NetRSA()
	{
		GenerateKeyPair(keyLength, publicKey, privateKey);
	}

	NetRSA::NetRSA(NetRSAKey privateKey, NetRSAKey publicKey)
	{
		this->privateKey = privateKey;
		this->publicKey = publicKey;

		Equalize(this->publicKey);
		Equalize(this->privateKey);

	}

	void NetRSA::GenerateKeyPair(const int keySize, NetRSAKey & publicKey, NetRSAKey & privateKey)
	{
		EVP_PKEY *pkey;
		pkey = EVP_PKEY_new();

		BIGNUM *bn;
		bn = BN_new();
		BN_set_word(bn, RSA_F4);

		RSA *rsa;
		rsa = RSA_new();

		RSA_generate_key_ex(
			rsa,
			keySize,
			bn,
			nullptr
		);

		EVP_PKEY_assign_RSA(pkey, rsa);
		const BIGNUM* modulus = RSA_get0_n(rsa);
		auto ms = BN_num_bytes(modulus);
		std::shared_ptr<unsigned char> modulusBuffer = std::shared_ptr<unsigned char>{ new unsigned char[ms * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(modulus, modulusBuffer.get());

		const BIGNUM* publicExponent = RSA_get0_e(rsa);
		auto ps = BN_num_bytes(publicExponent);
		std::shared_ptr<unsigned char> publicExponentBuffer = std::shared_ptr<unsigned char>{ new unsigned char[ps * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(publicExponent, publicExponentBuffer.get());

		const BIGNUM* privateExponent = RSA_get0_d(rsa);
		auto pvs = BN_num_bytes(privateExponent);

		std::shared_ptr<unsigned char> privateExponentBuffer = std::shared_ptr<unsigned char>{ new unsigned char[pvs * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(privateExponent, privateExponentBuffer.get());

		

		const BIGNUM* dmp1 = RSA_get0_dmp1(rsa);
		auto dmps = BN_num_bytes(dmp1);
		std::shared_ptr<unsigned char> dmp1Buffer = std::shared_ptr<unsigned char>{ new unsigned char[dmps * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(dmp1, dmp1Buffer.get());

		const BIGNUM* dmq1 = RSA_get0_dmq1(rsa);
		auto dmqs = BN_num_bytes(dmq1);
		std::shared_ptr<unsigned char> dmq1Buffer = std::shared_ptr<unsigned char>{ new unsigned char[dmqs * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(dmq1, dmq1Buffer.get());

		const BIGNUM* iqmp = RSA_get0_iqmp(rsa);
		auto iqms = BN_num_bytes(iqmp);
		std::shared_ptr<unsigned char> iqmpBuffer = std::shared_ptr<unsigned char>{ new unsigned char[iqms * sizeof(unsigned char)], [](unsigned char *p) { delete[] p; } };
		BN_bn2bin(iqmp, iqmpBuffer.get());

		publicKey.bitSize = ps << 3;
		publicKey.modBitSize = ms << 3;
		publicKey.crtBitSize = dmqs << 3;
		publicKey.modulus = modulusBuffer;
		publicKey.exponent = publicExponentBuffer;
		publicKey.dmp1 = dmp1Buffer;
		publicKey.dmq1 = dmq1Buffer;
		publicKey.iqmp = iqmpBuffer;

		privateKey.bitSize = pvs << 3;
		privateKey.modBitSize = ms << 3;
		privateKey.crtBitSize = dmqs << 3;
		privateKey.modulus = modulusBuffer;
		privateKey.exponent = privateExponentBuffer;
		privateKey.dmp1 = dmp1Buffer;
		privateKey.dmq1 = dmq1Buffer;
		privateKey.iqmp = iqmpBuffer;

		EVP_PKEY_free(pkey);

	}

	void NetRSA::Encrypt(const unsigned char * data, const size_t dataSize, std::unique_ptr<unsigned char[]>& encryptedData, size_t& encryptedDataSize)
	{
		RSA* key = RSA_new();

		RSA_set0_key(
			key,
			BN_bin2bn(static_cast<unsigned char*>(publicKey.modulus.get()), static_cast<int>(publicKey.modBitSize >> 3), nullptr),
			BN_bin2bn(static_cast<unsigned char*>(publicKey.exponent.get()), static_cast<int>(publicKey.bitSize >> 3), nullptr),
			nullptr
		);

		unsigned char* eData = new unsigned char[keyLength];

		int len = RSA_public_encrypt(static_cast<int>(dataSize * sizeof(unsigned char)), data, eData, key, RSA_PKCS1_PADDING);
		if (len == -1)
		{
			ERR_print_errors_fp(stderr);
		}

		encryptedData = std::unique_ptr<unsigned char[]>{ eData };
		encryptedDataSize = static_cast<size_t>(len);

		RSA_free(key);
	}

	void NetRSA::Decrypt(const unsigned char * data, const size_t dataSize, std::unique_ptr<unsigned char[]>& decryptedData, size_t& decryptedDataSize)
	{
		RSA* key = RSA_new();

		RSA_set0_key(
			key,
			BN_bin2bn(static_cast<unsigned char*>(publicKey.modulus.get()), static_cast<int>(publicKey.modBitSize >> 3), nullptr),
			BN_bin2bn(static_cast<unsigned char*>(publicKey.exponent.get()), static_cast<int>(publicKey.bitSize >> 3), nullptr),
			BN_bin2bn(static_cast<unsigned char*>(privateKey.exponent.get()), static_cast<int>(privateKey.bitSize >> 3), nullptr)
		);

		RSA_set0_crt_params(
			key,
			BN_bin2bn(static_cast<unsigned char*>(publicKey.dmp1.get()), static_cast<int>(publicKey.crtBitSize >> 3), nullptr),
			BN_bin2bn(static_cast<unsigned char*>(publicKey.dmq1.get()), static_cast<int>(publicKey.crtBitSize >> 3), nullptr),
			BN_bin2bn(static_cast<unsigned char*>(publicKey.iqmp.get()), static_cast<int>(publicKey.crtBitSize >> 3), nullptr)
		);

		unsigned char* deData = new unsigned char[dataSize];

		int len = RSA_private_decrypt(static_cast<int>(dataSize * sizeof(unsigned char)), data, deData, key, RSA_PKCS1_PADDING);
		if (len == -1)
		{
			ERR_print_errors_fp(stderr);
		}

		unsigned char* trimmedData = new unsigned char[len];
		memcpy(trimmedData, deData, len);

		delete[] deData;

		decryptedData = std::unique_ptr<unsigned char[]>{ trimmedData };
		decryptedDataSize = static_cast<size_t>(len);

		RSA_free(key);
	}

	void NetRSA::Equalize(NetRSAKey&)
	{
	}
}

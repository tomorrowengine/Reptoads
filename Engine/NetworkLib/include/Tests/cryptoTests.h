#pragma once

#include "catch/catch.hpp"
#include "Crypto/NetRSA.h"
#include "Crypto/NetAES.h"
#include <memory>


TEST_CASE("RSA Generate, Encrypt Decrypt", "[crypto]")
{
	auto rsa = net::NetRSA();
	std::string data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";

	std::unique_ptr<unsigned char[]> encryptedData;
	size_t encryptedDataSize;

	printf("RSA Encrypt Decrypt: \nPlain: \n");
	for (unsigned int i = 0; i < data.length(); i++)
	{
		printf("%c", data[i]);
	}

	rsa.Encrypt((unsigned char*)& data[0], data.length(), encryptedData, encryptedDataSize);

	printf("\n\nCipher: \n");
	for (unsigned int i = 0; i < encryptedDataSize; i++)
	{
		printf("%c", encryptedData.get()[i]);
	}

	std::unique_ptr<unsigned char[]> decryptedData;
	size_t decryptedDataSize;
	rsa.Decrypt(encryptedData.get(), encryptedDataSize, decryptedData, decryptedDataSize);

	std::string output{};

	printf("\n\nDecrypted: \n");
	for (unsigned int i = 0; i < decryptedDataSize; i++)
	{
		printf("%c", decryptedData.get()[i]);
		output.push_back(decryptedData.get()[i]);
	}
	printf("\n\n");


	REQUIRE(data.compare(output) == 0);
}

TEST_CASE("AES Generate, Encrypt Decrypt", "[crypto]")
{
	auto aes = net::NetAES();
	std::string data = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

	std::unique_ptr<unsigned char[]> encryptedData;
	size_t encryptedDataSize;
	
	std::unique_ptr<unsigned char[]> ivData;
	size_t paddingSize;

	printf("AES Encrypt Decrypt: \nPlain: \n");
	for (unsigned int i = 0; i < data.length(); i++)
	{
		printf("%c", data[i]);
	}

	aes.Encrypt((unsigned char*)& data[0], data.length(), ivData, encryptedData, encryptedDataSize, paddingSize);

	printf("\n\nCipher: \n");
	for (unsigned int i = 0; i < encryptedDataSize; i++)
	{
		printf("%c", encryptedData.get()[i]);
	}

	std::unique_ptr<unsigned char[]> decryptedData;
	size_t decryptedDataSize;
	aes.Decrypt(encryptedData.get(), encryptedDataSize, ivData.get(), decryptedData, decryptedDataSize, paddingSize);

	std::string output{};

	printf("\n\nDecrypted: \n");
	for (unsigned int i = 0; i < decryptedDataSize; i++)
	{
		printf("%c", decryptedData.get()[i]);
		output.push_back(decryptedData.get()[i]);
	}
	printf("\n\n");


	REQUIRE(data.compare(output) == 0);
}
#ifndef RIJNDAEL_INCLUDED
#define RIJNDAEL_INCLUDED






#define AES_MAXKC	(256/32)
#define AES_MAXKB	(256/8)
#define AES_MAXNR	14

int rijndaelKeySetupEnc(uint32 rk[], const uint8 cipherKey[],
			int keyBits);
int rijndaelKeySetupDec(uint32 rk[], const uint8 cipherKey[],
			int keyBits);
void rijndaelEncrypt(const uint32 rk[], int Nr,
		     const uint8 pt[16], uint8 ct[16]);
void rijndaelDecrypt(const uint32 rk[], int Nr,
		     const uint8 ct[16], uint8 pt[16]);

#endif 

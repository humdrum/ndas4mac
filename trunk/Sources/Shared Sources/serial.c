/***************************************************************************
 *
 *  serial.c
 *
 *    
 *
 *    Copyright (c) 2003 XiMeta, Inc. All rights reserved.
 *
 **************************************************************************/

#include	"serial.h"
#include	"des.h"
#include	"crc.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>

#ifdef WIN32
#include	<pshpack1.h>
#endif
struct _SERIAL_BUFF {
	union {
		unsigned char		ucSerialBuff[12];
		struct {
			unsigned char	ucAd0;
			unsigned char	ucVid;
			unsigned char	ucAd1;
			unsigned char	ucAd2;
			unsigned char	ucReserved0;
			unsigned char	ucAd3;
			unsigned char	ucAd4;
			unsigned char	ucReserved1;
			unsigned char	ucAd5;
			unsigned char	ucRandom;
			unsigned char	ucCRC[2];
		};
	};
}
#ifdef WIN32
;
#include	<poppack.h>
#else
__attribute__((packed));
#endif

typedef struct _SERIAL_BUFF	*PSERIAL_BUFF, SERIAL_BUFF;

void			Bin2Serial(unsigned char *bin, unsigned char *write_key, PSERIAL_INFO pInfo);
void			Serial2Bin(PSERIAL_INFO pInfo, unsigned char *bin, unsigned char *write_key);
unsigned char	Bin2Char(unsigned char c);
unsigned char	Char2Bin(unsigned char c);

BOOL EncryptSerial(PSERIAL_INFO pInfo)
{
	SERIAL_BUFF		encrypted1, encrypted2;
	unsigned short	crc;
	unsigned long	des_key[32];
	unsigned char	write_key[3];

	if(!pInfo) return(FALSE);

	srand(time(0)); rand(); rand();

	encrypted1.ucAd0 = pInfo->ucAddr[0];
	encrypted1.ucAd1 = pInfo->ucAddr[1];
	encrypted1.ucAd2 = pInfo->ucAddr[2];
	encrypted1.ucAd3 = pInfo->ucAddr[3];
	encrypted1.ucAd4 = pInfo->ucAddr[4];
	encrypted1.ucAd5 = pInfo->ucAddr[5];
	encrypted1.ucVid = pInfo->ucVid;
	encrypted1.ucReserved0 = pInfo->reserved[0];
	encrypted1.ucReserved1 = pInfo->reserved[1];
	encrypted1.ucRandom = pInfo->ucRandom = pInfo->ucRandom ? pInfo->ucRandom : (unsigned char) rand();
	crc = (unsigned short) CRC_calculate(encrypted1.ucSerialBuff, 10);
//	printf("Encrypt : %d\n", crc);
	encrypted1.ucCRC[0] = (char) (crc & 0xff);
	encrypted1.ucCRC[1] = (char) ((crc & 0xff00) >> 8);

	// 1st step
	des_ky(pInfo->ucKey1, des_key);
	des_ec((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[ 8] = encrypted2.ucSerialBuff[0];
	encrypted2.ucSerialBuff[ 9] = encrypted2.ucSerialBuff[2];
	encrypted2.ucSerialBuff[10] = encrypted2.ucSerialBuff[4];
	encrypted2.ucSerialBuff[11] = encrypted2.ucSerialBuff[6];

	encrypted2.ucSerialBuff[0] = encrypted1.ucSerialBuff[ 8];
	encrypted2.ucSerialBuff[2] = encrypted1.ucSerialBuff[ 9];
	encrypted2.ucSerialBuff[4] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[6] = encrypted1.ucSerialBuff[11];

	// 2nd step
	des_ky(pInfo->ucKey2, des_key);
	des_dc((void *) encrypted2.ucSerialBuff, (void *) encrypted1.ucSerialBuff, (void *) des_key);

	encrypted1.ucSerialBuff[ 8] = encrypted1.ucSerialBuff[1];
	encrypted1.ucSerialBuff[ 9] = encrypted1.ucSerialBuff[3];
	encrypted1.ucSerialBuff[10] = encrypted1.ucSerialBuff[5];
	encrypted1.ucSerialBuff[11] = encrypted1.ucSerialBuff[7];

	encrypted1.ucSerialBuff[1] = encrypted2.ucSerialBuff[8];
	encrypted1.ucSerialBuff[3] = encrypted2.ucSerialBuff[9];
	encrypted1.ucSerialBuff[5] = encrypted2.ucSerialBuff[10];
	encrypted1.ucSerialBuff[7] = encrypted2.ucSerialBuff[11];

	write_key[0] = encrypted1.ucSerialBuff[2];
	write_key[1] = encrypted1.ucSerialBuff[4];
	write_key[2] = encrypted1.ucSerialBuff[6];

	// 3rd step
	des_ky(pInfo->ucKey1, des_key);
	des_ec((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[8]  = encrypted1.ucSerialBuff[8];
	encrypted2.ucSerialBuff[9]  = encrypted1.ucSerialBuff[9];
	encrypted2.ucSerialBuff[10] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[11] = encrypted1.ucSerialBuff[11];

	// Encryption Finished !!
	// Now, Convert encrypted password to Serial Key Form

	Bin2Serial(encrypted2.ucSerialBuff, write_key, pInfo);

	return(TRUE);
}

BOOL DecryptSerial(PSERIAL_INFO pInfo)
{
	SERIAL_BUFF		encrypted1, encrypted2;
	unsigned short	crc;
	unsigned long	des_key[32];
	unsigned char	write_key[3], cwrite_key[3];

	if(!pInfo) return(FALSE);

	Serial2Bin(pInfo, encrypted1.ucSerialBuff, write_key);

	// reverse 3rd step
	des_ky(pInfo->ucKey1, des_key);
	des_dc((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[ 8] = encrypted2.ucSerialBuff[1];
	encrypted2.ucSerialBuff[ 9] = encrypted2.ucSerialBuff[3];
	encrypted2.ucSerialBuff[10] = encrypted2.ucSerialBuff[5];
	encrypted2.ucSerialBuff[11] = encrypted2.ucSerialBuff[7];

	encrypted2.ucSerialBuff[1] = encrypted1.ucSerialBuff[ 8];
	encrypted2.ucSerialBuff[3] = encrypted1.ucSerialBuff[ 9];
	encrypted2.ucSerialBuff[5] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[7] = encrypted1.ucSerialBuff[11];

	// save password decrypted for later comparison
	cwrite_key[0] = encrypted2.ucSerialBuff[2];
	cwrite_key[1] = encrypted2.ucSerialBuff[4];
	cwrite_key[2] = encrypted2.ucSerialBuff[6];


	// reverse 2nd step
	des_ky(pInfo->ucKey2, des_key);
	des_ec((void *) encrypted2.ucSerialBuff, (void *) encrypted1.ucSerialBuff, (void *) des_key);

	encrypted1.ucSerialBuff[ 8] = encrypted1.ucSerialBuff[0];
	encrypted1.ucSerialBuff[ 9] = encrypted1.ucSerialBuff[2];
	encrypted1.ucSerialBuff[10] = encrypted1.ucSerialBuff[4];
	encrypted1.ucSerialBuff[11] = encrypted1.ucSerialBuff[6];

	encrypted1.ucSerialBuff[0] = encrypted2.ucSerialBuff[ 8];
	encrypted1.ucSerialBuff[2] = encrypted2.ucSerialBuff[ 9];
	encrypted1.ucSerialBuff[4] = encrypted2.ucSerialBuff[10];
	encrypted1.ucSerialBuff[6] = encrypted2.ucSerialBuff[11];

	// reverse 1st step
	des_ky(pInfo->ucKey1, des_key);
	des_dc((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[ 8] = encrypted1.ucSerialBuff[ 8];
	encrypted2.ucSerialBuff[ 9] = encrypted1.ucSerialBuff[ 9];
	encrypted2.ucSerialBuff[10] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[11] = encrypted1.ucSerialBuff[11];

	// decryption Finished !!
	// Now, compare checksum
	// if checksum is mismatch, then invalid serial
	crc = (unsigned short) CRC_calculate(encrypted2.ucSerialBuff, 10);
//	printf("Encrypt : %d\n", crc);
	if( (encrypted2.ucCRC[0] != (crc & 0xff)) || (encrypted2.ucCRC[1] != ((crc & 0xff00) >> 8)) )

		return(FALSE);

//	printf("Valid Serial\n");
	// ok, it's valid serial
	// Now, move retrieved eth addr, vid, reserved words to pInfo and return
	pInfo->ucAddr[0] = encrypted2.ucAd0;
	pInfo->ucAddr[1] = encrypted2.ucAd1;
	pInfo->ucAddr[2] = encrypted2.ucAd2;
	pInfo->ucAddr[3] = encrypted2.ucAd3;
	pInfo->ucAddr[4] = encrypted2.ucAd4;
	pInfo->ucAddr[5] = encrypted2.ucAd5;

	pInfo->ucVid = encrypted2.ucVid;

	pInfo->reserved[0] = encrypted2.ucReserved0;
	pInfo->reserved[1] = encrypted2.ucReserved1;

	pInfo->ucRandom = encrypted2.ucRandom;

	pInfo->bIsReadWrite =	(cwrite_key[0] == write_key[0]) &&
				(cwrite_key[1] == write_key[1]) &&
				(cwrite_key[2] == write_key[2]);
	return(TRUE);
}

#if 0
BOOL DecryptSerial(PSERIAL_INFO pInfo)
{
	SERIAL_BUFF		encrypted1, encrypted2;
	unsigned short	crc;
	unsigned long	des_key[32];
	unsigned char	write_key[3], cwrite_key[3];

	if(!pInfo) return(FALSE);

	Serial2Bin(pInfo, encrypted1.ucSerialBuff, write_key);

	// reverse 3rd step
	des_ky(pInfo->ucKey1, des_key);
	des_dc((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[ 8] = encrypted2.ucSerialBuff[1];
	encrypted2.ucSerialBuff[ 9] = encrypted2.ucSerialBuff[3];
	encrypted2.ucSerialBuff[10] = encrypted2.ucSerialBuff[5];
	encrypted2.ucSerialBuff[11] = encrypted2.ucSerialBuff[7];

	encrypted2.ucSerialBuff[1] = encrypted1.ucSerialBuff[ 8];
	encrypted2.ucSerialBuff[3] = encrypted1.ucSerialBuff[ 9];
	encrypted2.ucSerialBuff[5] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[7] = encrypted1.ucSerialBuff[11];

	// save password decrypted for later comparison
	cwrite_key[0] = encrypted2.ucSerialBuff[2];
	cwrite_key[1] = encrypted2.ucSerialBuff[4];
	cwrite_key[2] = encrypted2.ucSerialBuff[6];


	// reverse 2nd step
	des_ky(pInfo->ucKey2, des_key);
	des_ec((void *) encrypted2.ucSerialBuff, (void *) encrypted1.ucSerialBuff, (void *) des_key);

	encrypted1.ucSerialBuff[ 8] = encrypted1.ucSerialBuff[0];
	encrypted1.ucSerialBuff[ 9] = encrypted1.ucSerialBuff[2];
	encrypted1.ucSerialBuff[10] = encrypted1.ucSerialBuff[4];
	encrypted1.ucSerialBuff[11] = encrypted1.ucSerialBuff[6];

	encrypted1.ucSerialBuff[0] = encrypted2.ucSerialBuff[ 8];
	encrypted1.ucSerialBuff[2] = encrypted2.ucSerialBuff[ 9];
	encrypted1.ucSerialBuff[4] = encrypted2.ucSerialBuff[10];
	encrypted1.ucSerialBuff[6] = encrypted2.ucSerialBuff[11];

	// reverse 1st step
	des_ky(pInfo->ucKey1, des_key);
	des_dc((void *) encrypted1.ucSerialBuff, (void *) encrypted2.ucSerialBuff, (void *) des_key);

	encrypted2.ucSerialBuff[ 8] = encrypted1.ucSerialBuff[ 8];
	encrypted2.ucSerialBuff[ 9] = encrypted1.ucSerialBuff[ 9];
	encrypted2.ucSerialBuff[10] = encrypted1.ucSerialBuff[10];
	encrypted2.ucSerialBuff[11] = encrypted1.ucSerialBuff[11];

	// decryption Finished !!
	// Now, compare checksum
	// if checksum is mismatch, then invalid serial
	crc = (unsigned short) CRC_calculate(encrypted2.ucSerialBuff, 10);
//	printf("Encrypt : %d\n", crc);
	if( (encrypted2.ucCRC[0] != (crc & 0xff)) || (encrypted2.ucCRC[1] != ((crc & 0xff00) >> 8)) )
		return(FALSE);

//	printf("Valid Serial\n");
	// ok, it's valid serial
	// Now, move retrieved eth addr, vid, reserved words to pInfo and return
	pInfo->ucAddr[0] = encrypted2.ucAd0;
	pInfo->ucAddr[1] = encrypted2.ucAd1;
	pInfo->ucAddr[2] = encrypted2.ucAd2;
	pInfo->ucAddr[3] = encrypted2.ucAd3;
	pInfo->ucAddr[4] = encrypted2.ucAd4;
	pInfo->ucAddr[5] = encrypted2.ucAd5;

	pInfo->ucVid = encrypted2.ucVid;

	pInfo->reserved[0] = encrypted2.ucReserved0;
	pInfo->reserved[1] = encrypted2.ucReserved1;

	pInfo->ucRandom = encrypted2.ucRandom;

	pInfo->bIsReadWrite =	(cwrite_key[0] == write_key[0]) &&
				(cwrite_key[1] == write_key[1]) &&
				(cwrite_key[2] == write_key[2]);
	return(TRUE);
}
#endif

void Bin2Serial(unsigned char *bin, unsigned char *write_key, PSERIAL_INFO pInfo)
{
	u_int32_t		uiTemp;

	// first 3 bytes => first five serial key
	uiTemp  = bin[0];	uiTemp <<= 8;
	uiTemp |= bin[1];	uiTemp <<= 8;
	uiTemp |= bin[2];	uiTemp <<= 1;

	pInfo->ucSN[0][0] = Bin2Char((unsigned char) (uiTemp >> 20) & (0x1f));
	pInfo->ucSN[0][1] = Bin2Char((unsigned char) (uiTemp >> 15) & (0x1f));
	pInfo->ucSN[0][2] = Bin2Char((unsigned char) (uiTemp >> 10) & (0x1f));
	pInfo->ucSN[0][3] = Bin2Char((unsigned char) (uiTemp >>  5) & (0x1f));
	pInfo->ucSN[0][4] = Bin2Char((unsigned char) (uiTemp)       & (0x1f));

	// second 3 bytes => second five serial key
	uiTemp  = bin[3];	uiTemp <<= 8;
	uiTemp |= bin[4];	uiTemp <<= 8;
	uiTemp |= bin[5];	uiTemp <<= 1;

	pInfo->ucSN[1][0] = Bin2Char((unsigned char) (uiTemp >> 20) & (0x1f));
	pInfo->ucSN[1][1] = Bin2Char((unsigned char) (uiTemp >> 15) & (0x1f));
	pInfo->ucSN[1][2] = Bin2Char((unsigned char) (uiTemp >> 10) & (0x1f));
	pInfo->ucSN[1][3] = Bin2Char((unsigned char) (uiTemp >>  5) & (0x1f));
	pInfo->ucSN[1][4] = Bin2Char((unsigned char) (uiTemp)       & (0x1f));

	// third 3 bytes => third five serial key
	uiTemp  = bin[6];	uiTemp <<= 8;
	uiTemp |= bin[7];	uiTemp <<= 8;
	uiTemp |= bin[8];	uiTemp <<= 1;

	pInfo->ucSN[2][0] = Bin2Char((unsigned char) (uiTemp >> 20) & (0x1f));
	pInfo->ucSN[2][1] = Bin2Char((unsigned char) (uiTemp >> 15) & (0x1f));
	pInfo->ucSN[2][2] = Bin2Char((unsigned char) (uiTemp >> 10) & (0x1f));
	pInfo->ucSN[2][3] = Bin2Char((unsigned char) (uiTemp >>  5) & (0x1f));
	pInfo->ucSN[2][4] = Bin2Char((unsigned char) (uiTemp)       & (0x1f));

	// fourth 3 bytes => fourth five serial key
	uiTemp  = bin[ 9];	uiTemp <<= 8;
	uiTemp |= bin[10];	uiTemp <<= 8;
	uiTemp |= bin[11];	uiTemp <<= 1;

	pInfo->ucSN[3][0] = Bin2Char((unsigned char) (uiTemp >> 20) & (0x1f));
	pInfo->ucSN[3][1] = Bin2Char((unsigned char) (uiTemp >> 15) & (0x1f));
	pInfo->ucSN[3][2] = Bin2Char((unsigned char) (uiTemp >> 10) & (0x1f));
	pInfo->ucSN[3][3] = Bin2Char((unsigned char) (uiTemp >>  5) & (0x1f));
	pInfo->ucSN[3][4] = Bin2Char((unsigned char) (uiTemp)       & (0x1f));
	
	// three password bytes => 5 password characters
	uiTemp  = write_key[0];	uiTemp <<= 8;
	uiTemp |= write_key[1];	uiTemp <<= 8;
	uiTemp |= write_key[2];	uiTemp <<= 1;

	pInfo->ucWKey[0] = Bin2Char((unsigned char) (uiTemp >> 20) & (0x1f));
	pInfo->ucWKey[1] = Bin2Char((unsigned char) (uiTemp >> 15) & (0x1f));
	pInfo->ucWKey[2] = Bin2Char((unsigned char) (uiTemp >> 10) & (0x1f));
	pInfo->ucWKey[3] = Bin2Char((unsigned char) (uiTemp >>  5) & (0x1f));
	pInfo->ucWKey[4] = Bin2Char((unsigned char) (uiTemp)       & (0x1f));
}

void Serial2Bin(PSERIAL_INFO pInfo, unsigned char *bin, unsigned char *write_key)
{
	u_int32_t		uiTemp;
	int			i;

	uiTemp = 0;
	for(i = 0; i < 5; i++) {
		uiTemp <<= 5;
		uiTemp |= Char2Bin(pInfo->ucSN[0][i]);
	}
	uiTemp >>= 1;
	bin[0] = (unsigned char) (uiTemp >> 16);
	bin[1] = (unsigned char) (uiTemp >> 8);
	bin[2] = (unsigned char) (uiTemp);

	uiTemp = 0;
	for(i = 0; i < 5; i++) {
		uiTemp <<= 5;
		uiTemp |= Char2Bin(pInfo->ucSN[1][i]);
	}
	uiTemp >>= 1;
	bin[3] = (unsigned char) (uiTemp >> 16);
	bin[4] = (unsigned char) (uiTemp >> 8);
	bin[5] = (unsigned char) (uiTemp);

	uiTemp = 0;
	for(i = 0; i < 5; i++) {
		uiTemp <<= 5;
		uiTemp |= Char2Bin(pInfo->ucSN[2][i]);
	}
	uiTemp >>= 1;
	bin[6] = (unsigned char) (uiTemp >> 16);
	bin[7] = (unsigned char) (uiTemp >> 8);
	bin[8] = (unsigned char) (uiTemp);

	uiTemp = 0;
	for(i = 0; i < 5; i++) {
		uiTemp <<= 5;
		uiTemp |= Char2Bin(pInfo->ucSN[3][i]);
	}
	uiTemp >>= 1;
	bin[9] = (unsigned char) (uiTemp >> 16);
	bin[10] = (unsigned char) (uiTemp >> 8);
	bin[11] = (unsigned char) (uiTemp);

	uiTemp = 0;
	for(i = 0; i < 5; i++) {
		uiTemp <<= 5;
		uiTemp |= Char2Bin(pInfo->ucWKey[i]);
	}
	uiTemp >>= 1;
	write_key[0] = (unsigned char) (uiTemp >> 16);
	write_key[1] = (unsigned char) (uiTemp >> 8);
	write_key[2] = (unsigned char) (uiTemp);
}

unsigned char Bin2Char(unsigned char c)
{
	char	ch;

	ch = (c >= 10) ? c - 10 + 'A' : c + '0';
	ch = (ch == 'I') ? 'X' : ch;
	ch = (ch == 'O') ? 'Y' : ch;
	return(ch);
}

unsigned char Char2Bin(unsigned char c)
{
	c = ((c >= 'a') && (c <= 'z')) ? (c - 'a' + 'A') : c;
	c = (c == 'X') ? 'I' : c;
	c = (c == 'Y') ? 'O' : c;
	return( ((c >= 'A') && (c <= 'Z')) ? c - 'A' + 10 : c - '0' );
}

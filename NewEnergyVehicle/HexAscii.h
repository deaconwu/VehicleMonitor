#pragma once

int hex2char(unsigned char c)
{
// 	return ((c >= '0') && (c <= '9')) ? int(c - '0') :
// 		((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
// 		((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
// 		-1;

	if (c < 10)
	{
		return c + '0';
	}
	else
	{
		return c + 'A';
	}
}

int Hex2Ascii(unsigned char* hex, char* ascii)
{
	size_t hexLen = strlen((char*)hex);
	int asciiLen = 0;

	for (size_t i = 0; i < hexLen; i++)
	{
		unsigned char chHigh = hex[i] >> 4;
		char c = hex2char(chHigh);
		ascii[asciiLen++] = c;

		unsigned char chLow = hex[i] & 0x0F;
		c = hex2char(chLow);
		ascii[asciiLen++] = c;
	}
	ascii[asciiLen++] = 0;
	return asciiLen;
}
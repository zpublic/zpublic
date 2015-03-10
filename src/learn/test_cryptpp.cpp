#include "stdafx.h"
#include "test_cryptpp.h"
#include "cryptopp/aes.h"

void test_cryptpp()
{
    CryptoPP::AESEncryption en;
    CryptoPP::AESDecryption de;

    const char *pIn = "1234567812345678";
    const char *pKey = "1234567812345678";
    byte *pOut = new byte[CryptoPP::AES::BLOCKSIZE];
    en.SetKey((const byte*)pKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    en.ProcessBlock((const byte*)pIn, pOut);

    de.SetKey((const byte*)pKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    de.ProcessBlock(pOut);
    delete[] pOut;
}

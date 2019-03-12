#include <iostream>
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

int crypt(string key, string intext, string output);

int main()
{
	string key;
	string line;
	int pos;
    ifstream myfile1("ciphertext");
    string output((std::istreambuf_iterator<char>(myfile1)), (std::istreambuf_iterator<char>()));
    ifstream myfile2("plaintext.txt");
    string intext((std::istreambuf_iterator<char>(myfile2)), (std::istreambuf_iterator<char>()));
	ifstream myfile3("words.txt");
	while(getline(myfile3, line))
	{
		if(crypt(line, intext, output))
		{
			cout<<line<<endl;
			break;
		}
	}
}

int crypt(string key, string intext, string output)
 {
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    unsigned char deneme[1024];
    int inlen, outlen;
    outlen = 0;
    string line, ln;
    EVP_CIPHER_CTX *ctx;
    unsigned char key2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned char iv[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i<key.length(); i++)
    {
        key2[i] = key[i];
    }
    ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, 1);
    //OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    //OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);

    EVP_CipherInit_ex(ctx, NULL, NULL, key2, iv, 1);
    for(int i = 0; i<21; i++)
    {
        inbuf[i] = intext[i];
    }
    if (!EVP_CipherUpdate(ctx, outbuf, &outlen, inbuf, 21)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    for(int i = 0; i<outlen; i++)
    {
        line+=outbuf[i];
    }
    if(!EVP_CipherFinal_ex(ctx, outbuf, &outlen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    for(int i = 0; i<outlen; i++)
    {
        line+=outbuf[i];
    }
    EVP_CIPHER_CTX_free(ctx);
    if(line.compare(output))
    {
        return 0;
    }
    return 1;
 }

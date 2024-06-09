#include "cryptography.h"

#include "Crypto.h"
#include "SHA1.h"
#include "Base64.h"

#define _WEB_DEFAULT_KEY_ "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define _HASHSIZE_ 20

CStrWithSize processWebsocketKey(const CStrWithSize &_inkey)
{

    char appended_array[61]; // 24+36+1
    uint16_t i = 0;

    for (; i < _inkey.length; i++)
    {
        appended_array[i] = _inkey.strptr[i];
    }

    const char *webdefkey = _WEB_DEFAULT_KEY_;
    for (; i < 61; i++)
    {
        appended_array[i] = webdefkey[i - _inkey.length];
    }

    SHA1 sh;
    char hashed_data[_HASHSIZE_];

    sh.update(appended_array, 60);
    sh.finalize(hashed_data, _HASHSIZE_);

    uint16_t len = Base64.encodedLength(_HASHSIZE_) + 1;
    CStrWithSize encodedString(new char[len], len);

    Base64.encode(encodedString.strptr, hashed_data, _HASHSIZE_);

    return encodedString;
}
#include "cryptography.h"

#include "Crypto.h"
#include "SHA1.h"
#include "Base64.h"

#define _webDefaultKey_ "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define _hashsize_ 20

cStrWithSize processWebsocketKey(cStrWithSize & inkey)
{

    char appendedarray[61]; // 24+36+1
    uint16_t i = 0;

    for(;i<inkey.length;i++)
    {
        appendedarray[i] = inkey.strptr[i]; 
    }

    const char* webdefkey = _webDefaultKey_;
    for(;i<61;i++)
    {
        appendedarray[i] = webdefkey[i-inkey.length];
    }


    SHA1 sh; 
    char hashed_data[_hashsize_];

    sh.update(appendedarray,60);
    sh.finalize(hashed_data,_hashsize_);

    uint16_t len = Base64.encodedLength(_hashsize_) + 1;
    cStrWithSize encodedString(new char[len],len);

    Base64.encode(encodedString.strptr, hashed_data, _hashsize_);


    return encodedString;
}
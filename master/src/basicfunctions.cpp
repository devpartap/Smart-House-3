#include "definations.h"


int16_t cStr_indexOf(const cStrWithSize & _bsting,const char * _tofindindex,const uint16_t & _stIndex)
{
    uint16_t indexlen = strlen(_tofindindex);
    uint16_t indexirr = 0;

    if(indexlen == 0)
        return -1;
    if(_stIndex >= _bsting.length)
        return -1;
    

    for(char* itr = (_bsting.strptr + _stIndex); itr != (_bsting.strptr+_bsting.length); itr++)
    {
        if(indexirr == indexlen)
            return (int16_t)(itr - _bsting.strptr - indexlen);
        
        if(*itr == _tofindindex[indexirr])
            indexirr++;
        
        else
            indexirr = 0;      
    }

    return -1;
}

void cStr_print(cStrWithSize & _bstring)
{
    for(uint16_t i = 0; i < _bstring.length; i++)
    {
        _CslLog(_bstring.strptr[i]);
    }
    _CslLog('\n');
}

void cStr_NullTerminate(cStrWithSize & _bstring)
{
    _bstring.strptr[_bstring.length] = '\0';
}

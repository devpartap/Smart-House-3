#include "definations.hpp"

int16_t CStrWithSize::indexOf(const CStrWithSize &_bsting, const char *_tofindindex, const uint16_t &_stIndex)
{
    uint16_t indexlen = strlen(_tofindindex);
    uint16_t indexirr = 0;

    if (indexlen == 0)
        return -1;
    if (_stIndex >= _bsting.length)
        return -1;

    for (char *itr = (_bsting.strptr + _stIndex); itr != (_bsting.strptr + _bsting.length); itr++)
    {
        if (indexirr == indexlen)
            return (int16_t)(itr - _bsting.strptr - indexlen);

        if (*itr == _tofindindex[indexirr])
            indexirr++;

        else
            indexirr = 0;
    }

    return -1;
}

#if ERROR_LED >= 0
void errLED()
{
    digitalWrite(ERROR_LED, HIGH);
    delay(400);
    digitalWrite(ERROR_LED, LOW);
    delay(400);
    digitalWrite(ERROR_LED, HIGH);
    delay(400);
    digitalWrite(ERROR_LED, LOW);
    delay(1000);
}
#endif


#ifdef _DEBUG_
void CStrWithSize::print(CStrWithSize &_bstring,bool inchar = true)
{
    if(inchar)
    {
        for (uint16_t i = 0; i < _bstring.length; i++)
        {
            CLOG(_bstring.strptr[i]);
        }
    }
    else
    {
        for (uint16_t i = 0; i < _bstring.length; i++)
        {
            CLOG((uint8_t)_bstring.strptr[i]);
            CLOG('-');
        }
    }
    
    CLOG('\n');
}

#endif
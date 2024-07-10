#include "definations.hpp"

#include "devicecontrol.hpp"
#include "connections_list.hpp"
#include "ESP_EEPROM.h"


// --- Inline Functions ---
inline uint8_t getSavedRelayState(uint8_t _inp)
{
    return EEPROM.read((_inp * 2) + 1);
}

inline uint8_t getSavedSwitchState(uint8_t _inp)
{
    return EEPROM.read((_inp * 2) + 2);
}

inline void putSavedRelayState(uint8_t _inp,uint8_t _value)
{
    EEPROM.write((_inp * 2) + 1,_value);
}

inline void putSavedSwitchState(uint8_t _inp,uint8_t _value)
{
    EEPROM.write((_inp * 2) + 2,_value);
}

inline bool getCurrentSwitchState(uint8_t _pin_no)
{
    return !(bool)digitalRead(_pin_no);
}
 
// --- Setup Functions ---
void setupEEPROM()
{
    CLOG_LN("\n=== SETTING UP EEPROM ===");

    EEPROM.begin((g_no_of_devices*2) + 1);

    CLOG("Init: ");
    CLOG_LN(EEPROM.read(0));


    if(EEPROM.read(0) != g_no_of_devices)
    {
        for(uint8_t i = 1; i <= (g_no_of_devices*2); i++)
        {
            EEPROM.write(i,0);
        }

        EEPROM.write(0,g_no_of_devices);
        EEPROM.commit();
        CLOG_LN("WRITING TO FLASH!");
    }
    CLOG_LN("Done");
}

void setupSwitches() // must be called after setupEEPROM()
{
    CLOG_LN("\n=== SETTING UP SWITCHES ===");
    bool to_commit = false;

    for(uint8_t i = 0; i < g_no_of_devices; i++)
    {
        pinMode(ConnectedDevices[i].m_relay_pin,OUTPUT);
        pinMode(ConnectedDevices[i].m_switch_pin,INPUT_PULLUP);

        ConnectedDevices[i].m_switch_state = getCurrentSwitchState(ConnectedDevices[i].m_switch_pin);
        ConnectedDevices[i].m_eeprom_index = i;

        CLOG("Switch State: ");
        CLOG(ConnectedDevices[i].m_eeprom_index);
        CLOG(", ");
        CLOG(ConnectedDevices[i].m_switch_pin);
        CLOG(" -> ");
        CLOG(ConnectedDevices[i].m_switch_state);
        CLOG("-");
        CLOG_LN(getSavedSwitchState(ConnectedDevices[i].m_eeprom_index));
        CLOG("Adress :- ");
        CLOG_LN((ConnectedDevices[i].m_eeprom_index * 2) + 1);

        if(getSavedSwitchState(ConnectedDevices[i].m_eeprom_index) == ConnectedDevices[i].m_switch_state)
        {
            ConnectedDevices[i].m_relay_state = getSavedRelayState(ConnectedDevices[i].m_eeprom_index);   
        }
        else
        {
            CLOG("Switch State Changed! -> ");
            CLOG(ConnectedDevices[i].m_eeprom_index);
            CLOG(", ");
            CLOG(ConnectedDevices[i].m_switch_pin);
            CLOG(" to ");
            CLOG_LN(ConnectedDevices[i].m_switch_state);

            ConnectedDevices[i].m_relay_state = ConnectedDevices[i].m_switch_state;
            putSavedRelayState(ConnectedDevices[i].m_eeprom_index,ConnectedDevices[i].m_relay_state);
            putSavedSwitchState(ConnectedDevices[i].m_eeprom_index,ConnectedDevices[i].m_relay_state);
            to_commit = true;
        }
        
        digitalWrite(ConnectedDevices[i].m_relay_pin,ConnectedDevices[i].m_relay_state);
    }

    if(to_commit)
    {
        EEPROM.commit();
        CLOG_LN("WRITING TO FLASH!!");
    }

    CLOG_LN("Done");
}

// --- Listening Functions ---
void listenSwitchChange()
{
    for(uint8_t i = 0; i < g_no_of_devices;i++ )
    {
        bool current_state = getCurrentSwitchState(ConnectedDevices[i].m_switch_pin);
        if(ConnectedDevices[i].m_switch_state != current_state)
        {
            CLOG("Switch State Changed -> ");
            CLOG(ConnectedDevices[i].m_switch_pin);
            CLOG(" to ");
            CLOG_LN(current_state);

            digitalWrite(ConnectedDevices[i].m_relay_pin,current_state);

            ConnectedDevices[i].m_switch_state = current_state;
            ConnectedDevices[i].m_relay_state = current_state;

            putSavedRelayState(ConnectedDevices[i].m_eeprom_index,current_state);
            putSavedSwitchState(ConnectedDevices[i].m_eeprom_index,current_state);
            EEPROM.commit();
            CLOG_LN("WRITING TO FLASH!");

            delay(50);
        }
    }
    
}


// --- Modifying Functions ---
void compressDevicesState(char * arry)
{
    uint8_t array_size = 1+ (uint8_t)((g_no_of_devices-1)/8);
    arry[array_size] = '\0';

    for(uint8 irr = 0;irr != array_size; irr++)
    {
        arry[irr] = 0b00000000;
    }


    uint8_t tmp;
    for(uint8_t i = 0; i < g_no_of_devices; i++)
    {
        CLOG((uint8_t)arry[0]);
        tmp = ConnectedDevices[i].m_relay_state;
        tmp = tmp << (7-(i%8));
        arry[i/8] +=  tmp;
    }


    CLOG("\n compressed:");
#ifdef _DEBUG_
    for(uint8_t irr = 0;irr != array_size + 1; irr++)
    {
        CLOG((uint8_t)arry[irr]);
        CLOG("-");
    }
    CLOG_LN(' ');

#endif
}

void changeDeviceState(const uint8_t deviceNo,const bool state)
{
    digitalWrite(ConnectedDevices[deviceNo].m_relay_pin,state);

    ConnectedDevices[deviceNo].m_relay_state = state;
    putSavedRelayState(ConnectedDevices[deviceNo].m_eeprom_index,state);
    EEPROM.commit();
    CLOG_LN("WRITING TO FLASH!");
}

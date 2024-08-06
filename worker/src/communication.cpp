#include <ESP8266WiFi.h>

#include "communication.hpp"
#include "definations.hpp"
#include "devicecontrol.hpp"

uint8_t rx_buffer[128] = {0};
uint8_t buffer_size = 0;

bool initial_report = true;

WiFiClient *master_order;
WiFiServer server(SERVER_PORT);

enum RequestType : char
{
    WORKER_STATUS = 'W',
    DEVICE_ALTER_REPORT = 'D',
    
};

void startServer()
{
    server.begin();
}

void connectToWiFi()
{
    CLOG_LN("\n === SETTING UP WIFI ===");

    WiFi.begin(g_wifi_ssid, g_wifi_password);
    WiFi.mode(WIFI_STA);
    
    CLOG("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(400);
        CLOG(".");
    }
    CLOG_LN(' ');

    WiFi.localIP().toString().toCharArray(&g_ip[0],16);

    CLOG("Connected, IP address: ");
    CLOG_LN(g_ip);
    CLOG_LN("Done");
    
}

inline bool connectToMaster(WiFiClient& masterObj)
{

#ifdef _DEBUG_
    bool status = masterObj.connect(master_ip, master_port);

    if(status)
    {
        CLOG_LN("Connected!");  
    }
    else
    {
        CLOG_LN("Master Not Connected!");
    }

    return status;
#endif

    return masterObj.connect(master_ip, master_port);
}

void reportToMaster()
{
    WiFiClient to_master;
    if(initial_report)
    {

        CLOG_LN("\n === CONNECTING TO MASTER ===");

        randomSeed(analogRead(RANDOM_SEED_PIN));
#ifdef _DEBUG_
        uint16 max_arrange_time = random(100,MAX_RANDOM_WAIT);
        CLOG("Wait : ");
        CLOG_LN(max_arrange_time);
        delay(random(100,max_arrange_time));

#else
        delay(random(100,MAX_RANDOM_WAIT));
    
#endif
        initial_report = false;
        to_master.setTimeout(2000);
    }
    else
    {
        to_master.setTimeout(MASTER_TIMEOUT_INLOOP);
    }
    
    if(!connectToMaster(to_master))
        return;

    char device_Status_compressed[2+ (uint8_t)((g_no_of_devices-1)/8)];
    compressDevicesState(device_Status_compressed);

    to_master.print(' ');

    to_master.print(RequestType::WORKER_STATUS);

    to_master.write(g_floor_id);
    to_master.write(g_room_id);
    to_master.write(g_board_id);

    to_master.print((const char*)(&g_ip[8]));
    to_master.print('-');

    to_master.write(g_no_of_devices);
    to_master.print((const char*) &(device_Status_compressed[0]));

    delay(10);

    while (!to_master.available())
    {    }

    char sl = to_master.read();
    if(sl == 'k')
    {
        master_acknowledged = true;
        CLOG_LN("Master acknowledged!");
    }
    else
    {
        CLOG_LN("acknowledge error!");
    }
 
    to_master.stop();
    
    CLOG_LN("Done");
}

void sendDeviceAlterReport(const uint8_t device_no, const bool new_state)
{
    WiFiClient to_master;
    to_master.setTimeout(MASTER_TIMEOUT_INLOOP);

    if(!connectToMaster(to_master))
    {
        master_acknowledged = false;
        return;
    }
    
    to_master.print(' ');

    to_master.print(RequestType::DEVICE_ALTER_REPORT);
    
    to_master.write(g_floor_id);
    to_master.write(g_room_id);
    to_master.write(g_board_id);

    to_master.write(device_no);
    to_master.write(new_state);

    delay(10);
    to_master.stop();

}

void processOrder()
{
    if(rx_buffer[0] == 0)
    {

    }
    else if(rx_buffer[0] == 1)
    {
        changeDeviceState(rx_buffer[1],(bool)rx_buffer[2]);
    }
    else
    {
        CLOG_LN("UNKNOWN ORDER!");
    }

}

void listenForMaster()
{
    WiFiClient master = server.accept();

    if (master.connected())
    {
        for(uint8_t i = 0; i < 255; i++)
        {
            if(master.available())
            {
                buffer_size = master.read(rx_buffer,127);
                master.stop();

                CLOG_LN("Master Here!");
#ifdef _DEBUG_
                for(uint8_t i = 0; i < buffer_size;i++)
                {
                    CLOG(rx_buffer[i]);
                    CLOG('-');
                }
                CLOG_LN(' ');
#endif

                processOrder();
                break;
            }
            delay(10);
        }
        
    }
    else
    {
        master.stop();
    }
  
}

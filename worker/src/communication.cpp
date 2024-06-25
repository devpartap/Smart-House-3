#include <ESP8266WiFi.h>

#include "communication.hpp"
#include "definations.hpp"
#include "devicecontrol.hpp"

#define SERVER_PORT 8080

uint8_t rx_buffer[128] = {0};
uint8_t buffer_size = 0;



const char* master_ip = "192.168.29.167";
short master_port = 80;

bool connected_to_master = false;

WiFiClient *master_order;
WiFiServer server(SERVER_PORT);

enum RequestType : char
{
    WORKERSTATUS = 'W',
};

void startServer()
{
    server.begin();
}

void connectToMaster()
{
    WiFiClient to_master;

    CLOG("Connecting to master ");
    while (to_master.connect(master_ip, master_port) == false)
    {
        delay(400);
        CLOG(".");
    }
    CLOG_LN("Connected!");

    char device_Status_compressed[2+ (uint8_t)((g_no_of_devices-1)/8)];
    compressDevicesState(device_Status_compressed);

    to_master.print(' ');

    to_master.print(RequestType::WORKERSTATUS);

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
        connected_to_master = true;
        CLOG("Master acknowledged!");
    }
    else
    {
        CLOG("acknowledge error!");
    }
 
    to_master.stop();
    
}

void connectToWiFi()
{
    WiFi.begin(g_wifi_ssid, g_wifi_password);
    
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
    
}

void listenForMaster()
{
    WiFiClient master = server.accept();

    if (master.connected())
    {
        CLOG_LN("Master Here!");

        while(!master.available())
        { }

        buffer_size = master.read(rx_buffer,127);

        for(uint8_t i = 0; i < buffer_size;i++)
        {
            CLOG(rx_buffer[i]);
            CLOG('-');
        }
        
            CLOG_LN('|');

        master.write('K');
    }

    master.stop();
}
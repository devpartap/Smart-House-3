#include <ESP8266WiFi.h>
#include "communication.hpp"

#define SERVER_PORT 80


const char* master_ip = "192.168.29.167";
short master_port = 80;

bool connected_to_master = false;

WiFiClient *master_order;
WiFiServer server(SERVER_PORT);




enum RequestType : char
{
    WORKERSTATUS = 'W',
};

// void startServer()
// {
//     server.begin();
// }

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
    to_master.print(' ');
    to_master.print(RequestType::WORKERSTATUS);
    to_master.write(g_floor_id);
    to_master.write(g_room_id);
    to_master.write(g_board_id);
    to_master.print('a');

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

    CLOG("Connected, IP address: ");
    CLOG_LN(WiFi.localIP());
}

void sendBoardStatus(uint8_t * _switchs_status)
{

}

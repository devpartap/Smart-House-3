#include "definations.h"
#include "esp8266.h"

enum RequestType {
    ROOM_STATUS = 'a',
    UPDATE_DEVICE = 'u'
};

static char connection_No;
static char request_type;
static unsigned int request_startingpt;
static unsigned int request_length;

void processRequest(const String & _newrequest_,unsigned int startIndex)
{    
    _Console(println("Decode:Inhere"));

    connection_No = _newrequest_[startIndex+5];
    request_startingpt = _newrequest_.indexOf('/',startIndex+12);
    request_length = _newrequest_.substring(startIndex+7,request_startingpt-5).toInt();
    request_type = _newrequest_[request_startingpt+1];

    _Console(println(connection_No));
    _Console(println(request_startingpt));
    _Console(println(request_length));
    _Console(println(request_type));

    espSendRead("AT+CIPSTATUS");

    if(request_type == ROOM_STATUS)
    {
        espSendData("101010",connection_No);
    }

    if(_newrequest_.length() - (request_length+startIndex) >= 100)
    {
        int new_stIndex = _newrequest_.indexOf("+IPD",request_length+startIndex);
        if(new_stIndex != -1)
            processRequest(_newrequest_,new_stIndex);
    }
            
}


static String TCPStatus;
static int starting_pt = 5;
void sendActiveCall()
{
    espSendRead("AT+CIPSTATUS");
    TCPStatus = espWaitRead();

    if(TCPStatus.length() > 270)
    {
        int starting_pt = TCPStatus.indexOf("+IPD");
        if(starting_pt != -1)
            processRequest(TCPStatus,starting_pt);
    }
    else
    {
        if(TCPStatus.indexOf("+",starting_pt) != -1)
        do{
            starting_pt = TCPStatus.indexOf("+",starting_pt);
        } while (starting_pt != -1);    
        
        
    }
    
}

// void getTCPConnections(String & TCP_data,unsigned int * startingIndex,bool & buffer)
// {

// }


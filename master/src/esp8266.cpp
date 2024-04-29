#include "esp8266.h"

static String msg = "";

static const char * header = "Content-Type: text/plain\nConnection: close\n";

String espSendRead(const String &command)
{
    _ESP8266.println(command);
    _ESP8266.flush();
    delay(10);

    return espRead();
}

String espRead()
{
    if(msg != "")
        msg = "";

    while(_ESP8266.available())
    {
        _Console(println("reading!!"));
        msg += _ESP8266.readString();
    }

#ifdef debug
    if(msg != "")
        _LogESP(msg);
#endif

    return msg;
    
}

String espWaitRead()
{
    while (!_ESP8266.available())
    {
        delay(10);
    }

    return espRead();
}


void espSendData(const String & _Data_,const char & _conectionNO_)
{
    // _ESP8266.print("AT+CIPSEND=");
    // _ESP8266.print(_conectionNO_);
    // _ESP8266.println(",50");
    // espRead();
    // _ESP8266.println("Access-Control-Allow-Origin: *");
    // _ESP8266.println("qwertyuioplkjhgfdsazxcvbnmwertyuioplkjhgfdsazxcvbnmwertyuioplkjhgfdsazxcvbnmwertyuioplkjhgfdsazxcvbnm");

    // // _ESP8266.print("HTTP/1.1 200 \nContent-Length:");
    // // _ESP8266.println("2");
    // // _ESP8266.print(header);
    // // _ESP8266.println(_Data_);





  String Command = "AT+CIPSEND=" + (String)_conectionNO_ + ",121";

  _ESP8266.println(Command);
  delay(10);
  
  _ESP8266.println("HTTP/1.1 200 OK");

  _ESP8266.println("Content-Length: 6");


  
  _ESP8266.println("Content-Type: text/plain");
  _ESP8266.println("Access-Control-Allow-Origin: *");
  _ESP8266.println("Connection: close");
  _ESP8266.println("");
  _ESP8266.println("101010");

  espWaitRead();


}

#include "WifiServer.h"
#include "../presenter/Presenter.h"
#include "../model/WifiUtils.h"
#include "Display.h"

#include "ConnectInfo.h"

CWifiServer_ptr CWifiServer::Create(CPresenter_ptr presenter,
                                    CDisplay_ptr display)
{
    auto server = CWifiServer_ptr(new CWifiServer(presenter, display));

    server->StartServer();

    return server;
}

void CWifiServer::Update()
{
    CUpdateable::Update();

    //

    HandleClient();
}

CWifiServer::CWifiServer(CPresenter_ptr presenter,
                         CDisplay_ptr display)
    : _presenter(presenter)
    , _display(display)
    , _server(80)
{
}

void CWifiServer::StartServer()
{
    _display->SetRow0("Starting Wifi...");

    int status = WL_IDLE_STATUS;

    if (WiFi.status() == WL_NO_MODULE)
    {

        LogLn("Communication with WiFi module failed!");

        // don't continue

        _display->SetRow1("Fail: No Module");

        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();

    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {

        LogLn("Please upgrade the firmware");

        _display->SetRow1("Fail: Upgrade FW");

        while (true)
            ;
    }

    // attempt to connect to Wifi network:

    while (status != WL_CONNECTED)
    {

        Log("Attempting to connect to WPA SSID: ");

        _display->SetRow1("Conn: " + get_ssid());

        LogLn(get_ssid());

        // Connect to WPA/WPA2 network:

        status = WiFi.begin(get_ssid().c_str(), get_wifi_password().c_str());

        // wait 10 seconds for connection:

        delay(10000);
    }

    // you're connected now, so print out the data:

    Log("You're connected to the network");

    LogConnectInfo();

    //

    _server.begin();
}

void CWifiServer::HandleClient()
{
    _client = _server.available(); // listen for incoming _clients

    if (_client)
    { // if you get a client,

        LogLn("new client"); // print a message out the serial port

        String currentLine = ""; // make a String to hold incoming data from the client

        while (_client.connected())
        { // loop while the client's connected

            if (_client.available())
            { // if there's bytes to read from the client,

                char c = _client.read(); // read a byte, then

                // Log(std::string(1, c)); // print it out the serial monitor

                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.

                    // that's the end of the client HTTP request, so send a response:

                    if (currentLine.length() == 0)
                    {

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

                        // and a content-type so the client knows what's coming, then a blank line:

                        _client.println("HTTP/1.1 200 OK");

                        _client.println("Content-type:text/html");

                        _client.println();

                        // the content of the HTTP response follows the header:

                        if (_presenter)
                        {
                            _client.println("{");
                            _client.print("\"PumpStatus\": ");
                            _client.print(_presenter->GetPumpStatus());
                            _client.println(",");
                            _client.print("\"Humidity\": ");
                            _client.print(_presenter->GetHumidityV(false).c_str());
                            _client.println("}");
                        }

                        // The HTTP response ends with another blank line:

                        _client.println();

                        // break out of the while loop:

                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine:

                        currentLine = "";
                    }
                }
                else if (c != '\r')
                { // if you got anything else but a carriage return character,

                    currentLine += c; // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":

                if (currentLine.endsWith("GET /on"))
                {
                    if (_presenter)
                    {
                        _presenter->TurnOnPumpFor(_presenter->GetFBPumpDurationMs());
                    }
                }

                if (currentLine.endsWith("GET /off"))
                {
                    if (_presenter)
                    {
                        _presenter->TurnOffPump();
                    }
                }
            }
        }

        // close the connection:

        _client.stop();

        LogLn("client disconnected");
    }
}

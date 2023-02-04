#include "WifiServer.h"
#include "../presenter/Presenter.h"
#include "../model/WifiUtils.h"
#include "Display.h"

#include "ConnectInfo.h"

static const std::string kGetPrefix("GET /");

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
        ErrorLedState(true);
        LogLn("Communication with WiFi module failed!");
        _display->SetRow1("Fail: No Module");

        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();

    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        ErrorLedState(true);
        LogLn("Please upgrade the firmware");
        _display->SetRow1("Fail: Upgrade FW");

        while (true)
            ;
    }

    // attempt to connect to Wifi network:
    int attempt(0);
    while (status != WL_CONNECTED && ++attempt < 6)
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

static bool endsWith(const std::string & str, const std::string & suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(const std::string & str, const std::string & prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

void CWifiServer::SendKeyValue(std::string key, std::string value, bool add_comma)
{
    _client.print("\"");
    _client.print(key.c_str());
    _client.print("\": ");
    _client.print(value.c_str());

    if (add_comma)
        _client.println(",");
}

void CWifiServer::HandleClient()
{
    _client = _server.available(); // listen for incoming _clients

    if (_client)
    { // if you get a client,

        LogLn("new client"); // print a message out the serial port

        std::string currentLine = ""; // make a String to hold incoming data from the client

        while (_client.connected())
        { // loop while the client's connected

            if (_client.available())
            { // if there's bytes to read from the client,

                char c = _client.read(); // read a byte, then

                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.

                    // that's the end of the client HTTP request, so send a response:

                    if (currentLine.empty())
                    {

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

                        // and a content-type so the client knows what's coming, then a blank line:

                        _client.println("HTTP/1.1 200 OK");

                        _client.println("Content-type:text/html");

                        _client.println();

                        // the content of the HTTP response follows the header:

                        if (_presenter)
                        {
                            _client.print("{");
                            //
                            SendKeyValue("PumpStatus", std::to_string(_presenter->GetPumpStatus()), true);
                            SendKeyValue("Humidity", _presenter->GetHumidityV(false), true);
                            //
                            SendKeyValue("FBHumidityV", _presenter->GetFBHumidityV(false), true);
                            SendKeyValue("FBOnTimeHour", _presenter->GetFBHour(), true);
                            SendKeyValue("FBPumpDurationS", _presenter->GetFBPumpDurationS(false), false);
                            _client.println("}");
                        }

                        // The HTTP response ends with another blank line:

                        _client.println();

                        // break out of the while loop:

                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine:

                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, "/on HTTP/1.1"))
                        {
                            if (_presenter)
                            {
                                _presenter->TurnOnPumpFor(_presenter->GetFBPumpDurationMs());
                            }
                        }

                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, "/off HTTP/1.1"))
                        {
                            if (_presenter)
                            {
                                _presenter->TurnOffPump();
                            }
                        }

                        //

                        std::string test_str = "/pump_on_time_s HTTP/1.1";
                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, test_str))
                        {
                            auto value = currentLine.substr(5, currentLine.size() - test_str.size() - 5);

                            if (_presenter)
                            {
                                _presenter->SetFBPumpDurationMs(stoi(value) * 1000);
                            }
                        }

                        //

                        test_str = "/fb_hour HTTP/1.1";
                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, test_str))
                        {
                            auto value = currentLine.substr(5, currentLine.size() - test_str.size() - 5);

                            if (_presenter)
                            {
                                _presenter->SetFBTime(stoi(value));
                            }
                        }

                        //

                        test_str = "/fb_humidity_mv HTTP/1.1";
                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, test_str))
                        {
                            auto value = currentLine.substr(5, currentLine.size() - test_str.size() - 5);
                            if (_presenter)
                            {
                                _presenter->SetFBHumidityV(static_cast<float>(stoi(value)) / 1000);
                            }
                        }

                        //

                        test_str = "/current_hour HTTP/1.1";
                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, test_str))
                        {
                            auto value = currentLine.substr(5, currentLine.size() - test_str.size() - 5);
                            if (_presenter)
                            {
                                _presenter->SetCurrentTimeHour(stoi(value));
                            }
                        }

                        //

                        test_str = "/current_minute HTTP/1.1";
                        if (startsWith(currentLine, kGetPrefix) && endsWith(currentLine, test_str))
                        {
                            auto value = currentLine.substr(5, currentLine.size() - test_str.size() - 5);
                            if (_presenter)
                            {
                                _presenter->SetCurrentTimeMinute(stoi(value));
                            }
                        }

                        //

                        currentLine.clear();
                    }
                }
                else if (c != '\r')
                { // if you got anything else but a carriage return character,

                    currentLine += std::string(1, c); // add it to the end of the currentLine
                }

                // Check to see if the client request was "GET /H" or "GET /L":
            }
        }

        // close the connection:

        _client.stop();

        LogLn("client disconnected");
    }
}

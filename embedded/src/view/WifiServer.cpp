#include "WifiServer.h"
#include "../presenter/Presenter.h"
#include "../model/WifiUtils.h"
#include "Display.h"

#include "ConnectInfo.h"

static const std::string kGetPrefix("GET /");
static constexpr int kReconnectInterval_ms = 1000 * 1800; // initially 30 min

CWifiServer_ptr CWifiServer::Create(CPresenter_ptr presenter,
                                    CDisplay_ptr display)
{
    auto server = CWifiServer_ptr(new CWifiServer(presenter, display));

    server->Init();

    return server;
}

void CWifiServer::Update()
{
    CUpdateable::Update();

    //

    if (ConnectionStatus() == WL_CONNECTED)
    {
        HandleClient();
    }
    else
    {
        UpdateConnection();
    }
}

CWifiServer::CWifiServer(CPresenter_ptr presenter,
                         CDisplay_ptr display)
    : _presenter(presenter)
    , _display(display)
    , _server(80)
{
}

void CWifiServer::Init()
{
    _display->SetRow0("Starting Wifi...");

    _last_connection_attempt_ms = millis();

    int status = WL_IDLE_STATUS;

    if (ConnectionStatus() == WL_NO_MODULE)
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

    // clean up any existing connection if necessary
    LogLn("_first_run: " + std::to_string(_first_run));

    if (_first_run)
    {
        _first_run = false;
    }
    else
    {
        WiFi.end();
    }

    delay(1000); // not sure if this is necessary

    // attempt to connect to Wifi network:
    int attempt(0);
    while (ConnectionStatus() != WL_CONNECTED && ++attempt < 6)
    {
        _display->SetRow0("Starting Wifi: " + std::to_string(attempt));

        Log("Attempting to connect to WPA SSID: ");
        _display->SetRow1(get_ssid());

        LogLn(get_ssid());

        // Connect to WPA/WPA2 network:

        status = WiFi.begin(get_ssid().c_str(), get_wifi_password().c_str());

        // wait 10 seconds for connection:

        int wait_attempts{0};
        while (ConnectionStatus() != WL_CONNECTED && wait_attempts < 20)
        {
            ++wait_attempts;
            delay(500);
        }
    }

    // you're connected now, so print out the data:

    Log("You're connected to the network");

    LogConnectInfo();

    //

    _server.begin(); // re(start) server
}

uint8_t CWifiServer::ConnectionStatus()
{
    return WiFi.status();
}

static bool starts_with(const std::string & str, const std::string & prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static bool ends_with(const std::string & str, const std::string & suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

void CWifiServer::UpdateConnection()
{
    LogLn("CWifiServer::UpdateConnection");
    if ((millis() - _last_connection_attempt_ms) < kReconnectInterval_ms)
        return;

    Init();
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
    if (not _client)
        return;

    LogLn("new client"); 

    std::string current_line = ""; 

    while (_client.connected())
    { 
        if (not _client.available())
            continue;

        char c = _client.read(); // read a byte, then

        if (c == '\n')
        { // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (current_line.empty())
            {
                SendResponse();
                break;
            }

            HandleRequest(current_line);
            //
            current_line.clear();
        }
        else if (c != '\r')
        { 
            current_line += std::string(1, c); 
        }
    }

    _client.stop();

    LogLn("client disconnected");
}

void CWifiServer::HandleRequest(const std::string & request)
{

    if (starts_with(request, kGetPrefix) && ends_with(request, "/on HTTP/1.1"))
    {
        if (_presenter)
        {
            _presenter->TurnOnPumpForMs(_presenter->GetFBPumpDurationMs());
        }
    }

    if (starts_with(request, kGetPrefix) && ends_with(request, "/off HTTP/1.1"))
    {
        if (_presenter)
        {
            _presenter->TurnOffPump();
        }
    }

    std::string test_str = "/pump_on_time_s HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        auto value = request.substr(5, request.size() - test_str.size() - 5);

        if (_presenter)
        {
            _presenter->SetFBPumpDurationMs(stoi(value) * 1000);
        }
    }

    test_str = "/fb_hour HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        auto value = request.substr(5, request.size() - test_str.size() - 5);

        if (_presenter)
        {
            _presenter->SetFBTime(stoi(value));
        }
    }

    test_str = "/fb_humidity_mv HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        auto value = request.substr(5, request.size() - test_str.size() - 5);
        if (_presenter)
        {
            _presenter->SetFBHumidityV(static_cast<float>(stoi(value)) / 1000);
        }
    }

    test_str = "/current_hour HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        auto value = request.substr(5, request.size() - test_str.size() - 5);
        if (_presenter)
        {
            _presenter->SetCurrentTimeHour(stoi(value));
        }
    }

    test_str = "/current_minute HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        auto value = request.substr(5, request.size() - test_str.size() - 5);
        if (_presenter)
        {
            _presenter->SetCurrentTimeMinute(stoi(value));
        }
    }

    test_str = "/save HTTP/1.1";
    if (starts_with(request, kGetPrefix) && ends_with(request, test_str))
    {
        if (_presenter)
        {
            _presenter->SaveFBSettingsToFlash();
        }
    }
}

void CWifiServer::SendResponse()
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
        SendKeyValue("PumpRan", std::to_string(_presenter->GetPumpRan()), true);
        SendKeyValue("Humidity", _presenter->GetHumidityV(false), true);
        //
        SendKeyValue("FBHumidityV", _presenter->GetFBHumidityV(false), true);
        SendKeyValue("FBOnTimeHour", _presenter->GetFBHour(), true);
        SendKeyValue("FBPumpDurationS", _presenter->GetFBPumpDurationS(false), false);
        _client.println("}");
    }

    // The HTTP response ends with another blank line:

    _client.println();
}
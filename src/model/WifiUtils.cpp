#include "../Types.h"

#include <WiFiNINA.h>

void LogConnectinfo()
{

    // print the SSID of the network you're attached to:

    Log("SSID: ");

    LogLn(WiFi.SSID());

    // print the received signal strength:

    long rssi = WiFi.RSSI();

    Log("signal strength (RSSI):");

    LogLn(std::to_string(rssi));

    // print your board's IP address:

    IPAddress ip = WiFi.localIP();

    Log("IP Address: ");

    for (int p{0}; p < 3; ++p)
    {
        Log(std::to_string(ip[p]) + ".");
    }
    LogLn(std::to_string(ip[3]));
}

void SetupWifi(const std::string & ssid, const std::string & pass)
{
    int status = WL_IDLE_STATUS;

    if (WiFi.status() == WL_NO_MODULE)
    {

        LogLn("Communication with WiFi module failed!");

        // don't continue

        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();

    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {

        LogLn("Please upgrade the firmware");
    }

    // attempt to connect to Wifi network:

    while (status != WL_CONNECTED)
    {

        Log("Attempting to connect to WPA SSID: ");

        LogLn(ssid);

        // Connect to WPA/WPA2 network:

        status = WiFi.begin(ssid.c_str(), pass.c_str());

        // wait 10 seconds for connection:

        delay(10000);
    }

    // you're connected now, so print out the data:

    Log("You're connected to the network");

    LogConnectinfo();
}

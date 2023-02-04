#include "../Types.h"

#include <WiFiNINA.h>

void LogConnectInfo()
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

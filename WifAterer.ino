
#include "src/Types.h"
#include "src/Application.h"
#include "src/presenter/Presenter.h"

#include "src/hardware/LCDPins.h"
#include "src/hardware/DisplayKeyPins.h"
#include "src/hardware/SmartPumpPins.h"

//
#include "src/model/WiFiUtils.h"
#include "ConnectInfo.h"

#include <memory>
#include <vector>

// defs

auto display_key_pins = SDisplayKeyPins(15, 21, 3, 20);
// auto display_key_pins = SDisplayKeyPins(15, 14, 3, 2);

auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8, 18);
auto smart_pump_pins = SSmartPumpPins(16, A3);

auto softkey_display_pins = SSoftkeyDisplayPins(display_key_pins, lcd_pins);

//

CApplication_ptr app;

WiFiServer server(80);
WiFiClient client;

//

void setup()
{
    LogBegin(9600);
    LogLn("Settings up");

    SetupWifi(get_ssid(), get_wifi_password());
    pinMode(LED_BUILTIN, OUTPUT);

    server.begin(); // start the web server on port 80
    app = CApplication::Create(smart_pump_pins, softkey_display_pins);
}

void loop()
{
    app->Update();

    //
    WiFiClient client = server.available(); // listen for incoming clients

    if (client)
    { // if you get a client,

        LogLn("new client"); // print a message out the serial port

        String currentLine = ""; // make a String to hold incoming data from the client

        while (client.connected())
        { // loop while the client's connected

            if (client.available())
            { // if there's bytes to read from the client,

                char c = client.read(); // read a byte, then

                // Log(std::string(1, c)); // print it out the serial monitor

                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.

                    // that's the end of the client HTTP request, so send a response:

                    if (currentLine.length() == 0)
                    {

                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

                        // and a content-type so the client knows what's coming, then a blank line:

                        client.println("HTTP/1.1 200 OK");

                        client.println("Content-type:text/html");

                        client.println();

                        // the content of the HTTP response follows the header:

                        if (auto presenter = app->Presenter())
                        {
                            client.println("{");
                            client.print("\"PumpStatus\": ");
                            client.print(presenter->GetPumpStatus());
                            client.println(",");
                            client.print("\"Humidity\": ");
                            client.print(presenter->GetHumidityV(false).c_str());
                            client.println("}");
                        }

                        // The HTTP response ends with another blank line:

                        client.println();

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
                    if (auto presenter = app->Presenter())
                    {
                        presenter->TurnOnPumpFor(presenter->GetFBPumpDurationMs());
                    }
                }

                if (currentLine.endsWith("GET /off"))
                {
                    if (auto presenter = app->Presenter())
                    {
                        presenter->TurnOffPump();
                    }
                }
            }
        }

        // close the connection:

        client.stop();

        LogLn("client disconnected");
    }
}

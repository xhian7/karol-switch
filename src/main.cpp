#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "k-switch_net";

const char *html = "<!DOCTYPE html><html lang=\"es\"><head><meta charset=\"UTF-8\">"
                   "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"
                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                   "<title>K-Switch</title>"
                   "<style>"
                   "h1 {"
                   "font-family: sans-serif;"
                   "font-weight: 300;"
                   "text-align: center;"
                   "}"
                   ".button-wrapper {"
                   "display: flex;"
                   "flex-direction: column;"
                   "max-width: 300px;"
                   "margin: 0 auto;"
                   "}"
                   "a {"
                   "font-family: sans-serif;"
                   "font-size: 30px;"
                   "margin: 20px 0;"
                   "padding: 10px 0;"
                   "border-radius: 10px;"
                   "text-align: center;"
                   "text-decoration: none;"
                   "}"
                   ".on-button {"
                   "background-color: steelblue;"
                   "color: whitesmoke;"
                   "}"
                   ".off-button {"
                   "background-color: indianred;"
                   "color: whitesmoke;"
                   "}"
                   "</style>"
                   "</head>"
                   "<body>"
                   "<h1>K-Switch</h1>"
                   "<div class=\"button-wrapper\">"
                   "<a class=\"on-button\" href=\"/encender\">ON</a>"
                   "<a class=\"off-button\" href=\"/apagar\">OFF</a>"
                   "</div>"
                   "</body>"
                   "</html>";

ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(0, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (!MDNS.begin("k-switch", IP))
  {
    Serial.println("Error iniciando multicast");
  }
  else
  {
    Serial.println("Multicast iniciado");
  }
  MDNS.addService("http", "tcp", 80);

  server.on("/", []()
            { server.send(200, "text/html", html); });

  server.on("/encender", []()
            { digitalWrite(0, HIGH);
            server.send(200, "text/html", html); });

  server.on("/apagar", []()
            { digitalWrite(0, LOW);
            server.send(200, "text/html", html); });

  server.begin();
}

void loop()
{
  MDNS.update();
  server.handleClient();
}
#include "network.h"
#include "OSCMessage.h"
#include <ESPmDNS.h>

Wifi32Network::Wifi32Network(char *name, char *ssid, char *pass) : server(80)
{
	this->name = name;
	this->ssid = ssid;
	this->pass = pass;
}

void Wifi32Network::addIP(IPAddress ip)
{
	for (int i = 0; i < targets.size(); i++)
	{
		if (ip == targets.at(i))
		{
			return;
		}
	}
	targets.push_back(ip);
	Serial.print(ip.toString().c_str());
	Serial.println(" is in list.");
}

void Wifi32Network::removeIP(IPAddress ip)
{
	for (auto it = targets.begin(); it != targets.end(); ++it)
	{
		if (*it == ip)
		{
			targets.erase(it);
			return;
		}
	}
	Serial.print(ip.toString().c_str());
	Serial.println(" is removed");
}
void Wifi32Network::subscribe()
{
	IPAddress clientIP = server.client().remoteIP();
	addIP(clientIP);
	Serial.println(clientIP);
	server.send(200, "text/plain", "subscribe!");
}

void Wifi32Network::unsubscribe()
{
	IPAddress clientIP = server.client().remoteIP();
	removeIP(clientIP);
	server.send(200, "text/plain", "unsubscribe!");
}

void Wifi32Network::setup()
{
	WiFi.mode(WIFI_STA); // Optional
	Serial.println("connecting ...");
	WiFi.begin(ssid, pass);

	if (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		Serial.println("WiFi Connect Failed! Rebooting...");
		delay(1000);
		ESP.restart();
	}

	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(2000);
	}
	Serial.print("connect: ");
	delay(400);
	Serial.println(WiFi.localIP());

	if (!MDNS.begin(name))
	{
		Serial.println("Error setting up MDNS responder!");
		while (1)
		{
			delay(1000);
		}
	}
	Serial.print("started mDNS...");

	server.on("/subscribe", std::bind(&Wifi32Network::subscribe, this));
	server.on("/unsubscribe", std::bind(&Wifi32Network::unsubscribe, this));

	server.begin();
	Serial.println("HTTP server started: ");
}

void Wifi32Network::loop()
{
	server.handleClient();
}

OSC32::OSC32(int port)
{
	this->port = port;
}

OSC32::OSC32(IPAddress ip, int port)
{
	this->ip = ip;
	this->port = port;
	// Udp.begin(port);
}

void OSC32::sendMessage(char *address, int *messages, uint8_t length)
{
	OSCMessage msg(address);

	for (int i = 0; i < length; i++)
	{
		int message = messages[i];
		msg.add(message);
	}
	udp.beginPacket(ip, port);
	msg.send(udp);
	udp.endPacket();
	msg.empty();
}

void OSC32::sendMessage(char *address, float messages[], uint8_t length )
{
	OSCMessage msg(address);

	for (int i = 0; i < length; i++)
	{
		float message = messages[i];
		msg.add(message);
	}

	udp.beginPacket(ip, port);
	msg.send(udp);
	udp.endPacket();
	msg.empty();
}

void OSC32::sendMessage(char *address, int16_t *messages, uint8_t length)
{
	OSCMessage msg(address);
	for (int i = 0; i < length; i++)
	{
		int message = messages[i];
		msg.add(message);
	}
	udp.beginPacket(ip, port);
	msg.send(udp);
	udp.endPacket();
	msg.empty();
}

void OSC32::set(IPAddress ip)
{
	this->ip = ip;
}

void OSC32::set(IPAddress ip, int port)
{
	this->ip = ip;
	this->port = port;
}

void OSC32::setup()
{
	udp.begin(WiFi.localIP(), 0);
}

void OSC32::loop()
{
}
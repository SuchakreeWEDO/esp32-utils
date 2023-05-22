#include "WiFi.h"
#include "WebServer.h"
#include <vector>
#include "utils.h"

#include <OSCMessage.h>

class OSC32 : public BoardSystem
{
public:
	IPAddress ip;
	int port;
	WiFiUDP udp;
	OSC32(int port);
	OSC32(IPAddress ip, int port);

	void sendMessage(char *address, int *messages, uint8_t length);
	void sendMessage(char *address, int16_t *messages, uint8_t length);
	void sendMessage(char *address, float *messages, uint8_t length);
	void set(IPAddress ip);
	void set(IPAddress ip, int port);

	void setup();

	void loop();
};

class Wifi32Network : public BoardSystem
{
public:
	int storage_array[5];
	std::vector<IPAddress> targets;
	char *name;

	int sendPort = 9999;

	WebServer server;
	WiFiUDP udp;
	int status = WL_IDLE_STATUS;
	char *ssid;
	char *pass;

	Wifi32Network(char *name, char *ssid, char *pass);

	void addIP(IPAddress ip);

	void removeIP(IPAddress ip);

	void subscribe();

	void unsubscribe();

	void setup();

	void loop();
};
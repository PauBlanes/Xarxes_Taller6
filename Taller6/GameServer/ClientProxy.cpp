#include "ClientProxy.h"

ClientProxy::ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos) {
	ip = myIp;
	port = myPort;
	position = pos;
}

bool ClientProxy::operator==(const ClientProxy& other) {
	if (ip == other.ip && port == other.port)
		return true;
	return false;
}
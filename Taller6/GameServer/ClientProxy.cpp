#include "ClientProxy.h"

ClientProxy::ClientProxy(IpAddress myIp, unsigned short myPort) {
	ip = myIp;
	port = myPort;
}

bool ClientProxy::operator==(const ClientProxy& other) {
	if (ip == other.ip && port == other.port)
		return true;
	return false;
}
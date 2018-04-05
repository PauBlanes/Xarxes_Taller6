#include "ClientProxy.h"

ClientProxy::ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos, string n) {
	ip = myIp;
	port = myPort;
	position = pos;
	nick = n;
}

bool ClientProxy::operator==(const ClientProxy& other) {
	if (ip == other.ip && port == other.port)
		return true;
	return false;
}
void ClientProxy::ResendMsgs(UdpSocket* sock) {
	for each (Packet p in unrespondedMsgs)
	{
		sock->send(p, ip, port);
	}
}
void ClientProxy::CheckACK(int idToErase) {
	for (int i = 0; i < unrespondedMsgs.size(); i++) {
		Packet currPacket = unrespondedMsgs[i];
		int pType;
		int id;
		currPacket >> pType >> id;
		if (id == idToErase) {
			unrespondedMsgs.erase(unrespondedMsgs.begin() + i);
			return;
		}
	}
}
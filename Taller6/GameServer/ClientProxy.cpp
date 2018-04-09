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
	Time currTime = resendClock.getElapsedTime();
	if (currTime.asMilliseconds() >  RESEND_TIME && !unrespondedMsgs.empty()) {		
		for each (Packet p in unrespondedMsgs)
		{
			cout << "resending msgs to " << ip << ":" << port << endl;
			sock->send(p, ip, port);
		}
		resendClock.restart();
	}
	
}
void ClientProxy::MesageResponded(int idToErase) {
	for (int i = 0; i < unrespondedMsgs.size(); i++) {
		Packet currPacket = unrespondedMsgs[i];
		int pType;
		int id;
		currPacket >> pType >> id;
		if (id == idToErase) { //borrem el misatge amb id que li hem passat
			unrespondedMsgs.erase(unrespondedMsgs.begin() + i);
			return;
		}
	}
}
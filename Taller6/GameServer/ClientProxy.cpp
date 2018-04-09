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
	if (currTime.asMilliseconds() >  RESEND_TIME) {		
		for (map<int, BufferAndLength>::iterator it = msgs2Resend.begin(); it != msgs2Resend.end(); ++it)
		{
			cout << "resending msgs to " << ip << ":" << port << endl;			
			sock->send(it->second.buffer, it->second.length, ip, port);
		}
		/*for each (Packet p in unrespondedMsgs)
		{
			cout << "resending msgs to " << ip << ":" << port << endl;
			sock->send(p, ip, port);
		}*/
		resendClock.restart();
	}
	
}
void ClientProxy::MesageResponded(int idToErase) {
	
	cout << "received confirmation for msg : " << idToErase << ", erasing msg." << endl;
	std::map<int, BufferAndLength>::iterator it = msgs2Resend.find(idToErase);
	if (it != msgs2Resend.end())
		msgs2Resend.erase(it);

	
	/*for (int i = 0; i < unrespondedMsgs.size(); i++) {
		Packet currPacket = unrespondedMsgs[i];
		int pType;
		int id;
		currPacket >> pType >> id;
		if (id == idToErase) { //borrem el misatge amb id que li hem passat
			unrespondedMsgs.erase(unrespondedMsgs.begin() + i);
			return;
		}
	}*/
}
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
			Send(sock, it->second.buffer, it->second.length);
		}
		
		resendClock.restart();
	}
	
}
void ClientProxy::MesageResponded(int idToErase) {
	
	cout << "received confirmation for msg : " << idToErase << ", erasing msg." << endl;
	std::map<int, BufferAndLength>::iterator it = msgs2Resend.find(idToErase);
	if (it != msgs2Resend.end())
		msgs2Resend.erase(it);
		
}

void ClientProxy::Send(UdpSocket* sock, char* buffer, int length) {
	
	srand(time(NULL));
	int rnd = rand() % 100 + 1;

	if (rnd > PERCENT_LOSS)
		sock->send(buffer, length, ip, port);
}

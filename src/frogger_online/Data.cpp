#include "pch.h"
#include "Data.h"


Data::Data(SDL_Rect position_rect, SDL_Rect mosquito_posic) {

	this->new_mosquito_posic = mosquito_posic;
		
	this->is_playing = true;
	
	this->GameOver = false;

	this->update_mosquito = false;

	this->direction = -1;

	this->ready = true;
}

Data::~Data() {}

bool Data::listen(SDLNet_SocketSet connection, TCPsocket socket, void *message, int message_size) 
{
	while (SDLNet_CheckSockets(connection, 0) > 0) {
		if (SDLNet_SocketReady(socket)) {
			if (SDLNet_TCP_Recv(socket, message, message_size) > 0) {
				return true;
			}
		}
	}
	return false;
}

#pragma once

#include "pch.h"

class Data
{
public:

	SDL_Rect new_mosquito_posic;
	
	bool update_mosquito;

	bool is_playing;

	bool GameOver;

	bool ready;

	int score;

	int direction;

	SDL_Rect position_rect;

	Data(SDL_Rect position_rect, SDL_Rect mosquito_posic);

	~Data();
	
	static bool listen(SDLNet_SocketSet connection, TCPsocket socket, void *message, int message_size);
};


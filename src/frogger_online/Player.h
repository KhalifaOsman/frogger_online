#pragma once

#include <pch.h>

class Player
{
public:
	Player(SDL_Renderer *renderTarget, int x, int y, int initial_direction); // Construtor da classe Player
	~Player(); // Destrutor da classe Player
	int Update(float delta, const Uint8 *keyState, int levelWidth, int levelHeight);
	void Update2(float delta, int levelWidth, int levelHeight, int direction);
	// Fun��o que atualiza e move o sapo
	void Draw(SDL_Renderer *renderTarget, SDL_Rect cameraRect, bool center_camera); // Fun��o que desenha o sapo
	bool IntersectsWith(SDL_Rect veichlePosition, bool isMosquito); // Fun��o para detectar as colis�es
	SDL_Rect positionRect; // Ret�ngulo da posi��o do jogador
	int get_current_direction();
	void set_current_frame(int frame);
private:
	SDL_Rect frogSize[4][5]; // Matriz dos ret�ngulos de todos os est�gios da aniuma��o do sapo
	SDL_Rect cropRect; // Ret�ngulo para o corte da anima��o
	SDL_Texture *texture[4][5]; // Ret�ngulos de todas aos est�gios de anima��o do sapo
	Mix_Chunk *croak; // Som do sapo
	int currentDirection, currentFrame; // Auxiliares para medir as dimens�es das texturas
	int initialX, initialY; // Posi��o inicial do jogador
	int initial_direction;
	float moveSpeed; // Velocidade do jogador
	float frameCounter; // Contador de frames
	bool isActive; // Verdadeiro se o jogador esta apertando algo, falso para o contr�rio
	bool hasJumped; // Verdadeiro para o segunda metade da anima��o, falso para a primeira metaade
};


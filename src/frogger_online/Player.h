#pragma once

#include <pch.h>

class Player
{
public:
	Player(SDL_Renderer *renderTarget, int x, int y, int initial_direction); // Construtor da classe Player
	~Player(); // Destrutor da classe Player
	int Update(float delta, const Uint8 *keyState, int levelWidth, int levelHeight);
	void Update2(float delta, int levelWidth, int levelHeight, int direction);
	// Função que atualiza e move o sapo
	void Draw(SDL_Renderer *renderTarget, SDL_Rect cameraRect, bool center_camera); // Função que desenha o sapo
	bool IntersectsWith(SDL_Rect veichlePosition, bool isMosquito); // Função para detectar as colisões
	SDL_Rect positionRect; // Retângulo da posição do jogador
	int get_current_direction();
	void set_current_frame(int frame);
private:
	SDL_Rect frogSize[4][5]; // Matriz dos retângulos de todos os estégios da aniumação do sapo
	SDL_Rect cropRect; // Retângulo para o corte da animação
	SDL_Texture *texture[4][5]; // Retângulos de todas aos estágios de animação do sapo
	Mix_Chunk *croak; // Som do sapo
	int currentDirection, currentFrame; // Auxiliares para medir as dimensões das texturas
	int initialX, initialY; // Posição inicial do jogador
	int initial_direction;
	float moveSpeed; // Velocidade do jogador
	float frameCounter; // Contador de frames
	bool isActive; // Verdadeiro se o jogador esta apertando algo, falso para o contrário
	bool hasJumped; // Verdadeiro para o segunda metade da animação, falso para a primeira metaade
};


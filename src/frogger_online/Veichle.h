#pragma once

#include<pch.h>

class Veichle
{
public:
	Veichle(char *fileName, SDL_Renderer *renderTarget, int levelWidth, int y, int direction, float moveSpeed); // Construtor da classe Veichle
	~Veichle(); // Destrutor da classe Veichle

	SDL_Rect GetPositionRect(); // M�todo get para positionRect
	void Update(float delta, int frameWidth, float speedMultiplyer); // Atualiza e move o veiculo
	void Draw(SDL_Renderer *renderTarget); // Renderiza o ve�culo na tela

private:
	SDL_Texture *texture; // Textura do ve�culo
	SDL_Rect positionRect; // Ret�ngulo de posi��o do ve�culo
	SDL_Rect cropRect; // Ret�ngulo de corte do ve�culo

	int direction; // Multiplicador da dire��o do ve�culo: 1 para direita e -1 para a esquerda
	float moveSpeed; // Modulo da velocidade do ve�culo
};
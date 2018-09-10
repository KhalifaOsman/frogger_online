#pragma once

#include<pch.h>

class Veichle
{
public:
	Veichle(char *fileName, SDL_Renderer *renderTarget, int levelWidth, int y, int direction, float moveSpeed); // Construtor da classe Veichle
	~Veichle(); // Destrutor da classe Veichle

	SDL_Rect GetPositionRect(); // Método get para positionRect
	void Update(float delta, int frameWidth, float speedMultiplyer); // Atualiza e move o veiculo
	void Draw(SDL_Renderer *renderTarget); // Renderiza o veículo na tela

private:
	SDL_Texture *texture; // Textura do veículo
	SDL_Rect positionRect; // Retângulo de posição do veículo
	SDL_Rect cropRect; // Retângulo de corte do veículo

	int direction; // Multiplicador da direção do veículo: 1 para direita e -1 para a esquerda
	float moveSpeed; // Modulo da velocidade do veículo
};
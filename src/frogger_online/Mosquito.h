#pragma once

#include<pch.h>

class Mosquito
{
public:
	Mosquito(SDL_Renderer *renderTarget); // Construtor da classe Mosquito
	~Mosquito(); // Destrutor da classe Mosquito

	void draw(SDL_Renderer *renderTarget); // Renderiza o Mosquito na tela
	void newPosition(); // Reposiciona o mosquito em um lugar aleatório nas ruas
	SDL_Rect getPositionRect(); // Método get para positionRect
	void setPositionRect(SDL_Rect new_posic);

private:
	SDL_Texture *texture; // Textura do mosquito
	SDL_Rect positionRect; // Retângulo de posição do mosquito
	SDL_Rect cropRect; // Retângulo de corte do mosquito

	bool isOnUpperRoad; // Variável para alternar o mosquito entre a rua de cima (verdadeiro) e baixo (falso)
};
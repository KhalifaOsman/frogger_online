#pragma once

#include<pch.h>

class Mosquito
{
public:
	Mosquito(SDL_Renderer *renderTarget); // Construtor da classe Mosquito
	~Mosquito(); // Destrutor da classe Mosquito

	void draw(SDL_Renderer *renderTarget); // Renderiza o Mosquito na tela
	void newPosition(); // Reposiciona o mosquito em um lugar aleat�rio nas ruas
	SDL_Rect getPositionRect(); // M�todo get para positionRect
	void setPositionRect(SDL_Rect new_posic);

private:
	SDL_Texture *texture; // Textura do mosquito
	SDL_Rect positionRect; // Ret�ngulo de posi��o do mosquito
	SDL_Rect cropRect; // Ret�ngulo de corte do mosquito

	bool isOnUpperRoad; // Vari�vel para alternar o mosquito entre a rua de cima (verdadeiro) e baixo (falso)
};
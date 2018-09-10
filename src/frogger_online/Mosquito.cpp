#include "pch.h"

// Reposiciona o mosquito em um lugar aleat�rio nas ruas
void Mosquito::newPosition()
{
	positionRect.x = (rand() % 1520); // Posi��o x recebe valor aleat�rio de 0 a 1519

	if (isOnUpperRoad) // Se � a vez de colocar na rua de cima
	{
		positionRect.y = 82 + rand() % 267; // Posi��o y recebe valor aleat�rio de 82 a 348
		isOnUpperRoad = false; // Pr�ximo mosquito sera posicionado na rua de baixo
	}
	else // Se � a vez de colocar na rua de baixo
	{
		positionRect.y = 478 + rand() % 267; // Posi��o y recebe valor aleat�rio de 478 a 745
		isOnUpperRoad = true; // Pr�ximo mosquito sera posicionado na rua de cima
	}
}

/* Construtor da classe Mosquito
* Par�metros:
*   *renderTarget: ponteiro para o renderizador
*/
Mosquito::Mosquito(SDL_Renderer *renderTarget)
{
	int textureWidth, textureHeight; // Vari�veis auxiliares para medir as dimens�es de texturas

	srand(time(NULL)); // Instru��o que faz a fun��o rand() receber como semente a data e hora da placa m�e

	SDL_Surface *surface = IMG_Load("Textures/Mosquito.png"); // Abre a superf�cie da imagem do mosquito
	texture = SDL_CreateTextureFromSurface(renderTarget, surface); // Cria a textura a partir desta superficie
	SDL_FreeSurface(surface); // Desaloca a superf�cie

	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight); // Mede as dimens�es da textura e armazena elas em textureWidth e textureHeight

	// Inicializa o ret�ngulo de corte
	cropRect.x = 0;
	cropRect.y = 0;
	cropRect.w = textureWidth;
	cropRect.h = textureHeight;

	// Mosqutio come�a na rua de cima
	isOnUpperRoad = true;

	// Posiciona o mosqutio
	newPosition();
	positionRect.w = textureWidth;
	positionRect.h = textureHeight;
}

// Destrutor da classe Mosquito
Mosquito::~Mosquito()
{
	SDL_DestroyTexture(texture); // Desaloca a textura do mosquito
}

// Fun��o que renderiza o mosquito na tela
void Mosquito::draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect); // Renderiza a textura do mosquto na tela
}

// M�todo get para positionRect
SDL_Rect Mosquito::getPositionRect()
{
	return positionRect;
}

// M�todo set para positionRect
void Mosquito::setPositionRect(SDL_Rect new_posic)
{
	this->positionRect = new_posic;
}
#include "pch.h"

// Reposiciona o mosquito em um lugar aleatório nas ruas
void Mosquito::newPosition()
{
	positionRect.x = (rand() % 1520); // Posição x recebe valor aleatório de 0 a 1519

	if (isOnUpperRoad) // Se é a vez de colocar na rua de cima
	{
		positionRect.y = 82 + rand() % 267; // Posição y recebe valor aleatório de 82 a 348
		isOnUpperRoad = false; // Próximo mosquito sera posicionado na rua de baixo
	}
	else // Se é a vez de colocar na rua de baixo
	{
		positionRect.y = 478 + rand() % 267; // Posição y recebe valor aleatório de 478 a 745
		isOnUpperRoad = true; // Próximo mosquito sera posicionado na rua de cima
	}
}

/* Construtor da classe Mosquito
* Parâmetros:
*   *renderTarget: ponteiro para o renderizador
*/
Mosquito::Mosquito(SDL_Renderer *renderTarget)
{
	int textureWidth, textureHeight; // Variáveis auxiliares para medir as dimensões de texturas

	srand(time(NULL)); // Instrução que faz a função rand() receber como semente a data e hora da placa mãe

	SDL_Surface *surface = IMG_Load("Textures/Mosquito.png"); // Abre a superfície da imagem do mosquito
	texture = SDL_CreateTextureFromSurface(renderTarget, surface); // Cria a textura a partir desta superficie
	SDL_FreeSurface(surface); // Desaloca a superfície

	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight); // Mede as dimensões da textura e armazena elas em textureWidth e textureHeight

	// Inicializa o retângulo de corte
	cropRect.x = 0;
	cropRect.y = 0;
	cropRect.w = textureWidth;
	cropRect.h = textureHeight;

	// Mosqutio começa na rua de cima
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

// Função que renderiza o mosquito na tela
void Mosquito::draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect); // Renderiza a textura do mosquto na tela
}

// Método get para positionRect
SDL_Rect Mosquito::getPositionRect()
{
	return positionRect;
}

// Método set para positionRect
void Mosquito::setPositionRect(SDL_Rect new_posic)
{
	this->positionRect = new_posic;
}
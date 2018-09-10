#include "pch.h"

/* Construtor da classe Veichle
* Par�metros:
*   fileName: ponteiro para a String do nome do arquivo .png da imagem
*   *renderTarget: ponteiro para o renderizador
*   x: coordenada x inicial da posi��o do ve�culo
*   y: coordenada y inicial da posi��o do ve�culo
*   direction: sentido do carro: 1 para direita e -1 para a esquerda
*/
Veichle::Veichle(char *fileName, SDL_Renderer *renderTarget, int x, int y, int direction, float moveSpeed)
{
	int textureWidth, textureHeight; // Vari�veis auxiliares para medir as dimens�es de texturas

	SDL_Surface *surface = IMG_Load(fileName); // Criando uma superficie atrav�s da imagem
	texture = SDL_CreateTextureFromSurface(renderTarget, surface);  // Renderizando uma textura a partir desta superf�cie
	SDL_FreeSurface(surface); // Desalocando a superf�cie
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight); // Medindo as dimens�es das texturas e armazendando elas em textureWidth e textureHeight

	// Inicializando o ret�ngulo de corte
	cropRect.x = 0;
	cropRect.y = 0;
	cropRect.w = textureWidth;
	cropRect.h = textureHeight;

	// Inicializando o ret�ngulo de posi��o
	positionRect.x = x;
	positionRect.y = y;
	positionRect.w = textureWidth;
	positionRect.h = textureHeight;

	// Inicializando as vari�veis de velocidade e sentido do ve�culo
	this->moveSpeed = moveSpeed;
	this->direction = direction;

}

// Destrutor da classe Veichle
Veichle::~Veichle()
{
	SDL_DestroyTexture(texture); // Desaloca a textura do ve�culo
}

/* Fun��o que atualiza e move os ve�culos
* Par�metros:
*   delta: varia��o do tempo
*   levelWidth: Largura do mapa
*   speedMultiplyer: Multiplicador de velocidade do ve�culo
*/
void Veichle::Update(float delta, int levelWidth, float speedMultiplyer)
{
	positionRect.x += moveSpeed * speedMultiplyer * direction * delta; // Movimenta o ve�culo

	if (direction == 1) // Se o sentido do carro � para a direita
	{
		if (positionRect.x > levelWidth) // Se o carro sumir do mapa
			positionRect.x = -cropRect.w; // Retorna ele para o in�cio
	}
	else // Se o sentido do carro � para a esquerda
	{
		if (positionRect.x < -cropRect.w) // Se o carro sair do mapa
			positionRect.x = levelWidth + cropRect.w; // Retorna ele para o fim do mapa
	}
}

/* Fun��o que imprime o ve�culo
* Par�metro:
*   *renderTarget: ponteiro para o renderizador
*/
void Veichle::Draw(SDL_Renderer *renderTarget)
{
	if (direction == -1) // Se o sentido do ve�culo � para a esquerda
		SDL_RenderCopyEx(renderTarget, texture, &cropRect, &positionRect, 0, 0, SDL_FLIP_HORIZONTAL); // Renderiza o ve�culo invertido
	else // Se o sentido � para a direita
		SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect); // Renderiza o ve�culo normalmente

}

// M�todo Get para positionRect
SDL_Rect Veichle::GetPositionRect()
{
	return positionRect;
}


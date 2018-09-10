#include "pch.h"

/* Construtor da classe Veichle
* Parâmetros:
*   fileName: ponteiro para a String do nome do arquivo .png da imagem
*   *renderTarget: ponteiro para o renderizador
*   x: coordenada x inicial da posição do veículo
*   y: coordenada y inicial da posição do veículo
*   direction: sentido do carro: 1 para direita e -1 para a esquerda
*/
Veichle::Veichle(char *fileName, SDL_Renderer *renderTarget, int x, int y, int direction, float moveSpeed)
{
	int textureWidth, textureHeight; // Variáveis auxiliares para medir as dimensões de texturas

	SDL_Surface *surface = IMG_Load(fileName); // Criando uma superficie através da imagem
	texture = SDL_CreateTextureFromSurface(renderTarget, surface);  // Renderizando uma textura a partir desta superfície
	SDL_FreeSurface(surface); // Desalocando a superfície
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight); // Medindo as dimensões das texturas e armazendando elas em textureWidth e textureHeight

	// Inicializando o retângulo de corte
	cropRect.x = 0;
	cropRect.y = 0;
	cropRect.w = textureWidth;
	cropRect.h = textureHeight;

	// Inicializando o retângulo de posição
	positionRect.x = x;
	positionRect.y = y;
	positionRect.w = textureWidth;
	positionRect.h = textureHeight;

	// Inicializando as variáveis de velocidade e sentido do veículo
	this->moveSpeed = moveSpeed;
	this->direction = direction;

}

// Destrutor da classe Veichle
Veichle::~Veichle()
{
	SDL_DestroyTexture(texture); // Desaloca a textura do veículo
}

/* Função que atualiza e move os veículos
* Parâmetros:
*   delta: variação do tempo
*   levelWidth: Largura do mapa
*   speedMultiplyer: Multiplicador de velocidade do veículo
*/
void Veichle::Update(float delta, int levelWidth, float speedMultiplyer)
{
	positionRect.x += moveSpeed * speedMultiplyer * direction * delta; // Movimenta o veículo

	if (direction == 1) // Se o sentido do carro é para a direita
	{
		if (positionRect.x > levelWidth) // Se o carro sumir do mapa
			positionRect.x = -cropRect.w; // Retorna ele para o início
	}
	else // Se o sentido do carro é para a esquerda
	{
		if (positionRect.x < -cropRect.w) // Se o carro sair do mapa
			positionRect.x = levelWidth + cropRect.w; // Retorna ele para o fim do mapa
	}
}

/* Função que imprime o veículo
* Parâmetro:
*   *renderTarget: ponteiro para o renderizador
*/
void Veichle::Draw(SDL_Renderer *renderTarget)
{
	if (direction == -1) // Se o sentido do veículo é para a esquerda
		SDL_RenderCopyEx(renderTarget, texture, &cropRect, &positionRect, 0, 0, SDL_FLIP_HORIZONTAL); // Renderiza o veículo invertido
	else // Se o sentido é para a direita
		SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect); // Renderiza o veículo normalmente

}

// Método Get para positionRect
SDL_Rect Veichle::GetPositionRect()
{
	return positionRect;
}


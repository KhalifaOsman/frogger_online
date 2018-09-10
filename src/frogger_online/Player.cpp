#include<pch.h>

/* Costrutor da classe Player
* Parâmetros:
*   *renderTarget: ponteiro para o renderizador;
*   x: coordenada x inicial do jogador;
*   y, coordenada y inicial do jogador
*/
Player::Player(SDL_Renderer *renderTarget, int x, int y, int initial_direction)
{
	SDL_Surface *surface; // Superfície para renderizar as texturas do sapo

	// Renderização dos estágios da animação do sapo virado para baixo
	surface = IMG_Load("Textures/Down_Stage 1.png");
	texture[0][0] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Down_Stage 2.png");
	texture[0][1] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Down_Stage 3.png");
	texture[0][2] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Down_Stage 4.png");
	texture[0][3] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Down_Stage 5.png");
	texture[0][4] = SDL_CreateTextureFromSurface(renderTarget, surface);

	// Renderização dos estágios da animação do sapo virado a a esquerda
	surface = IMG_Load("Textures/Left_Stage 1.png");
	texture[1][0] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Left_Stage 2.png");
	texture[1][1] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Left_Stage 3.png");
	texture[1][2] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Left_Stage 4.png");
	texture[1][3] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Left_Stage 5.png");
	texture[1][4] = SDL_CreateTextureFromSurface(renderTarget, surface);

	// Renderização dos estágios da animação do sapo virado para a direita
	surface = IMG_Load("Textures/Right_Stage 1.png");
	texture[2][0] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Right_Stage 2.png");
	texture[2][1] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Right_Stage 3.png");
	texture[2][2] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Right_Stage 4.png");
	texture[2][3] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Right_Stage 5.png");
	texture[2][4] = SDL_CreateTextureFromSurface(renderTarget, surface);

	// Renderização dos estágios da animação do sapo virado para cima
	surface = IMG_Load("Textures/Up_Stage 1.png");
	texture[3][0] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Up_Stage 2.png");
	texture[3][1] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Up_Stage 3.png");
	texture[3][2] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Up_Stage 4.png");
	texture[3][3] = SDL_CreateTextureFromSurface(renderTarget, surface);
	surface = IMG_Load("Textures/Up_Stage 5.png");
	texture[3][4] = SDL_CreateTextureFromSurface(renderTarget, surface);

	SDL_FreeSurface(surface); // Desalocamento da superfície

	// Setando os tamanhos do retângulo dos diferentes estágios da animação do sapo
	frogSize[0][0].w = frogSize[1][0].h = frogSize[2][0].h = frogSize[3][0].w = 51;
	frogSize[0][0].h = frogSize[1][0].w = frogSize[2][0].w = frogSize[3][0].h = 36;
	frogSize[0][1].w = frogSize[1][1].h = frogSize[2][1].h = frogSize[3][1].w = 57;
	frogSize[0][1].h = frogSize[1][1].w = frogSize[2][1].w = frogSize[3][1].h = 40;
	frogSize[0][2].w = frogSize[1][2].h = frogSize[2][2].h = frogSize[3][2].w = 56;
	frogSize[0][2].h = frogSize[1][2].w = frogSize[2][2].w = frogSize[3][2].h = 51;
	frogSize[0][3].w = frogSize[1][3].h = frogSize[2][3].h = frogSize[3][3].w = 53;
	frogSize[0][3].h = frogSize[1][3].w = frogSize[2][3].w = frogSize[3][3].h = 64;
	frogSize[0][4].w = frogSize[1][4].h = frogSize[2][4].h = frogSize[3][4].w = 56;
	frogSize[0][4].h = frogSize[1][4].w = frogSize[2][4].w = frogSize[3][4].h = 73;

	// Recebendo as posições iniciais do jogador
	initialX = x;
	initialY = y;

	// Inicializando o retângulo de posição
	positionRect.x = x;
	positionRect.y = y;
	positionRect.h = 79;
	positionRect.w = 79;

	// Inicializando o retângulo de corte
	cropRect.x = 0;
	cropRect.y = 0;
	cropRect.w = 79;
	cropRect.h = 79;

	croak = Mix_LoadWAV("Sound/Croak.ogg"); // Abrindo arquivo do som do sapo

	moveSpeed = 300.0f; // Inicializando a velocidade do sapo

	// Inicializando o estágio de animação do sapo
	this->initial_direction = initial_direction;
	currentDirection = initial_direction;
	currentFrame = 0;

	// Como o sapo ainda não pulou, hasJumped recebe falso
	hasJumped = false;
}

// Destrutor da classe Player
Player::~Player()
{
	// Desalocação da matriz de texuturas da animação do sapo
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 4; j++)
		{
			SDL_DestroyTexture(texture[i][j]);
		}
	}

	Mix_FreeChunk(croak); // Desalocamento do som do sapo
}

/* Função para mover e atualizar as animações do sapo
* Parâmetros:
*   delta: variação do tempo
*   *keystate: ponteiro para a tecla apertada
*   levelWidth: a expressura do mapa
*   levelHeight: a altura do mapa
*/
/* Função para mover e atualizar as animações do sapo
* Parâmetros:
*   delta: variação do tempo
*   *keystate: ponteiro para a tecla apertada
*   levelWidth: a expressura do mapa
*   levelHeight: a altura do mapa
*/
int Player::Update(float delta, const Uint8 *keyState, int levelWidth, int levelHeight)
{
	isActive = true; // Considera-se que o jogador se moveu
	int retorno = -1;

	if (keyState[SDL_SCANCODE_RIGHT]) // Se o jogador apertou a seta direita
	{
		positionRect.x += moveSpeed * delta; // Posição do sapo se move para a direita
		currentDirection = retorno = 2; // Animação se vira para a direita
		if (positionRect.x > levelWidth - cropRect.w) // Se o Jogador sair do mapa
			positionRect.x = levelWidth - cropRect.w; // Reposiciona para dentro do mapa
	}
	else if (keyState[SDL_SCANCODE_LEFT]) // Mesmo algoritmo para se o jogador apertou a seta esquerda
	{
		positionRect.x -= moveSpeed * delta;
		currentDirection = retorno = 1;
		if (positionRect.x < 0)
			positionRect.x = 0;
	}
	else if (keyState[SDL_SCANCODE_UP]) // Mesmo algoritmo para se o jogador apertou a seta de cima
	{
		positionRect.y -= moveSpeed * delta;
		currentDirection = retorno = 3;
		if (positionRect.y < 0)
			positionRect.y = 0;
		 
	}
	else if (keyState[SDL_SCANCODE_DOWN]) // Mesmo algoritmo para se o jogador apertou a seta de baixo
	{
		positionRect.y += moveSpeed * delta;
		currentDirection = retorno = 0;
		if (positionRect.y > levelHeight - cropRect.h)
			positionRect.y = levelHeight - cropRect.h;
	}
	else
		isActive = false; // Se o jogador não apertou nada, isActive recebe falso

	if (isActive) // Se isActive é verdadeiro
	{
		frameCounter += delta; // FrameCounter acumula a variação do tempo
		if (hasJumped) // Se o sapo está na metade do estágio da animação
		{
			if (frameCounter >= 0.05f) // Se passaram 0.5 millisegundos deste a última animacao
			{
				frameCounter = 0; // Zera frameCounter
				currentFrame--; // Muda o estágio da animaçãp
				if (currentFrame == 0) // Se a frame atual é do sapo sentado
					hasJumped = false; // Sapo não pulou
			}
		}
		else // Senao executa o mesmo algoritmo para a outra metade da animação
		{
			if (frameCounter >= 0.05f)
			{
				frameCounter = 0;
				currentFrame++;
				if (currentFrame == 4) {
					hasJumped = true;
					Mix_PlayChannel(-1, croak, 0);
				}
			}
		}
	}
	else // Caso o jogador pare de se mover na metade de uma animação
	{
		//Volta-se para a frame inicial
		frameCounter = 0;
		currentFrame = 0;
		hasJumped = false;
	}
	
	return retorno;
}

/* Função para mover e atualizar as animações do sapo
* Parâmetros:
*   delta: variação do tempo
*   *keystate: ponteiro para a tecla apertada
*   levelWidth: a expressura do mapa
*   levelHeight: a altura do mapa
*/
void Player::Update2(float delta, int levelWidth, int levelHeight, int direction)
{
	if (direction >= 0 && direction <= 3) // Se isActive é verdadeiro
	{
		this->currentDirection = direction;
		frameCounter += delta; // FrameCounter acumula a variação do tempo
		if (hasJumped) // Se o sapo está na metade do estágio da animação
		{
			if (frameCounter >= 0.05f) // Se passaram 0.5 millisegundos deste a última animacao
			{
				frameCounter = 0; // Zera frameCounter
				currentFrame--; // Muda o estágio da animaçãp
				if (currentFrame == 0) // Se a frame atual é do sapo sentado
					hasJumped = false; // Sapo não pulou
			}
		}
		else // Senao executa o mesmo algoritmo para a outra metade da animação
		{
			if (frameCounter >= 0.05f)
			{
				frameCounter = 0;
				currentFrame++;
				if (currentFrame == 4) {
					hasJumped = true;
					Mix_PlayChannel(-1, croak, 0);
				}
			}
		}
	}
	else // Caso o jogador pare de se mover na metade de uma animação
	{
		//Volta-se para a frame inicial
		frameCounter = 0;
		currentFrame = 0;
		hasJumped = false;
	}
}

/* Função que renderiza o jogador centralizado na tela
* Parâmetros:
*   *renderTarger: ponteiro para o renderizador
*   *cameraRect: retângulo da câmera
*/
void Player::Draw(SDL_Renderer *renderTarget, SDL_Rect cameraRect, bool center_camera)
{
	if (center_camera) {
		SDL_Rect drawingRect = { positionRect.x - cameraRect.x, positionRect.y - cameraRect.y, positionRect.w, positionRect.h };  // Retângulo que evita esticar a tela
		SDL_RenderCopy(renderTarget, texture[currentDirection][currentFrame], &cropRect, &drawingRect); // Imprime o jogador na tela com o novo retângulo
	} else {
		SDL_RenderCopy(renderTarget, texture[currentDirection][currentFrame], &cropRect, &positionRect); // Imprime o jogador na tela
	}
}

/* Função de detectação de colisões
* Parâmetros:
*   veichlePosition: retângulo da posição do veículo
*   isMosquto: verdadeiro se for para mosquito, falso para carros;
*/
bool Player::IntersectsWith(SDL_Rect veichlePosition, bool isMosquito)
{
	// Calculo de colisão através do método de retângulos
	int deltaX = (cropRect.w - frogSize[currentDirection][currentFrame].w) / 2;
	int deltaY = (cropRect.h - frogSize[currentDirection][currentFrame].h) / 2;

	// Se hover colisão
	if (positionRect.x + deltaX + frogSize[currentDirection][currentFrame].w < veichlePosition.x || positionRect.x + deltaX > veichlePosition.x + veichlePosition.w ||
		positionRect.y + deltaY + frogSize[currentDirection][currentFrame].h < veichlePosition.y || positionRect.y + deltaY > veichlePosition.y + veichlePosition.h)
		return false; // Retorna falso
	else // Se collidiu
	{
		if (!isMosquito) // Se não é mosquito
		{
			//Reposiciona o jogador
			positionRect.x = initialX;
			positionRect.y = initialY;
			currentDirection = initial_direction;
			hasJumped = false;
		}
		return true; // Senão retorna verdade para collidiu
	}
}

int Player::get_current_direction()
{
	return this->currentDirection;
}
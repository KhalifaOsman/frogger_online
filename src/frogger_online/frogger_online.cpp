#include "pch.h"

using namespace std;

typedef struct _Menu_Package {
	bool will_play;
} Menu_Package;

/*
* Função que cria uma textura de texto
* Parâmetros:
*   *renderTarget: ponteiro para o renderizador
*   *font: ponteiro para a variável de fonte
*   *word: ponteiro para a palavra a ser renderizado
*   number: número inteiro a ser renderizado
*   isSpeed: variável booleana que apenas será verdade quando for imprimir a porcentagem de velocidade
* Retorno:
*   Endereço da variável de textura renderizada
*/
SDL_Texture *createFontTexture(SDL_Renderer *renderTarget, TTF_Font *font, char *word, int number, bool isSpeed)
{
	char frase[50]; // String da frase a ser construída
	char stringNumber[30]; // String do numero a ser inserido na string "frase"
	SDL_Color color = { 255, 255, 255, 255 }; // Variável se struct que contêm 4 números inteiros correspondentes ás cores RGB e a transparência da fonte

	strcpy_s(frase, word); // Copia a string "word" na string "frase"

	if (number >= 0) // Flag para determinar se precisa-se inserir um número na "frase"
	{
		_itoa_s(number, stringNumber, 10); // Converte o número "number" para a string "stringNumber" na base 10
		strcat_s(frase, stringNumber); //Concatena "stringNumber" com a string "frase"
	}

	if (isSpeed) // Verifica se precisa inserir o caracter "%" na frase, que somente acontecerá para a impressão da velocidade
		strcat_s(frase, "%"); // Insere o caracter "%" na string "frase"

	SDL_Surface *surface = TTF_RenderText_Blended(font, frase, color); // Cria a superfície da frase
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderTarget, surface); // Renderiza a textura a partir da superfície criada

	SDL_FreeSurface(surface); // Desaloca a superfície criada

	return texture; // Retorno da superfície criada
}

/* Menu Principal
* Parâmetros:
*   *window: ponteiro para a janela do jogo
*   *renderTarget: ponteiro para o renderizador
*   *highscore: ponteiro para a variável highscore
*   screenWidth: largura da tela
*   screenHeight: altura da tela
* Retorno:
*   Verdadeiro se o jogador apertar espaço e Falso se ele fechar a janela
*/
bool mainMenu(SDL_Window *window, SDL_Renderer *renderTarget, int highscore, int screenWidth, int screenHeight, SDLNet_SocketSet connection, TCPsocket socket)
{
	int textureWidth, textureHeight; // Variáveis auxiliares para medir as dimensões de texturass renderizadas

	SDL_Surface *surface = IMG_Load("Textures/Map - Blur.jpg"); // Carrega a superfície do mapa borrado
	SDL_Texture *bottomTexture = SDL_CreateTextureFromSurface(renderTarget, surface); // Renderiza uma textura a partir desta superfície
	SDL_FreeSurface(surface); // Desaloca esta superfície
	SDL_QueryTexture(bottomTexture, NULL, NULL, &textureWidth, &textureHeight); // Mede as dimensões desta textura e armazena elas em textureWidth e textureHeight
	SDL_Rect cameraRect = { textureWidth / 2 - screenWidth / 2, textureHeight - screenHeight, screenWidth, screenHeight }; // Cria um retângulo da câmera do jogo com as dimensões medidas
	
	TTF_Font *font = TTF_OpenFont("Font/riffic-bold.otf", 120); // Abertura da fonte do título
	SDL_Texture *fontTexture = createFontTexture(renderTarget, font, (char *) "Frogger - SDL", -1, false); // Renderiza a textura desta fonte
	SDL_QueryTexture(fontTexture, NULL, NULL, &textureWidth, &textureHeight); // Mede as dimensões desta fonte e armazena em textureWidth e textureHeight
	SDL_Rect fontRect = { screenWidth / 2 - textureWidth / 2, 120, textureWidth, textureHeight }; // Cria um retângulo com as dimensões e a posição da mensagem na tela

	// Executa o mesmo processo para a mensagem da highscore
	TTF_Font *fontHighscore = TTF_OpenFont("Font/riffic-bold.otf", 40);
	SDL_Texture *fontHighscoreTexture = createFontTexture(renderTarget, fontHighscore, (char *) "Highscore : ", highscore, false);
	SDL_QueryTexture(fontHighscoreTexture, NULL, NULL, &textureWidth, &textureHeight);
	SDL_Rect fontHighscoreRect = { screenWidth / 2 - textureWidth / 2, 360, textureWidth, textureHeight };

	// Executa o mesmo processo para a mensagem de apertar espaço
	TTF_Font *fontSpace = TTF_OpenFont("Font/riffic-bold.otf", 40);
	SDL_Texture *fontSpaceTexture = createFontTexture(renderTarget, fontSpace, (char *) "Press Space to Start or Escape to Quit", -1, false);
	SDL_QueryTexture(fontSpaceTexture, NULL, NULL, &textureWidth, &textureHeight);
	SDL_Rect fontSpaceRect = { screenWidth / 2 - textureWidth / 2, 540, textureWidth, textureHeight };

	// Abre o arquivo da música do menu
	Mix_Music *menuMusic = Mix_LoadMUS("Sound/Menu Music.ogg");

	bool isRunning = true; // isRunning para verificar a continuidade do menu e willplay para verificar se o jogador aperta espaço
	SDL_Event event; // Variável de evento para a leitura do teclado

	Menu_Package this_player = {false};
	Menu_Package other_player = {false};
	bool other_player_decided = false;

	while (isRunning) // Ciclo principal do menu
	{
		if (!Mix_PlayingMusic()) // Verifica se alguma música está tocando
			Mix_PlayMusic(menuMusic, 0); // Se falso, exucuta a música do menu

		while (SDL_PollEvent(&event) != 0) // Ciclo que só executa se o jogador aperta algo
		{
			if (event.type == SDL_QUIT) // Se o jogador apertar a caixinha de fechar "X" da janela
				isRunning = false; // Flag para quebrar o ciclo do menu
			else if (event.type == SDL_KEYDOWN) // Se o jogador apertou o teclado
			{
				if (event.key.keysym.sym == SDLK_SPACE) // Se o jogador apertou espaço
				{
					isRunning = false; // Flag para quebrar o ciclo principal do menu
					this_player.will_play = true; // Flag para determinar que o jogador apertou espaço
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE) // Senão se o jogador apertou escape
				{
					isRunning = false; // Flag para quebrar o ciclo principal do menu
					this_player.will_play = false; // Flag para determinar que o jogador apertou escape
				}
			}
		} // Fim do ciclo que só executa se o jogador aperta algo
		
		if (Data::listen(connection, socket, &other_player, sizeof(Menu_Package))) {
			other_player_decided = true;
			if (!other_player.will_play) {
				isRunning = false;
				this_player.will_play = false;
			}
		}

		SDL_RenderClear(renderTarget); // Limpa a tela
		SDL_RenderCopy(renderTarget, bottomTexture, &cameraRect, NULL); // Copia a textura do mapa borrado na tela
		SDL_RenderCopy(renderTarget, fontTexture, NULL, &fontRect); // Copia a textura do título na tela
		SDL_RenderCopy(renderTarget, fontHighscoreTexture, NULL, &fontHighscoreRect); // Copia a textura da highscore na tela
		SDL_RenderCopy(renderTarget, fontSpaceTexture, NULL, &fontSpaceRect); // Copia a textura da mensagem na tela
		SDL_RenderPresent(renderTarget); // Mostra a tela atualizada
	} // Fim do ciclo principal do menu

	// Retorno de willPlay: verdadeiro se o jogador aperta espaço e falso se o jogador fecha o jogo
	SDLNet_TCP_Send(socket, &this_player, sizeof(Menu_Package));
	if (this_player.will_play && !other_player_decided) {
		while (!Data::listen(connection, socket, &other_player, sizeof(Menu_Package))) {
			if (!Mix_PlayingMusic()) // Verifica se alguma música está tocando
				Mix_PlayMusic(menuMusic, 0); // Se falso, exucuta a música do menu
		}
		this_player.will_play = other_player.will_play;
	}

	Mix_HaltMusic(); // Para a Música

	// Desalocamento das fontes
	TTF_CloseFont(font);
	TTF_CloseFont(fontHighscore);
	TTF_CloseFont(fontSpace);

	// Desalocamento das músicas
	Mix_FreeMusic(menuMusic);

	// Desalocamento das texturas
	SDL_DestroyTexture(fontTexture);
	SDL_DestroyTexture(fontHighscoreTexture);
	SDL_DestroyTexture(fontSpaceTexture);
	SDL_DestroyTexture(bottomTexture);

	if (this_player.will_play && other_player.will_play)
		return true;
	else
		return false;
}

/* Função principal do jogo
* Parâmetros:
*   *window: ponteiro para a tela do jogo
*   *renderTarget: ponteiro para o renderizador
*   *highscore: ponteiro para a variável Highscore
*   screenWidth: largura da tela
*   screenHeight: altura da tela
* Retorno:
*   Verdadeiro se o jogador perde ou aperta "escape" e falso se ele fechar o jogo
*/
bool game(SDL_Window *window, SDL_Renderer *renderTarget, int *highscore, int screenWidth, int screenHeight, SDLNet_SocketSet connection, TCPsocket socket, bool is_server)
{
	int score = 0, scoreCounter = 0; // Variáveis: remaining_time para vidas, score para pontuação e um auxiliar para verificar se o jogador pegou 5 moscas consecutivas
	int currentTime = 0; // Variável que contem o tempo atual
	int prevTime = 0; // Variável que contem o tempo anterior á execução do tempo atual
	int levelWidth, levelHeight; // Variáveis auxiliares para medir as dimensões de texutras
	float delta = 0.0f, deltaDeathTime = 10.0f, speedMultiplyer = 1.0f; // Variáveis: delta para conter a variação do tempo, deltaDeathTime para medir o tempo que se passa depois do jogador morrer e speedMuliplyer para conter velocidade dos carros
	const Uint8 *keyState; // Variável que contem a entrada do teclado
	bool  gameStarted = false, first_buzz = true; // GameOver verifica se o jogador perdeu ou apertou esc. GameStarted impede que o jogador se movimente antes de carregar a primeira janela
	int total_time = 60, start_time, remaining_time = 60;
	SDL_Rect cameraRect = { 0, 0, screenWidth, screenHeight }; // Retângulo da câmera do jogo, resolução 1280 x 720
	int caught_mosquitos = 0;

	SDL_Surface *surface = IMG_Load("Textures/Map.png"); // Carrega a superfície do mapa do jogo
	SDL_Texture *landscape = SDL_CreateTextureFromSurface(renderTarget, surface); // Renderiza a textura do mapa a partir desta superfície
	SDL_QueryTexture(landscape, NULL, NULL, &levelWidth, &levelHeight); // Mede as dimensões desta textura e armazena em levelWidth e levelHeight
	SDL_FreeSurface(surface); // Desaloca esta superfície

	TTF_Font *font = TTF_OpenFont("Font/riffic-bold.otf", 30); // Abre as fontes das mensagens do jogo

	SDL_Texture *remaining_timeFontTexture = createFontTexture(renderTarget, font, (char *) "Time x ", remaining_time, false); // Chama função createFontTexture() para criar a textura da mensagem de vidas
	SDL_Rect remaining_timeFontRect = { 20, 0, 0, 0 }; // Retângulo que posiciona a mensagem em seu devido lugar da tela
	SDL_QueryTexture(remaining_timeFontTexture, NULL, NULL, &remaining_timeFontRect.w, &remaining_timeFontRect.h); // Mede as dimensões da textura e armazena elas neste retângulo

	// Mesmo processo para a mensagem de pontuação
	SDL_Texture *scoreFontTexture = createFontTexture(renderTarget, font, (char *) "Score : ", score, false);
	SDL_Rect scoreFontRect = { 1000, 0, 0, 0 };
	SDL_QueryTexture(scoreFontTexture, NULL, NULL, &scoreFontRect.w, &scoreFontRect.h);

	// Mesmo processo para a mensagem de velocidade dos carros
	SDL_Texture *speedFontTexture = createFontTexture(renderTarget, font, (char *) "Speed : ", speedMultiplyer * 100, true);
	SDL_Rect speedFontRect = { 500, 0, 0, 0 };
	SDL_QueryTexture(speedFontTexture, NULL, NULL, &speedFontRect.w, &speedFontRect.h);

	// Mesmo processo para a mesagem de "GAME OVER", posicionando ela no meio da tela
	SDL_Texture *GameOverFontTexture = createFontTexture(renderTarget, font, (char *) "You Suck!", -1, false);
	SDL_Rect GameOverFontRect;
	SDL_QueryTexture(GameOverFontTexture, NULL, NULL, &GameOverFontRect.w, &GameOverFontRect.h);
	GameOverFontRect.x = cameraRect.w / 2 - GameOverFontRect.w / 2;
	GameOverFontRect.y = cameraRect.h / 2 - GameOverFontRect.h / 2;

	// Mesmo processo para a mesagem de "GAME OVER", posicionando ela no meio da tela
	SDL_Texture *GameOverFontTexture_win = createFontTexture(renderTarget, font, (char *) "You Win!", -1, false);
	SDL_Rect GameOverFontRect_win;
	SDL_QueryTexture(GameOverFontTexture_win, NULL, NULL, &GameOverFontRect_win.w, &GameOverFontRect_win.h);
	GameOverFontRect_win.x = cameraRect.w / 2 - GameOverFontRect_win.w / 2;
	GameOverFontRect_win.y = cameraRect.h / 2 - GameOverFontRect_win.h / 2;

	// Mesmo processo para a mesagem de "GAME OVER", posicionando ela no meio da tela
	SDL_Texture *DrawFontTexture = createFontTexture(renderTarget, font, (char *) "Draw!", -1, false);
	SDL_Rect DrawFontRect;
	SDL_QueryTexture(DrawFontTexture, NULL, NULL, &DrawFontRect.w, &DrawFontRect.h);
	DrawFontRect.x = cameraRect.w / 2 - DrawFontRect.w / 2;
	DrawFontRect.y = cameraRect.h / 2 - DrawFontRect.h / 2;

	// Textura auxiliar
	SDL_Texture *auxTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, levelWidth, levelHeight);

	// Abertura das músicas e sons do jogo
	Mix_Music *backgroundMusic = Mix_LoadMUS("Sound/Game Music.ogg");
	Mix_Music *gameOverTune = Mix_LoadMUS("Sound/Game Over.ogg");
	Mix_Chunk *lick = Mix_LoadWAV("Sound/Lick.ogg");
	Mix_Chunk *horn = Mix_LoadWAV("Sound/Horn.ogg");

	int this_plater_y;
	int other_plater_y;
	int this_player_direct;
	int other_player_direct;

	// Instanciamento do Objeto player1 de classe Player
	if (is_server) {
		this_plater_y = levelHeight - 81;
		other_plater_y = 0;
		this_player_direct = 3;
		other_player_direct = 0;
	}
	else {
		this_plater_y = 0;
		other_plater_y = levelHeight - 81;
		this_player_direct = 0;
		other_player_direct = 3;
	}

	Player player1(renderTarget, levelWidth / 2 - 39, this_plater_y, this_player_direct);
	Player player2(renderTarget, levelWidth / 2 - 39, other_plater_y, other_player_direct);

	// Instanciamento dos Objetos veículos da classe Veichle
	Veichle veichle1((char *) "Textures/Green Car.png", renderTarget, 0, 723, -1, 500.0f);
	Veichle veichle2((char *) "Textures/Fire Fighter.png", renderTarget, 654, 642, 1, 350.0f);
	Veichle veichle3((char *) "Textures/Yellow Car.png", renderTarget, levelWidth, 562, -1, 550.0f);
	Veichle veichle4((char *) "Textures/Big Truck.png", renderTarget, -284, 482, 1, 250.0f);
	Veichle veichle5((char *) "Textures/Blue Car.png", renderTarget, 425, 326, 1, 450.0f);
	Veichle veichle6((char *) "Textures/Big Truck.png", renderTarget, 575, 248, -1, 250.0f);
	Veichle veichle7((char *) "Textures/Yellow Car.png", renderTarget, -134, 165, 1, 550.0f);
	Veichle veichle8((char *) "Textures/Fire Fighter.png", renderTarget, levelWidth, 84, -1, 350.0f);

	// Instanciamento do objeto buzz da classe Mosquito
	Mosquito buzz(renderTarget);
	Data *my_data = new Data(player1.positionRect, buzz.getPositionRect());
	Data *his_data = new Data(player2.positionRect, buzz.getPositionRect());

	SDL_Event event; // Variável de evento para a leitura do teclado

	start_time = SDL_GetTicks();

	// Ciclo principal do jogo
	while (my_data->is_playing && his_data->is_playing)
	{
		if (!Mix_PlayingMusic() && !my_data->GameOver) // Se não está tocando música e se o jogo não acabou
			Mix_PlayMusic(backgroundMusic, 0); // Toca a música do jogo

		prevTime = currentTime; // tempo anterior recebe o tempo atual
		currentTime = SDL_GetTicks(); // tempo atual atualiza com o relógio da placa mãe
		delta = (currentTime - prevTime) / 1000.0f; // Diferença entre o tempo atual e anterior dividido por 1000, convertendo millisegundos para segundos
		deltaDeathTime += delta; // Acumulação do tempo após a morte do jogador

		while (SDL_PollEvent(&event) != 0) // Ciclo que só executa se o jogador pressionar algo
		{
			if (event.type == SDL_QUIT) // Se o jogado fechar a janela
			{
				my_data->is_playing = false; // Flag para quebrar o ciclo principal do jogo
				my_data->GameOver = false; // Flag para indicar para não retornar ao menu principal
			}
			else if (event.type == SDL_KEYDOWN) // Se o jogador apertou o teclado
			{
				if (event.key.keysym.sym == SDLK_ESCAPE) // Se o jogador apertou esc
				{
					Mix_HaltMusic(); // Pára a música
					my_data->GameOver = true; // Flag para indicar para voltar ao menu principal
					my_data->is_playing = false; // Flag para quebrar o ciclo do principal do jogo
				}
			}
		} // Fim do ciclo que só executa se o jogador pressionar algo

		keyState = SDL_GetKeyboardState(NULL); // keystate recebe qualquer botão do teclado inserido

		if (!my_data->GameOver && deltaDeathTime > 1.0f && gameStarted) { // Se o jogo não acabou e se passou um segundo após a última morte do jogador e se o primeiro ciclo do jogo foi executada
			my_data->direction = player1.Update(delta, keyState, levelWidth, levelHeight); // Atualizar a posição do jogador
			my_data->ready = true;
		} else
			my_data->ready = false;

		if (gameStarted && his_data->ready) {
			player2.Update2(delta, levelWidth, levelHeight, his_data->direction);
		}

		remaining_time = total_time - (currentTime - start_time) / 1000.0f;

		if (!my_data->GameOver && remaining_time < 1) // Se as vidas do jogador acabarem
		{
			Mix_HaltMusic(); // Para de tocar a musica do jogo
			my_data->GameOver = true; // Flag para indicar para voltar ao menu principal
			deltaDeathTime = 0.0f;
			remaining_time = 0;
		}
		else if (remaining_time < 1)
			remaining_time = 0;

		// Atualização da mensagem de vidas
		SDL_DestroyTexture(remaining_timeFontTexture);
		remaining_timeFontTexture = createFontTexture(renderTarget, font, (char *) "Time x ", remaining_time, false);
		SDL_QueryTexture(remaining_timeFontTexture, NULL, NULL, &remaining_timeFontRect.w, &remaining_timeFontRect.h);

		// Atualização das posições dos veículos
		if (gameStarted) // Se o primeiro ciclo principal do jogo foi executado
		{
			veichle1.Update(delta, levelWidth, speedMultiplyer);
			veichle2.Update(delta, levelWidth, speedMultiplyer);
			veichle3.Update(delta, levelWidth, speedMultiplyer);
			veichle4.Update(delta, levelWidth, speedMultiplyer);
			veichle5.Update(delta, levelWidth, speedMultiplyer);
			veichle6.Update(delta, levelWidth, speedMultiplyer);
			veichle7.Update(delta, levelWidth, speedMultiplyer);
			veichle8.Update(delta, levelWidth, speedMultiplyer);
		}
		// Se o jogador collidir com qualquer um dos carros e se o jogo não acabou
		if ((player1.IntersectsWith(veichle1.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle2.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle3.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle4.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle5.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle6.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle7.GetPositionRect(), false) ||
			 player1.IntersectsWith(veichle8.GetPositionRect(), false)
			 && !my_data->GameOver))
		{
			Mix_PlayChannel(-1, horn, 0); // Toca o som da buzina do carro

			deltaDeathTime = 0.0f; // Inicia-se a contagem do deltaDeathTime

			// Atualiza a mensagem do número de vidas
			SDL_DestroyTexture(remaining_timeFontTexture);
			remaining_timeFontTexture = createFontTexture(renderTarget, font, (char *) "Time x ", remaining_time, false);
			SDL_QueryTexture(remaining_timeFontTexture, NULL, NULL, &remaining_timeFontRect.w, &remaining_timeFontRect.h);
		}

		my_data->update_mosquito = false;

		if (player1.IntersectsWith(buzz.getPositionRect(), true)) // Se o jogador pegou uma mosca
		{
			Mix_PlayChannel(-1, lick, 0); // Toca o som do mosquito

			score = score + 100 * speedMultiplyer; // Ganho de pontos
			scoreCounter++; // Incremento de scoreCounter
			caught_mosquitos++;

			// Atualização da mensagem da pontuação
			SDL_DestroyTexture(scoreFontTexture);
			scoreFontTexture = createFontTexture(renderTarget, font, (char *) "Score : ", score, false);
			SDL_QueryTexture(scoreFontTexture, NULL, NULL, &scoreFontRect.w, &scoreFontRect.h);

			if (caught_mosquitos % 5 == 0) // Se o jogador pegou cinco moscas consecutivas
			{
				total_time += 60;

				speedMultiplyer = speedMultiplyer * 1.2f; // Incremento de 20% na velocidade dos carros

				// Atualização da mensagem da velocidade
				SDL_DestroyTexture(speedFontTexture);
				speedFontTexture = createFontTexture(renderTarget, font, (char *) "Speed : ", speedMultiplyer * 100, true);
				SDL_QueryTexture(speedFontTexture, NULL, NULL, &speedFontRect.w, &speedFontRect.h);
			}

			// Reposicionamento do mosquito
			buzz.newPosition();
			my_data->new_mosquito_posic = buzz.getPositionRect();
			my_data->update_mosquito = true;
		}

		if (!my_data->GameOver) // Se o jogo não acabou
		{
			// Centraliza a câmera no jogador
			cameraRect.x = player1.positionRect.x + 79 / 2 - screenWidth / 2;
			cameraRect.y = player1.positionRect.y + 79 / 2 - screenHeight / 2;
		}

		// Algoritmo que evita que a câmera saia do mapa do jogo
		if (cameraRect.x < 0)
			cameraRect.x = 0;
		if (cameraRect.y < 0)
			cameraRect.y = 0;
		if (cameraRect.x + cameraRect.w >= levelWidth)
			cameraRect.x = levelWidth - screenWidth;
		if (cameraRect.y + cameraRect.h >= levelHeight)
			cameraRect.y = levelHeight - screenHeight;

		SDL_RenderClear(renderTarget); // Limpa a tela
		SDL_SetRenderTarget(renderTarget, auxTexture); // Muda o alvo do renderizador para a textura auxiliar
		SDL_RenderCopy(renderTarget, landscape, NULL, NULL); // Desenha o mapa na textura auxiliar

		buzz.draw(renderTarget); // Desenho o mosquito na texutra auxiliar

		// Desenha os veículos na textura auxiliar
		veichle1.Draw(renderTarget);
		veichle2.Draw(renderTarget);
		veichle3.Draw(renderTarget);
		veichle4.Draw(renderTarget);
		veichle5.Draw(renderTarget);
		veichle6.Draw(renderTarget);
		veichle7.Draw(renderTarget);
		veichle8.Draw(renderTarget);

		player2.Draw(renderTarget, cameraRect, false);

		SDL_SetRenderTarget(renderTarget, NULL); // Retorna o alvo do renderizador para a tela
		SDL_RenderCopy(renderTarget, auxTexture, &cameraRect, NULL); // Renderizar a textura auxiliar na tela

		if (!my_data->GameOver) { // Se o jogo não acabou
			player1.Draw(renderTarget, cameraRect, true); // Desenha o jogador na tela
		} else if (deltaDeathTime) // Senão se passaram 2 segundos após a morte do jogador
		{
			if (!Mix_PlayingMusic()) // Se não está tocando música
				Mix_PlayMusic(gameOverTune, 0); // Tocar a música de "GAME OVER"

			if(score > his_data->score)
				SDL_RenderCopy(renderTarget, GameOverFontTexture_win, NULL, &GameOverFontRect_win); // Exibir a mensagem de "GAME OVER WIN"
			else if(score < his_data->score)
				SDL_RenderCopy(renderTarget, GameOverFontTexture, NULL, &GameOverFontRect); // Exibir a mensagem de "GAME OVER LOOSE"
			else
				SDL_RenderCopy(renderTarget, DrawFontTexture, NULL, &DrawFontRect); // Exibir a mensagem de "GAME OVER DRAW"
		}

		SDL_RenderCopy(renderTarget, scoreFontTexture, NULL, &scoreFontRect); // Renderizar a mensagem de pontução na tela
		SDL_RenderCopy(renderTarget, remaining_timeFontTexture, NULL, &remaining_timeFontRect); // Renderizar a mensagem de vidas na tela
		SDL_RenderCopy(renderTarget, speedFontTexture, NULL, &speedFontRect); // Renderizar a mensagem de velocidade na tela

		SDL_RenderPresent(renderTarget); // Mostrar a tela atualizada

		my_data->position_rect = player1.positionRect;
		my_data->score = score;

		if (is_server) {
			if (Data::listen(connection, socket, his_data, sizeof(Data)))
				SDLNet_TCP_Send(socket, my_data, sizeof(Data));
		}
		else {
			SDLNet_TCP_Send(socket, my_data, sizeof(Data));
			Data::listen(connection, socket, his_data, sizeof(Data));
		}

		player2.positionRect = his_data->position_rect;

		my_data->update_mosquito = false;
		if (his_data->update_mosquito) {
			caught_mosquitos++;
			buzz.setPositionRect(his_data->new_mosquito_posic);
			Mix_PlayChannel(-1, lick, 0); // Toca o som do mosquito

			// Atualização da mensagem da velocidade
			SDL_DestroyTexture(speedFontTexture);
			speedFontTexture = createFontTexture(renderTarget, font, (char *) "Speed : ", speedMultiplyer * 100, true);
			SDL_QueryTexture(speedFontTexture, NULL, NULL, &speedFontRect.w, &speedFontRect.h);

			if (caught_mosquitos % 5 == 0) // Se o jogador pegou cinco moscas consecutivas
			{
				total_time += 60;

				speedMultiplyer = speedMultiplyer * 1.2f; // Incremento de 20% na velocidade dos carros

				// Atualização da mensagem da velocidade
				SDL_DestroyTexture(speedFontTexture);
				speedFontTexture = createFontTexture(renderTarget, font, (char *) "Speed : ", speedMultiplyer * 100, true);
				SDL_QueryTexture(speedFontTexture, NULL, NULL, &speedFontRect.w, &speedFontRect.h);
			}
		}
		if (!is_server && first_buzz) {
			buzz.setPositionRect(his_data->new_mosquito_posic);
			first_buzz = false;
		}

		if (my_data->GameOver && deltaDeathTime > 7.0f) // Se o jogo terminou e se passaram 9 segundos após a última morte do jogador
			my_data->is_playing = false; // Flag para quebrar o ciclo principal do jogo
		
		gameStarted = true; // Flag para indicar que o primeiro ciclo foi executado
	}

	delete my_data;
	delete his_data;

	TTF_CloseFont(font); // Desalocamento da fonte das mensagens

	// Desalocamento dos sons e músicas
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeMusic(gameOverTune);
	Mix_FreeChunk(lick);
	Mix_FreeChunk(horn);

	// Desalocamento das texturas
	SDL_DestroyTexture(GameOverFontTexture);
	SDL_DestroyTexture(speedFontTexture);
	SDL_DestroyTexture(scoreFontTexture);
	SDL_DestroyTexture(remaining_timeFontTexture);
	SDL_DestroyTexture(auxTexture);
	SDL_DestroyTexture(landscape);

	if (score > *highscore) // Se a pontuação do jogador for maior que a highscore
	{
		// Atualização da highscore
		*highscore = score;
		FILE *file = fopen("Highscore.dat", "wb");
		fwrite(highscore, sizeof(int), 1, file);
		fclose(file);
	}
	
	return true;
}

int engine(SDLNet_SocketSet connection, TCPsocket socket, bool is_this_player)
{
	// Abertura do arquivo Highscore.dat e inserindo o seu conteúdo na variável highscore
	int highscore;
	FILE *file = fopen("Highscore.dat", "rb");

	if (file == NULL)
		highscore = 0;
	else {
		fread(&highscore, sizeof(int), 1, file);
		fclose(file);
	}

	//Inicialização das bibliotecas SDL
	SDL_Init(SDL_INIT_VIDEO);
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	IMG_Init(imgFlags);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// Declaração das variável de janela *window com o nome Frogger, centralizada no monitor e com resolução de 1280 x 720
	int screenWidth = 1280, screenHeight = 720;
	SDL_Window *window = SDL_CreateWindow("Frogger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // Muda para o modo tela cheia

	// Declaração de um renderizador com janela como alvo, aceleração da GPU e travado a 60 frames por segundo
	SDL_Renderer *renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	bool isRunning = true; // Variável para verificar o ciclo principal do sistema
	SDL_Event event; // Variável de evento para verificar se o jogador aperta algo

	while (isRunning) // Ciclo principal do sistema
	{
		while (SDL_PollEvent(&event) != 0) // Se o jogador aperta algo
		{
			if (event.type == SDL_QUIT) // Se ele aperta para fechar a janela
				isRunning = false; // Flag para quebrar o ciclo do sistema
		}

		if (mainMenu(window, renderTarget, highscore, screenWidth, screenHeight, connection, socket)) // Se a chamada da função do menu retornar verdadeiro
			isRunning = game(window, renderTarget, &highscore, screenWidth, screenHeight, connection, socket, is_this_player); // isRunning recebe o retorno da função game()
		else
			isRunning = false; // Flag para quebrar o ciclo do sistema
	}

	SDL_DestroyWindow(window); // Desalocamento da tela
	SDL_DestroyRenderer(renderTarget); // Desalocamento do renderizador

	// Fechamento das bibliotecas SDL
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}

int main(int argc, char *argv[]) {

	bool is_server;
	char answer = '\0';

	while (answer != 'y' && answer != 'n') {
		cout << "Are you a server? (y/n): ";
		cin >> answer;

		if (answer != 'y' && answer != 'n')
			cout << "ERROR: invalid answer!\n";
	}

	SDLNet_Init();

	if (answer == 'y')
		is_server = true;
	else
		is_server = false;

	IPaddress ip;

	if (is_server) {

		TCPsocket server = NULL, client = NULL;

		Uint16 port;
		//cout << "Insert a port number: ";
		//cin >> port;
		
		if (SDLNet_ResolveHost(&ip, NULL, 5555) == -1) {
			cout << "SDLNet_ResolveHost: " << SDLNet_GetError();
			exit(1);
		}

		server = SDLNet_TCP_Open(&ip);

		if (!server) {
			cout << "SDLNet_TCP_Open: " << SDLNet_GetError();
			exit(2);
		}

		bool has_connected = false;
		bool has_printed = false;

		while (!has_connected) {
			client = SDLNet_TCP_Accept(server);
			if (client) {
				SDLNet_SocketSet connection = SDLNet_AllocSocketSet(1);
				SDLNet_AddSocket(connection, (SDLNet_GenericSocket) client);
				engine(connection, client, true);
				has_connected = true;
				SDLNet_FreeSocketSet(connection);
			}
			else
				SDL_Delay(100);
		}
	}
	else {
		TCPsocket server = NULL;

		string ip_string;
		//cout << "Insert the server's IP address: ";
		//getline(cin, ip_string);
		//getline(cin, ip_string);

		Uint16 port;
		//cout << "Insert the server's port number: ";
		//cin >> port;

		if (SDLNet_ResolveHost(&ip, "192.168.0.11", 5555) == -1) {
			cout << "SDLNet_ResolveHost: " << SDLNet_GetError();
			exit(1);
		}

		server = SDLNet_TCP_Open(&ip);

		if (!server) {
			cout << "SDLNet_TCP_Open: " << SDLNet_GetError();
			exit(2);
		}
		
		SDLNet_SocketSet connection = SDLNet_AllocSocketSet(1);
		SDLNet_AddSocket(connection, (SDLNet_GenericSocket) server);

		engine(connection, server, false);

		SDLNet_FreeSocketSet(connection);
	}

	SDLNet_Quit();

	return 0;
}
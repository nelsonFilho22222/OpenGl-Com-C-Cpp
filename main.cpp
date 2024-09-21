#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erro ao inicializar SDL: " << SDL_GetError() << endl;
        return -1;
    }

    // Configurações de memória
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Criando a janela
    SDL_Window* window = SDL_CreateWindow("Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600, 400,
        SDL_WINDOW_OPENGL);

    if (!window) {
        cout << "Erro ao criar a janela: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Criando o contexto OpenGL
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Cor de fundo da janela
    glClearColor(1, 1, 1, 1);

    // Área exibida
    glViewport(0, 0, 600, 400);

    // Sombra
    glShadeModel(GL_SMOOTH);

    // Define o 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    // Renderização
    glClear(GL_COLOR_BUFFER_BIT);

    // Animação
    SDL_GL_SwapWindow(window);

    // Aguarda 5 segundos
    SDL_Delay(5000);

    // Limpa e encerra
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

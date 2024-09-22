#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#define BLOCOS 15

using namespace std;

bool colisao(float Ax, float Ay, float Acomp, float Aalt,
            float Bx, float By, float Bcomp, float Balt) {
    if(Ay+Aalt < By)  return false;
    if(Ay > By+Balt) return false;
    if (Ax+Acomp < Bx) return false;
    if (Ax > Bx+Bcomp) return false;

    return true;
}

struct Bloco {
    float x;
    float y;
    float comprimento;
    float altura;
    bool vivo;
};

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

    // 3D
    glDisable(GL_DEPTH_TEST);

    bool executando = true;
    SDL_Event eventos;


    float atorX = 300;
    float atorY = 300;
    float atorComprimento = 50;
    float atorAltura = 30;

    float inimigoX = 50;
    float inimigoY = 50;
    float inimigoComprimento = 30;
    float inimigoAltura = 30;

    float velX = 4;
    float velY = 2;

    Bloco blocos[BLOCOS];

    for (int i = 0; i < BLOCOS; i++, blocos->x += 65) {
        if(blocos->x > 560) {
            blocos->x = 4;
            blocos->y += 23;
        }

        blocos[i].x = blocos->x;
        blocos[i].y = blocos->y;
        blocos[i].comprimento = 60;
        blocos[i].altura = 20;
        blocos[i].vivo = true;
    }

    bool esquerda = false, direita = false;

    while (executando) {
        while(SDL_PollEvent(&eventos)) {
            if(eventos.type == SDL_QUIT) {
                executando = false;
            }
            if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE) {
                executando = false;
            }
            /* ---------------------------------- */
            if(eventos.type == SDL_KEYDOWN) {
                if(eventos.key.keysym.sym == SDLK_LEFT) {
                    esquerda = true;
                } else if(eventos.key.keysym.sym == SDLK_RIGHT) {
                    direita = true;
                }
            }
            else if(eventos.type == SDL_KEYUP) {
                if(eventos.key.keysym.sym == SDLK_LEFT) {
                    esquerda = false;
                }
                if(eventos.key.keysym.sym == SDLK_RIGHT) {
                    direita = false;
                }
            }
        }
        // logica

        if(esquerda == true) {
            atorX -= 8;
        } else if(direita == true) {
            atorX += 8;
        }
        if(atorX < 0) {
            atorX = 0;
        } else if(atorX + atorComprimento >= 600) {
            atorX = 600 - atorComprimento;
        }


        inimigoX += velX;
        for(int i = 0; i < BLOCOS; i++) {
            if(blocos[i].vivo == true) {
                if(colisao(inimigoX, inimigoY,inimigoComprimento, inimigoAltura,
                    blocos[i].x, blocos[i].y, blocos[i].comprimento, blocos[i].altura) == true) {
                    velX = -velX;
                    blocos[i].vivo = false;
                    break;
                }
            }
        }

        inimigoY += velY;
        for(int i = 0; i < BLOCOS; i++) {
            if(blocos[i].vivo == true) {
                if(colisao(inimigoX, inimigoY,inimigoComprimento, inimigoAltura,
                    blocos[i].x, blocos[i].y, blocos[i].comprimento, blocos[i].altura) == true) {
                    velY = -velY;
                    blocos[i].vivo = false;
                    break;
                }
            }
        }

        if(inimigoX < 0) {
            velX = -velX;
        } else if(inimigoX + inimigoComprimento >= 600) {
            velX = -velX;
        } else if(inimigoY < 0) {
            velY = -velY;
        } else if(inimigoY + inimigoComprimento >= 400) {
            executando = false;
        }

        if(colisao(inimigoX, inimigoY, inimigoComprimento, inimigoAltura,
                   atorX, atorY, atorComprimento, inimigoAltura)) {
            velY = -velY;
        }


        // Renderização
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glOrtho(0, 600,400 ,0 , -1, 1);
        glColor4ub(255,0,0,255);

        glBegin(GL_QUADS);

        glVertex2f(atorX, atorY);
        glVertex2f(atorX + atorComprimento, atorY);
        glVertex2f(atorX + atorComprimento, atorY + atorAltura);
        glVertex2f(atorX, atorY + atorAltura);

        glEnd();


        glColor4ub(0,0,255,255);
        glBegin(GL_QUADS);

        glVertex2f(inimigoX, inimigoY);
        glVertex2f(inimigoX + inimigoComprimento, inimigoY);
        glVertex2f(inimigoX + inimigoComprimento, inimigoY + inimigoAltura);
        glVertex2f(inimigoX, inimigoY + inimigoAltura);

        glEnd();

        glBegin(GL_QUADS);

        glColor4ub(0,0,255,255);

        for(int i = 0; i < BLOCOS; i++) {
            if(blocos[i].vivo == true) {
                glVertex2f(blocos[i].x, blocos[i].y);
                glVertex2f(blocos[i].x + blocos[i].comprimento, blocos[i].y);
                glVertex2f(blocos[i].x + blocos[i].comprimento, blocos[i].y + blocos[i].altura);
                glVertex2f(blocos[i].x, blocos[i].y + blocos[i].altura);
            }
        }

        glEnd();

        glPopMatrix();
        // Animação
        SDL_GL_SwapWindow(window);
    }


        // Aguarda 5 segundos
    // SDL_Delay(5000);
    SDL_Quit();

    return 0;
}

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include "string"


#define BLOCOS 15



GLuint loadTexture(const std::string&fileName) {
    SDL_Surface* ator = IMG_Load(fileName.c_str());
    if (!ator) {
        std::cout << "Erro ao carregar a textura: " << IMG_GetError() << std::endl;
        return 0; // ou algum valor que indique falha
    }

    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(ator, SDL_PIXELFORMAT_RGBA32, 0);
    if (!convertedSurface) {
        SDL_FreeSurface(ator);
        std::cout << "Erro ao converter a superfície: " << IMG_GetError() << std::endl;
        return 0;
    }

    unsigned objetct(0);
    glGenTextures(1, &objetct);
    glBindTexture(GL_TEXTURE_2D, objetct);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum format = ator->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, ator->w, ator->h, 0, format, GL_UNSIGNED_BYTE, ator->pixels);

    SDL_FreeSurface(ator);

    return objetct;
}

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
        std::cout << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
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
        std::cout << "Erro ao criar a janela: " << SDL_GetError() << std::endl;
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

    // Usando o ator
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool executando = true;
    SDL_Event eventos;


    float atorX = 300;
    float atorY = 350;
    float atorComprimento = 50;
    float atorAltura = 25;

    float inimigoX = 50;
    float inimigoY = 50;
    float inimigoComprimento = 30;
    float inimigoAltura = 30;

    float velX = 6;
    float velY = 4;

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

    unsigned int person_textura = 0;
    person_textura = loadTexture("/home/rothmans/CLionProjects/untitled/assets/ator0.png");

    if (person_textura == 0) {
        std::cout << "Falha ao carregar a textura." << std::endl;
    }


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

        glColor4ub(255,255,255,255);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, person_textura);

        glBegin(GL_QUADS);

        glTexCoord2d(0,0);            glVertex2f(atorX, atorY);
        glTexCoord2d(1,0);            glVertex2f(atorX + atorComprimento, atorY);
        glTexCoord2d(1,1);            glVertex2f(atorX + atorComprimento, atorY + atorAltura);
        glTexCoord2d(0,1);            glVertex2f(atorX, atorY + atorAltura);

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

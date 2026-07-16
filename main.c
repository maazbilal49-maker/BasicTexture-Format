#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <inttypes.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
}Game;

SDL_Texture* readFile(const char* filename, Game *game);
bool sdlInit(Game *game);
void gameCleanUp(Game *game);
void updatePlayer(SDL_FRect *player, float dt);

int main(int argc, char **argv){
    Game game = {
        .window = NULL,
        .renderer = NULL
    };
    if(!sdlInit(&game)){
        gameCleanUp(&game);
        return 1;
    }

    SDL_Texture* player = readFile("player.btex", &game);
    if(player == NULL){
        gameCleanUp(&game);
        return 1;
    }

    float w, h;
    SDL_GetTextureSize(player, &w, &h);


    SDL_FRect dst = {100, 100, w * 20, h * 20};
    
    Uint64 lastTime = SDL_GetTicks();

    bool running = true;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_EVENT_QUIT){
                running = false;
            }
            if(e.type == SDL_EVENT_KEY_DOWN){
                if(e.key.key == SDLK_ESCAPE){
                    running = false;
                }
            }
        }
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        updatePlayer(&dst, deltaTime);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

        SDL_RenderClear(game.renderer);

        SDL_RenderTexture(game.renderer, player, NULL, &dst);
        SDL_RenderPresent(game.renderer);
    }

    SDL_DestroyTexture(player);

    gameCleanUp(&game);
    return 0;
}

bool sdlInit(Game *game){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer("Test", 800, 600, 0, &game->window, &game->renderer)){
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void gameCleanUp(Game *game){
    SDL_DestroyRenderer(game->renderer);

    SDL_DestroyWindow(game->window);

    SDL_Quit();

}

SDL_Texture* readFile(const char* filename, Game *game){
    FILE *f = fopen(filename, "r");
    if(!f){
        fprintf(stderr, "Failed to open file.\n");
        return NULL;
    }

    int width, height;
    fscanf(f, "%d %d", &width, &height);

    SDL_Surface *surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
    Uint32* pixels = (Uint32 *)surface->pixels;

    char line[256];
    for(int y = 0; y < height; y++){
        fscanf(f, "%255s", line);
        for(int x = 0; x < width; x++){
            char pixel = line[x];
            Uint32 color;
            switch (pixel){
                case '.': color = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0); break; // transparent
                case 'R': color = SDL_MapSurfaceRGBA(surface, 255, 0, 0, 255); break; //red
                case 'G': color = SDL_MapSurfaceRGBA(surface, 0, 255, 0, 255); break; //green
                case 'B': color = SDL_MapSurfaceRGBA(surface, 0, 0, 255, 255); break; //blue
                case 'W': color = SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255); break; //white
                case 'K': color = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 255); break; // black
                default: color = SDL_MapSurfaceRGBA(surface, 255, 0, 255, 255); break; //error
            }
            pixels[y * width + x] = color;
        }
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    fclose(f);

    SDL_DestroySurface(surface);

    return texture;
}

void updatePlayer(SDL_FRect *player, float dt){
    const bool *state = SDL_GetKeyboardState(NULL);
    
    if(state[SDL_SCANCODE_W]) player->y -= 300.0f * dt;
    if(state[SDL_SCANCODE_S]) player->y += 300.0f * dt;
    if(state[SDL_SCANCODE_A]) player->x -= 300.0f * dt;
    if(state[SDL_SCANCODE_D]) player->x += 300.0f * dt;
}

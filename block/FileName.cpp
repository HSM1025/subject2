#include <SDL.h>
#include <stdio.h>
#include <math.h>

#define W 800
#define H 600
#define PW 25
#define PH 100
#define BS 20
#define PS 400.0f
#define BS 400.0f

typedef struct { float x, y, w, h; } Pdl;
typedef struct { float x, y, vx, vy, r; } Bl;

void initPdl(Pdl* p, float x, float y) {
    p->x = x; p->y = y; p->w = PW; p->h = PH;
}

void initBl(Bl* b, float x, float y) {
    b->x = x; b->y = y; b->vx = BS * cosf(45.0f * M_PI / 180.0f);
    b->vy = BS * sinf(45.0f * M_PI / 180.0f); b->r = BS / 2;
}

void drawPdl(SDL_Renderer* r, Pdl* p) {
    SDL_Rect rect = { (int)(p->x - p->w / 2), (int)(p->y - p->h / 2), (int)p->w, (int)p->h };
    SDL_RenderFillRect(r, &rect);
}

void drawBl(SDL_Renderer* r, Bl* b) {
    SDL_Rect rect = { (int)(b->x - b->r), (int)(b->y - b->r), (int)(b->r * 2), (int)(b->r * 2) };
    SDL_RenderFillRect(r, &rect);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
    if (!win) {
        printf("Window Creation Failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        printf("Renderer Creation Failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    Pdl lp, rp; Bl bl;
    initPdl(&lp, PW / 2 + 10, H / 2);
    initPdl(&rp, W - PW / 2 - 10, H / 2);
    initBl(&bl, W / 2, H / 2);

    int run = 1, play = 1;
    SDL_Event e;
    float dt = 0;
    Uint32 last = SDL_GetTicks();

    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) run = 0;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) run = 0;
                else if (e.key.keysym.sym == SDLK_SPACE) {
                    if (!play) {
                        play = 1;
                        initPdl(&lp, PW / 2 + 10, H / 2);
                        initPdl(&rp, W - PW / 2 - 10, H / 2);
                        initBl(&bl, W / 2, H / 2);
                        last = SDL_GetTicks();
                    }
                }
            }
        }

        if (play) {
            Uint32 now = SDL_GetTicks();
            dt = (now - last) / 1000.0f;
            last = now;

            const Uint8* ks = SDL_GetKeyboardState(NULL);
            if (ks[SDL_SCANCODE_UP] && lp.y - lp.h / 2 > 0) lp.y -= PS * dt;
            if (ks[SDL_SCANCODE_DOWN] && lp.y + lp.h / 2 < H) lp.y += PS * dt;

            if (bl.y > rp.y + rp.h / 2 && rp.y + rp.h / 2 < H) rp.y += PS * dt;
            else if (bl.y < rp.y - rp.h / 2 && rp.y - rp.h / 2 > 0) rp.y -= PS * dt;

            bl.x += bl.vx * dt; bl.y += bl.vy * dt;
            if (bl.y - bl.r < 0 || bl.y + bl.r > H) bl.vy = -bl.vy;

            if (bl.x - bl.r < lp.x + lp.w / 2 && bl.y > lp.y - lp.h / 2 && bl.y < lp.y + lp.h / 2) bl.vx = fabs(bl.vx);
            if (bl.x + bl.r > rp.x - rp.w / 2 && bl.y > rp.y - rp.h / 2 && bl.y < rp.y + rp.h / 2) bl.vx = -fabs(bl.vx);

            if (bl.x - bl.r < 0 || bl.x + bl.r > W) play = 0;
        }

        SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
        SDL_RenderClear(ren);

        if (play) {
            SDL_SetRenderDrawColor(ren, 100, 100, 200, 255);
            drawPdl(ren, &lp);

            SDL_SetRenderDrawColor(ren, 200, 100, 100, 255);
            drawPdl(ren, &rp);

            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            drawBl(ren, &bl);
        }
        else {
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_Rect msgRect = { W / 2 - 150, H / 2 - 25, 300, 50 };
            SDL_RenderFillRect(ren, &msgRect);
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderDrawRect(ren, &msgRect);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
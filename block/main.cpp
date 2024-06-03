#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BRICK_ROWS 4
#define BRICK_COLUMNS 7
#define BRICK_WIDTH (WINDOW_WIDTH / BRICK_COLUMNS - 10)
#define BRICK_HEIGHT 30
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_SIZE 15

typedef struct {
    int x, y, w, h;
    SDL_Color color;
    int alive;
} Brick;

typedef struct {
    float x, y, w, h;
    float dx, dy;
} Ball;

typedef struct {
    float x, y, w, h;
    float dx;
} Paddle;

typedef struct {
    int x, y, w, h;
} Title;

// ���� ���� ���� �Լ�
SDL_Color getRandomColor() {
    SDL_Color color[] = { {255, 255, 255, 0}, {255, 255, 255, 0}, {255, 255, 255, 0},
                          {255, 255, 255, 0}, {255, 255, 255, 0}, {255, 255, 255, 0},
                          {255, 255, 255, 0}, {255, 255, 255, 0},
                          {0, 0, 255, 0}, {255, 0, 0, 0} };
    return color[rand() % sizeof(color) / sizeof(color[0])];
}

// �浹 üũ �Լ�
int checkCollision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}

// ���� �Լ�
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(0));

    // ���� �迭 �ʱ�ȭ
    Brick bricks[BRICK_ROWS][BRICK_COLUMNS];
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLUMNS; col++) {
            bricks[row][col].x = col * BRICK_WIDTH + col * 10 + 5;
            bricks[row][col].y = row * BRICK_HEIGHT + row * 10 + 50;
            bricks[row][col].w = BRICK_WIDTH;
            bricks[row][col].h = BRICK_HEIGHT;
            bricks[row][col].color = getRandomColor();
            bricks[row][col].alive = 1;
        }
    }

    // �е� �� �� �ʱ�ȭ
    Paddle paddle = { (WINDOW_WIDTH - PADDLE_WIDTH) / 2, WINDOW_HEIGHT - PADDLE_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT, 0 };
    Ball ball = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, BALL_SIZE, BALL_SIZE, 3, 3 };
    Title title = { 100, 100, 600, 300 };

    int running = 1;    // ���α׷� ���� ����
    int start = 0;  // ���� ���� ����
    int ready = 1; // ���� ���� ����
    int item = 5;   // ���� ��� �ӵ� ����
    int score = 0;  // ����
    int level = 1;  // ����
    char s_text[] = "score : "; // ���� ǥ�� �ؽ�Ʈ
    char l_text[] = "level : "; // ���� ǥ�� �ؽ�Ʈ
    char p_score[10];   // �������� ���ڷ� ��ȯ�� ���ڿ�
    char p_level[10];   // �������� ���ڷ� ��ȯ�� ���ڿ�
    SDL_Event event;

    // ��Ʈ �ε�
    TTF_Font* font = TTF_OpenFont("c:\\windows\\fonts\\impact.ttf", 24);
    if (!font) {
        printf("��Ʈ �ε� ����: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // ���� ����
    while (running) {
        // ����ȭ�� ó��
        while (!start) {
            // �̺�Ʈ ó��
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    TTF_Quit();
                    return 0;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) { // �����̽��� ���� �� ���� ����
                        // ���� ������ ���� ���� �ʱ�ȭ
                        ball.x = WINDOW_WIDTH / 2;
                        ball.y = WINDOW_HEIGHT / 2;
                        paddle.x = (WINDOW_WIDTH - PADDLE_WIDTH) / 2;
                        paddle.dx = 0;
                        for (int row = 0; row < BRICK_ROWS; row++) {
                            for (int col = 0; col < BRICK_COLUMNS; col++) {
                                bricks[row][col].x = col * BRICK_WIDTH + col * 10 + 5;
                                bricks[row][col].y = row * BRICK_HEIGHT + row * 10 + 50;
                                bricks[row][col].w = BRICK_WIDTH;
                                bricks[row][col].h = BRICK_HEIGHT;
                                bricks[row][col].color = getRandomColor();
                                bricks[row][col].alive = 1;
                            }
                        }
                        ready = 1;  
                        level = 1;
                        score = 0;
                        item = 5;
                        start = 1;
                    }
                }
            }    
            // ���� ȭ�� �׸���
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect startDrawRect = { title.x, title.y, title.w, title.h };
            SDL_RenderFillRect(renderer, &startDrawRect);

            // ȭ�� ����
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        // ���� ���� �� �̺�Ʈ ó��
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    paddle.dx = -10;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    paddle.dx = 10;
                }
            }
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                    paddle.dx = 0;
                }
            }
        }
        // ���� ��� ������ ��� �̺�Ʈ ó��
        if (!ready) {
            while (!ready) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        TTF_Quit();
                        return 0;
                    }
                    if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_SPACE) {
                            ball.dx = 3; ball.dy = 3;
                            ready = 1;
                        }
                    }
                }
            }
        }

        // �е� �̵�
        paddle.x += paddle.dx;
        if (paddle.x < 0) paddle.x = 0;
        if (paddle.x + PADDLE_WIDTH > WINDOW_WIDTH) paddle.x = WINDOW_WIDTH - PADDLE_WIDTH;

        // �� �̵�
        ball.x += ball.dx * (item + level) * 0.2;
        ball.y += ball.dy * (item + level) * 0.2;

        if (ball.x < 0 || ball.x + BALL_SIZE > WINDOW_WIDTH) {
            ball.dx = -ball.dx;
        }
        if (ball.y < 20) {
            ball.dy = -ball.dy;
        }
        if (ball.y + BALL_SIZE > WINDOW_HEIGHT) {
            // ���� �ٴڿ� ������ �ʱ� ȭ������ �̵�
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            start = 0;
            continue;
        }

        // �е�� �� �浹
        SDL_Rect paddleRect = { paddle.x, paddle.y, paddle.w, paddle.h };
        SDL_Rect ballRect = { ball.x, ball.y, ball.w, ball.h };
        if (checkCollision(paddleRect, ballRect)) {
            // �浹 �� ���
            int leftDiff = ballRect.x + ballRect.w - paddleRect.x;
            int rightDiff = paddleRect.x + paddleRect.w - ballRect.x;
            int topDiff = ballRect.y + ballRect.h - paddleRect.y;
            int bottomDiff = paddleRect.y + paddleRect.h - ballRect.y;

            int minDiff = leftDiff;
            if (rightDiff < minDiff) minDiff = rightDiff;
            if (topDiff < minDiff) minDiff = topDiff;
            if (bottomDiff < minDiff) minDiff = bottomDiff;

            // �浹 �鿡 ���� ��ġ ����
            if (minDiff == leftDiff) {
                // ���� �浹
                ballRect.x = paddleRect.x - ballRect.w - 5;
                ball.dx = -ball.dx;
            }
            else if (minDiff == rightDiff) {
                // ������ �浹
                ballRect.x = paddleRect.x + paddleRect.w + 5;
                ball.dx = -ball.dx;
            }
            else if (minDiff == topDiff) {
                // ���� �浹
                ballRect.y = paddleRect.y - ballRect.h - 5;
                ball.dy = -ball.dy;
            }
            else if (minDiff == bottomDiff) {
                // �Ʒ��� �浹
                ballRect.y = paddleRect.y + paddleRect.h + 5;
                ball.dy = -ball.dy;
            }
        }

        // ������ �� �浹 ó��
        for (int row = 0; row < BRICK_ROWS; row++) {
            for (int col = 0; col < BRICK_COLUMNS; col++) {
                if (bricks[row][col].alive) {
                    SDL_Rect brickRect = { bricks[row][col].x, bricks[row][col].y, bricks[row][col].w, bricks[row][col].h };
                    if (checkCollision(brickRect, ballRect)) {                       
                        // ������ �� �浹 �� �� �ӵ� ����
                        if (bricks[row][col].color.r == 255 && bricks[row][col].color.g == 0) item++;
                        // �Ķ��� �� �ѵ� �� �� �ӵ� ����
                        if (bricks[row][col].color.b == 255 && bricks[row][col].color.g == 0) item--;   
                        // �浹 �� ���
                        int leftDiff = ballRect.x + ballRect.w - brickRect.x;
                        int rightDiff = brickRect.x + brickRect.w - ballRect.x;
                        int topDiff = ballRect.y + ballRect.h - brickRect.y;
                        int bottomDiff = brickRect.y + brickRect.h - ballRect.y;

                        int minDiff = leftDiff;
                        if (rightDiff < minDiff) minDiff = rightDiff;
                        if (topDiff < minDiff) minDiff = topDiff;
                        if (bottomDiff < minDiff) minDiff = bottomDiff;

                        // �浹 �鿡 ���� ��ġ ����
                        if (minDiff == leftDiff) {
                            // ���� �浹
                            ballRect.x = brickRect.x - ballRect.w - 5;
                            ball.dx = -ball.dx;
                        }
                        else if (minDiff == rightDiff) {
                            // ������ �浹
                            ballRect.x = brickRect.x + brickRect.w + 5;
                            ball.dx = -ball.dx;
                        }
                        else if (minDiff == topDiff) {
                            // ���� �浹
                            ballRect.y = brickRect.y - ballRect.h - 5;
                            ball.dy = -ball.dy;
                        }
                        else if (minDiff == bottomDiff) {
                            // �Ʒ��� �浹
                            ballRect.y = brickRect.y + brickRect.h + 5;
                            ball.dy = -ball.dy;
                        }
                        bricks[row][col].alive = 0;
                        score += 100 * level;
                    }
                }
            }
        }
        // ��� ����� ��������� Ȯ��
        int clear = 1;
        for (int row = 0; row < BRICK_ROWS; row++) {
            for (int col = 0; col < BRICK_COLUMNS; col++) {
                if (bricks[row][col].alive != 0) {
                    clear = 0;
                }
            }
        }
        // ��� ����� ������� ��� ���� �ʱ�ȭ �� �ӵ� ����(���� ��)
        if (clear) {
            ready = 0;  
            item = 5;
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            ball.dx = 0; ball.dy = 0;
            for (int row = 0; row < BRICK_ROWS; row++) {
                for (int col = 0; col < BRICK_COLUMNS; col++) {
                    bricks[row][col].x = col * BRICK_WIDTH + col * 10 + 5;
                    bricks[row][col].y = row * BRICK_HEIGHT + row * 10 + 50;
                    bricks[row][col].w = BRICK_WIDTH;
                    bricks[row][col].h = BRICK_HEIGHT;
                    bricks[row][col].color = getRandomColor();
                    bricks[row][col].alive = 1;
                }
            }
            level++;
        }

        // ȭ�� �����
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // ���� �׸���
        for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLUMNS; col++) {
                if (bricks[row][col].alive) {
                    SDL_SetRenderDrawColor(renderer, bricks[row][col].color.r, bricks[row][col].color.g, bricks[row][col].color.b, bricks[row][col].color.a);
                    SDL_Rect brickRect = { bricks[row][col].x, bricks[row][col].y, bricks[row][col].w, bricks[row][col].h };
                    SDL_RenderFillRect(renderer, &brickRect);
                }
            }
        }

        // �е� �׸���
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect paddleDrawRect = { paddle.x, paddle.y, paddle.w, paddle.h };
        SDL_RenderFillRect(renderer, &paddleDrawRect);

        // �� �׸���
        SDL_Rect ballDrawRect = { ball.x, ball.y, ball.w, ball.h };
        SDL_RenderFillRect(renderer, &ballDrawRect);

        // ���� ǥ��
        sprintf(p_score, "%d", score);
        

        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* surface1 = TTF_RenderText_Blended(font, l_text, textColor);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Rect destRect1 = { 10, 0, surface1->w, surface1->h };
        SDL_Surface* surface2 = TTF_RenderText_Blended(font, p_level, textColor);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_Rect destRect2 = { 70, 0, surface2->w, surface2->h }; 
        SDL_RenderCopy(renderer, texture1, NULL, &destRect1);
        SDL_RenderCopy(renderer, texture2, NULL, &destRect2);
        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(texture1);
        SDL_DestroyTexture(texture2);

        // ���� ǥ��
        sprintf(p_level, "%d", level);

        SDL_Surface* surface3 = TTF_RenderText_Blended(font, s_text, textColor);
        SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
        SDL_Rect destRect3 = { 650, 0, surface3->w, surface3->h };
        SDL_Surface* surface4 = TTF_RenderText_Blended(font, p_score, textColor);
        SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
        SDL_Rect destRect4 = { 720, 0, surface4->w, surface4->h };
        
        SDL_RenderCopy(renderer, texture3, NULL, &destRect3);
        SDL_RenderCopy(renderer, texture4, NULL, &destRect4);
        SDL_FreeSurface(surface3);
        SDL_FreeSurface(surface4);
        SDL_DestroyTexture(texture3);
        SDL_DestroyTexture(texture4);
   
        // ȭ�� ����
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // �޸� ���� �� ����
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
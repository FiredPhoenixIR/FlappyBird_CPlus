#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 50;
const int BIRD_HEIGHT = 50;
const int BIRD_JUMP_FORCE = -10;
const int GRAVITY = 1;

// SDL Variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* birdTexture = nullptr;

// Bird variables
int birdX, birdY;
int birdVelocity;

// Functions
bool initSDL();
void closeSDL();
SDL_Texture* loadTexture(const char* path);
bool loadMedia();

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!loadMedia()) {
        std::cerr << "Media loading failed." << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    // Initial bird position
    birdX = SCREEN_WIDTH / 4;
    birdY = SCREEN_HEIGHT / 2 - BIRD_HEIGHT / 2;
    birdVelocity = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                birdVelocity = BIRD_JUMP_FORCE;
            }
        }

        // Update bird position
        birdVelocity += GRAVITY;
        birdY += birdVelocity;

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the bird
        SDL_Rect birdRect = { birdX, birdY, BIRD_WIDTH, BIRD_HEIGHT };
        SDL_RenderCopy(renderer, birdTexture, nullptr, &birdRect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    closeSDL();

    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Flappy Bird Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == nullptr) {
        std::cerr << "Failed to load image " << path << ": " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Failed to create texture from " << path << ": " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia() {
    birdTexture = loadTexture("bird.png"); // Replace with the path to your bird image
    return birdTexture != nullptr;
}

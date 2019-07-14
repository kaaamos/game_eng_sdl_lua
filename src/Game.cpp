#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"

Game::Game()
{
    this->isRunning = false;
}

Game::~Game()
{
}

bool Game::IsRunning() const
{
    return this->isRunning;
}

glm::vec2 projectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVel = glm::vec2(20.0f, 20.0f);

void Game::Initialize(int width, int height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window)
    {
        std::cerr << "Error initializing window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer)
    {
        std::cerr << "Error initializing renderer." << std::endl;
        return;
    }

    ticksLastFrame = 0;
    isRunning = true;
}

void Game::ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;
        default:
            break;
    }
}

void Game::Update()
{
    // Wait until 16ms has ellapsed since the last frame (SDL_Delay)
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    // Delta Time = Difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    deltaTime = (deltaTime > 0.05f)? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    // Use deltaTime to update my game objects
    projectilePos = glm::vec2(
        projectilePos.x + projectileVel.x * deltaTime,
        projectilePos.y + projectileVel.y * deltaTime
    );
}

void Game::Render()
{
    // Set background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Clear the back buffer
    SDL_RenderClear(renderer);

    // Draw game objects
    SDL_Rect projectile {
        (int)projectilePos.x,
        (int)projectilePos.y,
        10,
        10
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &projectile);

    // Swap front and back buffers
    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
} 
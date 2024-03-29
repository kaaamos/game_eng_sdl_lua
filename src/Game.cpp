#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;

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

    LoadLevel(0);

    ticksLastFrame = 0;
    isRunning = true;
}

void Game::LoadLevel(int levelNumber)
{
    std::string textureFilePath("./assets/images/tank-big-right.png");
    assetManager->AddTexture("tank-image", textureFilePath.c_str());

    Entity& newEntity(manager.AddEntity("tank"));
    newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    newEntity.AddComponent<SpriteComponent>("tank-image");
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

    manager.Update(deltaTime);
}

void Game::Render()
{
    // Set background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Clear the back buffer
    SDL_RenderClear(renderer);

    if(manager.HasNoEntities())
    {
        return;
    }

    manager.Render();

    // Swap front and back buffers
    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
} 
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component
{
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRectangle;
        SDL_Rect destRectangle;
    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent(const char* filePath)
        {
            SetTexture(filePath);
        }
        void SetTexture(std::string assetTextureId)
        {
            texture = Game::assetManager->GetTexture(assetTextureId);
        }
        void Initialize() override
        {
            transform = owner->GetComponent<TransformComponent>();
            srcRectangle.x = 0;
            srcRectangle.y = 0;
            srcRectangle.w = transform->width;
            srcRectangle.h = transform->height;
        }

        void Update(float deltaTime) override
        {
            destRectangle.x = (int) transform->position.x;
            destRectangle.y = (int) transform->position.y;
            destRectangle.w = transform->width * transform->scale;
            destRectangle.h = transform->height * transform->scale;
        }

        void Render() override
        {
            TextureManager::Draw(texture, srcRectangle, destRectangle, spriteFlip);
        }
};

#endif
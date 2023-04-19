#include "stdafx.h"
#include "ImgObjectBase.h"

ImageObjectBase::ImageObjectBase()
{
    p_texture_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

ImageObjectBase::~ImageObjectBase()
{
    Free();
}

bool ImageObjectBase::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }

        SDL_FreeSurface(load_surface);
    }

    p_texture_ = new_texture;
    return p_texture_ != NULL;
}

void ImageObjectBase::Render(SDL_Renderer* des, const SDL_Rect* clip /* = NULL */)
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    SDL_RenderCopy(des, p_texture_, clip, &renderquad); // đẩy toàn bộ pobject lên des bởi in4 từ renderquad
}

void ImageObjectBase::Free()
{
    if (p_texture_ != NULL)
    {
        SDL_DestroyTexture(p_texture_);
        p_texture_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
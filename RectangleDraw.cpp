#include "stdafx.h"
#include "RectangleDraw.h"

void RectangleForm::RenderRectangle(const RectangleFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen)
{
    SDL_Rect fill_rect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_SetRenderDrawColor(screen, color_data.red_, color_data.green_, color_data.blue_, 0xFF);
    SDL_RenderFillRect(screen, &fill_rect);

}

void RectangleForm::RenderOutline(const RectangleFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen)
{
    SDL_Rect outRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_SetRenderDrawColor(screen, color_data.red_, color_data.green_, color_data.blue_, 0xFF);
    SDL_RenderDrawRect(screen, &outRect);
}
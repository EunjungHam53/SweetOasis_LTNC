#include "stdafx.h"
#include "items_object.h"

ItemsObject::ItemsObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	frame_ = 0;

    x_pos_ = 0;
    y_pos_ = 0;
    map_x_ = 0;
    map_y_ = 0;
}

ItemsObject::~ItemsObject()
{

}

SDL_Rect ItemsObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void ItemsObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < ITEM_FRAME_NUMS; i++)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

bool ItemsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = ImageObjectBase::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w / ITEM_FRAME_NUMS;
        height_frame_ = rect_.h;
    }
    return ret;
}

void ItemsObject::Show(SDL_Renderer* des)
{
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ >= ITEM_FRAME_NUMS)
            frame_ = 0;

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
        SDL_RenderCopy(des, p_texture_, currentClip, &renderQuad);
}
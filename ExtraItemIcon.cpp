#include "stdafx.h"
#include "ExtraItemIcon.h"

PlayerHP::PlayerHP()
{
    number_ = 0;
}

PlayerHP::~PlayerHP()
{

}

void PlayerHP::AddPos(const int& xp)
{
    pos_list_.push_back(xp);
}

void PlayerHP::Init(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//player-life.png", screen);
    if (ret)
    {
        number_ = 3;
        if (pos_list_.size() > 0)
            pos_list_.clear();

        AddPos(20);
        AddPos(60);
        AddPos(100);
    }
}

void PlayerHP::Show(SDL_Renderer* screen)
{
    for (int i = 0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(screen);
    }
}

void PlayerHP::Decrease()
{
    number_--;
    pos_list_.pop_back();
}

void PlayerHP::Increase()
{
    number_++;
    int last_pos;
    if (pos_list_.size() > 0)
    {
        last_pos = pos_list_.back();
        last_pos += 40;
    }
    else last_pos = 20;
    pos_list_.push_back(last_pos);

}

PlayerGem::PlayerGem()
{
    x_pos_ = 0;
    y_pos_ = 0;
}

PlayerGem::~PlayerGem()
{

}

void PlayerGem::Init(SDL_Renderer* screen)
{
    bool ret = LoadImg("img//img-gem-on-bar.png", screen);
}

void PlayerGem::Show(SDL_Renderer* screen)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(screen);
}
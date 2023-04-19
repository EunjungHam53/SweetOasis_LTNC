#include "stdafx.h"
#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = GEM_BULLET;
}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    if (bullet_type_ == EAGLE_BULLET)
    {
        ret = LoadImg("img//eagle_bullet.png", des);
    }
    else 
    {
        ret = LoadImg("img//bullet-gem.png", des);
    }
    return ret;
}

void BulletObject::HandleMove(const int& x_border_left, const int& x_border_right, const int& y_border_up, const int& y_border_down)
{
    if (bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += x_val_;
        if (rect_.x > x_border_right)
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= x_val_;
        if (rect_.x < x_border_left)
        {
            is_move_ = false;
        }
    }
}
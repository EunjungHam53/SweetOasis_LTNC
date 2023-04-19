#include "stdafx.h"
#include "EnemiesObject.h"

EnemyObject::EnemyObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;

    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    type_enemy_ = FROG;
    frog_jump_time_ = 0;
    sound_frog_played_ = false;
}

EnemyObject::~EnemyObject()
{

}

SDL_Rect EnemyObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

bool EnemyObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = ImageObjectBase::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w/ENEMY_FRAME_NUM;
        height_frame_ = rect_.h;
    }
    return ret;
}

void EnemyObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < ENEMY_FRAME_NUM; i++)
        {
            frame_clip_[i].x = width_frame_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void EnemyObject::Show(SDL_Renderer* des)
{
    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_ ++;
        if (frame_ >= ENEMY_FRAME_NUM)
            frame_ = 0;

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_texture_, currentClip, &renderQuad);
    }
}

void EnemyObject::HandleMove(Map& gMap, Mix_Chunk* frog_sound)
{
    if (come_back_time_ == 0)
    {
        x_val_ = 0;
        if (type_enemy_ != EAGLE)
        {
            y_val_ += GRAVITY_SPEED;
            if (y_val_ >= MAX_FALL_SPEED)
                y_val_ = MAX_FALL_SPEED;
        }

        if (type_enemy_ == FROG)
        {
            int end_screen_x = map_x_ + SCREEN_WIDTH;
            int end_screen_y = map_y_ + SCREEN_HEIGHT;
            if (end_screen_x >= x_pos_ && map_x_ <= x_pos_ &&
                end_screen_y >= y_pos_ && map_y_ <= y_pos_)
            {
                if (end_screen_x >= x_pos_ + FROG_JUMP && !sound_frog_played_)
                {
                    Mix_PlayChannel(-1, frog_sound, 0);
                    sound_frog_played_ = true;
                }
                if (on_ground_)
                {
                    frog_jump_time_++;
                    if (frog_jump_time_ == 60)
                        input_type_.jump_ = 1;
                }
                if (!on_ground_)
                {
                    x_val_ -= FROG_JUMP/4;
                    if (x_val_ < -FROG_JUMP) x_val_ = -FROG_JUMP;
                }
                if (input_type_.jump_ == 1 && on_ground_)
                {
                    x_val_ -= FROG_JUMP;
                    y_val_ -= FROG_JUMP;
                 
                    on_ground_ = 0;
                    input_type_.jump_ = 0;
                    frog_jump_time_ = 0;
                }
            }
        }

        if (input_type_.left_ == 1)
            x_val_ -= ENEMY_SPEED;
        else if (input_type_.right_ == 1)
            x_val_ += ENEMY_SPEED;

        CheckColliWithMap(gMap);
    }
    else if (come_back_time_ > 0)
    {
        come_back_time_ --;
        if (come_back_time_ == 0)
        {
            InitEnemies();
        }
    }
}

void EnemyObject::InitEnemies()
{
    x_val_ = 0;
    y_val_ = 0;
    if (x_pos_ > 4*TILE_SIZE)
    {
        x_pos_ -= 4*TILE_SIZE;
        animation_a_ -= 4*TILE_SIZE;
        animation_b_ -= 4*TILE_SIZE;
    }
    else x_pos_ = 0;
    y_pos_ = 0;
    come_back_time_ = 0;
    if (type_enemy_ == OPOSUM)
        input_type_.left_ = 1;
}

void EnemyObject::CheckColliWithMap(Map& map_data) //hàm quan trọng check colision 
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Check horizontal (ý nghĩa: check frame nhân vật đang nằm ở ô thứ bao nhiêu của bản đồ tile)
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;
    
    y1 = y_pos_/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // main obj mov to right
        {
            // bài 10: xử lí đối tượng hỗ trợ
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

                //hàm cũ
            if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 ||
                val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 ||
                val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3)
                {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -= (width_frame_ + 1);
                    x_val_ = 0;
                }
        }
        else if (x_val_ < 0)
        {
            // bài 10: xử lí đối tượng hỗ trợ
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

                // hàm cũ
            if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 ||
                val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 ||
                val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3)
                {
                    x_pos_ = (x1 + 1)*TILE_SIZE;
                    x_val_ = 0;
                }
        }
    }

    //Check vertical
    int width_min = min(width_frame_, TILE_SIZE);
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            // bài 10:
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

                // hàm cũ
            if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 ||
                val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 ||
                val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3)
                {
                    y_pos_ = y2*TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    y_val_ = 0;
                    on_ground_ = true;
                }
        }
        else if (y_val_ < 0)
        {
            // bài 10:
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

                // hàm cũ
            if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 ||
                val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 ||
                val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3) //check xem nhảy lên có cụng đầu viên gạch 0
                {
                    y_pos_ = (y1 + 1)*TILE_SIZE;
                    y_val_ = 0;
                }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
        x_pos_ = 0;
    else if (x_pos_ + width_frame_ > map_data.max_x_)
        x_pos_ = map_data.max_x_ - width_frame_ - 1;

    if (y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 60;
    }
}

void EnemyObject::UpdateImgFromMoveType(SDL_Renderer* screen)
{
    if (type_enemy_ == FROG)
    {
        if (on_ground_)
            LoadImg("img//frog-idle.png", screen);
        else LoadImg("img//frog-jump.png", screen);
    }
    else if (type_enemy_ == OPOSUM)
    {
        if (on_ground_ == true)
        {
            if (x_pos_ > animation_b_)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("img//oposum-left.png", screen);
            }
            else if (x_pos_ < animation_a_)
            {
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("img//oposum-right.png", screen);
            }
        }
        else
        {
            if (input_type_.left_ == 1)
            {
                LoadImg("img//oposum-left.png", screen);
            }
        }
    }
}

void EnemyObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
    if (p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject::EAGLE_BULLET);
        bool ret = p_bullet->LoadImgBullet(screen);

        if (ret)
        {
            p_bullet->set_is_move(true);
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            p_bullet->SetRect(rect_.x + 5, rect_.y + 10);
            p_bullet->set_x_val(15);
            bullet_list_.push_back(p_bullet);
        }
    }
}

void EnemyObject::HandleBullet(SDL_Renderer* screen, const int& x_limit_left, const int& x_limit_right, const int& y_limit_up, const int& y_limit_down)
{
    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if (bullet_distance < 300 && bullet_distance > 0)
                {
                    p_bullet->HandleMove(x_limit_left, x_limit_right, y_limit_up, y_limit_down);
                    p_bullet->Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else 
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 5, rect_.y + 10);
            }
        }
    }
}

void EnemyObject::RemoveBullet(const int& idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
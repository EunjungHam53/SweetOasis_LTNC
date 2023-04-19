#include "stdafx.h"
#include "BossObject.h"

BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    on_ground_ = false;
    status_ = IDLE_LEFT;

    time_to_load_attack2 = 0;
    appear_ = true;

    is_mechanical_sound_played = false;
}

BossObject::~BossObject()
{

}

bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = ImageObjectBase::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = rect_.w/FRAME_NUM_8;
        height_frame_ = rect_.h;
    }

    return ret;
}

void BossObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_8; i++)
        {
            frame_clip_[i].x = width_frame_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void BossObject::Show(SDL_Renderer* des, Mix_Chunk* broken_sound)
{
    UpdateImageObject(des, broken_sound);
        rect_.x = x_pos_ - map_x_; //x_pos_ vtri tren map, rect.x vtri trên screen
        rect_.y = y_pos_ - map_y_;
        frame_ ++;
        if (frame_ >= FRAME_NUM_8)
        {
            if (status_ == HURT_LEFT || status_ == HURT_RIGHT) is_hurt_ = false;
            if (status_ == DEADTH_LEFT || status_ == DEADTH_RIGHT)
            {
                appear_ = false;
            }

            frame_ = 0;
        }

        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        if (current_clip != NULL)
        {
            renderQuad.w = current_clip->w;
            renderQuad.h = current_clip->h;
        }

        SDL_RenderCopy(des, p_texture_, current_clip, &renderQuad);
    
}

void BossObject::HandleMove(Map& g_map, int player_x_pos, int player_y_pos, Mix_Chunk* mecha_sound)
{
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= MAX_FALL_SPEED)
            y_val_ = MAX_FALL_SPEED;
        
        //Check whether player is on left or right and handle move
        int relative_pos = x_pos_ - player_x_pos;
        if (relative_pos <= 600 && !is_mechanical_sound_played)
        {
            Mix_PlayChannel(-1, mecha_sound, 0);
            is_mechanical_sound_played = true;
        }
        int old_status = status_;
        if (relative_pos >= 0)
        {
            if (is_hurt_)
            {
                status_ = HURT_LEFT;
                input_type_.right_ = 0;
                input_type_.left_ = 0;
                if (old_status != status_) is_change_status_ = true;
                else is_change_status_ = false;
            }
            else if (is_dead_)
            {
                status_ = DEADTH_LEFT;
                input_type_.right_ = 0;
                input_type_.left_ = 0;
                if (old_status != status_) is_change_status_ = true;
                else is_change_status_ = false;
            }
            else
            {
                if (time_to_load_attack2 == 50)
                {
                    status_ = ATTACK2_LEFT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                    time_to_load_attack2 = 0;
                    relative_pos = -1;
                }
                if (relative_pos >= DISTANCE_TO_FIRE && relative_pos <= DISTANCE_TO_MOVE)
                {
                    status_ = WALK_LEFT;
                    input_type_.left_ = 1;
                    input_type_.right_ = 0;
                }
                else if (relative_pos > DISTANCE_TO_MOVE)
                {
                    status_ = IDLE_LEFT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                }
                else if (relative_pos >= 0)
                {
                    status_ = ATTACK1_LEFT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                }
            }
        }

        else if (relative_pos <= 0)
        {
            if (is_hurt_)
            {
                status_ = HURT_RIGHT;
                input_type_.right_ = 0;
                input_type_.left_ = 0;
                if (old_status != status_) is_change_status_ = true;
                else is_change_status_ = false;
            }
            else if (is_dead_)
            {
                status_ = DEADTH_RIGHT;
                input_type_.right_ = 0;
                input_type_.left_ = 0;
                if (old_status != status_) is_change_status_ = true;
                else is_change_status_ = false;
            }
            else
            {
                if (time_to_load_attack2 == 50)
                {
                    status_ = ATTACK2_RIGHT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                    time_to_load_attack2 = 0;
                    relative_pos = 1;
                }
                if (relative_pos <= -DISTANCE_TO_FIRE && relative_pos >= -DISTANCE_TO_MOVE)
                {
                    status_ = WALK_RIGHT;
                    input_type_.right_ = 1;
                    input_type_.left_ = 0;
                }
                else if (relative_pos < -DISTANCE_TO_MOVE)
                {
                    status_ = IDLE_RIGHT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                }
                else if (relative_pos < 0)
                {
                    status_ = ATTACK1_RIGHT;
                    input_type_.right_ = 0;
                    input_type_.left_ = 0;
                }
            }
        }


        if (input_type_.left_ == 1)
            x_val_ -= BOSS_SPEED;
        else if (input_type_.right_ == 1)
            x_val_ += BOSS_SPEED;
            
        if (input_type_.jump_ == 1)
        {
            if (on_ground_ == true)
                y_val_ = - BOSS_HIGHT_VAL;
            on_ground_ = false; // set lại ở đây để tránh lỗi rời rạc khi nhảy
            input_type_.jump_ = 0;
        }

        CheckColliWithMap(g_map);
}

//void BossObject::InitPlayer()
//{
//    on_ground_ = false;
//    if (x_pos_ > 4*TILE_SIZE)
//    {
//        x_pos_ -= 4*TILE_SIZE;
//    }
//    else x_pos_ = 0;
//    y_pos_ = 0;
//    x_val_ = 0;
//    y_val_ = 0;
//    think_time_ = 0;
//    input_type_.left_ = 1;
//}
//
//void BossObject::CenterEntityOnMap(Map& map_data)
//{
//    map_data.start_x_ = x_pos_ - SCREEN_WIDTH/2;
//    if (map_data.start_x_ < 0)
//        map_data.start_x_ = 0;
//    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
//        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
//
//    map_data.start_y_ = y_pos_ - SCREEN_HEIGHT/2;
//    if (map_data.start_y_ < 0)
//        map_data.start_y_ = 0;
//    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
//        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
//}

void BossObject::CheckColliWithMap(Map& map_data) //hàm quan trọng check colision 
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

    // Check x1, x2 with full width of map
    // Check y1, y2 with full height of map
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // main obj mov to right
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

                //hàm cũ
                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == RIGHT_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == RIGHT_BLOCK ||
                    val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3)
                {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -= (width_frame_ + 1);
                    x_val_ = 0;
                }
        }
        else if (x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

                // hàm cũ
                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == LEFT_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == LEFT_BLOCK ||
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
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

                // hàm cũ
            if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == DOWN_BLOCK ||
                val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 || val1 == SPECIAL_BLOCK ||
                val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == DOWN_BLOCK ||
                val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3 || val2 == SPECIAL_BLOCK)
                {
                    y_pos_ = y2*TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    y_val_ = 0;
                    on_ground_ = true;
                }
        }
        else if (y_val_ < 0)
        {
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

    //If there is not collision with map tile
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
        x_pos_ = 0;
    else if (x_pos_ + width_frame_ > map_data.max_x_)
        x_pos_ = map_data.max_x_ - width_frame_ - 1;

    /*if (y_pos_ > map_data.max_y_)
    {
        think_time_ = 60;
    }*/
}

void BossObject::InitBullet(SDL_Renderer* screen)
{
    BulletObject* p_bullet = new BulletObject();
    if (time_to_load_attack2 == 0)
    {
        bool ret = p_bullet->LoadImg("img//boss-attack2-bullet.png", screen);
        if (ret)
        {
            if (status_ == ATTACK2_LEFT)
                p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            else
                p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
            p_bullet->set_is_move(true);
            if (status_ == ATTACK2_LEFT)
                p_bullet->SetRect(rect_.x - 800, rect_.y + 5);
            else
                p_bullet->SetRect(rect_.x + width_frame_, rect_.y + 5);
            p_bullet->set_x_val(SPEED_OF_BULLET);
            bullet_list_.push_back(p_bullet);
        }
    }
    else
    {
        bool ret = p_bullet->LoadImg("img//boss-attack1-bullet.png", screen);
        if (ret)
        {
            if (status_ == ATTACK1_LEFT)
                p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            else
                p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
            p_bullet->set_is_move(true);
            if (status_ == ATTACK1_LEFT)
                p_bullet->SetRect(rect_.x - 50, rect_.y + 5);
            else
                p_bullet->SetRect(rect_.x + 50, rect_.y + 5);
            p_bullet->set_x_val(SPEED_OF_BULLET);
            bullet_list_.push_back(p_bullet);
        }
    }
}

void BossObject::MakeBullet(Mix_Chunk* Bullet_sound, SDL_Renderer* des, const int& x_limit_left, const int& x_limit_right, const int& y_limit_up, const int& y_limit_down, int player_x_pos)
{
    if (frame_ == FRAME_NUM_8/2 && abs(player_x_pos - x_pos_) <= DISTANCE_TO_FIRE && time_to_load_attack2 != 0)
        InitBullet(des);

    if ((frame_ == 1 || frame_ == 4) && (status_ == ATTACK2_LEFT || status_ == ATTACK2_RIGHT))
        InitBullet(des);

    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                Mix_PlayChannel(-1, Bullet_sound, 0);
                p_bullet->HandleMove(x_limit_left, x_limit_right, y_limit_up, y_limit_down);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet->Free();
                bullet_list_.erase(bullet_list_.begin() + i);
            }
        }
    }
}

void BossObject::RemoveBullet(const int& idx)
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

SDL_Rect BossObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void BossObject::UpdateImageObject(SDL_Renderer* des, Mix_Chunk* broken_sound)
{
    if (status_ == IDLE_LEFT) LoadImg("img//boss-idle-left.png", des);
    else if (status_ == IDLE_RIGHT) LoadImg("img//boss-idle-right.png", des);
    else if (status_ == WALK_LEFT) LoadImg("img//boss-walk-left.png", des);
    else if (status_ == WALK_RIGHT) LoadImg("img//boss-walk-right.png", des);
    else if (status_ == ATTACK1_LEFT) LoadImg("img//boss-attack1-left.png", des);
    else if (status_ == ATTACK1_RIGHT) LoadImg("img//boss-attack1-right.png", des);
    else if (status_ == ATTACK2_LEFT) LoadImg("img//boss-attack2-left.png", des);
    else if (status_ == ATTACK2_RIGHT) LoadImg("img//boss-attack2-right.png", des);
    else if (status_ == HURT_LEFT)
    {
        LoadImg("img//boss-hurt-left.png", des);
        if (is_change_status_) frame_ = 0;
    }
    else if (status_ == HURT_RIGHT)
    {
        LoadImg("img//boss-hurt-right.png", des);
        if (is_change_status_) frame_ = 0;
    }
    else if (status_ == DEADTH_LEFT)
    {
        Mix_PlayChannel(-1, broken_sound, 0);
        LoadImg("img//boss-death-left.png", des);
        if (is_change_status_) frame_ = 0;
    }
    else if (status_ == DEADTH_RIGHT)
    {
        Mix_PlayChannel(-1, broken_sound, 0);
        LoadImg("img//boss-death-right.png", des);
        if (is_change_status_) frame_ = 0;
    }
}
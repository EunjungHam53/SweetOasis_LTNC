#include "stdafx.h"
#include "Player.h"

PlayerObject::PlayerObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    delta_x_ = 0;
    delta_y_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = IDLE_RIGHT;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    return_time_ = 0;
    gem_count_ = 0; //b10
    num_falling_ = 0;
}

PlayerObject::~PlayerObject()
{

}

bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = ImageObjectBase::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = rect_.w/NUM_OF_FRAMES;
        height_frame_ = rect_.h;
    }

    return ret;
}

SDL_Rect PlayerObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void PlayerObject::set_clips_frame()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < NUM_OF_FRAMES; i++)
        {
            frame_clip_[i].x = width_frame_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void PlayerObject::Show(SDL_Renderer* des)
{
    UpdateImagePlayer(des);
    
    frame_++;

    if (frame_ >= NUM_OF_FRAMES)
    {
        frame_ = 0;
    }

    if (return_time_ == 0) // bớt tgian xử lí nvat ngoài vùng map
    {
        rect_.x = x_pos_ - map_x_; //x_pos_ vtri tren map, rect.x vtri trên screen
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];

        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        SDL_RenderCopy(des, p_texture_, current_clip, &renderQuad);
    }
}

void PlayerObject::HandleInput(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                UpdateImagePlayer(screen);
            }
            break;
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                UpdateImagePlayer(screen);
            }
            break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type_.right_ = 0;
                status_ = IDLE_RIGHT;
            }
            break;
        case SDLK_LEFT:
            {
                input_type_.left_ = 0;
                status_ = IDLE_LEFT;
            }
            break;
        }
    }

    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type_.jump_ = 1;
        }
        else if (events.button.button == SDL_BUTTON_LEFT)
        {
            if (gem_count_ > 0)
            {
                BulletObject* p_bullet = new BulletObject();
                p_bullet->set_bullet_type(BulletObject::GEM_BULLET);
                p_bullet->LoadImgBullet(screen);
                //Mix_PlayChannel(-1, bullet_sound, 0);

                if (status_ == WALK_LEFT || status_ == IDLE_LEFT)
                {
                    p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                    p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_ * 0.2);
                }
                else
                {
                    p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                    p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_ * 0.2);
                }

                p_bullet->set_x_val(20);
                p_bullet->set_y_val(20);
                p_bullet->set_is_move(true);
                p_bullet_list_.push_back(p_bullet);

                gem_count_--;
            }
        }
    }
}

void PlayerObject::HandleBullet(SDL_Renderer* des)
{
    for (int i = 0; i < p_bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                p_bullet->HandleMove(this->GetRectFrame().x-400, this->GetRectFrame().x + 450, 
                                     this->GetRectFrame().y-200, this->GetRectFrame().y + 250);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if (p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void PlayerObject::RemoveBullet(const int& idx)
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void PlayerObject::HandleMove(Map& map_data)
{
    if (return_time_ == 0)
    {
        delta_x_ = 0;
        delta_y_ += GRAVITY_SPEED;

        if (delta_y_ >= MAX_FALL_SPEED)
            delta_y_ = MAX_FALL_SPEED;

        if (input_type_.left_ == 1)
            delta_x_ -= PLAYER_SPEED;
        else if (input_type_.right_ == 1)
            delta_x_ += PLAYER_SPEED;

        if (input_type_.jump_ == 1)
        {
            if (on_ground_ == true)
                delta_y_ = - PLAYER_JUMP_VAL;
            on_ground_ = false; // set lại ở đây để tránh lỗi rời rạc khi nhảy
            input_type_.jump_ = 0;
        }

        CheckColliWithMap(map_data);
        CenterEntityOnMap(map_data);
    }

    if (return_time_ > 0)
    {
        return_time_ --;
        if (return_time_ == 0) //Reset again
        {
            on_ground_ = false;
            if (x_pos_ > 4*TILE_SIZE)
            {
                x_pos_ -= 4*TILE_SIZE;
            }
            else x_pos_ = 0;

            y_pos_ = 2*TILE_SIZE;
            delta_x_ = 0;
            delta_y_ = 0;
        }
    }
}

void PlayerObject::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x_ = x_pos_ - SCREEN_WIDTH/2;
    if (map_data.start_x_ < 0)
        map_data.start_x_ = 0;
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;

    map_data.start_y_ = y_pos_ - SCREEN_HEIGHT/2;
    if (map_data.start_y_ < 0)
        map_data.start_y_ = 0;
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
}

void PlayerObject::CheckColliWithMap(Map& map_data) //hàm quan trọng check colision 
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Check horizontal (ý nghĩa: check frame nhân vật đang nằm ở ô thứ bao nhiêu của bản đồ tile)
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = (x_pos_ + delta_x_)/TILE_SIZE;
    x2 = (x_pos_ + delta_x_ + width_frame_ - 1)/TILE_SIZE;
    
    y1 = y_pos_/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    // Check x1, x2 with full width of map
    // Check y1, y2 with full height of map
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (delta_x_ > 0) // main obj mov to right
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == LEFT_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 || val1 == BRICK ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == LEFT_BLOCK ||
                    val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3 || val2 == BRICK)
                {
                    x_pos_ = x2*TILE_SIZE;
                    x_pos_ -= (width_frame_ + 1);
                    delta_x_ = 0; // cannot moving
                }
        }
        else if (delta_x_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == RIGHT_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 || val1 == BRICK ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == RIGHT_BLOCK ||
                    val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3 || val2 == BRICK)
                {
                    x_pos_ = (x1 + 1)*TILE_SIZE;
                    delta_x_ = 0;
                }
        }
    }

    //Check vertical
    int width_min = min(width_frame_, TILE_SIZE);
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + delta_y_)/TILE_SIZE;
    y2 = (y_pos_ + delta_y_ + height_frame_ - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (delta_y_ > 0)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == WOOD || val1 == SPECIAL_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 || val1 == BRICK ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == WOOD || val2 == SPECIAL_BLOCK ||
                    val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3 || val2 == BRICK)
                {
                    y_pos_ = y2*TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    delta_y_ = 0;
                    on_ground_ = true;
                }
            
        }
        else if (delta_y_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

                if (val1 == EARTH1_1 || val1 == EARTH1_2 || val1 == EARTH1_3 || val1 == UP_BLOCK ||
                    val1 == EARTH2_1 || val1 == EARTH2_2 || val1 == EARTH2_3 || val1 == SPECIAL_BLOCK ||
                    val2 == EARTH1_1 || val2 == EARTH1_2 || val2 == EARTH1_3 || val2 == UP_BLOCK ||
                    val2 == EARTH2_1 || val2 == EARTH2_2 || val2 == EARTH2_3 || val1 == SPECIAL_BLOCK) //check xem nhảy lên có cụng đầu viên gạch 0
                {
                    y_pos_ = (y1 + 1)*TILE_SIZE;
                    delta_y_ = 0;
                }
            
        }
    }

    //If there is not collision with map tile
    x_pos_ += delta_x_;
    y_pos_ += delta_y_;

    if (x_pos_ < 0)
        x_pos_ = 0;
    else if (x_pos_ + width_frame_ > map_data.max_x_)
        x_pos_ = map_data.max_x_ - width_frame_ - 1;

    if (y_pos_ > map_data.max_y_)
    {
        num_falling_++;
        return_time_ = 60;
    }
}

void PlayerObject::IncreaseGem()
{
    gem_count_++;
}

void PlayerObject::UpdateImagePlayer(SDL_Renderer* des)
{
    if (on_ground_ == true)
    {
        if (status_ == WALK_LEFT) // Ko dùng input_type_.left_ vì khi ấn left rồi nhả ra sẽ về 0
            LoadImg("img//player-left.png", des);
        else if (status_ == WALK_RIGHT)
            LoadImg("img//player-right.png", des);
        else if (status_ == IDLE_LEFT)
            LoadImg("img//player-idle-left.png", des);
        else
            LoadImg("img//player-idle-right.png", des);
    }
    else
    {
        if (status_ == WALK_LEFT || status_ == IDLE_LEFT)
            LoadImg("img//player-jump-left.png", des);
        else
            LoadImg("img//player-jump-right.png", des);
    }
}
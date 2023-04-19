
#ifndef PLAYER_H_
#define PLAYER_H_

#include "General.h"
#include "ImgObjectBase.h"
#include "BulletObject.h"

#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 18
#define NUM_OF_FRAMES 6 

class PlayerObject : public ImageObjectBase
{
public:
    PlayerObject();
    ~PlayerObject();

    enum WalkType
    {
        IDLE_LEFT = 0,
        IDLE_RIGHT = 1,
        WALK_RIGHT = 2,
        WALK_LEFT = 3,
    };

    void HandleInput(SDL_Event events, SDL_Renderer* screen);
    bool LoadImg (std::string path, SDL_Renderer* screen);
    void set_clips_frame();
    void UpdateImagePlayer(SDL_Renderer* des);
    void Show(SDL_Renderer* des);

    void HandleMove (Map& map_data);
    void CheckColliWithMap (Map& map_data);
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data);

    SDL_Rect GetRectFrame();
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }

    void set_bullet_list(std::vector<BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std::vector<BulletObject*> get_bullet() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);

    void set_comeback_time(const int& cb_time) {return_time_ = cb_time;}
    bool get_num_falling() const { return num_falling_; }
    void erase_num_falling() { num_falling_ = 0; }

    void IncreaseGem();
    int GetGemCount() const {return gem_count_;}
private:
    int gem_count_; 
    std::vector<BulletObject*> p_bullet_list_;

    float delta_x_;
    float delta_y_;

    float x_pos_;
    float y_pos_;

    int width_frame_; 
    int height_frame_;

    SDL_Rect frame_clip_[NUM_OF_FRAMES];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    int return_time_;
    int num_falling_;
};

#endif
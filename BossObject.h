#ifndef BOSS_OBJECT_H_
#define BOSS_OBJECT_H_

#include "ImgObjectBase.h"
#include "General.h"
#include "BulletObject.h"

#define BOSS_SPEED 5
#define BOSS_HIGHT_VAL 18

#define FRAME_NUM_8 8

#define DISTANCE_TO_FIRE 200
#define DISTANCE_TO_LASER 1000
#define DISTANCE_TO_MOVE 800
#define SPEED_OF_BULLET 30

class BossObject : public ImageObjectBase
{
public:
    BossObject();
    ~BossObject();

    enum status
    {
        IDLE_LEFT = 0,
        IDLE_RIGHT = 1,
        WALK_LEFT = 2,
        WALK_RIGHT = 3,
        ATTACK1_LEFT = 4,
        ATTACK1_RIGHT = 5,
        ATTACK2_LEFT = 6,
        ATTACK2_RIGHT = 7,
        HURT_LEFT = 8,
        HURT_RIGHT = 9,
        DEADTH_LEFT = 10,
        DEADTH_RIGHT = 11,
    };

    void set_x_val(int xp) {x_val_ = xp;}
    void set_y_val(int yp) {y_val_ = yp;}
    void set_xpos(const int& xps) {x_pos_ = xps;}
    void set_ypos(const int& yps) {y_pos_ = yps;}
    int get_x_pos() const {return x_pos_;}
    int get_y_pos() const {return y_pos_;}
    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }

    void Show(SDL_Renderer* des, Mix_Chunk* broken_sound);
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void UpdateImageObject(SDL_Renderer* des, Mix_Chunk* broken_sound);
    void set_clips();

    SDL_Rect GetRectFrame();
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}

    void CheckColliWithMap(Map& map_data);
    void HandleMove(Map& g_map, int player_x_pos, int player_y_pos, Mix_Chunk* mecha_sound);
    std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const std::vector<BulletObject*> &am_list) {bullet_list_ = am_list;}

    void InitBullet(SDL_Renderer* screen);
    void MakeBullet(Mix_Chunk* Bullet_sound, SDL_Renderer* screen, const int& x_limit_left, const int& x_limit_right, const int& y_limit_up, const int& y_limit_down, int player_x_pos);
    void RemoveBullet(const int& idx);

    void IncreaseTimeToLoadAttack2() { time_to_load_attack2++; }

    void set_is_hurt(bool isHurt) { is_hurt_ = isHurt; }
    void set_is_dead(bool isDead) { is_dead_ = isDead; }
    bool get_whether_appear_or_not() const { return appear_; }
private:
    int map_x_;
    int map_y_;
    bool on_ground_;
    Input input_type_;
    int frame_;
    SDL_Rect frame_clip_[FRAME_NUM_8];
    int x_pos_;
    int y_pos_;
    float x_val_;
    float y_val_;
    int width_frame_;
    int height_frame_;
    std::vector<BulletObject*> bullet_list_;

    int status_;
    bool is_change_status_;
    bool is_hurt_;
    bool is_dead_;
    bool appear_;

    int time_to_load_attack2;
    bool is_mechanical_sound_played;
};

#endif
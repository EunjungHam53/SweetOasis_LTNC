#ifndef ENEMY_OBJECT_H_
#define ENEMY_OBJECT_H_

#include "General.h"
#include "ImgObjectBase.h"
#include "BulletObject.h"

#define ENEMY_FRAME_NUM 6
#define ENEMY_SPEED 3
#define FROG_JUMP 15

class EnemyObject : public ImageObjectBase
{
public:
    EnemyObject();
    ~EnemyObject();

    enum TypeEnemy
    {
        FROG = 0,
        OPOSUM = 1,
        EAGLE = 2,
    };

    void set_x_val(const float& xVal) {x_val_ = xVal;}
    void set_y_val(const float& yVal) {y_val_ = yVal;}
    void set_x_pos(const float& xp) {x_pos_ = xp;}
    void set_y_pos(const float& yp) {y_pos_ = yp;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    void set_type_enemy(const int& typeEnemy) { type_enemy_ = typeEnemy; }
    void set_input_left(const int& ipLeft) { input_type_.left_ = ipLeft; }
    void SetMapXY(const int& mp_x, const int& mp_y) {map_x_ = mp_x; map_y_ = mp_y;}

    SDL_Rect GetRectFrame();
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }

    void set_clips();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void UpdateImgFromMoveType(SDL_Renderer* screen);
    void Show(SDL_Renderer* des);

    void HandleMove(Map& gMap, Mix_Chunk* frog_sound);
    void CheckColliWithMap(Map& gMap);
    void SetAnimationLimit(const int& pos_a, const int& pos_b) {animation_a_ = pos_a; animation_b_ = pos_b;}

    std::vector<BulletObject*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const std::vector<BulletObject*>& bl_list) {bullet_list_ = bl_list;}
    
    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
    void HandleBullet(SDL_Renderer* screen, const int& x_limit_left, const int& x_limit_right, const int& y_limit_up, const int& y_limit_down);
    void RemoveBullet(const int& idx);

    void InitEnemies(); // hàm khởi tạo lại threats khi rơi xuống vực
private:
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    int map_x_;
    int map_y_;
    bool on_ground_;
    int come_back_time_;
    SDL_Rect frame_clip_[ENEMY_FRAME_NUM];
    int width_frame_;
    int height_frame_;
    int frame_;

    int type_enemy_;
    int animation_a_; // giới hạn di chuyển a -> b
    int animation_b_;
    Input input_type_;

    std::vector<BulletObject*> bullet_list_;

    int frog_jump_time_;
    bool sound_frog_played_;
};

#endif
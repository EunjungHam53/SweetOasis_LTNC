#ifndef EXP_H_
#define EXP_H_

#include "General.h"
#include "ImgObjectBase.h"

#define NUM_ENEMY_EXP 6

class ExplosionObject : public ImageObjectBase
{
public:
    ExplosionObject();
    ~ExplosionObject();

    void set_clip();
    void set_frame(const int& fr) {frame_ = fr;}
    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    int get_frame_width() const {return frame_width_;}
    int get_frame_height() const {return frame_height_;}

private:
    int frame_width_;
    int frame_height_;

    unsigned int frame_;
    SDL_Rect frame_clip_[NUM_ENEMY_EXP];
};

#endif
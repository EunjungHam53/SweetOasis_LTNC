#ifndef EXTRA_ITEM_H_
#define EXTRA_ITEM_H_

#include "General.h"
#include "ImgObjectBase.h"

class PlayerHP : public ImageObjectBase
{
public:
    PlayerHP();
    ~PlayerHP();

    void SetNum(const int& num) {number_ = num;}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);
    
    // hàm tăng & giảm hp
    void Increase();
    void Decrease();

private:
    int number_;
    std::vector<int> pos_list_;
};

class PlayerGem : public ImageObjectBase
{
public:
    PlayerGem();
    ~PlayerGem();

    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) {x_pos_ = x; y_pos_ = y;}
private:
    int x_pos_;
    int y_pos_;
};

#endif
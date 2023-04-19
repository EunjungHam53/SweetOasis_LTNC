#ifndef IMG_OBJECT_BASE
#define IMG_OBJECT_BASE

#include "General.h"

class ImageObjectBase
{
public:
    ImageObjectBase();
    ~ImageObjectBase();
    void SetRect(const int& x, const int& y) {rect_.x = x, rect_.y = y;}
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture* GetObject() const {return p_texture_;}

    virtual bool LoadImg(std::string path, SDL_Renderer* screen); // tính da hinh trong c++
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();
protected:
    SDL_Texture* p_texture_; // luu tru cac hinh anh
    SDL_Rect rect_; // luu vi tri
};

#endif
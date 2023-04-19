#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "General.h"
#include "ImgObjectBase.h"

class TextObject : public ImageObjectBase
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* screen, 
                    int xp, int yp, 
                    SDL_Rect* clip = NULL, 
                    double angle = 0, 
                    SDL_Point* center = NULL, 
                    SDL_RendererFlip flip = SDL_FLIP_NONE /* có mirror ko */);

    SDL_Rect GetRectFrame();
    int GetWidth() const {return width_;}
    int GetHeight() const {return height_;}

    void SetText(const std::string& text) {str_val_ = text;}
    std::string GetText() const {return str_val_;}

private:
    std::string str_val_;
    SDL_Color text_color_;
    int width_;
    int height_;
};

#endif
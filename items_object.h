#ifndef EXTRA_ITEMS_H_
#define EXTRA_ITEMS_H_

#include "General.h"
#include "ImgObjectBase.h"

#define ITEM_FRAME_NUMS 5

class ItemsObject : public ImageObjectBase
{
public:
	ItemsObject();
	~ItemsObject();

	enum Type
	{
		CHERRY = 0,
		GEM = 1,
	};

	void set_type(const int& type) { type_ = type; }
	int get_type() const { return type_; }

	SDL_Rect GetRectFrame();

	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }

private:
	int type_;

	SDL_Rect frame_clip_[ITEM_FRAME_NUMS];
	int width_frame_;
	int height_frame_;
	int frame_;

	float x_pos_;
	float y_pos_;
	int map_x_;
	int map_y_;
};

#endif // !EXTRA_ITEMS_H


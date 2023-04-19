### Họ và tên: Lê Xuân Hùng
### MSV: 22028172

# GAME SWEET OASIS
## Mục lục:

##### 1. Tổng quan về game:
###### --- [1.1 Giới thiệu:](#introduction)
###### ---[1.2 Kiến thưc áp dụng:](#knowledge)
###### ---[1.3 GamePlay:](#play)
##### 2. Chi tiết về các đối tượng (class) được sử dụng trong game:
###### --- [General.h](#gene)
###### --- [ImgObjectBase.h](#img)
###### --- [Player.h](#player_)
###### --- [EnemiesObject.h](#enemies_)
###### --- [BossObject.h](#boss_)
###### --- [BulletObject.h](#bullet)
###### --- [Explosion.h](#explosion)
###### --- [items_object.h](#item1)
###### --- [ExtraItemIcon.h](#item2)
###### --- [TextObject.h](#text)
###### --- [Timer.h](#timer)
###### --- [RectangleDraw.h](#rect)
###### --- [main.cpp](#main)

##
------------------------------------------------
------------------------------------------------
------------------------------------------------


## 1. Tổng quan về game:
### 1.1 Giới thiệu: <a name="introduction"></a>
#### [DEMO GAME TRÊN YOUTUBE]
SWEET OASIS là game được phát triển bởi Lê Xuân Hùng, MSV 22028172 cho bài tập lớn môn LTNC INT2215 23.
Game dựa trên game Mario tuổi thơ. Game có xử lí nổi bật về map tiles. 
Game dùng chuột và các nút <-, ->.

### 1.2 Kiến thức áp dụng: <a name="knowledge"></a>
####
- Ngôn ngữ lập trình C++.
- Thư viện đồ họa SDL2 qua slide cô Châu và lazyfoo.
- Kiến thức về OOP: lớp, đối tượng, tính đóng gói, tính thừa kế, lớp trừu tượng. (tham khảo qua giáo trình OOP_2013 của ĐHCN-ĐHQGHN)
### 1.3 ***GamePlay***: <a name="play"></a>
####
##### Cửa số menu: có 2 options: nhấn *play game* để chơi; nhán *Exit* để thoát
##### Về *luật chơi* trong game:
- Các nút điều khiển trong game:
Nút SDLK_RIGHT: di chuyển Player sang phải.
Nút SDLK_LEFT: di chuyển Player sang trái.
Chuột trái: Player nhảy.
Chuột phải: Player bắn 1 Gem.
- Ban đầu Player sẽ có 3 mạng sống. Player sẽ mất mạng khi *va chạm vào enemy*, *va chạm vào đạn của enemy hoặc boss*, *rơi xuống vực*. Khi còn hết biểu tượng tim trên thanh Hotbar, Player bị mất mạng thêm lần nữa thì sẽ thua. Player có thể tăng số mạng lên bằng cách ăn *CHERRY*. Khi Player còn dưới 3HP mà >= 3 CHERRY thì sẽ được hồi lại HP.
- Player có thể ăn *GEM* để làm đạn bắn. Mỗi lần Player bắn trúng được enemy, score sẽ tăng 1 đơn vị. Player cần chú ý tiết kiệm đạn bắn để đến cuối gặp boss.
- Player cần bắn trúng 4 lần thì mới giết được Boss.
- Player cần giết được Boss và đi đến cuối chặng đường để dành chiến thắng.
##### Về hiệu ứng của các nhân vật trong game:
###### *Player*: 
Do người chơi điều khiển có thể nhảy di chuyển trái phải, bắn,...; có animation ứng với từng trạng thái riêng.
###### *Enemies*:
- *FROG*: khi màn hình chuyển đến FROG sẽ có âm thanh đặc trưng và cách 1 khoảng thời gian FROG sẽ thực hiện việc nhảy.
- *OPOSUM*: có khả năng di chuyển qua lại trong 1 khoảng a, b nhất định.
- *EAGLE*: lơ lửng trên không và có thể bắn đạn.
###### *BOSS*:
- Đây là loại nhân vật được đầu tư nhất về hiệu ứng. Ứng với mỗi trạng thái sẽ có các hiệu ứng riêng.
- Ở trạng thái bình thường, Boss không di chuyển (**IDLE**).
- Khi Boss cách Player 600, Boss sẽ *phát ra âm thanh báo hiệu*.
- Khi Boss cách Player 1 đoạn DISTANCE_TO_MOVE, Boss sẽ tự di chuyển (**WALK**) đến vị trí của Player. Và khi Boss cách Player 1 đoạn DISTANCE_TO_FIRE, Boss sẽ bắn lazer thông thường (**ATTACK1**).
- Đặc biệt, sau 1 khoảng thời gian (**time_to_load_attack2**), Boss sẽ bắn lazer cấp cao, kĩ năng tấn công tầm xa của Boss (**ATTACK2**).
- Có các loại âm thanh riêng dành cho ATTACK1, ATTACK2.
- Khi Boss bị thương do ăn GEM của Player, Boss sẽ chuyển sang trạng thái **HURT**. Khi chết, Boss chuyển sang trạng thái cuối là **DEADTH** kèm hiệu ứng âm thanh.
##### Ngoài ra còn có các cửa sổ: GAME_OVER, WINNER với các âm thanh vui nhộn.

## 2. Chi tiết về các đối tượng (class) được sử dụng trong game:
### <a name="gene"></a>General.h:
#### Chứa những biến toàn cục liên quan đến window, screen(renderer), event, include các thư viện SDL2.
- 8 loại âm thanh trong game.
- Thông số về màn hình: SCREEN_WIDTH, SCREEN_HEIGHT, FPS, mã màu RGB để lọc ảnh.
- Define các ô tilemap đặc biệt để check collision với các đối tượng di chuyển (có hàm riêng CheckColliWithMap của từng đối tượng).
- Struct Input cho các đối tượng di chuyển để lưu trạng thái rồi xử lý ở các hàm HandleMove riêng từng đối tượng.
- Struct Map: lưu dữ liệu cho gamemap.
- Các hàm CheckCollision - hàm bool trả về trạng thái va chạm giữa 2 nhân vật; hàm CheckFocusWithRect - hàm bool trả về trạng thái chuột di vào chữ chưa để đổi màu(có trong menu game)

### <a name="img"></a>ImgObjectBase.h: 
#### Chứa lớp đối tượng về hình ảnh đơn giản là lớp cha để các lớp player, enemies, boss, extra_item... kế thừa gồm các thuộc tính rect (lưu vị trí xuất hiện trên renderer) và p_texture_ (biến texture lưu dữ liệu ảnh)
#### Chủ yếu hàm này để load các tile map và load background, các đối tượng ảnh đơn giản không có sprite

### <a name="player_"></a>Player.h:
#### Chứa lớp đối tượng Player về nhân vật người chơi điều khiển được kế thừa thuộc tính cơ bản từ lớp ImgObjectBase
#### Lưu ý: các nhóm hàm về xử lí hình ảnh, di chuyển, đạn của lớp Player cũng giống về mặt *xử lí* của lớp Enemies, Boss nhưng khác nhau về *kiểu* (vd: Player sẽ đi được trên ô tím nhưng các đối tượng khác sẽ không đi được do hàm CheckColliWithMap nhận kiểu khác nhau)
####
*public:*
##### Nhóm hàm xử lí hình ảnh
- enum WalkType: dành cho thuộc tính status_
- void HandleInput(SDL_Event events, SDL_Renderer* screen): nhận sự kiện từ bàn phím rồi lưu vào biến input_type_
- bool LoadImg (std::string path, SDL_Renderer* screen): load ảnh vào p_texture_
- void set_clips_frame(): hàm lưu vị trí các khung hình để tạo sprite
- void UpdateImagePlayer(SDL_Renderer* des): hàm load lại các ảnh theo từng trạng thái mới cập nhật
- void Show(SDL_Renderer* des): render ảnh vào màn hình
    
##### Nhóm hàm xử lí di chuyển
- void HandleMove (Map& map_data): hàm xử lí về di chuyển nhân vật
- void CheckColliWithMap (Map& map_data): hàm check những ô tile map gây ảnh hưởng lên nhân vật (vd: đứng trên mặt đất, bị chặn khi đâm đầu vào tường, nhân vật rơi ra khỏi màn hình...)
- void SetMapXY(const int map_x, const int map_y)
- void CenterEntityOnMap(Map& map_data): điều khiển màn hình di chuyển theo nhân vật

##### Nhóm hàm lấy dữ liệu các thuộc tính
- SDL_Rect GetRectFrame()
- float get_x_pos() const { return x_pos_; }
- float get_y_pos() const { return y_pos_; }

##### Nhóm hàm xử lí về đạn
- void set_bullet_list(std::vector<BulletObject*> bullet_list)
- std::vector<BulletObject*> get_bullet() const {return p_bullet_list_;}
- void HandleBullet(SDL_Renderer* des);
- void RemoveBullet(const int& idx);
    
##### Nhóm hàm xử lí việc hồi lai nhân vật sau khi mất mạng
- void set_comeback_time(const int& cb_time) {return_time_ = cb_time;}
- bool get_num_falling() const { return num_falling_; }
- void erase_num_falling() { num_falling_ = 0; }

##### Nhóm hàm xử lí về gem (đạn bán của nhân vật)
- void IncreaseGem();
- int GetGemCount() const {return gem_count_;}
####
*private:*
- int gem_count_; 
- std::vector<BulletObject*> p_bullet_list_;

- float delta_x_;
- float delta_y_;

- float x_pos_;
- float y_pos_;

- int width_frame_; 
- int height_frame_;

- SDL_Rect frame_clip_[NUM_OF_FRAMES];
- Input input_type_;
- int frame_;
- int status_;
- bool on_ground_;

- int map_x_;
- int map_y_;

- int return_time_;
- int num_falling_;

### <a name="enemies_"></a>EnemiesObject.h:
#### Lưu ý: xem trước giải thích các hàm ở lớp Player
#### Về lớp EnemiesObject có 3 loại Enemy: FROG, OPOSUM, EAGLE.
#### Các phương thức và thuộc tính khác biệt của lớp
*public:*
- void HandleMove(Map& gMap, Mix_Chunk* frog_sound): xử lí di chuyển ứng với từng kiểu enemy, còn có thêm *hiệu ứng âm thanh cho FROG* và *cách tự di chuyển của OPOSUM* 
- void SetAnimationLimit(const int& pos_a, const int& pos_b): hiệu ứng riêng của OPOSUM - có thể di chuyển trong khoảng vị trí từ a đến b

*private:*
- int type_enemy_;
- int animation_a_;
- int animation_b_;
- int frog_jump_time_;
- bool sound_frog_played_;

### <a name="boss_"></a>BossObject.h:
#### Lưu ý: xem trước giải thích các hàm ở lớp Player
#### Lớp BossObject có 6 trạng thái chính: IDLE, WALK, ATTACK1, ATTACK2, HURT, DEADTH.
#### Các phương thức và thuộc tính khác biệt của lớp
*public:*
- void HandleMove(Map& g_map, int player_x_pos, int player_y_pos, Mix_Chunk* mecha_sound): bao gồm các xử lí ứng với từng trạng thái nhân vật, lồng thêm âm thanh
- void MakeBullet(Mix_Chunk* Bullet_sound, SDL_Renderer* screen, const int& x_limit_left, const int& x_limit_right, const int& y_limit_up, const int& y_limit_down, int player_x_pos): xử lí đạn bắn theo từng kiểu ATTACK1, ATTACK2, có âm thanh đạn bắn
- void IncreaseTimeToLoadAttack2()
- void set_is_hurt(bool isHurt)
- void set_is_dead(bool isDead)
- bool get_whether_appear_or_not()

*private:*
- bool is_change_status_;
- bool is_hurt_;
- bool is_dead_;
- bool appear_;
- int time_to_load_attack2;
- bool is_mechanical_sound_played;

### <a name="bullet"></a>BulletObject.h
#### Chứa lớp BulletObject về đạn bắn cho các loại nhân vật trong game
####
*public:*
##### Nhóm hàm cài đặt tốc độ bắn
- void set_x_val(const int& xVal) {x_val_ = xVal;}
- void set_y_val(const int& yVal) {y_val_ = yVal;}
- int get_x_val() const {return x_val_;}
- int get_y_val() const {return y_val_;}
##### Nhóm hàm về điều kiện is_move_
- void set_is_move(const bool& isMove) {is_move_ = isMove;}
- bool get_is_move() const {return is_move_;}
##### Nhóm hàm về hướng đạn bắn
- void set_bullet_dir(const unsigned int& bulletDir) {bullet_dir_ = bulletDir;}
- unsigned int get_bullet_dir() const {return bullet_dir_;}
##### Nhóm hàm về loại đạn bắn
- void set_bullet_type(const unsigned  int& bulletType) {bullet_type_ = bulletType;}
- unsigned int get_bullet_type() const {return bullet_type_;}
##### Nhóm hàm về xử lí di chuyển đạn và load hình ảnh
- void HandleMove(const int& x_border_left, const int& x_border_right, const int& y_border_up, const int& y_border_down);
- bool LoadImgBullet(SDL_Renderer* des);
####
*private:*
- int x_val_: tốc độ đạn
- int y_val_;
- bool is_move_: điều kiện xét trước khi InitBullet
- unsigned int bullet_dir_: hướng đạn
- unsigned int bullet_type_: kiểu đạn

### <a name="explosion"></a>Explosion.h:
#### Chứa lớp ExplosionObject về vụ nổ. 
#### Trong game có 2 kiểu nổ:
- hiệu ứng khi ăn những items của Player
- hiệu ứng khi đạn va chạm vào các enemies
#### Chủ yếu là load sprite animation sau khi CheckCollison(obj1, obj2) == true

### <a name="item1"></a>items_object.h:
#### Chứa lớp ItemsObject quản lý các vật hỗ trợ trong game (CHERRY, GEM) 
#### Lớp có các hàm về setting vị trí; load sprite animation như các lớp đối tượng trước

### <a name="item2"></a>ExtraItemIcon.h:
#### Chứa lớp PlayerHP quản lí về Icon HP và lớp PlayerGem quản lí về Icon Gem trên thanh Hotbar
#### Đối tượng này chứa hàm xử lí về load ảnh và setting vị trí hiện trên thanh Hotbar (vd: nhân vật tăng thêm mạng thì thanh Hotbar sẽ hiện thêm 1 hình trái tim...)

### <a name="text"></a>TextObject.h:
#### Chứa lớp TextObject quản lí về các text trong game
####
*public:*
- bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen): ta load text vào texture
- void Free();
##### 2 hàm setting màu text
- void SetColor(Uint8 red, Uint8 green, Uint8 blue);
- void SetColor(int type);
- void RenderText(...): hàm render text lên màn hình
##### Các hàm lấy độ dài , rộng:
- SDL_Rect GetRectFrame();
- int GetWidth() const {return width_;}
- int GetHeight() const {return height_;}
##### Các hàm nhập & lấy dữ liệu kiểu string:
- void SetText(const std::string& text) {str_val_ = text;}
- std::string GetText() const {return str_val_;}

### <a name="timer"></a>Timer.h:
#### Chứa lớp Timer quản lí về mặt thời gian cho game:
- Xử lí FPS
- Thời gian thực trên thanh Hotbar

### <a name="rect"></a>RectangleDraw.h:
#### Chứa lớp RectangleForm cho việc vẽ thanh Hotbar bao gồm:
- Vẽ nền trong
- Vẽ 1 cái viền nhỏ bao quanh

### <a name="main"></a>main.cpp:

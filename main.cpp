#include "stdafx.h"
#include "General.h"
#include "ImgObjectBase.h"
#include "game_map.h"
#include "Player.h"
#include "Timer.h"
#include "EnemiesObject.h"
#include "Explosion.h"
#include "TextObject.h"
#include "ExtraItemIcon.h"
#include "RectangleDraw.h"
#include "BossObject.h"
#include "items_object.h"


ImageObjectBase g_background;
ImageObjectBase g_start_menu;
ImageObjectBase g_game_over;
ImageObjectBase g_you_win;
TTF_Font* font_time = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO); //Thiết lập môi trường cho thư viện SDL
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //Thiết lập chế độ tỷ lệ chất lượng

    g_window = SDL_CreateWindow("Game SDL LTNC", 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Tạo tiêu đề, setting dài rộng của cửa sổ win

    if(g_window == NULL)
        success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        // Init text ttf
        if (TTF_Init() == -1)
            success = false;
        font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if (font_time == NULL)
            success = false;

        //Init sound
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            return false;
        //Read file wav audio
        g_sound_blaster = Mix_LoadWAV("sound//blaster.wav");
        g_sound_collect = Mix_LoadWAV("sound//collect.wav");
        g_frog_quak = Mix_LoadWAV("sound//frog_quak.wav");
        g_wow_sound = Mix_LoadWAV("sound//wow.wav");
        g_game_lose = Mix_LoadWAV("sound//game_over.wav");
        g_mechanical_sound = Mix_LoadWAV("sound//mechanicalclamp.wav");
        g_break_robot = Mix_LoadWAV("sound//break_robot.wav");
        g_sound_pop = Mix_LoadWAV("sound//pop.wav");

        if (g_sound_blaster == NULL || g_sound_pop == NULL ||
            g_sound_collect == NULL || g_frog_quak == NULL ||
            g_wow_sound == NULL || g_break_robot == NULL || 
            g_mechanical_sound == NULL || g_game_lose == NULL) 
            return false;
    }
    return success;
}

bool LoadBackground()
{
    bool ret1 = g_background.LoadImg("img//background#.png", g_screen);
    bool ret2 = g_start_menu.LoadImg("img//start_img.jpg", g_screen);
    bool ret3 = g_game_over.LoadImg("img//game_over.png", g_screen);
    bool ret4 = g_you_win.LoadImg("img//you_win.png", g_screen);
    if (ret1 == false || ret2 == false || ret3 == false || ret4 == false)
        return false;
    return true;
}

void close()
{
    g_background.Free();
    g_start_menu.Free();
    g_game_over.Free();
    g_you_win.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

std::vector<EnemyObject*> MakeEnemyList()
{
    std::vector<EnemyObject*> list_enemies;

    EnemyObject* oposum_enemy = new EnemyObject[10];
    for (int i = 0; i < 10; i++)
    {
        EnemyObject* p_enemy = oposum_enemy + i;
        if (p_enemy != NULL)
        {
            p_enemy->LoadImg("img//oposum-left.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_type_enemy(EnemyObject::OPOSUM);
            p_enemy->set_x_pos(1500+500*i);
            p_enemy->set_y_pos(200);

            int pos1 = p_enemy->get_x_pos() - 60;
            int pos2 = p_enemy->get_x_pos() + 60;
            p_enemy->SetAnimationLimit(pos1, pos2);
            p_enemy->set_input_left(1);

            list_enemies.push_back(p_enemy);
        }
    }

    EnemyObject* frog_enemy = new EnemyObject[10];
    for (int i = 0; i < 10; i++)
    {
        EnemyObject* p_enemy = frog_enemy + i;
        if (p_enemy != NULL)
        {
            p_enemy->LoadImg("img//frog-idle.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_x_pos(1800+rand()%24600);
            p_enemy->set_y_pos(250);
            p_enemy->set_type_enemy(EnemyObject::FROG);
            
            list_enemies.push_back(p_enemy);
        }
    }

    EnemyObject* eagle_enemy = new EnemyObject[3];
    
    EnemyObject* p_enemy = eagle_enemy;
    if (p_enemy != NULL)
    {
        p_enemy->LoadImg("img//eagle-attack.png", g_screen);
        p_enemy->set_clips();
        p_enemy->set_x_pos(600);
        p_enemy->set_y_pos(250);
        p_enemy->set_type_enemy(EnemyObject::EAGLE);

        BulletObject* p_bullet = new BulletObject();
        p_enemy->InitBullet(p_bullet, g_screen);

        list_enemies.push_back(p_enemy);
    }
    for (int i = 1; i < 3; i++)
    {
        EnemyObject* p_enemy = eagle_enemy + i;
        if (p_enemy != NULL)
        {
            p_enemy->LoadImg("img//eagle-attack.png", g_screen);
            p_enemy->set_clips();
            p_enemy->set_x_pos(198 * TILE_SIZE + 10*TILE_SIZE*i);
            p_enemy->set_y_pos(250);
            p_enemy->set_type_enemy(EnemyObject::EAGLE);

            BulletObject* p_bullet = new BulletObject();
            p_enemy->InitBullet(p_bullet, g_screen);

            list_enemies.push_back(p_enemy);
        }
    }
    
    return list_enemies;
}

std::vector<ItemsObject*> MakeItemsList()
{
    std::vector<ItemsObject*> list_items;

    ItemsObject* cherry = new ItemsObject[20];
    for (int i = 0; i < 10; i++)
    {
        ItemsObject* p_item = cherry + i;
        if (p_item != NULL)
        {
            p_item->LoadImg("img//cherry.png", g_screen);
            p_item->set_clips();
            p_item->set_type(ItemsObject::CHERRY);
            p_item->set_x_pos(200 + 500 * i);
            p_item->set_y_pos(200);

            list_items.push_back(p_item);
        }
    }
    for (int i = 11; i < 20; i++)
    {
        ItemsObject* p_item = cherry + i;
        if (p_item != NULL)
        {
            p_item->LoadImg("img//cherry.png", g_screen);
            p_item->set_clips();
            p_item->set_type(ItemsObject::CHERRY);
            p_item->set_x_pos(200*TILE_SIZE + 100 * i);
            p_item->set_y_pos(6*TILE_SIZE);

            list_items.push_back(p_item);
        }
    }

    ItemsObject* gem = new ItemsObject[20];
    for (int i = 0; i < 10; i++)
    {
        ItemsObject* p_item = gem + i;
        if (p_item != NULL)
        {
            p_item->LoadImg("img//gem.png", g_screen);
            p_item->set_clips();
            p_item->set_type(ItemsObject::GEM);
            p_item->set_x_pos(10 + 100 * i);
            p_item->set_y_pos(150);

            list_items.push_back(p_item);
        }
    }
    for (int i = 11; i < 20; i++)
    {
        ItemsObject* p_item = gem + i;
        if (p_item != NULL)
        {
            p_item->LoadImg("img//gem.png", g_screen);
            p_item->set_clips();
            p_item->set_type(ItemsObject::GEM);
            p_item->set_x_pos(200*TILE_SIZE + 100 * i);
            p_item->set_y_pos(150);

            list_items.push_back(p_item);
        }
    }
    return list_items;
}

void GameOver()
{
    SDL_RenderClear(g_screen);
    g_game_over.Render(g_screen, NULL);
    SDL_RenderPresent(g_screen);
    Mix_PlayChannel(-1, g_game_lose, 0);
    SDL_Delay(5000);
    close();

    /*if (MessageBox(NULL, L"THUA RỒI", L":>", MB_OK | MB_ICONSTOP) == IDOK)
    {
        close();
        SDL_Quit();
    }*/
}

void YouWin()
{
    SDL_RenderClear(g_screen);
    g_you_win.Render(g_screen, NULL);
    SDL_RenderPresent(g_screen);
    Mix_PlayChannel(-1, g_wow_sound, 0);
    SDL_Delay(5000);
    close();

    /*if (MessageBox(NULL, L"THẮNG RỒI", L":>", MB_OK | MB_ICONSTOP) == IDOK)
    {
        close();
        SDL_Quit();
    }*/
}

int main(int argc, char* argv[])
{
    Timer fps_timer;

    if (InitData() == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMapData("map/map.dat");
    game_map.LoadTiles(g_screen);

    PlayerObject p_player;
    p_player.LoadImg("img//player-idle-right.png", g_screen);
    p_player.set_clips_frame();

    //Draw the Item Bar
    RectangleFormat retangle_size(0, 0, SCREEN_WIDTH, 40);
    ColorData color_data(66, 188, 245);
    RectangleFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
    ColorData color_data2(0, 0, 0);

    // Item hp of player & gems
    PlayerHP player_hp;
    player_hp.Init(g_screen);

    PlayerGem player_gem;
    player_gem.Init(g_screen);
    player_gem.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

    std::vector<EnemyObject*> enemies_list = MakeEnemyList();

    std::vector<ItemsObject*> items_list = MakeItemsList();

    //Boss Threat
    BossObject bossObject;
    bossObject.LoadImg("img//boss-idle-left.png", g_screen);
    bossObject.set_clips();
    int xPosBoss = 229*TILE_SIZE;
    bossObject.set_xpos(xPosBoss);
    bossObject.set_ypos(5*TILE_SIZE);

    //Explosion
    ExplosionObject exp_of_enemy;
    bool tRet = exp_of_enemy.LoadImg("img//enemy-deadth.png", g_screen);
    if (!tRet) return -1;
    exp_of_enemy.set_clip();

    // chỉ số mạng & cherry
    int player_num_die = 0;
    int num_of_cherrys = 0;
    int boss_num_die = 0;

    //text ttf
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject score_game;
    score_game.SetColor(TextObject::BLACK_TEXT);
    UINT score_val = 0;

    TextObject gem_game;
    gem_game.SetColor(TextObject::WHITE_TEXT);

    TextObject text_menu[NUM_OF_MENU_OPTIONS];
    text_menu[0].SetColor(TextObject::WHITE_TEXT);
    text_menu[1].SetColor(TextObject::WHITE_TEXT);
    bool is_selected[NUM_OF_MENU_OPTIONS] = { 0 };

    bool is_quit = false;
    bool is_quit_menu = false;
    // Menu game
    while (!is_quit_menu)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen); //clear lại màn

        g_start_menu.Render(g_screen, NULL); //fill ảnh vào màn

        text_menu[0].SetText("Play Game");
        text_menu[0].LoadFromRenderText(font_time, g_screen);
        text_menu[0].SetRect(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.3);
        text_menu[0].RenderText(g_screen, text_menu[0].GetRectFrame().x, text_menu[0].GetRectFrame().y);

        text_menu[1].SetText("Exit");
        text_menu[1].LoadFromRenderText(font_time, g_screen);
        text_menu[1].SetRect(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.3 + text_menu[0].GetHeight() + 5);
        text_menu[1].RenderText(g_screen, text_menu[1].GetRectFrame().x, text_menu[1].GetRectFrame().y);

        while (SDL_PollEvent(&g_event) != 0)
        {
            switch (g_event.type)
            {
            case SDL_QUIT:
            {
                is_quit_menu = true;
                is_quit = true;
            }
            break;
            case SDL_MOUSEMOTION:
            {
                int xm = g_event.motion.x;
                int ym = g_event.motion.y;
                for (int i = 0; i < NUM_OF_FRAMES; i++)
                {
                    if (General::CheckFocusWithRect(xm, ym, text_menu[i].GetRectFrame()))
                    {
                        if (is_selected[i] == false)
                        {
                            is_selected[i] = 1;
                            text_menu[i].SetColor(TextObject::RED_TEXT);
                        }
                    }
                    else
                    {
                        if (is_selected[i] == true)
                        {
                            is_selected[i] = 0;
                            text_menu[i].SetColor(TextObject::WHITE_TEXT);
                        }
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                int xm = g_event.button.x;
                int ym = g_event.button.y;

                for (int i = 0; i < NUM_OF_FRAMES; i++)
                {
                    if (General::CheckFocusWithRect(xm, ym, text_menu[i].GetRectFrame()))
                    {
                        if (i == 1) is_quit = true;
                        is_quit_menu = true;
                    }
                }
            }
            break;
            case SDL_KEYDOWN:
                if (g_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_quit_menu = true;
                    is_quit = true;
                }
            break;
            default:
            break;
            }
        }

        SDL_RenderPresent(g_screen);
    }

    //Game loops
    while (!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
                // nhấn thoát thì vòng while sẽ dừng
            }

            p_player.HandleInput(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen); //clear lại màn

        g_background.Render(g_screen, NULL); //fill ảnh vào màn

        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.HandleMove(map_data); // sau hàm này start_x&y sẽ có gtri mới
        if (p_player.get_num_falling())
        {
            player_num_die += p_player.get_num_falling();
            p_player.erase_num_falling();
            player_hp.Decrease(); //giảm mạng
            player_hp.Render(g_screen);
        }

        game_map.SetMap(map_data);
        game_map.DrawMapToScreen(g_screen);

        p_player.Show(g_screen); // show character in front of the tilemaps

        //Show Hotbar
        RectangleForm::RenderRectangle(retangle_size, color_data, g_screen);
        RectangleForm::RenderOutline(outLineSize, color_data2, g_screen);

        //Show HP & GEMS
        player_hp.Show(g_screen);
        player_gem.Show(g_screen);

        //Show game time
        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;
        if (val_time == 0)
        {
            GameOver();
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }

        //Show scores
        std::string str_score_val = std::to_string(score_val);
        std::string strScore("Scores: ");
        strScore += str_score_val;

        score_game.SetText(strScore);
        score_game.LoadFromRenderText(font_time, g_screen);
        score_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

        //Show gems
        int gem_count = p_player.GetGemCount();
        std::string gem_str = std::to_string(gem_count);

        gem_game.SetText(gem_str);
        gem_game.LoadFromRenderText(font_time, g_screen);
        gem_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

        //Show Boss
        if (bossObject.get_whether_appear_or_not())
        {
            bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
            if (abs(bossObject.get_x_pos() - p_player.get_x_pos()) <= DISTANCE_TO_LASER) bossObject.IncreaseTimeToLoadAttack2();
            bossObject.HandleMove(map_data, p_player.get_x_pos(), p_player.get_y_pos(), g_mechanical_sound);
            bossObject.MakeBullet(g_sound_blaster, g_screen, bossObject.GetRectFrame().x - 400, bossObject.GetRectFrame().x + 450,
                                            bossObject.GetRectFrame().y - 200, bossObject.GetRectFrame().y + 250, p_player.get_x_pos());
            bossObject.Show(g_screen, g_break_robot);
        }
        
        // Check collision with items
        for (int i = 0; i < items_list.size(); i++)
        {
            ItemsObject* p_item = items_list.at(i);
            if (p_item != NULL)
            {
                p_item->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_item->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                SDL_Rect rect_item = p_item->GetRectFrame();
                bool bCol = General::CheckCollision(rect_player, rect_item);

                if (bCol)
                {
                    Mix_PlayChannel(-1, g_sound_collect, 0);
                    if (p_item->get_type() == ItemsObject::CHERRY)
                    {
                        num_of_cherrys++;
                        while (num_of_cherrys >= 3 && player_num_die > 0)
                        {
                            num_of_cherrys -= 3;
                            player_num_die--;
                            player_hp.Increase(); //tăng mạng
                            player_hp.Render(g_screen);
                        }
                    }
                    
                    else if (p_item->get_type() == ItemsObject::GEM)
                    {
                        p_player.IncreaseGem();
                    }

                    bool tRet = exp_of_enemy.LoadImg("img//item-feedback.png", g_screen);
                    if (!tRet) return -1;
                    exp_of_enemy.set_clip();

                    int exp_width = exp_of_enemy.get_frame_width();
                    int exp_height = exp_of_enemy.get_frame_height();

                    for (int ex = 0; ex < NUM_ENEMY_EXP; ex++)
                    {
                        int x_pos = p_item->GetRect().x - exp_width * 0.5;
                        int y_pos = p_item->GetRect().y - exp_height * 0.5;

                        exp_of_enemy.set_frame(ex);
                        exp_of_enemy.SetRect(x_pos, y_pos);
                        exp_of_enemy.Show(g_screen);

                        SDL_RenderPresent(g_screen);
                        SDL_Delay(20);
                    }
                    p_item->Free();
                    items_list.erase(items_list.begin() + i);
                }
            }
        }


        // Check collision and handle with enemies
        for (int i = 0; i < enemies_list.size(); i++)
        {
            EnemyObject* p_enemy = enemies_list.at(i);
            if (p_enemy != NULL)
            {
                p_enemy->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_enemy->UpdateImgFromMoveType(g_screen);
                p_enemy->HandleMove(map_data, g_frog_quak);
                p_enemy->HandleBullet(g_screen, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
                p_enemy->Show(g_screen);

                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_enemy->get_bullet_list();
                for (int jj = 0; jj < tBullet_list.size(); jj++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if (pt_bullet != NULL)
                    {
                        bCol1 = General::CheckCollision(pt_bullet->GetRect(), rect_player) && p_player.get_x_pos() > 200;
                        if (bCol1)
                        {
                            p_enemy->RemoveBullet(jj);
                            break;
                        }
                    }
                }
                SDL_Rect rect_threat = p_enemy->GetRectFrame();
                bool bCol2 = General::CheckCollision(rect_player, rect_threat);

                if (bCol1 || bCol2)
                {
                    player_num_die++;
                    if (player_num_die <= 3)
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        player_hp.Decrease(); //giảm mạng
                        player_hp.Render(g_screen);
                        continue;
                    }
                    else
                    {
                        p_enemy->Free();
                        GameOver();
                    }
                }
            }
        }

        // Check collision and handle (bullet of player) with (enemies and boss)
        std::vector<BulletObject*> player_bullet_list = p_player.get_bullet();
        for (int r = 0; r < player_bullet_list.size(); r++)
        {
            BulletObject* p_player_bullet = player_bullet_list.at(r);
            if (p_player_bullet != NULL)
            {
                for (int t = 0; t < enemies_list.size(); t++)
                {
                    EnemyObject* p_enemy = enemies_list.at(t);
                    if (p_enemy != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = p_enemy->GetRect().x;
                        tRect.y = p_enemy->GetRect().y;
                        tRect.w = p_enemy->get_width_frame();
                        tRect.h = p_enemy->get_height_frame();

                        SDL_Rect bRect = p_player_bullet->GetRect();

                        bool bCol = General::CheckCollision(bRect, tRect);

                        if (bCol)
                        {
                            score_val++;

                            bool tRet = exp_of_enemy.LoadImg("img//enemy-deadth.png", g_screen);
                            if (!tRet) return -1;
                            exp_of_enemy.set_clip();
                            int frame_exp_width = exp_of_enemy.get_frame_width();
                            int frame_exp_height = exp_of_enemy.get_frame_height();
                            Mix_PlayChannel(-1, g_sound_pop, 0);

                            for (int ex = 0; ex < NUM_ENEMY_EXP; ex++)
                            {
                                int x_pos = p_player_bullet->GetRect().x - frame_exp_width * 0.5;
                                int y_pos = p_player_bullet->GetRect().y - frame_exp_height * 0.5;

                                exp_of_enemy.set_frame(ex);
                                exp_of_enemy.SetRect(x_pos, y_pos);
                                exp_of_enemy.Show(g_screen);

                                SDL_RenderPresent(g_screen);
                                SDL_Delay(20);
                            }

                            p_player.RemoveBullet(r);
                            p_enemy->Free();
                            enemies_list.erase(enemies_list.begin() + t);
                        }
                    }
                }

                SDL_Rect boRect = bossObject.GetRectFrame();
                SDL_Rect buRect = p_player_bullet->GetRect();

                bool BCol = General::CheckCollision(boRect, buRect);

                if (BCol)
                {
                    boss_num_die++;
                    p_player.RemoveBullet(r);
                    if (boss_num_die <= 3)
                    {
                        bossObject.set_is_hurt(true);
                    }
                    else
                    {
                        bossObject.set_is_dead(true);
                    }
                }
            }
        }

        //Check collision and handle bullets of the boss with player
        SDL_Rect rect_player = p_player.GetRectFrame();
        std::vector<BulletObject*> tBullet_list = bossObject.get_bullet_list();
        for (int b = 0; b < tBullet_list.size(); b++)
        {
            BulletObject* pt_bullet = tBullet_list.at(b);
            if (pt_bullet != NULL)
            {
                bool bCol1 = General::CheckCollision(pt_bullet->GetRect(), rect_player);
                if (bCol1)
                {
                    bossObject.RemoveBullet(b);

                    player_num_die++;
                    if (player_num_die <= 3)
                    {
                        p_player.SetRect(0, 0);
                        p_player.set_comeback_time(60);
                        player_hp.Decrease(); //giảm mạng
                        player_hp.Render(g_screen);
                        continue;
                    }
                    else
                    {
                        GameOver();
                    }

                    break;
                }
            }
        }

        // Check whether be the winner
        if (!bossObject.get_whether_appear_or_not() && p_player.get_x_pos() >= game_map.getMap().max_x_ - 5*TILE_SIZE)
            YouWin();

        SDL_RenderPresent(g_screen);


        // Control the FPS
        int real_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;

        if (real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            SDL_Delay(delay_time);
        }
    }

    for (int i = 0; i < enemies_list.size(); i++)
    {
        EnemyObject* p_threat = enemies_list.at(i);
        if (p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }

    for (int i = 0; i < items_list.size(); i++)
    {
        ItemsObject* p_item = items_list.at(i);
        if (p_item)
        {
            p_item->Free();
            p_item = NULL;
        }
    }

    enemies_list.clear();
    items_list.clear();

    close();
    return 0;
}
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int player_1_score = 0, player_2_score = 0;
float dt_menu = 0;
float goal_timer = 0;

enum PlayingMode {
	FIRST_TO_5,
	FIRST_TO_10, 
	UNLIMITED
};
u32 color1 = 0xff0000, color2 = 0x0031ff;
PlayingMode pmode;

PlayingMode int_to_pmode(int n) {
	if (n == 0) return FIRST_TO_5;
	if (n == 1) return FIRST_TO_10;
	if (n == 2) return UNLIMITED;
}
int pmode_to_int(PlayingMode pm) {
	if (pm == FIRST_TO_5) return 0;
	if (pm == FIRST_TO_10) return 1;
	if (pm == UNLIMITED) return 2;
}
u32 int_to_color(int n) {
	if (n == 0) return 0xff0000;
	if (n == 1) return 0x0031ff;
	if (n == 2) return 0x2cff00;
	if (n == 3) return 0xfffe00;
	if (n == 4) return 0x00cccc;
	if (n == 5) return 0xffffff;
}
int color_to_int(u32 c) {
	if (c == 0xff0000) return 0;
	if (c == 0x0031ff) return 1;
	if (c == 0x2cff00) return 2;
	if (c == 0xfffe00) return 3;
	if (c == 0x00cccc) return 4;
	if (c == 0xffffff) return 5;
}

internal void
simulate_player(float *p, float *dp, float ddp, float dt) {
	*p = *p + *dp * dt + ddp * dt * dt * 0.5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp *= 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp *= 0; 
	}
}

internal bool
aabb_vs_aabb (float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}


enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_PAUSE,
	GM_TYPING,
	GM_LEADERBOARD,
	GM_GOAL,
	GM_SETTINGS, 
	GM_GAMEOVER
};

Gamemode current_gamemode;
int hot_button = 0, hotbutton1 = 0, hotbutton2 = 1, hotbutton3 = 1;
bool enemy_is_ai;

bool typing = true;

FILE* myfile;
int gs, ga;
string name, name2;

string written_text = "";
internal void
write_text(Input* input, float x, float y, float size, u32 color) {
	if (pressed(BUTTON_A)) written_text += "A";
	else if (pressed(BUTTON_B)) written_text += "B";
	else if (pressed(BUTTON_C)) written_text += "C";
	else if (pressed(BUTTON_D)) written_text += "D";
	else if (pressed(BUTTON_E)) written_text += "E";
	else if (pressed(BUTTON_F)) written_text += "F";
	else if (pressed(BUTTON_G)) written_text += "G";
	else if (pressed(BUTTON_H)) written_text += "H";
	else if (pressed(BUTTON_I)) written_text += "I";
	else if (pressed(BUTTON_J)) written_text += "J";
	else if (pressed(BUTTON_K)) written_text += "K";
	else if (pressed(BUTTON_L)) written_text += "L";
	else if (pressed(BUTTON_M)) written_text += "M";
	else if (pressed(BUTTON_N)) written_text += "N";
	else if (pressed(BUTTON_O)) written_text += "O";
	else if (pressed(BUTTON_P)) written_text += "P";
	else if (pressed(BUTTON_Q)) written_text += "Q";
	else if (pressed(BUTTON_R)) written_text += "R";
	else if (pressed(BUTTON_S)) written_text += "S";
	else if (pressed(BUTTON_T)) written_text += "T";
	else if (pressed(BUTTON_U)) written_text += "U";
	else if (pressed(BUTTON_V)) written_text += "V";
	else if (pressed(BUTTON_W)) written_text += "W";
	else if (pressed(BUTTON_X)) written_text += "X";
	else if (pressed(BUTTON_Y)) written_text += "Y";
	else if (pressed(BUTTON_Z)) written_text += "Z";
	else if (pressed(BUTTON_SPACE)) written_text += " ";
	else if (pressed(BUTTON_BACKSPACE) && written_text.length() > 0) written_text.pop_back();
	draw_text2(written_text, 0, 0, 1, 0x000000);
	draw_text2(written_text, x, y, size, color);
}


internal void
simulate_game(Input* input, float dt) {
	clear_screen(0x000000);

	if (current_gamemode == GM_GAMEPLAY) {
		draw_text("MADE BY CAPTAIN KOUK", -30, -40, 0.5, 0x00cccc);
		float player_1_ddp = 0.f;
		draw_rect(-45.5, 48, 45.5, 3, 0xfffe00);
		draw_rect(-45.5, -48, 45.5, 3, 0xff0000);
		draw_rect(45.5, 48, 45.5, 3, 0x2cff00);
		draw_rect(45.5, -48, 45.5, 3, 0x0031ff);
		draw_rect(-88, 22.5, 3, 22.5, 0xfffe00);
		draw_rect(-88, -22.5, 3, 22.5, 0xff0000);
		draw_rect(88, 22.5, 3, 22.5, 0x2cff00);
		draw_rect(88, -22.5, 3, 22.5, 0x0031ff);
		if (is_down(BUTTON_P)) {
			current_gamemode = GM_PAUSE;
		}
		if (!enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_1_ddp += 2000;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
		}
		else {
			player_1_ddp = (ball_p_y - player_1_p) * 100;
			if (player_1_ddp > 1300) player_1_ddp = 1300;
			if (player_1_ddp < -1300) player_1_ddp = -1300;
		}
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += 2000;
		if (is_down(BUTTON_S)) player_2_ddp -= 2000;

		player_1_ddp -= player_1_dp * 10.f;
		player_2_ddp -= player_2_dp * 10.f;

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		//Simulate Ball
		{
			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y += (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f;
			}
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y += (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -.8;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -.8;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_p_x = 0;
				ball_p_y = 0;
				ball_dp_x *= -1;
				ball_dp_y = 0;
				player_2_score++;
				
				if (pmode == FIRST_TO_5 && player_2_score == 5) 
					current_gamemode = GM_GAMEOVER;
				else if (pmode == FIRST_TO_10 && player_2_score == 10) 
					current_gamemode = GM_GAMEOVER;
				else 
					current_gamemode = GM_GOAL;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_p_x = 0;
				ball_p_y = 0;
				ball_dp_x *= -1;
				ball_dp_y = 0;
				player_1_score++;
				
				if (pmode == FIRST_TO_5 && player_1_score == 5) 
					current_gamemode = GM_GAMEOVER;
				else if (pmode == FIRST_TO_10 && player_1_score == 10) 
					current_gamemode = GM_GAMEOVER;
				else 
					current_gamemode = GM_GOAL;
			}
		}
		
		//Render
		draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff);

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, color1);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, color2);

		draw_number(player_1_score, 13, 40, 0x00cccc);
		draw_number(player_2_score, -13, 40, 0x00cccc);
		
	}

	else if (current_gamemode == GM_GOAL) {
		draw_text("MADE BY CAPTAIN KOUK", -30, -40, 0.5, 0x00cccc);
		if (is_down(BUTTON_P)) {
			current_gamemode = GM_PAUSE;
		}
		float player_1_ddp = 0.f;
		if (!enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_1_ddp += 2000;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
		}
		else {
			player_1_ddp = (ball_p_y - player_1_p) * 100;
			if (player_1_ddp > 1300) player_1_ddp = 1300;
			if (player_1_ddp < -1300) player_1_ddp = -1300;
		}
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += 2000;
		if (is_down(BUTTON_S)) player_2_ddp -= 2000;

		player_1_ddp -= player_1_dp * 10.f;
		player_2_ddp -= player_2_dp * 10.f;

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

		//Render
		draw_rect(-45.5, 48, 45.5, 3, 0xfffe00);
		draw_rect(-45.5, -48, 45.5, 3, 0xff0000);
		draw_rect(45.5, 48, 45.5, 3, 0x2cff00);
		draw_rect(45.5, -48, 45.5, 3, 0x0031ff);
		draw_rect(-88, 22.5, 3, 22.5, 0xfffe00);
		draw_rect(-88, -22.5, 3, 22.5, 0xff0000);
		draw_rect(88, 22.5, 3, 22.5, 0x2cff00);
		draw_rect(88, -22.5, 3, 22.5, 0x0031ff);

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, color1);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, color2);

		draw_number(player_1_score, 13, 40, 0x00cccc);
		draw_number(player_2_score, -13, 40, 0x00cccc);

		if (goal_timer > 2) {
			current_gamemode = GM_GAMEPLAY;
			goal_timer = 0;
		}
		else {
			draw_text("GOAL", -11, 0, 1.1, 0x00cccc);
			goal_timer += dt;
		}
	}
	
	else if (current_gamemode == GM_MENU) {
		draw_text("MADE BY CAPTAIN KOUK", -30, -43, 0.5, 0x00cccc);
		draw_text("PONG", -39, 35, 3.7, 0x005fff);

		if (pressed(BUTTON_DOWN)) {
			hot_button++;
			if (hot_button > 3) hot_button = 0;
		}
		if (pressed(BUTTON_UP)) {
			hot_button--;
			if (hot_button < 0) hot_button = 3;
		}

		if (pressed(BUTTON_ENTER)) {
			dt_menu += dt;
			enemy_is_ai = hot_button ? 0 : 1;
		}

		if (hot_button == 0) {
			draw_text("SINGLE PLAYER", -30.5, 1, 0.8, 0xff0000);
			draw_text("MULTIPLAYER", -25.5, -10, 0.8, 0xFF4848);
			draw_text("SETTINGS", -18, -20.5, 0.8, 0xFF4848);
			draw_text("LEADERBOARD", -25.5, -32.5, 0.8, 0xFF4848);
		}
		else if (hot_button == 1){
			draw_text("SINGLE PLAYER", -30.5, 1, 0.8, 0xFF4848);
			draw_text("MULTIPLAYER", -25.5, -10, 0.8, 0xff0000);
			draw_text("SETTINGS", -18, -20.5, 0.8, 0xFF4848);
			draw_text("LEADERBOARD", -25.5, -32.5, 0.8, 0xFF4848);
		}
		else if (hot_button == 2) {
			draw_text("SINGLE PLAYER", -30.5, 1, 0.8, 0xFF4848);
			draw_text("MULTIPLAYER", -25.5, -10, 0.8, 0xFF4848);
			draw_text("SETTINGS", -18, -20.5, 0.8, 0xff0000);
			draw_text("LEADERBOARD", -25.5, -32.5, 0.8, 0xFF4848);
		}
		else {
			draw_text("SINGLE PLAYER", -30.5, 1, 0.8, 0xFF4848);
			draw_text("MULTIPLAYER", -25.5, -10, 0.8, 0xFF4848);
			draw_text("SETTINGS", -18, -20.5, 0.8, 0xFF4848);
			draw_text("LEADERBOARD", -25.5, -32.5, 0.8, 0xff0000);
		}

		if (dt_menu > 0) {
			if (dt_menu < .2) {
				if (hot_button == 0) {
					draw_rect(0, -1.25, 39, 5, 0xff0000);
					draw_text("SINGLE PLAYER", -30.5, 1, 0.8, 0xFF4848);
				}
				else if (hot_button == 1){
					draw_rect(0, -12.25, 39, 4.5, 0xff0000);
					draw_text("MULTIPLAYER", -25.5, -10, 0.8, 0xFF4848);
				}
				else if (hot_button == 2) {
					draw_rect(0, -22.75, 39, 4.5, 0xff0000);
					draw_text("SETTINGS", -18, -20.5, 0.8, 0xFF4848);
				}
				else {
					draw_rect(0, -34.5, 39, 4.5, 0xff0000);
					draw_text("LEADERBOARD", -25.5, -32.5, 0.8, 0xFF4848);
					draw_text("COMING", 45, -29, 0.7, 0x005fff);
					draw_text("SOON", 48, -36, 0.7, 0x005fff);
				}
				dt_menu += dt;
			}
			else {
				dt_menu = 0;
				if (hot_button == 0 || hot_button == 1) {
					current_gamemode = GM_GAMEPLAY;
					player_1_score = player_2_score = 0;
				}
				else if (hot_button == 2) {
					current_gamemode = GM_SETTINGS;
					hot_button = 0;
				}

				else {
					current_gamemode = GM_LEADERBOARD;

					// reading file 
//					myfile = fopen("leaderboard.txt", "r");
//					fscanf(myfile, "%d %d", &gs, &ga);
//					fscanf(myfile, "%s", &name);
					
//					fclose(myfile);
				}
			}
		}

// single player
		draw_rect(0, 3, 39, .2, 0xff0000);
		draw_rect(0, 3.4, 39.5, .2, 0xffffff);
		draw_rect(0, 3.8, 40, .2, 0xff0000);	
		draw_rect(0, -5.5, 39, .2, 0xff0000);
		draw_rect(0, -5.9, 39.5, .2, 0xffffff);
		draw_rect(0, -6.3, 40, .2, 0xff0000);
		
// multiplayer
		draw_rect(0, -7.7, 40, .2, 0xff0000);
		draw_rect(0, -8.1, 39.5, .2, 0xffffff);
		draw_rect(0, -8.5, 39, .2, 0xff0000);
		draw_rect(0, -16, 39, .2, 0xff0000);
		draw_rect(0, -16.4, 39.5, .2, 0xffffff);
		draw_rect(0, -16.8, 40, .2, 0xff0000);

		draw_rect(-39, -1.25, .2, 4.5, 0xff0000);
		draw_rect(39, -1.25, .2, 4.5, 0xff0000);
		draw_rect(-39, -12.25, .2, 3.85, 0xff0000);
		draw_rect(39, -12.25, .2, 3.85, 0xff0000);
		draw_rect(-39.4, -1.25, .2, 5, 0xffffff);
		draw_rect(39.4, -1.25, .2, 5, 0xffffff);
		draw_rect(-39.4, -12.25, .2, 4.25, 0xffffff);
		draw_rect(39.4, -12.25, .2, 4.25, 0xffffff);
		draw_rect(-39.8, -1.25, .2, 5, 0xff0000);
		draw_rect(39.8, -1.25, .2, 5, 0xff0000);
		draw_rect(-39.8, -12.25, .2, 4.5, 0xff0000);
		draw_rect(39.8, -12.25, .2, 4.5, 0xff0000);

//settings
		draw_rect(0, -18.2, 40, .2, 0xff0000);
		draw_rect(0, -18.6, 39.5, .2, 0xffffff);
		draw_rect(0, -19, 39, .2, 0xff0000);
		draw_rect(0, -26.5, 39, .2, 0xff0000);
		draw_rect(0, -26.9, 39.5, .2, 0xffffff);
		draw_rect(0, -27.3, 40, .2, 0xff0000);

		draw_rect(-39, -22.75, .2, 3.85, 0xff0000);
		draw_rect(39, -22.75, .2, 3.85, 0xff0000);
		draw_rect(-39.4, -22.75, .2, 4.25, 0xffffff);
		draw_rect(39.4, -22.75, .2, 4.25, 0xffffff);
		draw_rect(-39.8, -22.75, .2, 4.5, 0xff0000);
		draw_rect(39.8, -22.75, .2, 4.5, 0xff0000);

//leaderboard
		draw_rect(-39, -34.5, .2, 4.5, 0xff0000);
		draw_rect(39, -34.5, .2, 4.5, 0xff0000);
		draw_rect(-39.4, -34.5, .2, 5, 0xffffff);
		draw_rect(39.4, -34.5, .2, 5, 0xffffff);
		draw_rect(-39.8, -34.5, .2, 5.5, 0xff0000);
		draw_rect(39.8, -34.5, .2, 5.5, 0xff0000);

		draw_rect(0, -30, 39, .2, 0xff0000);
		draw_rect(0, -39, 39, .2, 0xff0000);
		draw_rect(0, -29.6, 39.5, .2, 0xffffff);
		draw_rect(0, -39.5, 39.5, .2, 0xffffff);
		draw_rect(0, -29.2, 40, .2, 0xff0000);
		draw_rect(0, -39.8, 40, .2, 0xff0000);


	}

	else if (current_gamemode == GM_SETTINGS) {
		draw_text("SETTINGS", -23, 37, 1, 0x00cccc);
		draw_text("PLAYER  COLOR", -60, 20, 0.7, 0xffffff);
		draw_number(1, -32, 18, 0xffffff);
		draw_text("PLAYER  COLOR", -60, 10, 0.7, 0xffffff);
		draw_number(2, -32, 8, 0xffffff);
		draw_text("PLAYING MODE", -60, 0, 0.7, 0xffffff);
		if (pressed(BUTTON_DOWN)) {
			hot_button++;
			if (hot_button > 4) hot_button = 4;
		}
		if (pressed(BUTTON_UP)) {
			hot_button--;
			if (hot_button < 0) hot_button = 0;
		}
		switch (hot_button) {
		case 0: {
			draw_text("PLAYER  COLOR", -60, 20, 0.7, 0x00cccc);
			draw_number(1, -32, 18, 0x00cccc);
			draw_text("PLAYER  COLOR", -60, 10, 0.7, 0xffffff);
			draw_number(2, -32, 8, 0xffffff);
			draw_text("PLAYING MODE", -60, 0, 0.7, 0xffffff);

			draw_text("CANCEL", -25, -20, 0.8, 0xffffff);
			draw_text("OK", 15, -20, 0.8, 0xffffff);

			if (pressed(BUTTON_RIGHT)) {
				hotbutton1++;
				if (hotbutton1 > 5) hotbutton1 = 0;
			}
			if (pressed(BUTTON_LEFT)) {
				hotbutton1--;
				if (hotbutton1 < 0) hotbutton1 = 5;
			}

		} break;
		case 1: {
			draw_text("PLAYER  COLOR", -60, 20, 0.7, 0xffffff);
			draw_number(1, -32, 18, 0xffffff);
			draw_text("PLAYER  COLOR", -60, 10, 0.7, 0x00cccc);
			draw_number(2, -32, 8, 0x00cccc);
			draw_text("PLAYING MODE", -60, 0, 0.7, 0xffffff);

			draw_text("CANCEL", -25, -20, 0.8, 0xffffff);
			draw_text("OK", 15, -20, 0.8, 0xffffff);

			if (pressed(BUTTON_RIGHT)) {
				hotbutton2++;
				if (hotbutton2 > 5) hotbutton2 = 0;
			}
			if (pressed(BUTTON_LEFT)) {
				hotbutton2--;
				if (hotbutton2 < 0) hotbutton2 = 5;
			}

		} break;
		case 2: {
			draw_text("PLAYER  COLOR", -60, 20, 0.7, 0xffffff);
			draw_number(1, -32, 18, 0xffffff);
			draw_text("PLAYER  COLOR", -60, 10, 0.7, 0xffffff);
			draw_number(2, -32, 8, 0xffffff);
			draw_text("PLAYING MODE", -60, 0, 0.7, 0x00cccc);

			draw_text("CANCEL", -25, -20, 0.8, 0xffffff);
			draw_text("OK", 15, -20, 0.8, 0xffffff);

			if (pressed(BUTTON_RIGHT)) {
				hotbutton3++;
				if (hotbutton3 > 2) hotbutton3 = 0;
			}
			if (pressed(BUTTON_LEFT)) {
				hotbutton3--;
				if (hotbutton3 < 0) hotbutton3 = 2;
			}

		} break;
		case 3: {
			draw_text("PLAYER  COLOR", -60, 20, 0.7, 0xffffff);
			draw_number(1, -32, 18, 0xffffff);
			draw_text("PLAYER  COLOR", -60, 10, 0.7, 0xffffff);
			draw_number(2, -32, 8, 0xffffff);
			draw_text("PLAYING MODE", -60, 0, 0.7, 0xffffff);

			draw_text("CANCEL", -25, -20, 0.8, 0x00cccc);
			draw_text("OK", 15, -20, 0.8, 0xffffff);

			if (pressed(BUTTON_RIGHT)) {
				hot_button = 4;
			}
			if (pressed(BUTTON_ENTER)) {
				current_gamemode = GM_MENU;
				hotbutton1 = color_to_int(color1);
				hotbutton2 = color_to_int(color2);
				hotbutton3 = pmode_to_int(pmode);
			}

		} break;
		case 4: {
			draw_text("PLAYER  COLOR", -60, 20, 0.7, 0xffffff);
			draw_number(1, -32, 18, 0xffffff);
			draw_text("PLAYER  COLOR", -60, 10, 0.7, 0xffffff);
			draw_number(2, -32, 8, 0xffffff);
			draw_text("PLAYING MODE", -60, 0, 0.7, 0xffffff);

			draw_text("CANCEL", -25, -20, 0.8, 0xffffff);
			draw_text("OK", 15, -20, 0.8, 0x00cccc);

			if (pressed(BUTTON_LEFT)) {
				hot_button = 3;
			}
			if (pressed(BUTTON_ENTER)) {
				current_gamemode = GM_MENU;
				color1 = int_to_color(hotbutton1);
				color2 = int_to_color(hotbutton2);
				pmode = int_to_pmode(hotbutton3);
			}

		} break;
		}

		switch (hotbutton1) {
		case 0: {
			draw_rect(50, 18.5, 8, 3, 0xff0000);
		} break;
		case 1: {
			draw_rect(50, 18.5, 8, 3, 0x0031ff);
		} break;
		case 2: {
			draw_rect(50, 18.5, 8, 3, 0x2cff00);
		} break;
		case 3: {
			draw_rect(50, 18.5, 8, 3, 0xfffe00);
		} break;
		case 4: {
			draw_rect(50, 18.5, 8, 3, 0x00cccc);
		} break;
		case 5: {
			draw_rect(50, 18.5, 8, 3, 0xffffff);
		} break;
		}

		switch (hotbutton2) {
		case 0: {
			draw_rect(50, 8.5, 8, 3, 0xff0000);
		} break;
		case 1: {
			draw_rect(50, 8.5, 8, 3, 0x0031ff);
		} break;
		case 2: {
			draw_rect(50, 8.5, 8, 3, 0x2cff00);
		} break;
		case 3: {
			draw_rect(50, 8.5, 8, 3, 0xfffe00);
		} break;
		case 4: {
			draw_rect(50, 8.5, 8, 3, 0x00cccc);
		} break;
		case 5: {
			draw_rect(50, 8.5, 8, 3, 0xffffff);
		} break;
		}

		u32 c;
		if (hot_button == 2) c = 0x00cccc;
		else c = 0xffffff;
		switch (hotbutton3) {
		case 0: {
			draw_text("FIRST TO", 18, 0, 0.7, c);
			draw_number(5, 56, -2, c);
		} break;
		case 1: {
			draw_text("FIRST TO", 18, 0, 0.7, c);
			draw_number(10, 57, -2, c);
		} break;
		case 2: {
			draw_text("UNLIMITED", 20.5, 0, 0.7, c);
		} break;
		}

	}

	else if (current_gamemode == GM_PAUSE){
		draw_rect(-45.5, 48, 45.5, 3, 0xfffe00);
		draw_rect(-45.5, -48, 45.5, 3, 0xff0000);
		draw_rect(45.5, 48, 45.5, 3, 0x2cff00);
		draw_rect(45.5, -48, 45.5, 3, 0x0031ff);
		draw_rect(-88, 22.5, 3, 22.5, 0xfffe00);
		draw_rect(-88, -22.5, 3, 22.5, 0xff0000);
		draw_rect(88, 22.5, 3, 22.5, 0x2cff00);
		draw_rect(88, -22.5, 3, 22.5, 0x0031ff); // arena

		draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff); // ball
		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, color1);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, color2); // players
		draw_number(player_1_score, 13, 40, 0x00cccc);
		draw_number(player_2_score, -13, 40, 0x00cccc); // scores

		draw_rect(0, -3, 70, 36, 0x060914);
		draw_text("PAUSE", -15, 15, 1, 0x00cccc);

		if (pressed(BUTTON_DOWN) || pressed(BUTTON_UP)) {
			hot_button = !hot_button;
		}
		if (pressed(BUTTON_ENTER)) {
			dt_menu += dt;
		}

		if (hot_button == 0) {
			draw_text("RESUME", -18, -10, 1, 0xff0000);
			draw_text("MAIN MENU", -27, -23, 1, 0xFF4848);
		}
		else {
			draw_text("RESUME", -18, -10, 1, 0xFF4848);
			draw_text("MAIN MENU", -27, -23, 1, 0xff0000);
		}

		if (dt_menu > 0) {
			if (dt_menu < .2) {
				if (!hot_button) {
					draw_rect(0, -13, 39, 5, 0xff0000);
					draw_text("RESUME", -18, -10, 1, 0xFF4848);
				}
				else {
					draw_rect(0, -26, 39, 4.5, 0xff0000);
					draw_text("MAIN MENU", -27, -23, 1, 0xFF4848);
				}
				dt_menu += dt;
			}
			else {
				dt_menu = 0;
				if (!hot_button) current_gamemode = GM_GAMEPLAY;
				/*				else if (enemy_is_ai) {
									typing = true;
									written_text = "";
									current_gamemode = GM_TYPING;
								}*/
				else {
					current_gamemode = GM_MENU;
					ball_dp_y = 0;
					ball_p_x = 0;
					ball_p_y = 0;
				}
			}
		}

		draw_rect(0, -8, 39, .2, 0xff0000);
		draw_rect(0, -17.5, 39, .2, 0xff0000);
		draw_rect(0, -21.5, 39, .2, 0xff0000);
		draw_rect(0, -30.5, 39, .2, 0xff0000);
		draw_rect(0, -7.6, 39.5, .2, 0xffffff);
		draw_rect(0, -17.9, 39.5, .2, 0xffffff);
		draw_rect(0, -21.1, 39.5, .2, 0xffffff);
		draw_rect(0, -30.9, 39.5, .2, 0xffffff);
		draw_rect(0, -7.2, 40, .2, 0xff0000);
		draw_rect(0, -18.3, 40, .2, 0xff0000);
		draw_rect(0, -20.7, 40, .2, 0xff0000);
		draw_rect(0, -31.3, 40, .2, 0xff0000);

		draw_rect(-39, -12.75, .2, 4.5, 0xff0000);
		draw_rect(39, -12.75, .2, 4.5, 0xff0000);
		draw_rect(-39, -26, .2, 4.5, 0xff0000);
		draw_rect(39, -26, .2, 4.5, 0xff0000);
		draw_rect(-39.4, -12.75, .2, 5, 0xffffff);
		draw_rect(39.4, -12.75, .2, 5, 0xffffff);
		draw_rect(-39.4, -26, .2, 5, 0xffffff);
		draw_rect(39.4, -26, .2, 5, 0xffffff);
		draw_rect(-39.8, -12.75, .2, 5.5, 0xff0000);
		draw_rect(39.8, -12.75, .2, 5.5, 0xff0000);
		draw_rect(-39.8, -26, .2, 5.5, 0xff0000);
		draw_rect(39.8, -26, .2, 5.5, 0xff0000);

	}

	else if (current_gamemode == GM_GAMEOVER) {
		draw_rect(-45.5, 48, 45.5, 3, 0xfffe00);
		draw_rect(-45.5, -48, 45.5, 3, 0xff0000);
		draw_rect(45.5, 48, 45.5, 3, 0x2cff00);
		draw_rect(45.5, -48, 45.5, 3, 0x0031ff);
		draw_rect(-88, 22.5, 3, 22.5, 0xfffe00);
		draw_rect(-88, -22.5, 3, 22.5, 0xff0000);
		draw_rect(88, 22.5, 3, 22.5, 0x2cff00);
		draw_rect(88, -22.5, 3, 22.5, 0x0031ff); // arena

		draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff); // ball
		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, color1);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, color2); // players
		draw_number(player_1_score, 13, 40, 0x00cccc);
		draw_number(player_2_score, -13, 40, 0x00cccc); // scores

		draw_rect(0, -3, 70, 36, 0x060914);
		draw_text("GAME OVER", -25, 15, 1, 0x00cccc);

		if (pressed(BUTTON_ENTER)) {
			dt_menu += dt;
		}

		draw_text("MAIN MENU", -27, -23, 1, 0xff0000);

		if (dt_menu > 0) {
			if (dt_menu < .2) {
				draw_rect(0, -26, 39, 4.5, 0xff0000);
				draw_text("MAIN MENU", -27, -23, 1, 0xFF4848);

				dt_menu += dt;
			}
			else {
				dt_menu = 0;
				/*				else if (enemy_is_ai) {
									typing = true;
									written_text = "";
									current_gamemode = GM_TYPING;
								}*/
				current_gamemode = GM_MENU;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;

			}
		}

		draw_rect(0, -21.5, 39, .2, 0xff0000);
		draw_rect(0, -30.5, 39, .2, 0xff0000);
		draw_rect(0, -21.1, 39.5, .2, 0xffffff);
		draw_rect(0, -30.9, 39.5, .2, 0xffffff);
		draw_rect(0, -20.7, 40, .2, 0xff0000);
		draw_rect(0, -31.3, 40, .2, 0xff0000);

		draw_rect(-39, -26, .2, 4.5, 0xff0000);
		draw_rect(39, -26, .2, 4.5, 0xff0000);
		draw_rect(-39.4, -26, .2, 5, 0xffffff);
		draw_rect(39.4, -26, .2, 5, 0xffffff);
		draw_rect(-39.8, -26, .2, 5.5, 0xff0000);
		draw_rect(39.8, -26, .2, 5.5, 0xff0000);

	}

	else if (current_gamemode == GM_TYPING) {
		if (typing) {
			draw_rect(-45.5, 48, 45.5, 3, 0xfffe00);
			draw_rect(-45.5, -48, 45.5, 3, 0xff0000);
			draw_rect(45.5, 48, 45.5, 3, 0x2cff00);
			draw_rect(45.5, -48, 45.5, 3, 0x0031ff);
			draw_rect(-88, 22.5, 3, 22.5, 0xfffe00);
			draw_rect(-88, -22.5, 3, 22.5, 0xff0000);
			draw_rect(88, 22.5, 3, 22.5, 0x2cff00);
			draw_rect(88, -22.5, 3, 22.5, 0x0031ff); // arena
			
			draw_text("YOU HAVE ACHIEVED A HIGHSCORE", -60, 30, .7, 0x00cccc);
			draw_text("TYPE YOUR NAME", -60, 15, .8, 0x00cccc);
			write_text(input, -60, -10, .85, 0x00cccc);
			if (pressed(BUTTON_ENTER)) {
				typing = false;
				current_gamemode = GM_MENU;
			}
		}
		

	}

	else if (current_gamemode == GM_LEADERBOARD) {
		draw_text("LEADERBOARD", -45, 40, 1.5, 0x00cccc);

		//string word = "KATI";
		//word += "A";
		//draw_text2(word, -30, 10, 1, 0xffffff);

//		string name = "NICOLAS";
//		draw_text2(name, -30, -10, 1, 0xffffff);
//		draw_text2(name2, -30, -20, 1, 0xffffff);
//		draw_number(gs, 0, 0, 0xffffff);
//		draw_number(ga, 10, 0, 0xffffff);
	}

}

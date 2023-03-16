internal void 
clear_screen(u32 color) {
	unsigned int* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void 
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width	;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

internal void 
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {

	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;
	
	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void
draw_number(int num, float x, float y, u32 color) {
	int digit = 1;
	bool drew_number = false;
	while (num != 0 || !drew_number) {
		digit = num % 10;
		num = num / 10;
		switch (digit) {
		case 0: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y + 1.25, .5, 1.25, color);
			draw_rect(x + .75, y - 1.25, .5, 1.25, color);
			draw_rect(x + .75, y + 1.25, .5, 1.25, color);
			draw_rect(x - .75, y - 1.25, .5, 1.25, color);
			x -= 3;
		} break;

		case 1: {
			draw_rect(x+.5, y, .5, 3, color);
			x -= 1.75;
		} break;

		case 2: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y - 1.25, .5, .75, color);
			draw_rect(x + .75, y + 1.25, .5, .75, color);
			x -= 3;
		} break;
		case 3: {
			draw_rect(x + 1, y, .5, 3, color);
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x + .1, y, 1, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			x -= 3;
		} break;

		case 4: {
			draw_rect(x + 1, y, .5, 3, color);
			draw_rect(x, y, 1.5, .5, color);
			draw_rect(x - 1, y + 1.5, .5, 1.5, color);
			x -= 3.3;
		} break;

		case 5: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y + 1.25, .5, .75, color);
			draw_rect(x + .75, y - 1.25, .5, .75, color);
			x -= 3;
		} break;

		case 6: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y + 1.25, .5, .75, color);
			draw_rect(x + .75, y - 1.25, .5, .75, color);
			draw_rect(x - .75, y - 1.25, .5, .75, color);
			x -= 3;
		} break;

		case 7: {
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x + 1, y, .5, 3, color);
			x -= 3;
		} break;

		case 8: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y + 1.25, .5, .75, color);
			draw_rect(x + .75, y - 1.25, .5, .75, color);
			draw_rect(x + .75, y + 1.25, .5, .75, color);
			draw_rect(x - .75, y - 1.25, .5, .75, color);
			x -= 3;
		} break;

		case 9: {
			draw_rect(x, y - 2.5, 1.25, .5, color);
			draw_rect(x, y, 1.25, .5, color);
			draw_rect(x, y + 2.5, 1.25, .5, color);
			draw_rect(x - .75, y + 1.25, .5, .75, color);
			draw_rect(x + .75, y - 1.25, .5, .75, color);
			draw_rect(x + .75, y + 1.25, .5, .75, color);
			x -= 3;
		} break;
		}
		drew_number = true;
	}
}

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

string letters2[28][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
draw_text2(string text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;
	int k = 0; //the letters being typed

	while (k <= text.length()) {
		if (int(text[k]) != 32) {
			int letter_index;  // the index of the letters2 matrix which indicate which letter is to be used
			if (int(text[k]) == 47) letter_index = 27;
			else if (int(text[k]) == 46) letter_index = 26;
			else letter_index = int(text[k]) - 'A';
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				string row = letters2[letter_index][i];
				int j = 0;
				while (row[j]) {
					if (row[j] == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					j++;
				}
				y -= size;
				x = original_x;
			}
		}
		k++;
		x += size * 6.f;
		y = original_y;
	}
}


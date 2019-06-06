#include "raylib.h"
#include "raymath.h"
#include <string.h>
#include "starfield.h"
#include <time.h>
#include <emscripten/emscripten.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

Vector2 Screen = (Vector2) { 640, 480 };
Vector2 VirtualScreen = (Vector2) { 640, 480 };

Texture2D font_data;
Texture2D select_bar;
Texture2D bar_data;
Texture2D grey_bar;

Texture2D balle1;
Texture2D balle2;
Texture2D balle3;

//--------------------------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------------------------
float sinx = 0;
float siny = 0;
float oldsinx = 0;
float oldsiny = 0;

char * text1 =  "    PARADOX PROUDLY PRESENTS    \n" \
					"        COUNT DUCKULA +         \n" \
					" TRAINED BY SHAPIRO OF PARADOX  \n" \
					"                                \n" \
					"       GREETS TO THE BEST       \n" \
					"      SKID ROW - FAIRLIGHT      \n" \
					"                                \n" \
					"                                \n" \
					"     UNLIMITED TIME      NO     \n" \
					"          START DUCKULA         \n" \
					"                                \n" \
					"   MADE BY ANATA WITH RAYLIB    \n";

char *scrollText = "PARADOX PROUDLY PRESENTS -- COUNT DUCKULA + --      TRAINER,INTRO AND MUSIC RIP BY SHAPIRO OF PARADOX              THIS GAME WAS CRACKED BY N.O.M.A.D OF GENESIS                                      TO CONTACT PARADOX CALL THEESE COOL BOARDS          SLEEPY HOLLOW - 703 276 0724        INVOLUNTARY DEATH - 708 599 1537       HALLOWED GROUND - (45) 4343 9398       MIDDLE EARTH - 801 822 4215         PARADOX CANADA - 418 843 5174         THE JUNGLE - 708 983 5764                        OR WRITE TO        BP 110 - 7700 MOUSCRON 1 - BELGIUM             OR         T.N.B. - POST BOX 560 - 2620 ALBERTSLUND - DENMARK             OR          P.O.BOX 2221 - BRIDGEVIEW, IL - 60455 USA              CUL8R                        ";

int ySin[1024];
float sinparam = 0;
float textX = 0;
int textLen = 0;

Starfield2D starfield1;
Starfield2D starfield2;
Starfield2D starfield3;

void UpdateDrawFrame(void);

// -------------------------------------------------------------------------------------------------------------
// Main Game
// -------------------------------------------------------------------------------------------------------------

int main() {
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow( Screen.x, Screen.y,"Demo Paradox HTML");
	InitAudioDevice();

	font_data = LoadTexture("resources/font.png");
	bar_data = LoadTexture("resources/bars.png");
	select_bar = LoadTexture("resources/select.png");
	grey_bar = LoadTexture("resources/grey.png");
	balle1 = LoadTexture("resources/ball1.png");
	balle2 = LoadTexture("resources/ball2.png");
	balle3 = LoadTexture("resources/ball3.png");

	starfield1 = Init_Starfield2D(balle1, (Vector2) {-64,-64}, (Vector2) {768,608});
	starfield2 = Init_Starfield2D(balle2, (Vector2) {-64,-64}, (Vector2) {768,608});
	starfield3 = Init_Starfield2D(balle3, (Vector2) {-64,-64}, (Vector2) {768,608});

	SetSpeed_Starfield2D(&starfield1, (Vector2){4,0});
	SetSpeed_Starfield2D(&starfield2, (Vector2){2,0});
	SetSpeed_Starfield2D(&starfield3, (Vector2){1,0});

	sinparam = 0;
	textX = VirtualScreen.x;
	textLen = strlen(scrollText);

    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);

	UnloadTexture(font_data);
	UnloadTexture(select_bar);
	UnloadTexture(grey_bar);
	UnloadTexture(balle1);
	UnloadTexture(balle2);
	UnloadTexture(balle3);
	CloseWindow();
	return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void) {
	// Update
	//----------------------------------------------------------------------------------
	// TODO: Update your variables here
	//----------------------------------------------------------------------------------

	oldsinx = sinx;
	oldsiny = siny;

	sinparam += 0.04f;
	float x = sinparam;

	for (int i = 0; i < textLen; i++) {
		ySin[i] = (float)sin(x*2)*12;
		x += ((PI*2) / 24);
	}

	textX -= 2;
	if(textX < -textLen*16 ) textX = VirtualScreen.x;

	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();
		ClearBackground(BLACK);

		Draw_Starfield2D(&starfield3, (Vector2){1,0});
		Draw_Starfield2D(&starfield2, (Vector2){1,0});
		Draw_Starfield2D(&starfield1, (Vector2){1,0});

		DrawTextureQuad(bar_data, (Vector2){160,1}, (Vector2){0},(Rectangle){0,0,VirtualScreen.x,VirtualScreen.y},WHITE);

		for(int y = 100; y < 400; y += 40) {
			for(int x = 60; x < 580; x += 20) {
				DrawRectangle(x+sin(sinx) * 32, y + sin(siny) * 32 , 3, 3, (Color) { 85,153,255,255 } );
				sinx += 0.3;
				siny += 0.2;
			}
			siny += 0.5;
			sinx = oldsinx; // + 0.2;
		}

		sinx = oldsinx + 0.08;
		siny = oldsiny + 0.1;


		DrawTextureQuad(select_bar, (Vector2){160,1}, (Vector2){0},(Rectangle){0,282,VirtualScreen.x,14},WHITE);

		// -------------------------------------------------------------------------------------------------------------
		// Draw Page Text
		// -------------------------------------------------------------------------------------------------------------

		Vector2 textOffset = { 0, 0 };

		for(int i = 0; i < strlen(text1); i++) {
			if( (unsigned char)text1[i] == '\n') {
				textOffset.y += 18;
				textOffset.x = 0-(i*16);
			} else {
				DrawTexturePro(
					font_data,
					(Rectangle) { 0, (text1[i] - 32) * 16, 16, 16 },
					(Rectangle) { 60+textOffset.x + ( i * 16 + 1 ) , 120+textOffset.y , 16, 16 },
					(Vector2) {0},
					0,
					WHITE
				);
			}
		}

		for(int i=0; i < textLen; i++) {
			if(textX + ( i * 16 + 1 ) < VirtualScreen.x + 16 && textX + ( i * 16 + 1 ) > -16) {
				DrawTexturePro(
					font_data,
					(Rectangle) { 0, (scrollText[i] - 32) * 16, 16, 16 },
					(Rectangle) { textX + ( i * 16 + 1 ) , 380 + ySin[i] , 16, 16 },
					(Vector2) {0},
					0,
					WHITE
				);
			}
		}

		DrawFPS(10,VirtualScreen.y - 20);
	EndDrawing();
	//----------------------------------------------------------------------------------
}

#include "raylib.h"
#include "resource_dir.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 800

Color COLOR[] = {WHITE, YELLOW, RED, GREEN, MAGENTA, BLUE};
int color_index = 0;

// ================================================

void initializeStuff();
void changeTitleColor();
void handleInputs(Vector2* pos, Vector2* vel, Vector2* acc);
void drawStuff(Texture wabbit, Vector2 pos, Vector2 vel, Vector2 acc);
void updatePhysics(Texture wabbit, Vector2 *pos, Vector2 *vel, Vector2 *acc);

// ================================================

int main ()
{	
	initializeStuff();

	// Get "Character" Texture
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	wabbit.width *= 3; // Scale up
	wabbit.height *= 3; // Scale up

	// Position, Velocity, Acceleration vectors
	Vector2 pos = {WIN_WIDTH/2, WIN_HEIGHT/2};
	Vector2 vel = {5, 6};
	Vector2 acc = {0, -0.05};

	// Main Loop
	while (!WindowShouldClose())
	{
		BeginDrawing();		
			
			handleInputs(&pos, &vel, &acc);

			drawStuff(wabbit, pos, vel, acc);

			updatePhysics(wabbit, &pos, &vel, &acc);
	
		EndDrawing();
	}

	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}

// ================================================

void initializeStuff(){
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "Physics Sandbox");
	SetTargetFPS(60);
	SearchAndSetResourceDir("resources");
}

void changeTitleColor(){
	color_index = (color_index + 1) % 6;
}

void handleInputs(Vector2* pos, Vector2* vel, Vector2* acc){
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
		(*pos) = GetMousePosition();
		pos->y = WIN_HEIGHT - pos->y;
		vel->x = 0;
		vel->y = 0;

	}else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){

		float drag_duration = GetGestureHoldDuration();
		Vector2 mouse_drag_vec = GetMouseDelta();
		vel->x = mouse_drag_vec.x;
		vel->y = -1 * mouse_drag_vec.y;
	}

	if(IsKeyUp(KEY_Y)){
		ClearBackground(BLACK);
	}
	PollInputEvents();
}

void drawStuff(Texture wabbit, Vector2 pos, Vector2 vel, Vector2 acc){
	// Write Stuff
	DrawText("Physics!", WIN_WIDTH/2 - 150, 0.2 * WIN_HEIGHT, 72, COLOR[color_index]);
	DrawText(TextFormat("Position - x: %.3f  y: %.3f", pos.x, pos.y), 50, WIN_HEIGHT - 90, 18, GREEN);
	DrawText(TextFormat("Velocity - x: %.3f  y: %.3f", vel.x, vel.y), 50, WIN_HEIGHT - 70, 18, GREEN);
	DrawText(TextFormat("Acceleration - x: %.3f  y: %.3f", acc.x, acc.y), 50, WIN_HEIGHT - 50, 18, GREEN);

	// Draw "Character"
	DrawTexture(wabbit, pos.x, WIN_HEIGHT - pos.y, WHITE);
}

void updatePhysics(Texture wabbit, Vector2 *pos, Vector2 *vel, Vector2 *acc){
	pos->x = pos->x + vel->x;
	pos->y = pos->y + vel->y;
	
	vel->x = vel->x + acc->x;
	vel->y = vel->y + acc->y;

	if(pos->x >= WIN_WIDTH - wabbit.width){
		pos->x = WIN_WIDTH - wabbit.width;
		vel->x = -1 * vel->x;

		changeTitleColor();		
	}
	if(pos->x <= 0){
		pos->x = 0;
		vel->x = -1*vel->x;

		changeTitleColor();
	}

	if(pos->y >= 800){
		pos->y = 800;

		// Inelastic collision (40% of loss)
		vel->y = -0.6 * vel->y;

		changeTitleColor();
	}
	
	if(pos->y <= wabbit.height){
		pos->y = wabbit.height;

		// Inelastic collision (40% of loss)
		vel->y = -0.6 * vel->y;

		// Friction (10% loss)
		vel->x = 0.9 * vel->x;
		
		changeTitleColor();
	}
}
#include "raylib.h"
#include "resource_dir.h"
#include "solver.hpp"
#include <iostream>
#include <cmath>
#include <array>

constexpr int WIN_WIDTH = 1280;
constexpr int WIN_HEIGHT = 800;
double TIME_SCALE = 1;
std::array<Color, 6> COLOR = {WHITE, YELLOW, RED, GREEN, MAGENTA, BLUE};
int color_index = 0;

Vector2 acc;

// ================================================

void initializeStuff();
void changeTitleColor();
void handleInputs(Texture& wabbit, Vector2& pos, Vector2& vel, double& t, double& k);
void drawStuff(Texture wabbit, Vector2 pos, Vector2 vel, double& t, double& k);
void updatePhysics(Texture& wabbit, Vector2& pos, Vector2& vel);

std::vector<double> dy_dt (double t, std::vector<double> y, std::vector<double> params);
// ================================================

int main ()
{	
	initializeStuff();

	// Get "Character" Texture
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	wabbit.width *= 3; // Scale up
	wabbit.height *= 3; // Scale up

	// Position, Velocity vectors
	Vector2 pos = {WIN_WIDTH/2, WIN_HEIGHT/2};
	Vector2 vel = {10, 12};

	double k = 0.005;
	
	double h = GetFrameTime();
	
	// Instantiate ODE solver
	Euler solver;
	std::vector<double> y0 = {
		vel.x, vel.y, pos.x, pos.y
	};
	double t = 1; // T0 = 1
	
	// Main Loop
	while (!WindowShouldClose())
	{
		BeginDrawing();
			
			handleInputs(wabbit, pos, vel, t, k);

			drawStuff(wabbit, pos, vel, t, k);

			updatePhysics(wabbit, pos, vel);

			h = TIME_SCALE * GetFrameTime();

			y0 = {
				vel.x, vel.y, pos.x, pos.y
			};
			
			std::vector<double> y = solver.solveIvpStep(dy_dt, t, y0, h, {k});

			/*
			for(int dim = 0; dim < y.size(); dim++){
				std::cout << dim << ": y[" << t / TIME_SCALE << "] = " << y[dim] << std::endl;
			}
			std::cout << std::endl;
			*/

			vel.x = y[0];
			vel.y = y[1];
			pos.x = y[2];
			pos.y = y[3];

			t = t + h;
	
		EndDrawing();
	}

	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}

// ================================================

std::vector<double> dy_dt (double t, std::vector<double> y, std::vector<double> params){
	double k = params[0]; // air drag coefficient

	acc.x = - k * y[0] * std::sqrt(y[0]*y[0] + y[1]*y[1]);
	acc.y = - 300 - k * y[1] * std::sqrt(y[0]*y[0] + y[1]*y[1]);

    return {
		- k * y[0] * std::sqrt(y[0]*y[0] + y[1]*y[1]),
		- 300 - k * y[1] * std::sqrt(y[0]*y[0] + y[1]*y[1]),
		y.at(0),
		y.at(1),
	};
}

void initializeStuff(){
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "Physics Sandbox");
	SetTargetFPS(60);
	SearchAndSetResourceDir("resources");
}

void changeTitleColor(){
	color_index = (color_index + 1) % 6;
}

void handleInputs(Texture& wabbit, Vector2& pos, Vector2& vel, double& t, double& k){
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

		// Set texture center to mouse position
		pos = GetMousePosition();
		pos.x -= wabbit.width/2;
		pos.y = WIN_HEIGHT - pos.y + wabbit.height/2;
		
		// Set velocity to zero while dragging
		vel.x = 0;
		vel.y = 0;

		t = 0;

	}else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
		// Get mouse dragging direction
		Vector2 mouse_drag_vec = GetMouseDelta();
		vel.x = mouse_drag_vec.x;
		vel.y = -1 * mouse_drag_vec.y;
	}

	if(IsKeyUp(KEY_Y)){
		ClearBackground(BLACK);
	}

	if(IsKeyDown(KEY_UP)){
		TIME_SCALE += 0.5;
	} else if(IsKeyDown(KEY_DOWN)){
		if(TIME_SCALE >= 1.5){
			TIME_SCALE -= 0.5;
		}
	}

	if(IsKeyDown(KEY_RIGHT)){
		k += 0.005;
	} else if(IsKeyDown(KEY_LEFT)){
		if(k >= 0.004){
			k -= 0.005;
		}
		k = std::abs(k);
	}
	
	if(IsKeyPressed(KEY_ENTER)){
		pos = {WIN_WIDTH/2, WIN_HEIGHT/2};
		vel = {10, 12};
	}

	PollInputEvents();
}

void drawStuff(Texture wabbit, Vector2 pos, Vector2 vel, double& t, double& k){
	// Write Stuff
	DrawText("Physics!", WIN_WIDTH/2 - 150, 0.2 * WIN_HEIGHT, 72, COLOR[color_index]);
	DrawText(TextFormat("Position - x: %.3f  y: %.3f", pos.x, pos.y), 50, WIN_HEIGHT - 90, 18, GREEN);
	DrawText(TextFormat("Velocity - x: %.3f  y: %.3f", vel.x, vel.y), 50, WIN_HEIGHT - 70, 18, GREEN);
	DrawText(TextFormat("Acceleration - x: %.3f  y: %.3f", acc.x, acc.y), 50, WIN_HEIGHT - 50, 18, GREEN);

	if(TIME_SCALE > 1){
		DrawText(TextFormat("Time Speedup: %.1fx", TIME_SCALE), WIN_WIDTH - 250, WIN_HEIGHT - 50, 16, WHITE);
	}
	DrawText(TextFormat("Air Drag Coeff. (k): %.3f", k), WIN_WIDTH - 250, WIN_HEIGHT - 20, 18, WHITE);

	// Draw "Character"
	DrawTexture(wabbit, pos.x, WIN_HEIGHT - pos.y, WHITE);
}

void updatePhysics(Texture& wabbit, Vector2& pos, Vector2& vel){

	if(pos.x >= WIN_WIDTH - wabbit.width){
		pos.x = WIN_WIDTH - wabbit.width;
		vel.x = -1 * vel.x;

		changeTitleColor();		
	}
	if(pos.x <= 0){
		pos.x = 0;
		vel.x = -1*vel.x;

		changeTitleColor();
	}

	if(pos.y >= 800){
		pos.y = 800;

		// Inelastic collision (40% of loss)
		vel.y = -0.6 * vel.y;

		changeTitleColor();
	}
	
	if(pos.y <= wabbit.height){
		pos.y = wabbit.height;

		// Inelastic collision (40% of loss)
		vel.y = -0.6 * vel.y;

		// Friction (10% loss)
		vel.x = 0.9 * vel.x;
		
		changeTitleColor();
	}
}
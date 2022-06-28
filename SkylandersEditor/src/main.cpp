#include <stdio.h>
#include <wchar.h>

#include "Portal.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Color portalColor = RAYWHITE;
Portal* portal;

void DrawPortalControls();

int main()
{
	InitWindow(800, 600, "Skylanders Editor");
	SetTargetFPS(120);
	hid_init();

	portal = new Portal();

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		GuiDrawRectangle({ 0, 0, (float)GetScreenWidth(), 30 }, 1, BLACK, RAYWHITE);

		DrawPortalControls();

		GuiStatusBar({ 0, static_cast<float>(GetScreenHeight() - 20), 150, 20 }, portal->status);

		EndDrawing();
	}

	portal = NULL;
	delete portal;

	hid_exit();
	CloseWindow();
}

void DrawPortalControls()
{
	if (!portal->connected)
	{

		if (GuiButton({ 0, 30, 100, 20 }, "Connect"))
		{
			portal->Connect();
		}

	}
	else
	{
		GuiButton({ 0, 30, 100, 20 }, "Portal connected");
	}


	portalColor = GuiColorPicker({ (float)GetScreenWidth() - 130, 35, 100, 100}, "Select portal color", portalColor);

	if (GuiButton({ (float)GetScreenWidth() - 130, 140, 126, 20}, "Set color"))
	{
		portal->SetColor(portalColor.r, portalColor.g, portalColor.b);
	}
}
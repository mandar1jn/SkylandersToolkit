#include <stdio.h>
#include <wchar.h>

#include "Portal.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Color portalColor = RED;
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

		GuiDrawRectangle({ -1, 0, (float)GetScreenWidth() + 2, 30 }, 1, BLACK, RAYWHITE);

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

	const static Rectangle COLOR_SELECTION_AREA = { (float)GetScreenWidth() - 140, 29, 141, 136 };

	GuiDrawRectangle(COLOR_SELECTION_AREA, 1, BLACK, RAYWHITE);

	if (portal->features.color)
	{
		portalColor = GuiColorPicker({ (float)GetScreenWidth() - 135, 35, 100, 100 }, "Select portal color", portalColor);

		if (GuiButton({ (float)GetScreenWidth() - 135, 140, 126, 20 }, "Set color"))
		{
			portal->SetColor(portalColor.r, portalColor.g, portalColor.b);
		}
	}
	else
	{
		GuiDrawText("Color not supported", COLOR_SELECTION_AREA, TEXT_ALIGN_CENTER, BLACK);
	}
}
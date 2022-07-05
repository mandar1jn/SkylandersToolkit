#include <stdio.h>
#include <wchar.h>

#include "Portal.h"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

enum Menu
{
	PORTAL_CONTROLS = 0,
	DEBUG
};

Color portalColor = {0xFF, 0x00, 0x00};
Portal* portal;
int colorSide = 1;
bool editColorSide = false;
Menu currentMenu = PORTAL_CONTROLS;

inline void DrawNavigation();
inline void DrawPortalControls();
inline void DrawDebugControls();
inline void DrawStatus();

int main()
{
	InitWindow(800, 600, "Skylanders Editor");
	SetTargetFPS(120);
	hid_init();

	portal = new Portal();

	portal->Connect();

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawNavigation();

		switch (currentMenu)
		{
		case PORTAL_CONTROLS:
			DrawPortalControls();
			break;
		case DEBUG:
			DrawDebugControls();
			break;
		}

		DrawStatus();

		EndDrawing();
	}

	portal->~Portal();

	portal = NULL;
	delete portal;

	hid_exit();
	CloseWindow();
}

inline void DrawNavigation()
{
	GuiDrawRectangle({ -1, 0, (float)GetScreenWidth() + 2, 30 }, 1, BLACK, RAYWHITE);

	if (GuiButton({ 5, 5, 50, 20 }, "Portal"))
	{
		currentMenu = PORTAL_CONTROLS;
	}

	if (GuiButton({ 60, 5, 50, 20 }, "Debug"))
	{
		currentMenu = DEBUG;
	}
}

inline void DrawStatus()
{
	GuiStatusBar({ 0, (float)GetScreenHeight() - 20, 150, 20 }, portal->status.c_str());
}

inline void DrawPortalControls()
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
		if (GuiButton({ 0, 30, 100, 20 }, "Disconnect"))
		{
			portal->Disconnect();
			portal->status = "Portal disconnected";
		}
	}

	const static Rectangle COLOR_SELECTION_AREA = { (float)GetScreenWidth() - 140, 29, 141, 136 };

	GuiDrawRectangle(COLOR_SELECTION_AREA, 1, BLACK, RAYWHITE);

	if (portal->features.color)
	{
		portalColor = GuiColorPicker({ (float)GetScreenWidth() - 135, 35, 100, 100 }, "Select portal color", portalColor);

		if (GuiButton({ (float)GetScreenWidth() - 135, 200, 126, 20 }, "Set color"))
		{
			switch (colorSide)
			{
			case 0:
				portal->SetColor(portalColor.r, portalColor.g, portalColor.b);
				break;
			case 1:
				portal->SetColorAlternative(0x00, portalColor.r, portalColor.g, portalColor.b, 0x00, 0x00);
				break;
			case 2:
				portal->SetColorAlternative(0x02, portalColor.r, portalColor.g, portalColor.b, 0x00, 0x00);
				break;
			}
		}

		const static Rectangle SIDE_SELECTOR_AREA = { (float)GetScreenWidth() - 135, 140, 126, 20 };
		if (portal->features.sidedColor)
		{
			if (GuiDropdownBox(SIDE_SELECTOR_AREA, "all;right;left", &colorSide, editColorSide))
			{
				editColorSide = !editColorSide;
			}
		}
		else
		{
			colorSide = 0;
			GuiDropdownBox(SIDE_SELECTOR_AREA, "full portal", &colorSide, false);
		}
	}
	else
	{
		GuiDrawText("Color not supported", COLOR_SELECTION_AREA, TEXT_ALIGN_CENTER, BLACK);
	}
}

inline void DrawDebugControls()
{

	char string[20];

	sprintf(string, "Portal ID: 0x%X 0x%X", portal->Id[0], portal->Id[1]);

	GuiDrawText(string, {2, 40}, TEXT_ALIGN_LEFT, BLACK);
}
#include <stdio.h>
#include <wchar.h>

#include "Portal.h"

#include "raylib.h"

int main()
{
	InitWindow(800, 600, "Skylanders Editor");
	hid_init();

	Portal* portal = new Portal();

	while (!WindowShouldClose())
	{

	}

	portal = NULL;
	delete portal;

	hid_exit();
	CloseWindow();
}
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include <vector>
#include <string>

#include "Row.h"

namespace Editor
{
	struct Config
	{
		static struct termios orig_termios;
		static int screenrows, screencols;
		static int paddingX;
		static int cursorX, cursorY;
		static std::vector<Row> row;
		static int rowoff, coloff;
		static int renderX;
		static std::string filename;

		inline static bool saveOriginalTerminalSettings() { return tcgetattr(STDIN_FILENO, &orig_termios) != -1; };
		inline static bool restoreOriginalTerminalSettings() { return tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) != -1; };
		static void initTerminal();

		static bool getCursorPosition();
		static bool getWindowSize();

		inline static Row& currentRow() { return row[cursorY]; };
	};
}

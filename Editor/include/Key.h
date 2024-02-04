#pragma once

#include <array>

namespace Editor
{

	enum class Key
	{
		BACKSPACE = '\x7f',
		QUIT = 'q',
		ARROW_LEFT = 'a',
		ARROW_RIGHT = 'd',
		ARROW_UP = 'w',
		ARROW_DOWN = 's',
		PAGE_UP = '{',
		PAGE_DOWN = '}',
		DEL = 'x',
		HOME = '[',
		END = ']',
		COMMAND_MODE = ':',
		INSERT_MODE = 'i',
		DEFAULT = '\x1lb',
		ESC = '\e',
		RETURN = '\r'

	};

	Key decodeSequence(std::array<char, 3> seq);
}
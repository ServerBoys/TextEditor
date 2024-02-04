#include "State.h"

namespace Editor
{
	Mode State::mode{Mode::NORMAL};
	Buffer State::appendBuffer{};
	Buffer State::commandBuffer{};
	char State::statusMessage[80] = "";
	std::time_t State::statusMessageTime{};
	bool State::isDirty{false};

	void State::appendBufferInit()
	{
		Editor::State::appendBuffer.clear();
		Editor::State::appendBuffer.append("\x1b[?25l", 6);
		Editor::State::appendBuffer.append("\x1b[H", 3);
	}

	void State::setCursorPosition()
	{
		char buf[32];

		if (Editor::State::mode == Editor::Mode::NORMAL)
			snprintf(buf, sizeof(buf), "\x1b[%d;%dH\e[%d q", Editor::Config::cursorY - Editor::Config::rowoff + 1, Editor::Config::renderX - Editor::Config::coloff + 1, 2);

		else if (Editor::State::mode == Editor::Mode::COMMAND)
			snprintf(buf, sizeof(buf), "\x1b[%d;%dH\e[%d q", Editor::Config::screenrows, 2, 5);
		
		else if (Editor::State::mode == Editor::Mode::INSERT)
			snprintf(buf, sizeof(buf), "\x1b[%d;%dH\e[%d q", Editor::Config::cursorY - Editor::Config::rowoff + 1, Editor::Config::renderX - Editor::Config::coloff + 1, 5);
		Editor::State::appendBuffer.appendNormal(buf, strlen(buf));
	}

	void State::setStatusMessage(const char* fmt,...)
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(State::statusMessage, sizeof(State::statusMessage), fmt, args);
		va_end(args);
		State::statusMessageTime = std::time(nullptr);
	}
}
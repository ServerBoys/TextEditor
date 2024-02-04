#pragma once

#include <ctime>
#include <stdarg.h>

#include "Config.h"
#include "Buffer.h"

namespace Editor {
	enum class Mode {
		NORMAL, INSERT, COMMAND
	};

	struct State {
		static Mode mode;
		static Buffer appendBuffer;
		static Buffer commandBuffer;
		static char statusMessage[80];
		static std::time_t statusMessageTime;
		static bool isDirty;

		static void appendBufferInit();
		static void setCursorPosition();
		static void setStatusMessage(const char* fmt,...);
	};
}
#pragma once


#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "Log.h"
#include "Config.h"
#include "Buffer.h"
#include "State.h"
#include "Painter.h"
#include "Key.h"
#include "Row.h"

#define EDITOR_VERSION "0.0.1"

#define DIE_IF_ERR(x) if (x) Editor::die(__LINE__, __FILE__)

namespace Editor {
    void init();
    void open(const char* filename);
    void save();
    void refreshScreen();
    void die(unsigned int line, const char* file);
    void exitNormally();

    void disableRawMode();
    void enableRawMode();

    void insertChar(char ch);
    void deleteChar();
    void deleteRow(std::size_t at);
    std::string rowsToString();
    void insertNewLine();
}
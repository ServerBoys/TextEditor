#pragma once

#include "Config.h"
#include "State.h"

namespace Editor {
    class Painter {
    public:
        static void refresh();
        static void paintRow(unsigned int row);
        static void paintRows();
        static void paintStatusBar();
        static void paintMessageBar();
    };
}
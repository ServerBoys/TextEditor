#pragma once

#include <cstdlib>
#include <cstring>
#include <string>

#define TAB_STOP 8

namespace Editor
{

    struct Row
    {
        int size;
        int render_size;
        char *chars;
        std::string render;

        Row(const char* string, ssize_t size);
        Row(Row&& source);
        ~Row();
        Row& operator=(Row&& source);
        
        void insertChar(unsigned int at, char ch);
        void appendString(const char* str, std::size_t len);
        void deleteChar(unsigned int at);
        void draw() const;
        void update();
        unsigned int toRenderX(int cursorX);
        static void insert(unsigned int at, const char* str, ssize_t len);
    };
}
#include "Painter.h"
#include "Editor.h"

namespace Editor
{

    void Painter::refresh()
    {
        Editor::State::appendBufferInit();

        Painter::paintRows();
        Painter::paintStatusBar();
        Painter::paintMessageBar();

        Editor::State::setCursorPosition();
        Editor::State::appendBuffer.appendNormal("\x1b[?25h", 6);
        Editor::State::appendBuffer.draw();
    }

    void welcomeMessage()
    {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
                                  "Editor -- version %s", EDITOR_VERSION);
        if (welcomelen > Editor::Config::screencols)
            welcomelen = Editor::Config::screencols;

        int padding = (Editor::Config::screencols - welcomelen) / 2;
        if (padding)
        {
            Editor::State::appendBuffer.append("~", 1);
            padding--;
        }

        while (padding--)
            Editor::State::appendBuffer.append(" ", 1);

        Editor::State::appendBuffer.append(welcome, welcomelen);
    }

    void Painter::paintRow(unsigned int row)
    {

        Editor::State::appendBuffer.append("\x1b[K", 3);
        int filerow = row + Editor::Config::rowoff;
        if (Editor::Config::row.size() == 0 && row == Editor::Config::screenrows / 3)
            welcomeMessage();
        else if (filerow >= Editor::Config::row.size())
            Editor::State::appendBuffer.append("~", 1);
        else
            Editor::Config::row[filerow].draw();
    }

    void Painter::paintRows()
    {
        int y;
        for (y = 0; y < Editor::Config::screenrows; y++)
        {
            paintRow(y);
            Editor::State::appendBuffer.append("\r\n", 2);
        }
    }

    void Painter::paintStatusBar()
    {
        Editor::State::appendBuffer.append("\x1b[7m", 4);

        if (Editor::State::mode == Editor::Mode::COMMAND)
        {
            Editor::State::appendBuffer.appendNormal(":", 1);
            Editor::State::appendBuffer.append(Editor::State::commandBuffer.buffer(), Editor::State::commandBuffer.size());
        }
        else
        {
            char status[80], rstatus[40];
            int len = snprintf(
                    status, sizeof(status), "%.20s - %d lines%s", 
                    Editor::Config::filename.length() ? Editor::Config::filename.c_str() : "[No Name]", Editor::Config::row.size(),
                    Editor::State::isDirty? " (modified)" : "");
            int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",
                                Editor::Config::cursorY + 1, Editor::Config::row.size());
            Editor::State::appendBuffer.append(status, len);
            if (len > Editor::Config::screencols)
                len = Editor::Config::screencols;
            for (; len < Editor::Config::screencols; len++)
            {
                if (Editor::Config::screencols - len == rlen)
                {
                    Editor::State::appendBuffer.append(rstatus, rlen);
                    break;
                }
                Editor::State::appendBuffer.append(" ", 1);
            }
        }
        Editor::State::appendBuffer.append("\x1b[m", 3);
        Editor::State::appendBuffer.append("\r\n", 2);
    }

    void Painter::paintMessageBar()
    {
        Editor::State::appendBuffer.append("\x1b[K", 3);
        if (Editor::State::statusMessageTime + 5 < std::time(nullptr))
            Editor::State::statusMessage[0] = '\0';
        if (Editor::State::statusMessage[0]!= '\0')
        {
            Editor::State::appendBuffer.appendNormal(Editor::State::statusMessage, strlen(Editor::State::statusMessage));
        }
    }
}
#include "Editor.h"

namespace Editor
{

    void init()
    {
        Editor::enableRawMode();
        DIE_IF_ERR(!Editor::Config::getWindowSize());
    }

    void open(const char *filename)
    {
        Config::filename.assign(filename);

        Log::info("Opening file %s, %d", Config::filename.c_str(), Config::filename.length());
        FILE *fp = fopen(filename, "r");
        DIE_IF_ERR(!fp);

        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;
        while ((linelen = getline(&line, &linecap, fp)) != EOF)
        {
            while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
                linelen--;
            Config::row.emplace_back(line, linelen);
        }
        free(line);
        fclose(fp);
        State::isDirty = false;
    }

    void save()
    {
        std::string output = rowsToString();
        FILE *fp = fopen((Editor::Config::filename + "output").c_str(), "w");
        if (fp &&
            ftruncate(fileno(fp), output.length()) != -1 &&
            write(fileno(fp), output.c_str(), output.length()) != -1)
        {
            Editor::State::setStatusMessage("%d bytes written to %s", output.length(), Editor::Config::filename.c_str());
        } else {
            Editor::State::setStatusMessage("Cannot save! I/O error %s", strerror(errno));
            return;
        }
        fclose(fp);
    }

    void refreshScreen()
    {
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
    }

    void die(unsigned int line, const char *file)
    {
        char *err = strerror(errno);
        char errorMessage[256];
        snprintf(errorMessage, 256, "Error at line %d in file %s: %s", line, file, err);

        Log::error(errorMessage);
        perror(errorMessage);
        Log::close();
        exit(1);
    }

    void exitNormally()
    {
        Log::close();
        exit(0);
    }

    void disableRawMode()
    {
        refreshScreen();
        DIE_IF_ERR(!Config::restoreOriginalTerminalSettings());
    }

    void enableRawMode()
    {
        DIE_IF_ERR(!Config::saveOriginalTerminalSettings());

        atexit(disableRawMode);
        Config::initTerminal();
    }

    void insertChar(char ch)
    {
        if (Config::cursorY == Config::row.size())
            Config::row.emplace_back( "", 0 );
        Config::currentRow().insertChar(Config::cursorX, ch);
        Config::cursorX++;
    }

    void deleteChar()
    {
        if (Config::cursorY == Config::row.size())
            return;
        if (Config::cursorX == 0 && Config::cursorY == 0)
            return;
        
        Row& row = Config::currentRow();
        if (Config::cursorX == 0)
        {
            Config::cursorY--;
            Config::cursorX = Config::currentRow().size;
            Config::currentRow().appendString(row.chars, row.size);
            deleteRow(Config::cursorY + 1);
        } else {
            row.deleteChar(Config::cursorX - 1);
            Config::cursorX--;
        }
    }

    void deleteRow(std::size_t at) {
        if (at >= Config::row.size()  || at < 0)
            return;
        Config::row.erase(Config::row.begin() + at);
        State::isDirty = true;
    }

    std::string rowsToString()
    {
        std::stringstream ss;
        for (auto &row : Config::row)
            ss << row.chars << "\n";
        
        return ss.str();
    }

    void insertNewLine() {
        if (Config::cursorX == 0)
            Config::row.insert(Config::row.begin() + Config::cursorY, Row("", 0));
        else {
            Row& row = Config::currentRow();
            Row::insert(Config::cursorY + 1, row.chars + Config::cursorX, row.size - Config::cursorX);
            row.chars[Config::cursorX] = '\0';
            row.size = Config::cursorX;
            row.update();
        }
        Config::cursorY++;
        Config::cursorX = 0;
    }
}
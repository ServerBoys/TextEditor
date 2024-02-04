#include "Config.h"

namespace Editor
{

    struct termios Config::orig_termios{};
    int Config::paddingX{0};
    int Config::screenrows{0};
    int Config::screencols{0};
    int Config::cursorX{paddingX};
    int Config::cursorY{0};
    std::vector<Row> Config::row{};
    int Config::rowoff{0};
    int Config::coloff{0};
    int Config::renderX{0};
    std::string Config::filename{};

    bool Config::getCursorPosition()
    {
        char buf[32];
        int i = -1;
        if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
            return false;

        while (++i < sizeof(buf) - 1)
            if (read(STDIN_FILENO, &buf[i], 1) != 1 || buf[i] == 'R')
                break;
        buf[i] = '\0';

        if (buf[0] != '\x1b' || buf[1] != '[')
            return false;
        if (sscanf(&buf[2], "%d;%d", &screenrows, &screencols) != 2)
            return false;
            
        Config::screenrows -= 2;

        return true;
    }

    bool Config::getWindowSize()
    {
        struct winsize ws;
        
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        {
            if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
                return false;
            
            return getCursorPosition();
        }
        else
        {
            screencols = ws.ws_col;
            screenrows = ws.ws_row;
            Config::screenrows -= 2;
            return true;
        }
    }

    void Config::initTerminal()
    {
        struct termios raw = orig_termios;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        raw.c_cflag |= (CS8);
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }


}
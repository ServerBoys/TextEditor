// TextEditor.cpp : Defines the entry point for the application.
//

#include "TextEditor.h"

char editorReadKey()
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
		DIE_IF_ERR(nread == -1 && errno != EAGAIN);

	if (c == '\x1b')
	{
		std::array<char, 3> seq;
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return '\x1b';

		if (seq[1] >= '0' && seq[1] <= '9' && read(STDIN_FILENO, &seq[2], 1) != 1)
			return (char)Editor::Key::DEFAULT;
		return (char)Editor::decodeSequence(seq);
	}

	else
		return c;
}

/*** input ***/
void editorMoveCursor(Editor::Key key)
{
	Editor::Row *row = (Editor::Config::cursorY >= Editor::Config::row.size()) ? NULL : &Editor::Config::row[Editor::Config::cursorY];
	
	switch (key)
	{
	case Editor::Key::ARROW_LEFT:
		if (Editor::Config::cursorX != 0)
			Editor::Config::cursorX--;
		else if (Editor::Config::cursorY > 0)
		{
			Editor::Config::cursorY--;
			Editor::Config::cursorX = Editor::Config::row[Editor::Config::cursorY].size;
		}
		break;
	case Editor::Key::ARROW_RIGHT:
		if (row && Editor::Config::cursorX < row->size)
			Editor::Config::cursorX++;
		else if (row && Editor::Config::cursorX == row->size)
		{
			Editor::Config::cursorY++;
			Editor::Config::cursorX = 0;
		}
		break;
	case Editor::Key::ARROW_UP:
		if (Editor::Config::cursorY != Editor::Config::paddingX)
			Editor::Config::cursorY--;
		break;
	case Editor::Key::ARROW_DOWN:
		if (Editor::Config::cursorY < Editor::Config::row.size())
			Editor::Config::cursorY++;
		break;
	}

	row = (Editor::Config::cursorY >= Editor::Config::row.size()) ? NULL : &Editor::Config::row[Editor::Config::cursorY];
	int rowlen = row ? row->size : 0;
	if (Editor::Config::cursorX > rowlen)
	{
		Editor::Config::cursorX = rowlen;
	}
}

void editorProcessKeypress()
{
	int times;
	Editor::Key c = Editor::Key(editorReadKey());
	if (Editor::State::mode == Editor::Mode::NORMAL)
		switch (c)
		{
		case Editor::Key::ESC:
			Editor::save();
			break;
		case Editor::Key::QUIT:
			Editor::exitNormally();
			break;
		case Editor::Key::ARROW_LEFT:
		case Editor::Key::ARROW_RIGHT:
		case Editor::Key::ARROW_UP:
		case Editor::Key::ARROW_DOWN:
			editorMoveCursor(c);
			break;
		case Editor::Key::PAGE_UP:
			// Editor::Config::cursorY = Editor::Config::rowoff;
			goto SIMILARITY;
		case Editor::Key::PAGE_DOWN:
			//   Editor::Config::cursorY = Editor::Config::rowoff + Editor::Config::screenrows - 1;
			//   if (Editor::Config::cursorY > Editor::Config::row.size()) Editor::Config::cursorY = Editor::Config::row.size();
		SIMILARITY:
			times = Editor::Config::screenrows;
			while (times--)
				editorMoveCursor(c == Editor::Key::PAGE_UP ? Editor::Key::ARROW_UP : Editor::Key::ARROW_DOWN);
			break;
		case Editor::Key::INSERT_MODE:
			Editor::State::mode = Editor::Mode::INSERT;
			break;
		case Editor::Key::COMMAND_MODE:
			Editor::State::mode = Editor::Mode::COMMAND;
        	Editor::State::commandBuffer.clear();
			break;
		default:
			break;
		}
	else if (Editor::State::mode == Editor::Mode::INSERT)
	{
		switch (c)
		{
		case Editor::Key::RETURN:
			Editor::insertNewLine();
			break;
		case Editor::Key::ESC:
			Editor::State::mode = Editor::Mode::NORMAL;
			break;
		case Editor::Key::BACKSPACE:
			Editor::deleteChar();
			break;
		default:
			char ch = (char)c;
			if (ch >= ' ' && ch < '~')
				Editor::insertChar(ch);
			break;
		}
	}
	else if (Editor::State::mode == Editor::Mode::COMMAND)
	{
		switch (c)
		{
		case Editor::Key::ESC:
			Editor::State::mode = Editor::Mode::NORMAL;
		default:
			Editor::State::commandBuffer.append((char *)&c, 1);
		}
	}
}

void editorScroll()
{

	Editor::Config::renderX = 0;

	if (Editor::Config::cursorY < Editor::Config::row.size())
	{
		Editor::Config::renderX = Editor::Config::row[Editor::Config::cursorY].toRenderX(Editor::Config::cursorX);
	}

	if (Editor::Config::cursorY < Editor::Config::rowoff)
	{
		Editor::Config::rowoff = Editor::Config::cursorY;
	}

	if (Editor::Config::cursorY >= Editor::Config::rowoff + Editor::Config::screenrows)
	{
		Editor::Config::rowoff = Editor::Config::cursorY - Editor::Config::screenrows + 1;
	}

	if (Editor::Config::renderX < Editor::Config::coloff)
		Editor::Config::coloff = Editor::Config::renderX;

	if (Editor::Config::renderX >= Editor::Config::coloff + Editor::Config::screencols)
		Editor::Config::coloff = Editor::Config::renderX - Editor::Config::screencols + 1;
}

/*** file i/o ***/

/**init **/

static void resize_handler(int sig)
{
	if (sig == SIGWINCH)
	{
		DIE_IF_ERR(!Editor::Config::getWindowSize());
		editorScroll();
		Editor::Painter::refresh();
	}
}

int main(int argc, char* argv[])
{
	Editor::init();
	signal(SIGWINCH, resize_handler);
	Editor::open("./TextEditor.cpp");

	Editor::State::setStatusMessage("HELP: :wq = save | :q! = quit");

	while (1)
	{
		editorScroll();
		Editor::Painter::refresh();
		editorProcessKeypress();
	}

	return 0;
}
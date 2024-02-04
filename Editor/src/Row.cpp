#include "Config.h"
#include "State.h"
#include "Row.h"

namespace Editor
{

	Row::Row(const char *string, ssize_t size)
		: size(size), chars((char *)malloc(size + 1))
	{
		std::memcpy(chars, string, size);
		chars[size] = '\0';
		render_size = 0;
		update();
		State::isDirty = true;
	}

	Row::Row(Row &&source)
		: size(source.size), render_size(source.render_size), chars(source.chars), render(std::move(source.render))
	{
		source.chars = nullptr;
	}

	Row::~Row()
	{
		free(chars);
	}

	Row &Row::operator=(Row &&source)
	{
		if (this != &source)
		{
			free(chars);
			size = source.size;
			render_size = source.render_size;
			chars = source.chars;
			render = std::move(source.render);
			source.chars = nullptr;
		}
		return *this;
	}

	void Row::insertChar(unsigned int at, char ch)
	{
		if (at > size)
			at = size;
		if (at < 0)
			at = 0;
		size++;
		chars = (char *)realloc(chars, size + 1);
		if (at != size)
			std::memmove(&chars[at + 1], &chars[at], size - at);
		chars[at] = ch;
		update();
		State::isDirty = true;
	}

    void Row::appendString(const char *str, std::size_t len)
    {
		chars = (char *)realloc(chars, size + len + 1);
		std::memcpy(&chars[size], str, len);
		size += len;
		chars[size] = '\0';
		update();
		State::isDirty = true;
    }

    void Row::deleteChar(unsigned int at)
	{
		if (at >= size)
			at = size - 1;
		if (at < 0)
			at = 0;
		size--;
		chars = (char *)realloc(chars, size + 1);
		std::memmove(&chars[at], &chars[at + 1], size - at);
		update();
		State::isDirty = true;
	}

	void Row::draw() const
	{
		int len = render_size - Editor::Config::coloff;
		if (len < 0)
			len = 0;
		if (len > Editor::Config::screencols)
			len = Editor::Config::screencols;
		Editor::State::appendBuffer.append(&render[Editor::Config::coloff], len);
	}

	void Row::update()
	{
		int tabs = 0;
		int j, idx = 0;

		for (j = 0; j < size; j++)
			if (chars[j] == '\t')
				tabs++;

		render.resize(size + tabs * (TAB_STOP - 1) + 1);
		for (j = 0; j < size; j++)
		{
			if (chars[j] == '\t')
				do
				{
					render[idx++] = ' ';
				} while (idx % TAB_STOP != 0);

			else
				render[idx++] = chars[j];
		}
		render[idx] = '\0';
		render_size = idx;
	}

	unsigned int Row::toRenderX(int cursorX)
	{
		int renderX = 0;
		int j;
		for (j = 0; j < cursorX; j++)
		{
			if (chars[j] == '\t')
				renderX += (TAB_STOP - 1) - (renderX % TAB_STOP);
			renderX++;
		}
		return renderX;
	}

	void Row::insert(unsigned int at, const char* string, ssize_t len) {
		if (at > Config::row.size()) {
			at = Config::row.size();
		}
		Config::row.emplace(Config::row.begin() + at, string, len);
	}
}
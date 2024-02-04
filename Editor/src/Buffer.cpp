#include "Buffer.h"

namespace Editor {
	Buffer::Buffer()
		:_buffer{} {

	}

	void Buffer::append(const char* str, std::size_t len) {
		if (_buffer.size() + len > _buffer.capacity())
			_buffer.reserve(_buffer.size() + _buffer_increment);
		for (int i = 0; i < len; i++)
			_buffer.push_back(str[i]);
	}

	
	void Buffer::appendNormal(const char* str, std::size_t len) {
		if (_buffer.size() + len > _buffer.capacity())
			_buffer.reserve(_buffer.size() + len);
		for (int i = 0; i < len; i++)
			_buffer.push_back(str[i]);
	}

	void Buffer::draw() const {
		write(STDOUT_FILENO, buffer(), size());
	}
}
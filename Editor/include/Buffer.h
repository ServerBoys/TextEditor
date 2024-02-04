#pragma once

#include <unistd.h>
#include <vector>

namespace Editor {

	class Buffer {

	private:
		std::vector<char> _buffer;

		static const int _buffer_increment = 128;
	
	public:
		Buffer();
		void append(const char* str, std::size_t len);
		void appendNormal(const char* str, std::size_t len);
		inline const char* buffer() const { return _buffer.data(); }
		inline std::size_t size() const { return _buffer.size(); }
		void clear() { return _buffer.clear(); };
		void draw() const;
	};
}
#include "Key.h"

namespace Editor {
    
	Key decodeSequence(std::array<char, 3> seq)
	{
		if (seq[0] == '[')
		{
			if (seq[1] >= '0' && seq[1] <= '9')
			{
				if (seq[2] == '~')
				{
					switch (seq[1])
					{
					case '1':
						return Key::HOME;
					case '3':
						return Key::DEL;
					case '4':
						return Key::END;
					case '5':
						return Key::PAGE_UP;
					case '6':
						return Key::PAGE_DOWN;
					case '7':
						return Key::HOME;
					case '8':
						return Key::END;
					}
				}
			}
			else
			{
				switch (seq[1])
				{
				case 'A':
					return Key::ARROW_UP;
				case 'B':
					return Key::ARROW_DOWN;
				case 'C':
					return Key::ARROW_RIGHT;
				case 'D':
					return Key::ARROW_LEFT;
				case 'H':
					return Key::HOME;
				case 'F':
					return Key::END;
				}
			}
		}

		else if (seq[0] == 'O')
		{
			switch (seq[1])
			{
			case 'H':
				return Key::HOME;
			case 'F':
				return Key::END;
			}
		}

		return Key::DEFAULT;
	}
}
#ifndef SYMBOLS
#define SYMBOLS

enum class Symbol { NONE, WALL, CHARGER, ROBOT, CHARGING,
	DIRT0, DIRT1, DIRT2, DIRT3, DIRT4, DIRT5, DIRT6, DIRT7, DIRT8, DIRT9 };

char sym_char(Symbol sym) {
	switch (sym) {
		case Symbol::NONE: return ' ';
		case Symbol::WALL: return '.';
		case Symbol::CHARGER: return '+';
		case Symbol::ROBOT: return 'x';
		case Symbol::CHARGING: return '*';
		case Symbol::DIRT0: return '0';
		case Symbol::DIRT1: return '1';
		case Symbol::DIRT2: return '2';
		case Symbol::DIRT3: return '3';
		case Symbol::DIRT4: return '4';
		case Symbol::DIRT5: return '5';
		case Symbol::DIRT6: return '6';
		case Symbol::DIRT7: return '7';
		case Symbol::DIRT8: return '8';
		case Symbol::DIRT9: return '9';
		default: return '?';
	}
}

#endif
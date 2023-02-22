#ifndef SYMBOLS
#define SYMBOLS

namespace Sym {

	constexpr char NONE = ' ';
	constexpr char WALL = '.';
	constexpr char CHARGER = '+';
	constexpr char ROBOTSYM = 'x';
	constexpr char DIRT0 = '0';
	constexpr char DIRT1 = '1';
	constexpr char DIRT2 = '2';
	constexpr char DIRT3 = '3';
	constexpr char DIRT4 = '4';
	constexpr char DIRT5 = '5';
	constexpr char DIRT6 = '6';
	constexpr char DIRT7 = '7';
	constexpr char DIRT8 = '8';
	constexpr char DIRT9 = '9';

	inline bool is_wall(char ch) {
		return ch == WALL || ch == NONE;
	}

	inline bool is_clean(char ch) {
		return ch == DIRT0 || ch == CHARGER;
	}

	inline int get_dirt_level(char ch) {
		if (is_clean(ch)) return 0;
		switch (ch) {
			case DIRT1: return 1;
			case DIRT2: return 2;
			case DIRT3: return 3;
			case DIRT4: return 4;
			case DIRT5: return 5;
			case DIRT6: return 6;
			case DIRT7: return 7;
			case DIRT8: return 8;
			case DIRT9: return 9;
			default: return -1;  // Todo: what to do in case of error?
		}
	}

	inline bool is_valid(char ch) {
		return ch == NONE
			|| ch == WALL
			|| ch == CHARGER
			|| ch == ROBOTSYM
			|| ch == DIRT0
			|| ch == DIRT1
			|| ch == DIRT2
			|| ch == DIRT3
			|| ch == DIRT4
			|| ch == DIRT5
			|| ch == DIRT6
			|| ch == DIRT7
			|| ch == DIRT8
			|| ch == DIRT9;
	}

	inline void decrement_dirt(char& ch) {
		switch (ch) {
			case DIRT9: ch = DIRT8;
			case DIRT8: ch = DIRT7;
			case DIRT7: ch = DIRT6;
			case DIRT6: ch = DIRT5;
			case DIRT5: ch = DIRT4;
			case DIRT4: ch = DIRT3;
			case DIRT3: ch = DIRT2;
			case DIRT2: ch = DIRT1;
			case DIRT1: ch = DIRT0;
		}
	}

	// Working copy, use if above doesn't wind up working.
	/*inline char decrement_dirt(char ch) {
		switch (ch) {
		case DIRT9: return DIRT8;
		case DIRT8: return DIRT7;
		case DIRT7: return DIRT6;
		case DIRT6: return DIRT5;
		case DIRT5: return DIRT4;
		case DIRT4: return DIRT3;
		case DIRT3: return DIRT2;
		case DIRT2: return DIRT1;
		case DIRT1: return DIRT0;
		default: return NONE;
		}
	}*/

}

// The graveyard:

//#define SYM_NONE ' '
//#define SYM_WALL '.'
//#define SYM_CHARGER '+'
//#define SYM_ROBOT 'x'
//#define SYM_DIRT0 '0'
//#define SYM_DIRT1 '1'
//#define SYM_DIRT2 = '2';
//#define SYM_DIRT3 '3'
//#define SYM_DIRT4 '4'
//#define SYM_DIRT5 '5'
//#define SYM_DIRT6 '6'
//#define SYM_DIRT7 '7'
//#define SYM_DIRT8 '8'
//#define SYM_DIRT9 '9'

//enum class Symbol : char {
//	NONE = ' ',
//	WALL = '.',
//	CHARGER = '+',
//	ROBOTSYM = 'x',
//	DIRT0 = '0',
//	DIRT1 = '1',
//	DIRT2 = '2',
//	DIRT3 = '3',
//	DIRT4 = '4',
//	DIRT5 = '5',
//	DIRT6 = '6',
//	DIRT7 = '7',
//	DIRT8 = '8',
//	DIRT9 = '9'
//};

//constexpr char sym_char(Symbol sym) {
//	switch (sym) {
//		case Symbol::NONE: return ' ';
//		case Symbol::WALL: return '.';
//		case Symbol::CHARGER: return '+';
//		case Symbol::ROBOTSYM: return 'x';
//		case Symbol::CHARGING: return '*';
//		case Symbol::DIRT0: return '0';
//		case Symbol::DIRT1: return '1';
//		case Symbol::DIRT2: return '2';
//		case Symbol::DIRT3: return '3';
//		case Symbol::DIRT4: return '4';
//		case Symbol::DIRT5: return '5';
//		case Symbol::DIRT6: return '6';
//		case Symbol::DIRT7: return '7';
//		case Symbol::DIRT8: return '8';
//		case Symbol::DIRT9: return '9';
//		default: return '?';
//	}
//}
//
//Symbol char_sym(char ch) {
//	switch (ch) {
//		case ' ': return Symbol::NONE;
//		case '.': return Symbol::WALL;
//		case '+': return Symbol::CHARGER;
//		...unfinished
//	}
//}

#endif
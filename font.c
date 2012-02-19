#include "font.h"
#include <strings.h>
#include <curses.h>

static char** font = NULL;

static void add_char(char character, char* line_0, char* line_1, char* line_2, char* line_3) {
  font[character*8+0] = line_0;
  font[character*8+1] = line_1;
  font[character*8+2] = line_2;
  font[character*8+3] = line_3;
}

static void init_font() {
  font = malloc(256*8*sizeof(char*));
  bzero(font, 256*8*sizeof(char*));

  /* Raw font

       _   _  _   _  _  __     ___                      _   _   _   _   __ ___                      __ 
  /\  |_) /  | \ |_ |_ /__ |_|  |    | |/ |  |\/| |\ | / \ |_) / \ |_) (_   | | | \  / \    / \/ \_/ / 
 /--\ |_) \_ |_/ |_ |  \_| | | _|_ \_| |\ |_ |  | | \| \_/ |   \_X | \ __)  | |_|  \/   \/\/  /\  | /_ 
                                                                                                       

                     _                                                                     
  _. |_   _  _|  _ _|_ _  |_  o o |  | ._ _  ._   _  ._   _. ._ _ _|_                   _  
 (_| |_) (_ (_| (/_ | (_| | | | | |< | | | | | | (_) |_) (_| | _>  |_ |_| \/ \/\/ >< \/ /_ 
                       _|      _|                    |     |                         /     


    _  _        _   _  __  _   _   _    __          _     _ _    
 /|  ) _) |_|_ |_  |_   / (_) (_| / \  /  \ -|-|- (|   / |   |   
  | /_ _)   |   _) |_) /  (_)   | \_/ | (|/ -|-|- _|) /  |_ _| o 
                                       \__                       

  */
  
  add_char('0', 
	   "    ",
	   " _  ",
	   "/ \\ ",
	   "\\_/ "
	   );
  
  add_char('1',
	   "   ",
	   "   ",
	   "/| ",
	   " | "
	   );

  add_char('2',
	   "   ",
	   "_  ",
	   " ) ",
	   "/_ "
	   );

  add_char('3',
	   "   ",
	   "_  ",
	   "_) ",
	   "_) "
	   );

  add_char('4',
	   "     ",
	   "     ",
	   "|_|_ ",
	   "  |  "
	   );

  add_char('5',
	   "    ",
	   " _  ",
	   "|_  ",
	   " _) "
	   );

  add_char('6',
	   "    ",
	   " _  ",
	   "|_  ",
	   "|_) "
	   );

  add_char('7',
	   "   ",
	   "__ ",
	   " / ",
	   "/  "
	   );

  add_char('8',
	   "    ",
	   " _  ",
	   "(_) ",
	   "(_) "
	   );

  add_char('9',
	   "    ",
	   " _  ",
	   "(_| ",
	   "  | "
	   );

  add_char(' ',
	   "   ",
	   "   ",
	   "   ",
	   "   ");

  add_char('.',
	   "  ",
	   "  ",
	   "  ",
	   "o ");

  add_char('A',
	   "     ",
	   "     ",
	   " /\\  ",
	   "/--\\ ");

  add_char('B',
	   "    ",
	   " _  ",
	   "|_) ",
	   "|_) ");

  add_char('C',
	   "   ",
	   " _ ",
	   "/  ",
	   "\\_ ");

  add_char('D',
	   "    ",
	   " _  ",
	   "| \\ ",
	   "|_/ ");

  add_char('E',
	   "   ",
	   " _ ",
	   "|_ ",
	   "|_ ");

  add_char('F',
	   "   ",
	   " _ ",
	   "|_ ",
	   "|  ");

  add_char('G',
	   "    ",
	   " __ ",
	   "/__ ",
	   "\\_| ");

  add_char('H',
	   "   ",
	   "   ",
	   "|_|",
	   "| |");

  add_char('I',
	   "    ",
	   "___ ",
	   " |  ",
	   "_|_ ");

  add_char('J',
	   "    ",
	   "    ",
	   "  | ",
	   "\\_| ");

  add_char('K',
	   "   ",
	   "   ",
	   "|/ ",
	   "|\\ ");

  add_char('L',
	   "   ",
	   "   ",
	   "|  ",
	   "|_ ");

  add_char('M',
	   "     ",
	   "     ",
	   "|\\/| ",
	   "|  | ");
	  
  add_char('N',
	   "     ",
	   "     ",
	   "|\\ | ",
	   "| \\| ");
  
  add_char('O',
	   "    ",
	   " _  ",
	   "/ \\ ",
	   "\\_/ ");

  add_char('P',
	   "    ",
	   " _ ",
	   "|_)",
	   "|  ");

  add_char('Q',
	   "    ",
	   " _  ",
	   "/ \\ ",
	   "\\_X ");

  add_char('R',
	   "    ",
	   " _  ",
	   "|_) ",
	   "| \\ ");

  add_char('S',
	   "    ",
	   " __ ",
	   "(_  ",
	   "__) ");

  add_char('T',
	   "   ",
	   "___",
	   " | ",
	   " | ");

  add_char('U',
	   "    ",
	   "    ",
	   "| | ",
	   "|_| ");

  add_char('V',
	   "     ",
	   "     ",
	   "\\  / ",
	   " \\/  ");

  add_char('W',
	   "       ",
	   "       ",
	   "\\    / ",
	   " \\/\\/  ");

  add_char('X',
	   "   ",
	   "   ",
	   "\\/ ",
	   "/\\ ");

  add_char('Y',
	   "   ",
	   "   ",
	   "\\_/",
	   " | ");

  add_char('Z',
	   "   ",
	   "__ ",
	   " / ",
	   "/_ ");

  add_char('a',
	   "    ",
	   " _. ",
	   "(_| ",
	   "    ");

  add_char('b',
	   "    ",
	   "|_  ",
	   "|_) ",
	   "    ");

  add_char('c',
	   "   ",
	   " _ ",
	   "(_ ",
	   "   ");

  add_char('d',
	   "    ",
	   " _| ",
	   "(_| ",
	   "    ");

  add_char('e',
	   "   ",
	   " _ ",
	   "(/_",
	   "   ");

  add_char('f',
	   "  _",
	   "_|_",
	   " | ",
	   "   ");

  add_char('g',
	   "    ",
	   " _  ",
	   "(_| ",
	   " _| ");

  add_char('h',
	   "    ",
	   "|_  ",
	   "| | ",
	   "    ");

  add_char('i',
	   " ",
	   "o",
	   "|",
	   " ");

  add_char('j',
	   "   ",
	   " o ",
	   " | ",
	   "_| ");

  add_char('k',
	   "   ",
	   "|  ",
	   "|< ",
	   "   ");

  add_char('l',
	   "  ",
	   "| ",
	   "| ",
	   "  ");

  add_char('m',
	   "      ",
	   "._ _  ",
	   "| | | ",
	   "      ");

  add_char('n',
	   "    ",
	   "._  ",
	   "| | ",
	   "    ");

  add_char('o',
	   "   ",
	   " _ ",
	   "(_)",
	   "   ");

  add_char('p',
	   "    ",
	   "._  ",
	   "|_) ",
	   "|   ");

  add_char('q',
	   "    ",
	   " _. ",
	   "(_| ",
	   "  | ");

  add_char('r',
	   "  ",
	   "._",
	   "| ",
	   "  ");

  add_char('s',
	   "   ",
	   " _ ",
	   "_> ",
	   "   ");

  add_char('t',
	   "    ",
	   "_|_ ",
	   " |_ ",
	   "    ");

  add_char('u',
	   "    ",
	   "    ",
	   "|_| ",
	   "    ");

  add_char('v',
	   "    ",
	   "    ",
	   " \\/ ",
	   "    ");

  add_char('w',
	   "     ",
	   "     ",
	   "\\/\\/ ",
	   "     ");

  add_char('x',
	   "   ",
	   "   ",
	   ">< ",
	   "   ");

  add_char('y',
	   "   ",
	   "   ",
	   "\\/ ",
	   "/  ");

  add_char('z',
	   "   ",
	   "_  ",
	   "/_ ",
	   "   ");
}

const char* font_line(char character, unsigned char line) {
  if(font == NULL) {
    init_font();
  }

  if(line<FONT_LINES) {
    char* result = font[character*8+line];
    if(result == NULL) {
      result = "#### ";
    }

    return result;
  }

  return NULL;
}

void print_string(char* to_print, char* prefix, char* suffix) {
  unsigned char line;
  char* tmp;

  for(line = 0; line < FONT_LINES; line++) {
    if(prefix) {
      addstr(prefix);
    }
    tmp = to_print;
    while(*tmp) {
      addstr(font_line(*tmp, line));
      tmp++;
    }
    if(suffix) {
      addstr(suffix);
    }
    addstr("\n");
  }
}

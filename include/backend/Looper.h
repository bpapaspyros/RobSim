#ifndef LOOPER_H
#define LOOPER_H

class Looper{
	public:
		void changemode(int dir);
		int kbhit();				// returns 0 if there is a character waiting in the buffer
};

#endif
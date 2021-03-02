#include "gui.h"
#include "log.h"

int main()
{
	while (true) {
		if (!program()) {
			system("CLS");
		}
	}

	return 0;
}
#include <stdio.h>
#include "openhmddialog.h"

int main() {
	struct openhmdindex hmdindex = test();
	printf("hmd index             : %d, tracker (%d): %d\n\
left controller index : %d, tracker (%d): %d\n\
right controller index: %d, tracker (%d): %d\n",
		hmdindex.hmd, hmdindex.separate_hmdtracker, hmdindex.hmdtracker,
		hmdindex.leftcontroller, hmdindex.separate_leftcontrollertracker, hmdindex.leftcontrollertracker,
		hmdindex.rightcontroller, hmdindex.separate_rightcontrollertracker, hmdindex.rightcontrollertracker
      	);
}

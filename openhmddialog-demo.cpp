#include <stdio.h>
#include "openhmddialog.h"

int main() {
	struct openhmdindex hmdindex = test();
	printf("\
hmd index             : %d, separate tracker: %s (%d)\n\
left controller index : %d, separate tracker: %s (%d)\n\
right controller index: %d, separate tracker %s (%d)\n",
		hmdindex.hmd, hmdindex.has_separate_hmdtracker ? "yes" : "no", hmdindex.hmdtracker,
		hmdindex.leftcontroller, hmdindex.has_separate_leftcontrollertracker ? "yes" : "no", hmdindex.leftcontrollertracker,
		hmdindex.rightcontroller, hmdindex.has_separate_rightcontrollertracker ? "yes" : "no", hmdindex.rightcontrollertracker
      	);
}

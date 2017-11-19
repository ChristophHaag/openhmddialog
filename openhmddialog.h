#ifndef OPENHMDDIALOG
#define OPENHMDDIALOG

struct openhmdindex {
	struct nkc* nkcHandle;
	int hmd;
	int has_separate_hmdtracker;
	int hmdtracker;
	int leftcontroller;
	int has_separate_leftcontrollertracker;
	int leftcontrollertracker;
	int rightcontroller;
	int has_separate_rightcontrollertracker;
	int rightcontrollertracker;
};
struct openhmdindex test();
#endif

#ifndef OPENHMDDIALOG
#define OPENHMDDIALOG

struct openhmdindex {
	struct nkc* nkcHandle;
	int hmd;
	int separate_hmdtracker;
	int hmdtracker;
	int leftcontroller;
	int separate_leftcontrollertracker;
	int leftcontrollertracker;
	int rightcontroller;
	int separate_rightcontrollertracker;
	int rightcontrollertracker;
};
struct openhmdindex test();
#endif

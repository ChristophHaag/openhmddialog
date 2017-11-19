#include <stdio.h>
#define NKC_IMPLEMENTATION
#include "nuklear_cross.h"
#include "openhmddialog.h"

#include <openhmd/openhmd.h>

int num_devices;
ohmd_context* ohmdctx;
const char* WINDOWNAME = "OpenHMD Device Chooser";
int totalwidth = 800;
int height = 800;
int i = 0;
void mainLoop(void* loopArg){
	struct openhmdindex* myapp = (struct openhmdindex*)loopArg;
	struct nk_context *ctx = nkc_get_ctx(myapp->nkcHandle);

	union nkc_event e = nkc_poll_events(myapp->nkcHandle);
	if( (e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT) ){
		nkc_stop_main_loop(myapp->nkcHandle);
	}

	/*
	 printf("device %d\n", i);
	 printf("  vendor:  %s\n", ohmd_list_gets(ohmdctx, i, OHMD_VENDOR));
	 printf("  product: %s\n", ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT));
	 printf("  path:    %s\n", ohmd_list_gets(ohmdctx, i, OHMD_PATH));
	 printf("  class:   %s\n", device_class_s[device_class > OHMD_DEVICE_CLASS_GENERIC_TRACKER ? 4 : device_class]);
	 printf("  flags:   %02x\n",  device_flags);
	 printf("    null device:         %s\n", device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE ? "yes" : "no");
	 printf("    rotational tracking: %s\n", device_flags & OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING ? "yes" : "no");
	 printf("    positional tracking: %s\n", device_flags & OHMD_DEVICE_FLAGS_POSITIONAL_TRACKING ? "yes" : "no");
	 printf("    left controller:     %s\n", device_flags & OHMD_DEVICE_FLAGS_LEFT_CONTROLLER ? "yes" : "no");
	 printf("    right controller:    %s\n\n", device_flags & OHMD_DEVICE_FLAGS_RIGHT_CONTROLLER ? "yes" : "no");
	 */

	int parts = 3;
	int buttonheight = 100;
	int chooserheight = height - buttonheight;
	int partheight = chooserheight / parts;
	int textheight = 17;

	/* Nuklear GUI code */
	int currentwidth = totalwidth /2 /*myapp->separate_hmdtracker ? totalwidth / 2 : totalwidth*/;
	if (nk_begin(ctx, "HMD", nk_rect(0, 0, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, textheight, 1);
		nk_checkbox_label(ctx, "Use different HMD tracker", &myapp->has_separate_hmdtracker);
		for(int i = 0; i < num_devices; i++){
			int device_class = 0, device_flags = 0;
			const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);

			//if (device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE) continue;
			if (device_flags & (OHMD_DEVICE_FLAGS_LEFT_CONTROLLER | OHMD_DEVICE_FLAGS_RIGHT_CONTROLLER)) continue;
			char entry[1000];
			sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
			if (nk_option_label(ctx, entry, myapp->hmd == i)) myapp->hmd = i;
		}
		nk_end(ctx);
	}

	if (true /*myapp->separate_hmdtracker*/) {
		if (nk_begin(ctx, "HMD Tracker", nk_rect(currentwidth, 0, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
			nk_layout_row_dynamic(ctx, textheight, 1);
			for(int i = 0; i < num_devices; i++){
				int device_class = 0, device_flags = 0;
				const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);

				//if (device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE) continue;
				if (!(device_flags & (OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING | OHMD_DEVICE_FLAGS_POSITIONAL_TRACKING))) continue;
				char entry[1000];
				sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
				if (nk_option_label(ctx, entry, myapp->hmdtracker == i)) myapp->hmdtracker = i;
			}
			nk_end(ctx);
		}
	}


	currentwidth = totalwidth /2 /*myapp->separate_leftcontrollertracker ? totalwidth / 2 : totalwidth*/;
	if (nk_begin(ctx, "Left Controller", nk_rect(0, chooserheight/parts, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, textheight, 1);
		nk_checkbox_label(ctx, "Use different left controller tracker", &myapp->has_separate_leftcontrollertracker);
		for(int i = 0; i < num_devices; i++){
			int device_class = 0, device_flags = 0;
			const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);
			if (!(device_flags & (OHMD_DEVICE_FLAGS_LEFT_CONTROLLER | OHMD_DEVICE_FLAGS_RIGHT_CONTROLLER))) continue;
			char entry[1000];
			sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
			if (nk_option_label(ctx, entry, myapp->leftcontroller == i)) myapp->leftcontroller= i;
		}
		if (nk_option_label(ctx, "None", myapp->leftcontroller == -1)) myapp->leftcontroller= -1;
		nk_end(ctx);
	}
	if (true /*myapp->separate_leftcontrollertracker*/) {
		if (nk_begin(ctx, "Left Controller Tracker", nk_rect(currentwidth, chooserheight/parts, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
			nk_layout_row_dynamic(ctx, textheight, 1);
			for(int i = 0; i < num_devices; i++){
				int device_class = 0, device_flags = 0;
				const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);

				//if (device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE) continue;
				if (!(device_flags & (OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING | OHMD_DEVICE_FLAGS_POSITIONAL_TRACKING))) continue;
				char entry[1000];
				sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
				if (nk_option_label(ctx, entry, myapp->leftcontrollertracker == i)) myapp->leftcontrollertracker = i;
			}
			nk_end(ctx);
		}
	}

	currentwidth = totalwidth /2 /*myapp->separate_rightcontrollertracker ? totalwidth / 2 : totalwidth*/;
	if (nk_begin(ctx, "Right Controller", nk_rect(0, 2* chooserheight/parts, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, textheight, 1);
		nk_checkbox_label(ctx, "Use different right controller tracker", &myapp->has_separate_rightcontrollertracker);
		for(int i = 0; i < num_devices; i++){
			int device_class = 0, device_flags = 0;
			const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);
			if (!(device_flags & (OHMD_DEVICE_FLAGS_LEFT_CONTROLLER | OHMD_DEVICE_FLAGS_RIGHT_CONTROLLER))) continue;
			char entry[1000];
			sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
			if (nk_option_label(ctx, entry, myapp->rightcontroller == i)) myapp->rightcontroller = i;
		}
		if (nk_option_label(ctx, "None", myapp->rightcontroller == -1)) myapp->rightcontroller= -1;
		nk_end(ctx);
	}
	if (true /*myapp->separate_rightcontrollertracker*/) {
		if (nk_begin(ctx, "Right Controller Tracker", nk_rect(currentwidth, 2* chooserheight/parts, currentwidth, partheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
			nk_layout_row_dynamic(ctx, textheight, 1);
			for(int i = 0; i < num_devices; i++){
				int device_class = 0, device_flags = 0;
				const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
				ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);

				//if (device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE) continue;
				if (!(device_flags & (OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING | OHMD_DEVICE_FLAGS_POSITIONAL_TRACKING))) continue;
				char entry[1000];
				sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
				if (nk_option_label(ctx, entry, myapp->rightcontrollertracker == i)) myapp->rightcontrollertracker = i;
			}
			nk_end(ctx);
		}
	}

	if (nk_begin(ctx, "Action", nk_rect(0, height - buttonheight, totalwidth, buttonheight), NK_WINDOW_TITLE | NK_WINDOW_BORDER)) {
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "Save")) {
			/* event handling */
			//printf("Button pressed\n");
			nkc_stop_main_loop(myapp->nkcHandle);
		}
		nk_end(ctx);
	}
	/* End Nuklear GUI */

	nkc_render(myapp->nkcHandle, nk_rgb(40,40,40) );
}

struct openhmdindex test() {
	struct openhmdindex ret;
	ret.hmd = 0;
	ret.hmdtracker = -1;
	ret.has_separate_hmdtracker = nk_false;
	ret.leftcontroller = 1;
	ret.rightcontroller = 2;
	ret.has_separate_leftcontrollertracker = nk_false;
	ret.has_separate_rightcontrollertracker = nk_false;
	ret.leftcontrollertracker = -1;
	ret.rightcontrollertracker = -1;

	ohmdctx = ohmd_ctx_create();

	// Probe for devices
	num_devices = ohmd_ctx_probe(ohmdctx);
	if(num_devices < 0){
		printf("failed to probe devices: %s\n", ohmd_ctx_get_error(ohmdctx));
		return ret;
	}

	//printf("num devices: %d\n\n", num_devices);

	struct nkc nkcx; /* Allocate memory for Nuklear+ handle */
	ret.nkcHandle = &nkcx;
	/* init some user data */
	ret.hmd = 0;

	if( nkc_init( ret.nkcHandle, WINDOWNAME, totalwidth, height, NKC_WIN_NORMAL ) ){
		//printf("Successfull init. Starting 'infinite' main loop...\n");
		nkc_set_main_loop(ret.nkcHandle, mainLoop, (void*)&ret );
	} else {
		printf("Can't init NKC\n");
	}
	//printf("Value after quit = %d\n", myapp.value);
	nkc_shutdown( ret.nkcHandle );

	return ret;
}

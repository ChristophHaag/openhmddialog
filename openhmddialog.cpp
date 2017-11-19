#include <stdio.h>
#define NKC_IMPLEMENTATION
#include "nuklear_cross.h"
#include <openhmd/openhmd.h>
int num_devices;
ohmd_context* ohmdctx;
const char* WINDOWNAME = "OpenHMD Device Chooser";
bool quit = false;
enum radioOptions {
	EASY,
	HARD
};

struct my_nkc_app {
	struct nkc* nkcHandle;

	/* some user data */
	int value;
	int dev;
};

void mainLoop(void* loopArg){
	struct my_nkc_app* myapp = (struct my_nkc_app*)loopArg;
	struct nk_context *ctx = nkc_get_ctx(myapp->nkcHandle);

	union nkc_event e = nkc_poll_events(myapp->nkcHandle);
	if( (e.type == NKC_EWINDOW) && (e.window.param == NKC_EQUIT) ){
		nkc_stop_main_loop(myapp->nkcHandle);
	}

	/* Nuklear GUI code */
	if (nk_begin(ctx, "Show", nk_rect(0, 0, 800, 600), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
		nk_layout_row_dynamic(ctx, 30, 1);
		for(int i = 0; i < num_devices; i++){
			int device_class = 0, device_flags = 0;
			const char* device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_CLASS, &device_class);
			ohmd_list_geti(ohmdctx, i, OHMD_DEVICE_FLAGS, &device_flags);

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

			//if (device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE) continue;
			char entry[1000];
			sprintf(entry, "%d: %s %s %s", i, ohmd_list_gets(ohmdctx, i, OHMD_VENDOR), ohmd_list_gets(ohmdctx, i, OHMD_PRODUCT), ohmd_list_gets(ohmdctx, i, OHMD_PATH));
			if (nk_option_label(ctx, entry, myapp->dev == i)) myapp->dev = i;
		}


		/* fixed widget window ratio width */
		//if (nk_option_label(ctx, "easy", myapp->op == EASY)) myapp->op = EASY;
		//if (nk_option_label(ctx, "hard", myapp->op == HARD)) myapp->op = HARD;

		/* fixed widget pixel width */
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "Choose")) {
			/* event handling */
// 			//printf("Button pressed\n");
			myapp->value = myapp->dev;
			nkc_stop_main_loop(myapp->nkcHandle);
		}
	}
	nk_end(ctx);
	/* End Nuklear GUI */

	nkc_render(myapp->nkcHandle, nk_rgb(40,40,40) );

}

int test() {

	ohmdctx = ohmd_ctx_create();

	// Probe for devices
	num_devices = ohmd_ctx_probe(ohmdctx);
	if(num_devices < 0){
		printf("failed to probe devices: %s\n", ohmd_ctx_get_error(ohmdctx));
		return -1;
	}

	//printf("num devices: %d\n\n", num_devices);

	struct my_nkc_app myapp;
	struct nkc nkcx; /* Allocate memory for Nuklear+ handle */
	myapp.nkcHandle = &nkcx;
	/* init some user data */
	myapp.value = 0;
	myapp.dev = 0;

	if( nkc_init( myapp.nkcHandle, WINDOWNAME, 640, 480, NKC_WIN_NORMAL ) ){
		//printf("Successfull init. Starting 'infinite' main loop...\n");
		nkc_set_main_loop(myapp.nkcHandle, mainLoop, (void*)&myapp );
	} else {
		printf("Can't init NKC\n");
	}
	//printf("Value after quit = %d\n", myapp.value);
	nkc_shutdown( myapp.nkcHandle );
	return myapp.value;
}

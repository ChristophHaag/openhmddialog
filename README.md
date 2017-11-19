Library that can be integrated into applications to display a dialog for choosing OpenHMD devices.

Uses [nuklear_cross](https://github.com/DeXP/nuklear_cross) for the GUI.

Usage:

Build openhmddialog. Then link to

    openhmddialog

Currently there is only `int test()` that returns the HMD index.

TODO:

Separate HMDs, Trackers and Controllers and allow choosing a sensible combination.

Define a format for returning the configuration.

Save the default in a config file.

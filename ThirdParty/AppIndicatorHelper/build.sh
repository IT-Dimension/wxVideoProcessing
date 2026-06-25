#!/bin/sh

$(wx-config --cxx --cppflags --libs) \
	-I/usr/lib/x86_64-linux-gnu/gtk-2.0/include \
	-I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
	-I/usr/include/libappindicator3-0.1 \
	-I/usr/include/gtk-3.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/cairo \
	-I/usr/include/pango-1.0 \
	-I/usr/lib/gtk-2.0/include \
	-I/usr/include/gdk-pixbuf-2.0 \
	-I/usr/include/atk-1.0 \
	-lappindicator3 \
	-lgobject-2.0 \
	-lgtk-x11-2.0 \
	appindic.cpp AIHTest.cpp -o aihtest `wx-config --libs` -lappindicator3 -lgtk-3 -lgobject-2.0 -lgtk-x11-2.0  -Wno-deprecated-declarations

exit 0

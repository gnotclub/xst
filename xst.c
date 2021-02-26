/* an attempt to keep some xst-only stuff out of the codebase */

int cursorblinkstate = 0;

#define XRESOURCE_LOAD_META(NAME)					\
	if(!XrmGetResource(xrdb, "st." NAME, "st." NAME, &type, &ret))	\
		XrmGetResource(xrdb, "*." NAME, "*." NAME, &type, &ret); \
	if (ret.addr != NULL && !strncmp("String", type, 64))

#define XRESOURCE_LOAD_STRING(NAME, DST)	\
	XRESOURCE_LOAD_META(NAME)		\
		DST = ret.addr;

#define XRESOURCE_LOAD_CHAR(NAME, DST)		\
	XRESOURCE_LOAD_META(NAME)		\
		DST = ret.addr[0];

#define XRESOURCE_LOAD_INTEGER(NAME, DST)		\
	XRESOURCE_LOAD_META(NAME)			\
		DST = strtoul(ret.addr, NULL, 10);

#define XRESOURCE_LOAD_FLOAT(NAME, DST)		\
	XRESOURCE_LOAD_META(NAME)		\
		DST = strtof(ret.addr, NULL);

void
xrdb_load(void)
{
	/* XXX */
	char *xrm;
	char *type;
	XrmDatabase xrdb;
	XrmValue ret;
	Display *dpy;

	if(!(dpy = XOpenDisplay(NULL)))
		die("Can't open display\n");

	XrmInitialize();
	xrm = XResourceManagerString(dpy);

	if (xrm != NULL) {
		xrdb = XrmGetStringDatabase(xrm);

		/* handling colors here without macros to do via loop. */
		int i = 0;
		/* this value is also used for keybinds  */
		char loadValue[100] = "";
		for (i = 0; i < 256; i++)
		{
			sprintf(loadValue, "%s%d", "st.color", i);

			if(!XrmGetResource(xrdb, loadValue, loadValue, &type, &ret))
			{
				sprintf(loadValue, "%s%d", "*.color", i);
				if (!XrmGetResource(xrdb, loadValue, loadValue, &type, &ret))
					/* reset if not found (unless in range for defaults). */
					if (i > 15)
						colorname[i] = NULL;
			}

			if (ret.addr != NULL && !strncmp("String", type, 64))
				colorname[i] = ret.addr;
		}

		XRESOURCE_LOAD_STRING("foreground", colorname[defaultfg]);
		XRESOURCE_LOAD_STRING("background", colorname[defaultbg]);
		XRESOURCE_LOAD_STRING("cursorfg", colorname[defaultcs])
		else {
		  // this looks confusing because we are chaining off of the if
		  // in the macro. probably we should be wrapping everything blocks
		  // so this isn't possible...
		  defaultcs = defaultfg;
		}
		XRESOURCE_LOAD_STRING("cursorbg", colorname[defaultrcs])
		else {
		  // see above.
		  defaultrcs = defaultbg;
		}

		XRESOURCE_LOAD_STRING("font", font);
		XRESOURCE_LOAD_STRING("termname", termname);

		XRESOURCE_LOAD_INTEGER("blinktimeout", blinktimeout);
		XRESOURCE_LOAD_INTEGER("bellvolume", bellvolume);
		XRESOURCE_LOAD_INTEGER("disablebold", disablebold);
		XRESOURCE_LOAD_INTEGER("disableitalic", disableitalic);
		XRESOURCE_LOAD_INTEGER("disableroman", disableroman);
		XRESOURCE_LOAD_INTEGER("borderpx", borderpx);
		XRESOURCE_LOAD_INTEGER("borderless", borderless);
		XRESOURCE_LOAD_INTEGER("cursorshape", cursorshape);
		XRESOURCE_LOAD_INTEGER("disable_alpha_correction", disable_alpha_correction);

		cursorblinkstate = 1; // in case if cursor shape was changed from a blinking one to a non-blinking
		XRESOURCE_LOAD_INTEGER("cursorthickness", cursorthickness);
		XRESOURCE_LOAD_INTEGER("cursorblinkstyle", cursorblinkstyle);
		XRESOURCE_LOAD_INTEGER("cursorblinkontype", cursorblinkontype);

		XRESOURCE_LOAD_INTEGER("scrollrate", scrollrate);

		XRESOURCE_LOAD_FLOAT("cwscale", cwscale);
		XRESOURCE_LOAD_FLOAT("chscale", chscale);
		XRESOURCE_LOAD_FLOAT("opacity", alpha);

		XRESOURCE_LOAD_INTEGER("boxdraw", boxdraw);
		XRESOURCE_LOAD_INTEGER("boxdraw_bold", boxdraw_bold);
		XRESOURCE_LOAD_INTEGER("boxdraw_braille", boxdraw_braille);

		XRESOURCE_LOAD_INTEGER("depth", opt_depth);

		/*
		  st.bind_alt_{a-z}
		  st.bind_shift_{a-z}
		  st.bind_shift_alt_{a-z}
		  st.bind_ctrl_alt_{a-z}
		  st.bind_ctrl_shift_{a-z}
		  all with optional suffix `_withcontent`, for
		 */

		const char* bind_types[5] = {
			"alt",
			"shift",
			"shift_alt",
			"ctrl_alt",
			"ctrl_shift",
		};

		const int bind_masks[5] = {
			Mod1Mask,
			ShiftMask,
			(Mod1Mask|ShiftMask),
			(ControlMask|Mod1Mask),
			(ControlMask|ShiftMask),
		};


		/* int i = 0; */
		int xres_shortcut_index = 0;
		int j = 0;
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 26; j++)
			{
				char letter = 'a' + j;
				sprintf(loadValue, "st.kb_%s_%c", bind_types[i], letter);
				/* printf("checking: %s\n", loadValue); */

				if(XrmGetResource(xrdb, loadValue, loadValue, &type, &ret))
					{
						printf("found shortcut: %s\n", loadValue);
						printf("value: %s\n", ret.addr);

						/* char *command[5] = { "/bin/sh", "-c", ret.addr, "externalpipe", NULL }; */

						/* xres_shortcuts_commands[xres_shortcut_index] = command; */
							/* { "/bin/sh", "-c", ret.addr, "externalpipe", NULL }; */
						/* Arg bindarg = {.v = command}; */
						/* Arg bindarg = {.v = xres_shortcuts_commands[xres_shortcut_index]}; */

						/* Shortcut bind; */
						Shortcut bind = {
							.mod = bind_masks[i],
							.keysym = letter,
							.func = externalpipe,
							/* .arg = bindarg, */
							.arg = {.v = (char *[]){ "/bin/sh", "-c", ret.addr, "externalpipe", NULL }}
							/* .arg = {.v = command}, */
						};

						xres_shortcuts[xres_shortcut_index++] = bind;
					}
			}
		}
	}
	XFlush(dpy);
}

void
reload(int sig)
{
	signal(SIGUSR1, reload);

	if (sig == -1) {
		return;
	}

	xrdb_load();

	/* colors, fonts */
	xloadcols();
	xunloadfonts();
	xloadfonts(getusedfont(), 0);
	xsetcursor(cursorshape);

	/* pretend the window just got resized */
	cresize(win.w, win.h);
	redraw();
	/* triggers re-render if we're visible. */
	ttywrite("\033[O", 3, 1);
}

enum motif_wm {
	MWM_HINTS_FUNCTIONS	= (1L << 0),
	MWM_HINTS_DECORATIONS	= (1L << 1),
	MWM_FUNC_ALL		= (1L << 0),
	MWM_FUNC_RESIZE		= (1L << 1),
	MWM_FUNC_MOVE		= (1L << 2),
	MWM_FUNC_MINIMIZE	= (1L << 3),
	MWM_FUNC_MAXIMIZE	= (1L << 4),
	MMW_FUNC_CLOSE		= (1L << 5),
};

/* Motif Window hints */
typedef struct {
	unsigned long	flags;
	unsigned long	functions;
	unsigned long	decorations;
	unsigned long	status;
	long		input_mode;
} Mwm_hints;

void
removewindecorations(void)
{
	Mwm_hints	hints;
	Atom		mwm_hints_property;

	mwm_hints_property = XInternAtom(xw.dpy, "_MOTIF_WM_HINTS", 0);
	hints.flags = MWM_HINTS_DECORATIONS;
	hints.decorations = 0;
	XChangeProperty(xw.dpy, xw.win, mwm_hints_property, mwm_hints_property, 32, PropModeReplace,
			(uchar *)&hints, 5);
}


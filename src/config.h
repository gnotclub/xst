// ref: http://freedesktop.org/software/fontconfig/fontconfig-user.html
static char *font = "Liberation Mono:pixelsize=12:antialias=true:autohint=true;";

// exec precedence: -e arg, utmp option, SHELL env var, /etc/passwd shell, config.h value.
// (we override with xresources on start)
static char *shell = "/bin/sh";

// identification sequence returned in DA and DECID
static char vtiden[] = "\033[?6c";

// Kerning / character bounding-box multipliers
static float cwscale = 1.0;
static float chscale = 1.0;

// work delimter strings. more advanced example : " `'\"()[]{}"
static char worddelimiters[] = " ";

// alt screens
static int allowaltscreen = 1;

// blinking timeout for terminal blinking (0 disables)
static unsigned int blinktimeout = 800;

// thickness of underline and bar cursors
static unsigned int cursorthickness = 2;

// bell volume. Value between -100 and 100. (0 disables)
static int bellvolume = 100;

// other
static unsigned int doubleclicktimeout = 300;
static unsigned int tripleclicktimeout = 600;
static char *utmp = NULL;
static int borderpx = 10;
static int bold_font = 0;
static char stty_args[] = "stty raw pass8 nl -echo -iexten -cstopb 38400";
static unsigned int xfps = 120;
static unsigned int actionfps = 30;
static char *termname = "xterm-256color";
static unsigned int tabspaces = 4;
static const char *colorname[] = {
	"#1e1e1e",
	"#5f5a60",
	"#cf6a4c",
	"#cf6a4c",
	"#8f9d6a",
	"#8f9d6a",
	"#f9ee98",
	"#f9ee98",
	"#7587a6",
	"#7587a6",
	"#9b859d",
	"#9b859d",
	"#afc4db",
	"#afc4db",
	"#a7a7a7",
	"#ffffff",

	[255] = 0,

	"#a7a7a7",
	"#171717",
};

// fg, bg, cursor, reverse cursor (references colorname indexes)
static unsigned int defaultfg = 256;
static unsigned int defaultbg = 257;
static unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

// 2 4 6 7: █ _ | ☃
static unsigned int cursorshape = 2;

// mouse (again colors reference colorname indexes)
static unsigned int mouseshape = XC_xterm;
static unsigned int mousefg = 7;
static unsigned int mousebg = 0;

// Colors used when the specific fg == defaultfg.
static unsigned int defaultitalic = 11;
static unsigned int defaultunderline = 7;


// Internal mouse shortcuts.
// Beware that overloading Button1 will disable the selection.
static MouseShortcut mshortcuts[] = {
	/* button               mask            string */
	{ Button4,              XK_ANY_MOD,     "\031" },
	{ Button5,              XK_ANY_MOD,     "\005" },
};

// Internal keyboard shortcuts.
#define MODKEY Mod1Mask

static Shortcut shortcuts[] = {
	/* mask                 keysym          function        argument */
	{ XK_ANY_MOD,           XK_Break,       sendbreak,      {.i =  0} },
	{ ControlMask,          XK_Print,       toggleprinter,  {.i =  0} },
	{ ShiftMask,            XK_Print,       printscreen,    {.i =  0} },
	{ XK_ANY_MOD,           XK_Print,       printsel,       {.i =  0} },
	{ MODKEY|ShiftMask,     XK_Prior,       xzoom,          {.f = +1} },
	{ MODKEY|ShiftMask,     XK_Next,        xzoom,          {.f = -1} },
	{ MODKEY|ShiftMask,     XK_Home,        xzoomreset,     {.f =  0} },
	{ ShiftMask,            XK_Insert,      selpaste,       {.i =  0} },
	{ MODKEY|ShiftMask,     XK_Insert,      clippaste,      {.i =  0} },
	{ MODKEY|ShiftMask,     XK_C,           clipcopy,       {.i =  0} },
	{ MODKEY|ShiftMask,     XK_V,           clippaste,      {.i =  0} },
	{ MODKEY,               XK_Num_Lock,    numlock,        {.i =  0} },
};

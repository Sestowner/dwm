/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of MODKEY */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;             /* 0 means no systray */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const char *fonts[] = {"monospace:size=10"};
static const char dmenufont[] = "monospace:size=10";
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";
static const char col_yellow[]		= "#b58900";

static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { s_base0, s_base03, s_base01 },      /* SchemeNorm dark */
	[SchemeSel] = { s_base0, s_base02, col_yellow },      /* SchemeSel dark */
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class    instance    title    tags mask    isfloating    monitor */
	{NULL, NULL, "Picture in picture", ~0, 1, -1},
};

/* layout(s) */
static const float mfact = 0.50;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;     /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[]=", tile}, /* first entry is default */
	{"><>", NULL},
	{"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                      \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                     \
	{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
	{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
	{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define CMD(cmd)                                             \
	{                                                        \
	    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", s_base02, "-nf", s_base0, "-sb", s_base02, "-sf", s_base2, NULL};
static const char *termcmd[] = {"st", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{Mod1Mask, XK_F2, spawn, {.v = dmenucmd}},
	{Mod1Mask | ControlMask, XK_t, spawn, {.v = termcmd}},
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},
	{MODKEY, XK_Up, incnmaster, {.i = +1}},
	{MODKEY, XK_Down, incnmaster, {.i = -1}},
	{MODKEY, XK_Left, setmfact, {.f = -0.05}},
	{MODKEY, XK_Right, setmfact, {.f = +0.05}},
	{MODKEY, XK_Return, zoom, {0}},
	{Mod1Mask, XK_Tab, view, {0}},
	{Mod1Mask, XK_F4, killclient, {0}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_space, setlayout, {0}},
	{MODKEY, XK_x, togglefloating, {0}},
	{MODKEY, XK_z, resizemouse, {0}},
	{Mod1Mask | ControlMask, XK_Down, view, {.ui = ~0}},
	{MODKEY, XK_0, tag, {.ui = ~0}},
	{Mod1Mask, XK_comma, focusmon, {.i = -1}},
	{Mod1Mask, XK_period, focusmon, {.i = +1}},
	{Mod1Mask | ControlMask, XK_comma, tagmon, {.i = -1}},
	{Mod1Mask | ControlMask, XK_period, tagmon, {.i = +1}},
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	{MODKEY | ControlMask, XK_q, quit, {0}},
	{MODKEY, XK_BackSpace, quit, {1}},
	{MODKEY | ControlMask, XK_t, spawn, CMD("synclient TouchpadOff=$(synclient -l | grep -c 'TouchpadOff.*=.*0')")},
	{ControlMask | ShiftMask, XK_Escape, spawn, CMD("xkill")},
	{0, XF86XK_MonBrightnessUp, spawn, CMD("brightnessctl s +1%")},
	{0, XF86XK_MonBrightnessDown, spawn, CMD("brightnessctl s 1%-")},
	{0, XF86XK_AudioRaiseVolume, spawn, CMD("pactl set-sink-volume 0 +1%")},
	{0, XF86XK_AudioLowerVolume, spawn, CMD("pactl set-sink-volume 0 -1%")},
	{0, XF86XK_AudioMute, spawn, CMD("pactl set-sink-mute 0 toggle")},
	{0, XF86XK_AudioPrev, spawn, CMD("mpctl prev")},
	{0, XF86XK_AudioPlay, spawn, CMD("mpctl toggle")},
	{0, XF86XK_AudioNext, spawn, CMD("mpctl next")},
	{Mod1Mask | ControlMask, XK_l, spawn, CMD("slock xset dpms force off")},
	{0, XK_Print, spawn, CMD("imshot")},
	{ShiftMask, XK_Print, spawn, CMD("imshot area")},
	{Mod1Mask, XK_Print, spawn, CMD("imshot window")},
	{0, XF86XK_PowerOff, spawn, CMD("dpower")},
	{MODKEY, XK_p, spawn, CMD("passmenu")},
	{MODKEY, XK_q, spawn, CMD("qrscanner")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click    event mask    button    function    argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, Mod1Mask, Button1, tag, {0}},
	{ClkTagBar, Mod1Mask, Button3, toggletag, {0}},
};

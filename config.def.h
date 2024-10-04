/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "UbuntuMono NerdFont Mono:size=15" };
static const char dmenufont[]       = "monospace:size=20";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_focus[] = "#8ab9e0"; // alternatives #b58900, #8ab9e0
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_focus },
};

/* tagging */
static const char *tags[] = { "n", "e", "i", "o" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor float-x,y,w,h floatborderpx CenterThisWindow?*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,     -1,-1,-1,-1,  -1,           0},
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	// {
	// 	NULL, NULL, "panda-trains", // class, instance, title
	// 	0, 1, -1, // tags mask, isfloating, monitor
	// 	-1, -1, -1, -1, -1, // float-x,y,w,h; floatborderpx
	// 	0, // CenterThisWindow?
	// },
	{
		"vlc", NULL, NULL, // class, instance, title
		0, 1, -1, // tags mask, isfloating, monitor
		-1, -1, -1, -1, -1, // float-x,y,w,h; floatborderpx
		0, // CenterThisWindow?
	},
	// {
	// 	"pstart", NULL, NULL, // class, instance, title
	// 	~0, 1, -1, // tags mask, isfloating, monitor
	// 	-1, -1, -1, -1, -1, // float-x,y,w,h; floatborderpx
	// 	0, // CenterThisWindow?
	// },
	{
		"neovide-flip-flop", NULL, NULL, // class, instance, title
		0, 1, -1, // tags mask, isfloating, monitor
		5, 5, 3830, 2140, 5, // float-x,y,w,h; floatborderpx
		0, // CenterThisWindow?
	},
	// doesnt quite work as nicely as I hoped
	// {
	// 	"Google-chrome", NULL, NULL, // class, instance, title
	// 	0, 0, -1, // tags mask, isfloating, monitor
	// 	-1, -1, -1, -1, -1, // float-x,y,w,h; floatborderpx
	// 	1, // CenterThisWindow?
	// }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	// { "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-i", "-l", "50", "-p", ">", NULL };
static const char *pstartsubcmd[] = { "pstart", "sub", NULL };
static const char *pstartallcmd[] = { "pstart", "all", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *lockcmd[] = { "standby-lock", NULL };
// static const char *sleepcmd[] = { "st", "-c", "pstart", "-t", "pstart", "-g", "110x35+1000+100", "-e", "go-sleep", NULL };
static const char *sleepcmd[] = { "go-sleep", NULL };
// static const char *hibernatecmd[] = { "st", "-c", "pstart", "-t", "pstart", "-g", "110x35+1000+100", "-e", "go-hibernate", NULL };
static const char *hibernatecmd[] = { "go-hibernate", NULL };
static const char *spotifyPreviousCmd[] = { "dbus-send", "--type=method_call", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Previous", NULL };
static const char *spotifyPlayPauseCmd[] = { "dbus-send", "--type=method_call", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.PlayPause", NULL };
static const char *spotifyNextCmd[] = { "dbus-send", "--type=method_call", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Next", NULL };
//static const char *pavuDownCmd[] = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
//static const char *pavuUpCmd[] = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
//static const char *pavuMuteToggleCmd[] = { "/usr/bin/pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *pavuMuteCmd[] = { "/usr/bin/pactl", "set-sink-mute", "@DEFAULT_SINK@", "1", NULL };
static const char *pavuUnmuteCmd[] = { "/usr/bin/pactl", "set-sink-mute", "@DEFAULT_SINK@", "0", NULL };
//static const char *useSpeakersCmd[] = { "use-speakers", NULL };
//static const char *useHeadphonesCmd[] = { "use-headphones", NULL };
//static const char *useBoseCmd[] = { "use-bose", NULL };
static const char *chromeCmd[] = { "chrome", NULL };
static const char *ichromeCmd[] = { "ichrome", NULL };
static const char *firefoxCmd[] = { "zen", NULL };
static const char *toggleRedshiftCmd[] = { "toggle-redshift", NULL };

void
self_restart(const Arg *arg)
{
	execlp("dwm", "dwm", (char *) NULL);
}

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,      spawn,          {.v = pstartsubcmd } },
	{ MODKEY|ShiftMask,             XK_space,      spawn,          {.v = pstartallcmd } },
	{ MODKEY,             XK_u, spawn,          {.v = termcmd } },
	// { MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_h,      focusstack,     {.i = +1 } },
	// { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	// { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	// { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	// { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	// { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_comma, zoom,           {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_k,    view,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_k,      killclient,     {0} },
	// { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	// { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_v,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_d,  togglefloating, {0} },
	// { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	// { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	// { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_n,                      0)
	TAGKEYS(                        XK_e,                      1)
	TAGKEYS(                        XK_i,                      2)
	TAGKEYS(                        XK_o,                      3)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	// { MODKEY|ShiftMask, XK_l, spawn, {.v = lockcmd} },
	{ 0, XK_F19, spawn, {.v = lockcmd} },
	{ MODKEY, XK_l, spawn, {.v = lockcmd} },
	{ MODKEY, XK_F19, spawn, {.v = sleepcmd} },
	{ MODKEY|ShiftMask, XK_F19, spawn, {.v = hibernatecmd} },
	// for kmonad configs (f19 doesnt work there)
	// { MODKEY, XK_End, spawn, {.v = lockcmd} },
	// { MODKEY, XK_Home, spawn, {.v = sleepcmd} },
	// { MODKEY, XK_Page_Down, spawn, {.v = hibernatecmd} },
	{ MODKEY, XK_r, spawn, {.v = spotifyPreviousCmd} },
	{ MODKEY, XK_s, spawn, {.v = spotifyPlayPauseCmd} },
	{ MODKEY, XK_t, spawn, {.v = spotifyNextCmd} },
	{ MODKEY, XK_f, spawn, {.v = pavuMuteCmd} },
	{ MODKEY, XK_p, spawn, {.v = pavuUnmuteCmd} },
	//{ MODKEY, XK_F16, spawn, {.v = useSpeakersCmd} },
	//{ MODKEY, XK_F17, spawn, {.v = useHeadphonesCmd} },
	//{ MODKEY, XK_F18, spawn, {.v = useBoseCmd} },
	{ MODKEY, XK_y, spawn, {.v = chromeCmd} },
	{ MODKEY|ShiftMask|ControlMask, XK_y, spawn, {.v = ichromeCmd} },
	{ MODKEY|ShiftMask, XK_y, spawn, {.v = firefoxCmd} },
	// { MODKEY, XK_e, killclient, {0} },
	{ MODKEY, XK_F1, spawn, {.v = toggleRedshiftCmd} },
	{ MODKEY|ControlMask, XK_q, self_restart, {0} },
	// { MODKEY, XK_n, togglemousefocusonly, {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


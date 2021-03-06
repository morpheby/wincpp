/*
 * vssym32.h
 *
 *  VisualStyle symbols definitions from
 *  WinSDK
 *
 *  Created on: 17.05.2012
 *  Copyright: broken Microsoft's one
 */

//-------------------------------------------------------------------------//
//  VisualStyle Core Win32/ComCtl32 symbols
//-------------------------------------------------------------------------//

#ifndef VSSYM32_H_
#define VSSYM32_H_


#include "VSStyle.h"

//
//  enumerated property values
//

// resolve conflicts with uxtheme.h
#ifdef BT_IMAGEFILE
#undef BT_IMAGEFILE
#endif
#ifdef BT_BORDERFILL
#undef BT_BORDERFILL
#endif
#ifdef BT_NONE
#undef BT_NONE
#endif

enum BGTYPE {
	BT_IMAGEFILE = 0,
	BT_BORDERFILL = 1,
	BT_NONE = 2,
};


//
//  enumerated property values
//

enum IMAGELAYOUT {
	IL_VERTICAL = 0,
	IL_HORIZONTAL = 1,
};


//
//  enumerated property values
//

enum BORDERTYPE {
	BT_RECT = 0,
	BT_ROUNDRECT = 1,
	BT_ELLIPSE = 2,
};


//
//  enumerated property values
//

enum FILLTYPE {
	FT_SOLID = 0,
	FT_VERTGRADIENT = 1,
	FT_HORZGRADIENT = 2,
	FT_RADIALGRADIENT = 3,
	FT_TILEIMAGE = 4,
};


//
//  enumerated property values
//

enum SIZINGTYPE {
	ST_TRUESIZE = 0,
	ST_STRETCH = 1,
	ST_TILE = 2,
};


//
//  enumerated property values
//

enum HALIGN {
	HA_LEFT = 0,
	HA_CENTER = 1,
	HA_RIGHT = 2,
};


//
//  enumerated property values
//

enum CONTENTALIGNMENT {
	CA_LEFT = 0,
	CA_CENTER = 1,
	CA_RIGHT = 2,
};


//
//  enumerated property values
//

enum VALIGN {
	VA_TOP = 0,
	VA_CENTER = 1,
	VA_BOTTOM = 2,
};


//
//  enumerated property values
//

enum OFFSETTYPE {
	OT_TOPLEFT = 0,
	OT_TOPRIGHT = 1,
	OT_TOPMIDDLE = 2,
	OT_BOTTOMLEFT = 3,
	OT_BOTTOMRIGHT = 4,
	OT_BOTTOMMIDDLE = 5,
	OT_MIDDLELEFT = 6,
	OT_MIDDLERIGHT = 7,
	OT_LEFTOFCAPTION = 8,
	OT_RIGHTOFCAPTION = 9,
	OT_LEFTOFLASTBUTTON = 10,
	OT_RIGHTOFLASTBUTTON = 11,
	OT_ABOVELASTBUTTON = 12,
	OT_BELOWLASTBUTTON = 13,
};


//
//  enumerated property values
//

enum ICONEFFECT {
	ICE_NONE = 0,
	ICE_GLOW = 1,
	ICE_SHADOW = 2,
	ICE_PULSE = 3,
	ICE_ALPHA = 4,
};


//
//  enumerated property values
//

enum TEXTSHADOWTYPE {
	TST_NONE = 0,
	TST_SINGLE = 1,
	TST_CONTINUOUS = 2,
};


//
//  enumerated property values
//

enum GLYPHTYPE {
	GT_NONE = 0,
	GT_IMAGEGLYPH = 1,
	GT_FONTGLYPH = 2,
};


//
//  enumerated property values
//

enum IMAGESELECTTYPE {
	IST_NONE = 0,
	IST_SIZE = 1,
	IST_DPI = 2,
};


//
//  enumerated property values
//

enum TRUESIZESCALINGTYPE {
	TSST_NONE = 0,
	TSST_SIZE = 1,
	TSST_DPI = 2,
};


//
//  enumerated property values
//

enum GLYPHFONTSIZINGTYPE {
	GFST_NONE = 0,
	GFST_SIZE = 1,
	GFST_DPI = 2,
};

//
//  property symbol definitions
//

#define TMT_RESERVEDLOW	0
#define TMT_RESERVEDHIGH	7999

#define TMT_DIBDATA	2
#define TMT_GLYPHDIBDATA	8
#define TMT_ENUM	200
#define TMT_STRING	201
#define TMT_INT	202
#define TMT_BOOL	203
#define TMT_COLOR	204
#define TMT_MARGINS	205
#define TMT_FILENAME	206
#define TMT_SIZE	207
#define TMT_POSITION	208
#define TMT_RECT	209
#define TMT_FONT	210
#define TMT_INTLIST	211
#define TMT_HBITMAP	212
#define TMT_DISKSTREAM	213
#define TMT_STREAM	214
#define TMT_BITMAPREF	215
#define TMT_COLORSCHEMES	401
#define TMT_SIZES	402
#define TMT_CHARSET	403
#define TMT_NAME	600

#ifndef TMT_DISPLAYNAME
#define TMT_DISPLAYNAME	601
#endif

#ifndef TMT_TOOLTIP
#define TMT_TOOLTIP	602
#endif

#ifndef TMT_COMPANY
#define TMT_COMPANY	603
#endif

#ifndef TMT_AUTHOR
#define TMT_AUTHOR	604
#endif

#ifndef TMT_COPYRIGHT
#define TMT_COPYRIGHT	605
#endif

#ifndef TMT_URL
#define TMT_URL	606
#endif

#ifndef TMT_VERSION
#define TMT_VERSION	607
#endif

#ifndef TMT_DESCRIPTION
#define TMT_DESCRIPTION	608
#endif

#define TMT_FIRST_RCSTRING_NAME	TMT_DISPLAYNAME
#define TMT_LAST_RCSTRING_NAME	TMT_DESCRIPTION

#ifndef TMT_CAPTIONFONT
#define TMT_CAPTIONFONT	801
#endif

#ifndef TMT_SMALLCAPTIONFONT
#define TMT_SMALLCAPTIONFONT	802
#endif

#ifndef TMT_MENUFONT
#define TMT_MENUFONT	803
#endif

#ifndef TMT_STATUSFONT
#define TMT_STATUSFONT	804
#endif

#ifndef TMT_MSGBOXFONT
#define TMT_MSGBOXFONT	805
#endif

#ifndef TMT_ICONTITLEFONT
#define TMT_ICONTITLEFONT	806
#endif

#define TMT_HEADING1FONT	807
#define TMT_HEADING2FONT	808
#define TMT_BODYFONT	809
#define TMT_FIRSTFONT	TMT_CAPTIONFONT
#define TMT_LASTFONT	TMT_BODYFONT

#ifndef TMT_FLATMENUS
#define TMT_FLATMENUS	1001
#endif

#define TMT_FIRSTBOOL	TMT_FLATMENUS
#define TMT_LASTBOOL	TMT_FLATMENUS
#define TMT_SIZINGBORDERWIDTH	1201
#define TMT_SCROLLBARWIDTH	1202
#define TMT_SCROLLBARHEIGHT	1203
#define TMT_CAPTIONBARWIDTH	1204
#define TMT_CAPTIONBARHEIGHT	1205
#define TMT_SMCAPTIONBARWIDTH	1206
#define TMT_SMCAPTIONBARHEIGHT	1207
#define TMT_MENUBARWIDTH	1208
#define TMT_MENUBARHEIGHT	1209
#define TMT_PADDEDBORDERWIDTH	1210
#define TMT_FIRSTSIZE	TMT_SIZINGBORDERWIDTH
#define TMT_LASTSIZE	TMT_PADDEDBORDERWIDTH

#ifndef TMT_MINCOLORDEPTH
#define TMT_MINCOLORDEPTH	1301
#endif

#define TMT_FIRSTINT	TMT_MINCOLORDEPTH
#define TMT_LASTINT	TMT_MINCOLORDEPTH

#ifndef TMT_CSSNAME
#define TMT_CSSNAME	1401
#endif

#ifndef TMT_XMLNAME
#define TMT_XMLNAME	1402
#endif

#define TMT_LASTUPDATED	1403
#define TMT_ALIAS	1404
#define TMT_FIRSTSTRING	TMT_CSSNAME
#define TMT_LASTSTRING	TMT_ALIAS

#ifndef TMT_SCROLLBAR
#define TMT_SCROLLBAR	1601
#endif

#ifndef TMT_BACKGROUND
#define TMT_BACKGROUND	1602
#endif

#ifndef TMT_ACTIVECAPTION
#define TMT_ACTIVECAPTION	1603
#endif

#ifndef TMT_INACTIVECAPTION
#define TMT_INACTIVECAPTION	1604
#endif

#define TMT_MENU	1605

#ifndef TMT_WINDOW
#define TMT_WINDOW	1606
#endif

#ifndef TMT_WINDOWFRAME
#define TMT_WINDOWFRAME	1607
#endif

#ifndef TMT_MENUTEXT
#define TMT_MENUTEXT	1608
#endif

#ifndef TMT_WINDOWTEXT
#define TMT_WINDOWTEXT	1609
#endif

#ifndef TMT_CAPTIONTEXT
#define TMT_CAPTIONTEXT	1610
#endif

#ifndef TMT_ACTIVEBORDER
#define TMT_ACTIVEBORDER	1611
#endif

#ifndef TMT_INACTIVEBORDER
#define TMT_INACTIVEBORDER	1612
#endif

#ifndef TMT_APPWORKSPACE
#define TMT_APPWORKSPACE	1613
#endif

#ifndef TMT_HIGHLIGHT
#define TMT_HIGHLIGHT	1614
#endif

#ifndef TMT_HIGHLIGHTTEXT
#define TMT_HIGHLIGHTTEXT	1615
#endif

#ifndef TMT_BTNFACE
#define TMT_BTNFACE	1616
#endif

#ifndef TMT_BTNSHADOW
#define TMT_BTNSHADOW	1617
#endif

#ifndef TMT_GRAYTEXT
#define TMT_GRAYTEXT	1618
#endif

#ifndef TMT_BTNTEXT
#define TMT_BTNTEXT	1619
#endif

#ifndef TMT_INACTIVECAPTIONTEXT
#define TMT_INACTIVECAPTIONTEXT	1620
#endif

#ifndef TMT_BTNHIGHLIGHT
#define TMT_BTNHIGHLIGHT	1621
#endif

#ifndef TMT_DKSHADOW3D
#define TMT_DKSHADOW3D	1622
#endif

#ifndef TMT_LIGHT3D
#define TMT_LIGHT3D	1623
#endif

#ifndef TMT_INFOTEXT
#define TMT_INFOTEXT	1624
#endif

#ifndef TMT_INFOBK
#define TMT_INFOBK	1625
#endif

#ifndef TMT_BUTTONALTERNATEFACE
#define TMT_BUTTONALTERNATEFACE	1626
#endif

#ifndef TMT_HOTTRACKING
#define TMT_HOTTRACKING	1627
#endif

#ifndef TMT_GRADIENTACTIVECAPTION
#define TMT_GRADIENTACTIVECAPTION	1628
#endif

#ifndef TMT_GRADIENTINACTIVECAPTION
#define TMT_GRADIENTINACTIVECAPTION	1629
#endif

#ifndef TMT_MENUHILIGHT
#define TMT_MENUHILIGHT	1630
#endif

#ifndef TMT_MENUBAR
#define TMT_MENUBAR	1631
#endif

#define TMT_FIRSTCOLOR	TMT_SCROLLBAR
#define TMT_LASTCOLOR	TMT_MENUBAR
#define TMT_FROMHUE1	1801
#define TMT_FROMHUE2	1802
#define TMT_FROMHUE3	1803
#define TMT_FROMHUE4	1804
#define TMT_FROMHUE5	1805
#define TMT_TOHUE1	1806
#define TMT_TOHUE2	1807
#define TMT_TOHUE3	1808
#define TMT_TOHUE4	1809
#define TMT_TOHUE5	1810
#define TMT_FROMCOLOR1	2001
#define TMT_FROMCOLOR2	2002
#define TMT_FROMCOLOR3	2003
#define TMT_FROMCOLOR4	2004
#define TMT_FROMCOLOR5	2005
#define TMT_TOCOLOR1	2006
#define TMT_TOCOLOR2	2007
#define TMT_TOCOLOR3	2008
#define TMT_TOCOLOR4	2009
#define TMT_TOCOLOR5	2010

#ifndef TMT_TRANSPARENT
#define TMT_TRANSPARENT	2201
#endif

#ifndef TMT_AUTOSIZE
#define TMT_AUTOSIZE	2202
#endif

#ifndef TMT_BORDERONLY
#define TMT_BORDERONLY	2203
#endif

#ifndef TMT_COMPOSITED
#define TMT_COMPOSITED	2204
#endif

#ifndef TMT_BGFILL
#define TMT_BGFILL	2205
#endif

#ifndef TMT_GLYPHTRANSPARENT
#define TMT_GLYPHTRANSPARENT	2206
#endif

#ifndef TMT_GLYPHONLY
#define TMT_GLYPHONLY	2207
#endif

#ifndef TMT_ALWAYSSHOWSIZINGBAR
#define TMT_ALWAYSSHOWSIZINGBAR	2208
#endif

#ifndef TMT_MIRRORIMAGE
#define TMT_MIRRORIMAGE	2209
#endif

#ifndef TMT_UNIFORMSIZING
#define TMT_UNIFORMSIZING	2210
#endif

#ifndef TMT_INTEGRALSIZING
#define TMT_INTEGRALSIZING	2211
#endif

#ifndef TMT_SOURCEGROW
#define TMT_SOURCEGROW	2212
#endif

#ifndef TMT_SOURCESHRINK
#define TMT_SOURCESHRINK	2213
#endif

#define TMT_DRAWBORDERS	2214
#define TMT_NOETCHEDEFFECT	2215
#define TMT_TEXTAPPLYOVERLAY	2216
#define TMT_TEXTGLOW	2217
#define TMT_TEXTITALIC	2218
#define TMT_COMPOSITEDOPAQUE	2219
#define TMT_LOCALIZEDMIRRORIMAGE	2220

#ifndef TMT_IMAGECOUNT
#define TMT_IMAGECOUNT	2401
#endif

#ifndef TMT_ALPHALEVEL
#define TMT_ALPHALEVEL	2402
#endif

#ifndef TMT_BORDERSIZE
#define TMT_BORDERSIZE	2403
#endif

#ifndef TMT_ROUNDCORNERWIDTH
#define TMT_ROUNDCORNERWIDTH	2404
#endif

#ifndef TMT_ROUNDCORNERHEIGHT
#define TMT_ROUNDCORNERHEIGHT	2405
#endif

#ifndef TMT_GRADIENTRATIO1
#define TMT_GRADIENTRATIO1	2406
#endif

#ifndef TMT_GRADIENTRATIO2
#define TMT_GRADIENTRATIO2	2407
#endif

#ifndef TMT_GRADIENTRATIO3
#define TMT_GRADIENTRATIO3	2408
#endif

#ifndef TMT_GRADIENTRATIO4
#define TMT_GRADIENTRATIO4	2409
#endif

#ifndef TMT_GRADIENTRATIO5
#define TMT_GRADIENTRATIO5	2410
#endif

#ifndef TMT_PROGRESSCHUNKSIZE
#define TMT_PROGRESSCHUNKSIZE	2411
#endif

#ifndef TMT_PROGRESSSPACESIZE
#define TMT_PROGRESSSPACESIZE	2412
#endif

#ifndef TMT_SATURATION
#define TMT_SATURATION	2413
#endif

#ifndef TMT_TEXTBORDERSIZE
#define TMT_TEXTBORDERSIZE	2414
#endif

#ifndef TMT_ALPHATHRESHOLD
#define TMT_ALPHATHRESHOLD	2415
#endif

#ifndef TMT_WIDTH
#define TMT_WIDTH	2416
#endif

#ifndef TMT_HEIGHT
#define TMT_HEIGHT	2417
#endif

#ifndef TMT_GLYPHINDEX
#define TMT_GLYPHINDEX	2418
#endif

#ifndef TMT_TRUESIZESTRETCHMARK
#define TMT_TRUESIZESTRETCHMARK	2419
#endif

#ifndef TMT_MINDPI1
#define TMT_MINDPI1	2420
#endif

#ifndef TMT_MINDPI2
#define TMT_MINDPI2	2421
#endif

#ifndef TMT_MINDPI3
#define TMT_MINDPI3	2422
#endif

#ifndef TMT_MINDPI4
#define TMT_MINDPI4	2423
#endif

#ifndef TMT_MINDPI5
#define TMT_MINDPI5	2424
#endif

#define TMT_TEXTGLOWSIZE	2425
#define TMT_FRAMESPERSECOND	2426
#define TMT_PIXELSPERFRAME	2427
#define TMT_ANIMATIONDELAY	2428
#define TMT_GLOWINTENSITY	2429
#define TMT_OPACITY	2430
#define TMT_COLORIZATIONCOLOR	2431
#define TMT_COLORIZATIONOPACITY	2432

#ifndef TMT_GLYPHFONT
#define TMT_GLYPHFONT	2601
#endif

#ifndef TMT_IMAGEFILE
#define TMT_IMAGEFILE	3001
#endif

#ifndef TMT_IMAGEFILE1
#define TMT_IMAGEFILE1	3002
#endif

#ifndef TMT_IMAGEFILE2
#define TMT_IMAGEFILE2	3003
#endif

#ifndef TMT_IMAGEFILE3
#define TMT_IMAGEFILE3	3004
#endif

#ifndef TMT_IMAGEFILE4
#define TMT_IMAGEFILE4	3005
#endif

#ifndef TMT_IMAGEFILE5
#define TMT_IMAGEFILE5	3006
#endif

#ifndef TMT_GLYPHIMAGEFILE
#define TMT_GLYPHIMAGEFILE	3008
#endif

#ifndef TMT_TEXT
#define TMT_TEXT	3201
#endif

#define TMT_CLASSICVALUE	3202

#ifndef TMT_OFFSET
#define TMT_OFFSET	3401
#endif

#ifndef TMT_TEXTSHADOWOFFSET
#define TMT_TEXTSHADOWOFFSET	3402
#endif

#ifndef TMT_MINSIZE
#define TMT_MINSIZE	3403
#endif

#ifndef TMT_MINSIZE1
#define TMT_MINSIZE1	3404
#endif

#ifndef TMT_MINSIZE2
#define TMT_MINSIZE2	3405
#endif

#ifndef TMT_MINSIZE3
#define TMT_MINSIZE3	3406
#endif

#ifndef TMT_MINSIZE4
#define TMT_MINSIZE4	3407
#endif

#ifndef TMT_MINSIZE5
#define TMT_MINSIZE5	3408
#endif

#ifndef TMT_NORMALSIZE
#define TMT_NORMALSIZE	3409
#endif

#ifndef TMT_SIZINGMARGINS
#define TMT_SIZINGMARGINS	3601
#endif

#ifndef TMT_CONTENTMARGINS
#define TMT_CONTENTMARGINS	3602
#endif

#ifndef TMT_CAPTIONMARGINS
#define TMT_CAPTIONMARGINS	3603
#endif

#ifndef TMT_BORDERCOLOR
#define TMT_BORDERCOLOR	3801
#endif

#ifndef TMT_FILLCOLOR
#define TMT_FILLCOLOR	3802
#endif

#ifndef TMT_TEXTCOLOR
#define TMT_TEXTCOLOR	3803
#endif

#ifndef TMT_EDGELIGHTCOLOR
#define TMT_EDGELIGHTCOLOR	3804
#endif

#ifndef TMT_EDGEHIGHLIGHTCOLOR
#define TMT_EDGEHIGHLIGHTCOLOR	3805
#endif

#ifndef TMT_EDGESHADOWCOLOR
#define TMT_EDGESHADOWCOLOR	3806
#endif

#ifndef TMT_EDGEDKSHADOWCOLOR
#define TMT_EDGEDKSHADOWCOLOR	3807
#endif

#ifndef TMT_EDGEFILLCOLOR
#define TMT_EDGEFILLCOLOR	3808
#endif

#ifndef TMT_TRANSPARENTCOLOR
#define TMT_TRANSPARENTCOLOR	3809
#endif

#ifndef TMT_GRADIENTCOLOR1
#define TMT_GRADIENTCOLOR1	3810
#endif

#ifndef TMT_GRADIENTCOLOR2
#define TMT_GRADIENTCOLOR2	3811
#endif

#ifndef TMT_GRADIENTCOLOR3
#define TMT_GRADIENTCOLOR3	3812
#endif

#ifndef TMT_GRADIENTCOLOR4
#define TMT_GRADIENTCOLOR4	3813
#endif

#ifndef TMT_GRADIENTCOLOR5
#define TMT_GRADIENTCOLOR5	3814
#endif

#ifndef TMT_SHADOWCOLOR
#define TMT_SHADOWCOLOR	3815
#endif

#ifndef TMT_GLOWCOLOR
#define TMT_GLOWCOLOR	3816
#endif

#ifndef TMT_TEXTBORDERCOLOR
#define TMT_TEXTBORDERCOLOR	3817
#endif

#ifndef TMT_TEXTSHADOWCOLOR
#define TMT_TEXTSHADOWCOLOR	3818
#endif

#ifndef TMT_GLYPHTEXTCOLOR
#define TMT_GLYPHTEXTCOLOR	3819
#endif

#ifndef TMT_GLYPHTRANSPARENTCOLOR
#define TMT_GLYPHTRANSPARENTCOLOR	3820
#endif

#ifndef TMT_FILLCOLORHINT
#define TMT_FILLCOLORHINT	3821
#endif

#ifndef TMT_BORDERCOLORHINT
#define TMT_BORDERCOLORHINT	3822
#endif

#ifndef TMT_ACCENTCOLORHINT
#define TMT_ACCENTCOLORHINT	3823
#endif

#define TMT_TEXTCOLORHINT	3824
#define TMT_HEADING1TEXTCOLOR	3825
#define TMT_HEADING2TEXTCOLOR	3826
#define TMT_BODYTEXTCOLOR	3827

#ifndef TMT_BGTYPE
#define TMT_BGTYPE	4001
#endif

#ifndef TMT_BORDERTYPE
#define TMT_BORDERTYPE	4002
#endif

#ifndef TMT_FILLTYPE
#define TMT_FILLTYPE	4003
#endif

#ifndef TMT_SIZINGTYPE
#define TMT_SIZINGTYPE	4004
#endif

#ifndef TMT_HALIGN
#define TMT_HALIGN	4005
#endif

#ifndef TMT_CONTENTALIGNMENT
#define TMT_CONTENTALIGNMENT	4006
#endif

#ifndef TMT_VALIGN
#define TMT_VALIGN	4007
#endif

#ifndef TMT_OFFSETTYPE
#define TMT_OFFSETTYPE	4008
#endif

#ifndef TMT_ICONEFFECT
#define TMT_ICONEFFECT	4009
#endif

#ifndef TMT_TEXTSHADOWTYPE
#define TMT_TEXTSHADOWTYPE	4010
#endif

#ifndef TMT_IMAGELAYOUT
#define TMT_IMAGELAYOUT	4011
#endif

#ifndef TMT_GLYPHTYPE
#define TMT_GLYPHTYPE	4012
#endif

#ifndef TMT_IMAGESELECTTYPE
#define TMT_IMAGESELECTTYPE	4013
#endif

#ifndef TMT_GLYPHFONTSIZINGTYPE
#define TMT_GLYPHFONTSIZINGTYPE	4014
#endif

#ifndef TMT_TRUESIZESCALINGTYPE
#define TMT_TRUESIZESCALINGTYPE	4015
#endif

#ifndef TMT_USERPICTURE
#define TMT_USERPICTURE	5001
#endif

#ifndef TMT_DEFAULTPANESIZE
#define TMT_DEFAULTPANESIZE	5002
#endif

#ifndef TMT_BLENDCOLOR
#define TMT_BLENDCOLOR	5003
#endif

#define TMT_CUSTOMSPLITRECT	5004
#define TMT_ANIMATIONBUTTONRECT	5005
#define TMT_ANIMATIONDURATION	5006
#define TMT_TRANSITIONDURATIONS	6000
#define TMT_SCALEDBACKGROUND	7001
#define TMT_ATLASIMAGE	8000
#define TMT_ATLASINPUTIMAGE	8001
#define TMT_ATLASRECT	8002


//
//  LINK class parts and states
//
#define VSCLASS_LINK	L"LINK"

enum LINKPARTS {
	LP_HYPERLINK = 1,
};

enum HYPERLINKSTATES {
	HLS_NORMALTEXT = 1,
	HLS_LINKTEXT = 2,
};


//
//  EMPTYMARKUP class parts and states
//
#define VSCLASS_EMPTYMARKUP	L"EMPTYMARKUP"

enum EMPTYMARKUPPARTS {
	EMP_MARKUPTEXT = 1,
};

enum MARKUPTEXTSTATES {
	EMT_NORMALTEXT = 1,
	EMT_LINKTEXT = 2,
};


//
//  STATIC class parts and states
//
#define VSCLASS_STATIC	L"STATIC"

enum STATICPARTS {
	STAT_TEXT = 1,
};


//
//  PAGE class parts and states
//
#define VSCLASS_PAGE	L"PAGE"

enum PAGEPARTS {
	PGRP_UP = 1,
	PGRP_DOWN = 2,
	PGRP_UPHORZ = 3,
	PGRP_DOWNHORZ = 4,
};


//
//  MONTHCAL class parts and states
//
#define VSCLASS_MONTHCAL	L"MONTHCAL"

enum MONTHCALPARTS {
	MC_BACKGROUND = 1,
	MC_BORDERS = 2,
	MC_GRIDBACKGROUND = 3,
	MC_COLHEADERSPLITTER = 4,
	MC_GRIDCELLBACKGROUND = 5,
	MC_GRIDCELL = 6,
	MC_GRIDCELLUPPER = 7,
	MC_TRAILINGGRIDCELL = 8,
	MC_TRAILINGGRIDCELLUPPER = 9,
	MC_NAVNEXT = 10,
	MC_NAVPREV = 11,
};

enum GRIDCELLBACKGROUNDSTATES {
	MCGCB_SELECTED = 1,
	MCGCB_HOT = 2,
	MCGCB_SELECTEDHOT = 3,
	MCGCB_SELECTEDNOTFOCUSED = 4,
	MCGCB_TODAY = 5,
};

enum GRIDCELLSTATES {
	MCGC_HOT = 1,
	MCGC_HASSTATE = 2,
	MCGC_HASSTATEHOT = 3,
	MCGC_TODAY = 4,
};

enum GRIDCELLUPPERSTATES {
	MCGCU_HOT = 1,
	MCGCU_HASSTATE = 2,
	MCGCU_HASSTATEHOT = 3,
};

enum TRAILINGGRIDCELLSTATES {
	MCTGC_HOT = 1,
	MCTGC_HASSTATE = 2,
	MCTGC_HASSTATEHOT = 3,
	MCTGC_TODAY = 4,
};

enum TRAILINGGRIDCELLUPPERSTATES {
	MCTGCU_HOT = 1,
	MCTGCU_HASSTATE = 2,
	MCTGCU_HASSTATEHOT = 3,
};

enum NAVNEXTSTATES {
	MCNN_NORMAL = 1,
	MCNN_HOT = 2,
	MCNN_PRESSED = 3,
	MCNN_DISABLED = 4,
};

enum NAVPREVSTATES {
	MCNP_NORMAL = 1,
	MCNP_HOT = 2,
	MCNP_PRESSED = 3,
	MCNP_DISABLED = 4,
};


//
//  CLOCK class parts and states
//
#define VSCLASS_CLOCK	L"CLOCK"

enum CLOCKPARTS {
	CLP_TIME = 1,
};

enum CLOCKSTATES {
	CLS_NORMAL = 1,
	CLS_HOT = 2,
	CLS_PRESSED = 3,
};


//
//  TRAYNOTIFY class parts and states
//
#define VSCLASS_TRAYNOTIFY	L"TRAYNOTIFY"

enum TRAYNOTIFYPARTS {
	TNP_BACKGROUND = 1,
	TNP_ANIMBACKGROUND = 2,
};


//
//  TASKBAR class parts and states
//
#define VSCLASS_TASKBAR	L"TASKBAR"

enum TASKBARPARTS {
	TBP_BACKGROUNDBOTTOM = 1,
	TBP_BACKGROUNDRIGHT = 2,
	TBP_BACKGROUNDTOP = 3,
	TBP_BACKGROUNDLEFT = 4,
	TBP_SIZINGBARBOTTOM = 5,
	TBP_SIZINGBARRIGHT = 6,
	TBP_SIZINGBARTOP = 7,
	TBP_SIZINGBARLEFT = 8,
};


//
//  TASKBAND class parts and states
//
#define VSCLASS_TASKBAND	L"TASKBAND"

enum TASKBANDPARTS {
	TDP_GROUPCOUNT = 1,
	TDP_FLASHBUTTON = 2,
	TDP_FLASHBUTTONGROUPMENU = 3,
};


//
//  STARTPANEL class parts and states
//
#define VSCLASS_STARTPANEL	L"STARTPANEL"

enum STARTPANELPARTS {
	SPP_USERPANE = 1,
	SPP_MOREPROGRAMS = 2,
	SPP_MOREPROGRAMSARROW = 3,
	SPP_PROGLIST = 4,
	SPP_PROGLISTSEPARATOR = 5,
	SPP_PLACESLIST = 6,
	SPP_PLACESLISTSEPARATOR = 7,
	SPP_LOGOFF = 8,
	SPP_LOGOFFBUTTONS = 9,
	SPP_USERPICTURE = 10,
	SPP_PREVIEW = 11,
	SPP_MOREPROGRAMSTAB = 12,
	SPP_NSCHOST = 13,
	SPP_SOFTWAREEXPLORER = 14,
	SPP_OPENBOX = 15,
	SPP_SEARCHVIEW = 16,
	SPP_MOREPROGRAMSARROWBACK = 17,
	SPP_TOPMATCH = 18,
	SPP_LOGOFFSPLITBUTTONDROPDOWN = 19,
};

enum MOREPROGRAMSTABSTATES {
	SPMPT_NORMAL = 1,
	SPMPT_HOT = 2,
	SPMPT_SELECTED = 3,
	SPMPT_DISABLED = 4,
	SPMPT_FOCUSED = 5,
};

enum SOFTWAREEXPLORERSTATES {
	SPSE_NORMAL = 1,
	SPSE_HOT = 2,
	SPSE_SELECTED = 3,
	SPSE_DISABLED = 4,
	SPSE_FOCUSED = 5,
};

enum OPENBOXSTATES {
	SPOB_NORMAL = 1,
	SPOB_HOT = 2,
	SPOB_SELECTED = 3,
	SPOB_DISABLED = 4,
	SPOB_FOCUSED = 5,
};

enum MOREPROGRAMSARROWSTATES {
	SPS_NORMAL = 1,
	SPS_HOT = 2,
	SPS_PRESSED = 3,
};

enum MOREPROGRAMSARROWBACKSTATES {
	SPSB_NORMAL = 1,
	SPSB_HOT = 2,
	SPSB_PRESSED = 3,
};

enum LOGOFFBUTTONSSTATES {
	SPLS_NORMAL = 1,
	SPLS_HOT = 2,
	SPLS_PRESSED = 3,
};


//
//  MENUBAND class parts and states
//
#define VSCLASS_MENUBAND	L"MENUBAND"

enum MENUBANDPARTS {
	MDP_NEWAPPBUTTON = 1,
	MDP_SEPERATOR = 2,
};

enum MENUBANDSTATES {
	MDS_NORMAL = 1,
	MDS_HOT = 2,
	MDS_PRESSED = 3,
	MDS_DISABLED = 4,
	MDS_CHECKED = 5,
	MDS_HOTCHECKED = 6,
};

#endif /* VSSYM32_H_ */

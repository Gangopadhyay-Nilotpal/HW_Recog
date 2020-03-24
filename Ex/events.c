/* evtest.c,v 1.23 2005/02/06 13:51:42 vojtech Exp $
 *
 *  *  Copyright (c) 1999-2000 Vojtech Pavlik
 *   *
 *    *  Event device test program
 *     */

/*
 *  * This program is free software; you can redistribute it and/or modify
 *   * it under the terms of the GNU General Public License as published by
 *    * the Free Software Foundation; either version 2 of the License, or 
 *     * (at your option) any later version.
 *      * 
 *       * This program is distributed in the hope that it will be useful,
 *        * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *         * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *          * GNU General Public License for more details.
 *           * 
 *            * You should have received a copy of the GNU General Public License
 *             * along with this program; if not, write to the Free Software
 *              * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *               * 
 *                * Should you need to contact me, the author, you can do so either by
 *                 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 *                  * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 *                   */

#include <stdint.h>
#include <linux/input.h>
#include <fcntl.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include<math.h>
#ifdef BLACKBOARD_CHANGES
#include"pensense/pensense.h"
#endif
#include"master.h"

#ifndef EV_SYN
#define EV_SYN 0
#endif


char *events[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = "Sync",			[EV_KEY] = "Key",
	[EV_REL] = "Relative",			[EV_ABS] = "Absolute",
	[EV_MSC] = "Misc",			[EV_LED] = "LED",
	[EV_SND] = "Sound",			[EV_REP] = "Repeat",
	[EV_FF] = "ForceFeedback",		[EV_PWR] = "Power",
	[EV_FF_STATUS] = "ForceFeedbackStatus",
};

char *keys[KEY_MAX + 1] = {
	[0 ... KEY_MAX] = NULL,
	[KEY_RESERVED] = "Reserved",		[KEY_ESC] = "Esc",
	[KEY_1] = "1",				[KEY_2] = "2",
	[KEY_3] = "3",				[KEY_4] = "4",
	[KEY_5] = "5",				[KEY_6] = "6",
	[KEY_7] = "7",				[KEY_8] = "8",
	[KEY_9] = "9",				[KEY_0] = "0",
	[KEY_MINUS] = "Minus",			[KEY_EQUAL] = "Equal",
	[KEY_BACKSPACE] = "Backspace",		[KEY_TAB] = "Tab",
	[KEY_Q] = "Q",				[KEY_W] = "W",
	[KEY_E] = "E",				[KEY_R] = "R",
	[KEY_T] = "T",				[KEY_Y] = "Y",
	[KEY_U] = "U",				[KEY_I] = "I",
	[KEY_O] = "O",				[KEY_P] = "P",
	[KEY_LEFTBRACE] = "LeftBrace",		[KEY_RIGHTBRACE] = "RightBrace",
	[KEY_ENTER] = "Enter",			[KEY_LEFTCTRL] = "LeftControl",
	[KEY_A] = "A",				[KEY_S] = "S",
	[KEY_D] = "D",				[KEY_F] = "F",
	[KEY_G] = "G",				[KEY_H] = "H",
	[KEY_J] = "J",				[KEY_K] = "K",
	[KEY_L] = "L",				[KEY_SEMICOLON] = "Semicolon",
	[KEY_APOSTROPHE] = "Apostrophe",	[KEY_GRAVE] = "Grave",
	[KEY_LEFTSHIFT] = "LeftShift",		[KEY_BACKSLASH] = "BackSlash",
	[KEY_Z] = "Z",				[KEY_X] = "X",
	[KEY_C] = "C",				[KEY_V] = "V",
	[KEY_B] = "B",				[KEY_N] = "N",
	[KEY_M] = "M",				[KEY_COMMA] = "Comma",
	[KEY_DOT] = "Dot",			[KEY_SLASH] = "Slash",
	[KEY_RIGHTSHIFT] = "RightShift",	[KEY_KPASTERISK] = "KPAsterisk",
	[KEY_LEFTALT] = "LeftAlt",		[KEY_SPACE] = "Space",
	[KEY_CAPSLOCK] = "CapsLock",		[KEY_F1] = "F1",
	[KEY_F2] = "F2",			[KEY_F3] = "F3",
	[KEY_F4] = "F4",			[KEY_F5] = "F5",
	[KEY_F6] = "F6",			[KEY_F7] = "F7",
	[KEY_F8] = "F8",			[KEY_F9] = "F9",
	[KEY_F10] = "F10",			[KEY_NUMLOCK] = "NumLock",
	[KEY_SCROLLLOCK] = "ScrollLock",	[KEY_KP7] = "KP7",
	[KEY_KP8] = "KP8",			[KEY_KP9] = "KP9",
	[KEY_KPMINUS] = "KPMinus",		[KEY_KP4] = "KP4",
	[KEY_KP5] = "KP5",			[KEY_KP6] = "KP6",
	[KEY_KPPLUS] = "KPPlus",		[KEY_KP1] = "KP1",
	[KEY_KP2] = "KP2",			[KEY_KP3] = "KP3",
	[KEY_KP0] = "KP0",			[KEY_KPDOT] = "KPDot",
	[KEY_ZENKAKUHANKAKU] = "Zenkaku/Hankaku", [KEY_102ND] = "102nd",
	[KEY_F11] = "F11",			[KEY_F12] = "F12",
	[KEY_RO] = "RO",			[KEY_KATAKANA] = "Katakana",
	[KEY_HIRAGANA] = "HIRAGANA",		[KEY_HENKAN] = "Henkan",
	[KEY_KATAKANAHIRAGANA] = "Katakana/Hiragana", [KEY_MUHENKAN] = "Muhenkan",
	[KEY_KPJPCOMMA] = "KPJpComma",		[KEY_KPENTER] = "KPEnter",
	[KEY_RIGHTCTRL] = "RightCtrl",		[KEY_KPSLASH] = "KPSlash",
	[KEY_SYSRQ] = "SysRq",			[KEY_RIGHTALT] = "RightAlt",
	[KEY_LINEFEED] = "LineFeed",		[KEY_HOME] = "Home",
	[KEY_UP] = "Up",			[KEY_PAGEUP] = "PageUp",
	[KEY_LEFT] = "Left",			[KEY_RIGHT] = "Right",
	[KEY_END] = "End",			[KEY_DOWN] = "Down",
	[KEY_PAGEDOWN] = "PageDown",		[KEY_INSERT] = "Insert",
	[KEY_DELETE] = "Delete",		[KEY_MACRO] = "Macro",
	[KEY_MUTE] = "Mute",			[KEY_VOLUMEDOWN] = "VolumeDown",
	[KEY_VOLUMEUP] = "VolumeUp",		[KEY_POWER] = "Power",
	[KEY_KPEQUAL] = "KPEqual",		[KEY_KPPLUSMINUS] = "KPPlusMinus",
	[KEY_PAUSE] = "Pause",			[KEY_KPCOMMA] = "KPComma",
	[KEY_HANGUEL] = "Hanguel",		[KEY_HANJA] = "Hanja",
	[KEY_YEN] = "Yen",			[KEY_LEFTMETA] = "LeftMeta",
	[KEY_RIGHTMETA] = "RightMeta",		[KEY_COMPOSE] = "Compose",
	[KEY_STOP] = "Stop",			[KEY_AGAIN] = "Again",
	[KEY_PROPS] = "Props",			[KEY_UNDO] = "Undo",
	[KEY_FRONT] = "Front",			[KEY_COPY] = "Copy",
	[KEY_OPEN] = "Open",			[KEY_PASTE] = "Paste",
	[KEY_FIND] = "Find",			[KEY_CUT] = "Cut",
	[KEY_HELP] = "Help",			[KEY_MENU] = "Menu",
	[KEY_CALC] = "Calc",			[KEY_SETUP] = "Setup",
	[KEY_SLEEP] = "Sleep",			[KEY_WAKEUP] = "WakeUp",
	[KEY_FILE] = "File",			[KEY_SENDFILE] = "SendFile",
	[KEY_DELETEFILE] = "DeleteFile",	[KEY_XFER] = "X-fer",
	[KEY_PROG1] = "Prog1",			[KEY_PROG2] = "Prog2",
	[KEY_WWW] = "WWW",			[KEY_MSDOS] = "MSDOS",
	[KEY_COFFEE] = "Coffee",		[KEY_DIRECTION] = "Direction",
	[KEY_CYCLEWINDOWS] = "CycleWindows",	[KEY_MAIL] = "Mail",
	[KEY_BOOKMARKS] = "Bookmarks",		[KEY_COMPUTER] = "Computer",
	[KEY_BACK] = "Back",			[KEY_FORWARD] = "Forward",
	[KEY_CLOSECD] = "CloseCD",		[KEY_EJECTCD] = "EjectCD",
	[KEY_EJECTCLOSECD] = "EjectCloseCD",	[KEY_NEXTSONG] = "NextSong",
	[KEY_PLAYPAUSE] = "PlayPause",		[KEY_PREVIOUSSONG] = "PreviousSong",
	[KEY_STOPCD] = "StopCD",		[KEY_RECORD] = "Record",
	[KEY_REWIND] = "Rewind",		[KEY_PHONE] = "Phone",
	[KEY_ISO] = "ISOKey",			[KEY_CONFIG] = "Config",
	[KEY_HOMEPAGE] = "HomePage",		[KEY_REFRESH] = "Refresh",
	[KEY_EXIT] = "Exit",			[KEY_MOVE] = "Move",
	[KEY_EDIT] = "Edit",			[KEY_SCROLLUP] = "ScrollUp",
	[KEY_SCROLLDOWN] = "ScrollDown",	[KEY_KPLEFTPAREN] = "KPLeftParenthesis",
	[KEY_KPRIGHTPAREN] = "KPRightParenthesis", [KEY_F13] = "F13",
	[KEY_F14] = "F14",			[KEY_F15] = "F15",
	[KEY_F16] = "F16",			[KEY_F17] = "F17",
	[KEY_F18] = "F18",			[KEY_F19] = "F19",
	[KEY_F20] = "F20",			[KEY_F21] = "F21",
	[KEY_F22] = "F22",			[KEY_F23] = "F23",
	[KEY_F24] = "F24",			[KEY_PLAYCD] = "PlayCD",
	[KEY_PAUSECD] = "PauseCD",		[KEY_PROG3] = "Prog3",
	[KEY_PROG4] = "Prog4",			[KEY_SUSPEND] = "Suspend",
	[KEY_CLOSE] = "Close",			[KEY_PLAY] = "Play",
	[KEY_FASTFORWARD] = "Fast Forward",	[KEY_BASSBOOST] = "Bass Boost",
	[KEY_PRINT] = "Print",			[KEY_HP] = "HP",
	[KEY_CAMERA] = "Camera",		[KEY_SOUND] = "Sound",
	[KEY_QUESTION] = "Question",		[KEY_EMAIL] = "Email",
	[KEY_CHAT] = "Chat",			[KEY_SEARCH] = "Search",
	[KEY_CONNECT] = "Connect",		[KEY_FINANCE] = "Finance",
	[KEY_SPORT] = "Sport",			[KEY_SHOP] = "Shop",
	[KEY_ALTERASE] = "Alternate Erase",	[KEY_CANCEL] = "Cancel",
	[KEY_BRIGHTNESSDOWN] = "Brightness down", [KEY_BRIGHTNESSUP] = "Brightness up",
	[KEY_MEDIA] = "Media",			[KEY_UNKNOWN] = "Unknown",
	[BTN_0] = "Btn0",			[BTN_1] = "Btn1",
	[BTN_2] = "Btn2",			[BTN_3] = "Btn3",
	[BTN_4] = "Btn4",			[BTN_5] = "Btn5",
	[BTN_6] = "Btn6",			[BTN_7] = "Btn7",
	[BTN_8] = "Btn8",			[BTN_9] = "Btn9",
	[BTN_LEFT] = "LeftBtn",			[BTN_RIGHT] = "RightBtn",
	[BTN_MIDDLE] = "MiddleBtn",		[BTN_SIDE] = "SideBtn",
	[BTN_EXTRA] = "ExtraBtn",		[BTN_FORWARD] = "ForwardBtn",
	[BTN_BACK] = "BackBtn",			[BTN_TASK] = "TaskBtn",
	[BTN_TRIGGER] = "Trigger",		[BTN_THUMB] = "ThumbBtn",
	[BTN_THUMB2] = "ThumbBtn2",		[BTN_TOP] = "TopBtn",
	[BTN_TOP2] = "TopBtn2",			[BTN_PINKIE] = "PinkieBtn",
	[BTN_BASE] = "BaseBtn",			[BTN_BASE2] = "BaseBtn2",
	[BTN_BASE3] = "BaseBtn3",		[BTN_BASE4] = "BaseBtn4",
	[BTN_BASE5] = "BaseBtn5",		[BTN_BASE6] = "BaseBtn6",
	[BTN_DEAD] = "BtnDead",			[BTN_A] = "BtnA",
	[BTN_B] = "BtnB",			[BTN_C] = "BtnC",
	[BTN_X] = "BtnX",			[BTN_Y] = "BtnY",
	[BTN_Z] = "BtnZ",			[BTN_TL] = "BtnTL",
	[BTN_TR] = "BtnTR",			[BTN_TL2] = "BtnTL2",
	[BTN_TR2] = "BtnTR2",			[BTN_SELECT] = "BtnSelect",
	[BTN_START] = "BtnStart",		[BTN_MODE] = "BtnMode",
	[BTN_THUMBL] = "BtnThumbL",		[BTN_THUMBR] = "BtnThumbR",
	[BTN_TOOL_PEN] = "ToolPen",		[BTN_TOOL_RUBBER] = "ToolRubber",
	[BTN_TOOL_BRUSH] = "ToolBrush",		[BTN_TOOL_PENCIL] = "ToolPencil",
	[BTN_TOOL_AIRBRUSH] = "ToolAirbrush",	[BTN_TOOL_FINGER] = "ToolFinger",
	[BTN_TOOL_MOUSE] = "ToolMouse",		[BTN_TOOL_LENS] = "ToolLens",
	[BTN_TOUCH] = "Touch",			[BTN_STYLUS] = "Stylus",
	[BTN_STYLUS2] = "Stylus2",		[BTN_TOOL_DOUBLETAP] = "Tool Doubletap",
	[BTN_TOOL_TRIPLETAP] = "Tool Tripletap", [BTN_GEAR_DOWN] = "WheelBtn",
	[BTN_GEAR_UP] = "Gear up",		[KEY_OK] = "Ok",
	[KEY_SELECT] = "Select",		[KEY_GOTO] = "Goto",
	[KEY_CLEAR] = "Clear",			[KEY_POWER2] = "Power2",
	[KEY_OPTION] = "Option",		[KEY_INFO] = "Info",
	[KEY_TIME] = "Time",			[KEY_VENDOR] = "Vendor",
	[KEY_ARCHIVE] = "Archive",		[KEY_PROGRAM] = "Program",
	[KEY_CHANNEL] = "Channel",		[KEY_FAVORITES] = "Favorites",
	[KEY_EPG] = "EPG",			[KEY_PVR] = "PVR",
	[KEY_MHP] = "MHP",			[KEY_LANGUAGE] = "Language",
	[KEY_TITLE] = "Title",			[KEY_SUBTITLE] = "Subtitle",
	[KEY_ANGLE] = "Angle",			[KEY_ZOOM] = "Zoom",
	[KEY_MODE] = "Mode",			[KEY_KEYBOARD] = "Keyboard",
	[KEY_SCREEN] = "Screen",		[KEY_PC] = "PC",
	[KEY_TV] = "TV",			[KEY_TV2] = "TV2",
	[KEY_VCR] = "VCR",			[KEY_VCR2] = "VCR2",
	[KEY_SAT] = "Sat",			[KEY_SAT2] = "Sat2",
	[KEY_CD] = "CD",			[KEY_TAPE] = "Tape",
	[KEY_RADIO] = "Radio",			[KEY_TUNER] = "Tuner",
	[KEY_PLAYER] = "Player",		[KEY_TEXT] = "Text",
	[KEY_DVD] = "DVD",			[KEY_AUX] = "Aux",
	[KEY_MP3] = "MP3",			[KEY_AUDIO] = "Audio",
	[KEY_VIDEO] = "Video",			[KEY_DIRECTORY] = "Directory",
	[KEY_LIST] = "List",			[KEY_MEMO] = "Memo",
	[KEY_CALENDAR] = "Calendar",		[KEY_RED] = "Red",
	[KEY_GREEN] = "Green",			[KEY_YELLOW] = "Yellow",
	[KEY_BLUE] = "Blue",			[KEY_CHANNELUP] = "ChannelUp",
	[KEY_CHANNELDOWN] = "ChannelDown",	[KEY_FIRST] = "First",
	[KEY_LAST] = "Last",			[KEY_AB] = "AB",
	[KEY_NEXT] = "Next",			[KEY_RESTART] = "Restart",
	[KEY_SLOW] = "Slow",			[KEY_SHUFFLE] = "Shuffle",
	[KEY_BREAK] = "Break",			[KEY_PREVIOUS] = "Previous",
	[KEY_DIGITS] = "Digits",		[KEY_TEEN] = "TEEN",
	[KEY_TWEN] = "TWEN",			[KEY_DEL_EOL] = "Delete EOL",
	[KEY_DEL_EOS] = "Delete EOS",		[KEY_INS_LINE] = "Insert line",
	[KEY_DEL_LINE] = "Delete line",
};

char *absval[5] = { "Value", "Min  ", "Max  ", "Fuzz ", "Flat " };

char *relatives[REL_MAX + 1] = {
	[0 ... REL_MAX] = NULL,
	[REL_X] = "X",			[REL_Y] = "Y",
	[REL_Z] = "Z",			[REL_HWHEEL] = "HWheel",
	[REL_DIAL] = "Dial",		[REL_WHEEL] = "Wheel", 
	[REL_MISC] = "Misc",	
};

char *absolutes[ABS_MAX + 1] = {
	[0 ... ABS_MAX] = NULL,
	[ABS_X] = "X",			[ABS_Y] = "Y",
	[ABS_Z] = "Z",			[ABS_RX] = "Rx",
	[ABS_RY] = "Ry",		[ABS_RZ] = "Rz",
	[ABS_THROTTLE] = "Throttle",	[ABS_RUDDER] = "Rudder",
	[ABS_WHEEL] = "Wheel",		[ABS_GAS] = "Gas",
	[ABS_BRAKE] = "Brake",		[ABS_HAT0X] = "Hat0X",
	[ABS_HAT0Y] = "Hat0Y",		[ABS_HAT1X] = "Hat1X",
	[ABS_HAT1Y] = "Hat1Y",		[ABS_HAT2X] = "Hat2X",
	[ABS_HAT2Y] = "Hat2Y",		[ABS_HAT3X] = "Hat3X",
	[ABS_HAT3Y] = "Hat 3Y",		[ABS_PRESSURE] = "Pressure",
	[ABS_DISTANCE] = "Distance",	[ABS_TILT_X] = "XTilt",
	[ABS_TILT_Y] = "YTilt",		[ABS_TOOL_WIDTH] = "Tool Width",
	[ABS_VOLUME] = "Volume",	[ABS_MISC] = "Misc",
};

char *misc[MSC_MAX + 1] = {
	[ 0 ... MSC_MAX] = NULL,
	[MSC_SERIAL] = "Serial",	[MSC_PULSELED] = "Pulseled",
	[MSC_GESTURE] = "Gesture",	[MSC_RAW] = "RawData",
	[MSC_SCAN] = "ScanCode",
};

char *leds[LED_MAX + 1] = {
	[0 ... LED_MAX] = NULL,
	[LED_NUML] = "NumLock",		[LED_CAPSL] = "CapsLock", 
	[LED_SCROLLL] = "ScrollLock",	[LED_COMPOSE] = "Compose",
	[LED_KANA] = "Kana",		[LED_SLEEP] = "Sleep", 
	[LED_SUSPEND] = "Suspend",	[LED_MUTE] = "Mute",
	[LED_MISC] = "Misc",
};

char *repeats[REP_MAX + 1] = {
	[0 ... REP_MAX] = NULL,
	[REP_DELAY] = "Delay",		[REP_PERIOD] = "Period"
};

char *sounds[SND_MAX + 1] = {
	[0 ... SND_MAX] = NULL,
	[SND_CLICK] = "Click",		[SND_BELL] = "Bell",
	[SND_TONE] = "Tone"
};

char **names[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = events,			[EV_KEY] = keys,
	[EV_REL] = relatives,			[EV_ABS] = absolutes,
	[EV_MSC] = misc,			[EV_LED] = leds,
	[EV_SND] = sounds,			[EV_REP] = repeats,
};

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

//int main (int argc, char **argv)
int sense(int ind, int be, int bx, int by, int bw, int bh)
{
	int fd, rd, i, j, k;
	struct input_event ev[64];
	int version;
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	char name[256] = "Unknown";
	int abs[5];

#ifdef BLACKBOARD_CHANGES
	int toWrite=0;
	int pair=0;
	int xF=0;
	int yF=0;
	int count=0;
	int alreadySent=1;
	//int xx=0,yy=0;
	double xx=0,yy=0;
	int xxx,yyy;
#endif
    
    // Declaration of 4 Dynamic Arrays using "calloc"
	// “calloc” or “contiguous allocation” method in C is used to dynamically allocate the specified number of blocks of memory of the specified type. It initializes each block with a default value ‘0’.
	// ---------------------------------------------------------
	double timestamp = 0.0;
	int js_type = 0;
	//char typestring[] = events[ev[i].type] ? events[ev[i].type] : "?";
	int js_code = 0;
	//char codestring[] = names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?";
	int js_value = 0;
	
    double *t_val;
    int *x_val;
    int *y_val;    
    int *p_val;
	
	int MAX = 0;
	int array_ele_no = 0;
	
	double pre_t_val = 0.0;
	int pre_x_val = 0;
	int pre_y_val = 0;
	int pre_p_val = 0;
	
	int js_flag = 0;
	int x_tkn_flg = 0;
	int y_tkn_flg = 0;
	int p_tkn_flg = 0;
    // ---------------------------------------------------------
    
	//if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
	if ((fd = open("/dev/input/event0", O_RDONLY)) < 0) {
		perror("evtest");
		return 1;
	}

	if (ioctl(fd, EVIOCGVERSION, &version)) {
		perror("evtest: can't get version");
		return 1;
	}

	DPRINT(INFO,"Input driver version is %d.%d.%d\n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);

	ioctl(fd, EVIOCGID, id);
	DPRINT(INFO,"Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	DPRINT(INFO,"Input device name: \"%s\"\n", name);

	memset(bit, 0, sizeof(bit));
	ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	DPRINT(INFO,"Supported events:\n");

	for (i = 0; i < EV_MAX; i++)
		if (test_bit(i, bit[0])) {
			DPRINT(INFO,"  Event type %d (%s)\n", i, events[i] ? events[i] : "?");
			if (!i) continue;
			ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
			for (j = 0; j < KEY_MAX; j++) 
				if (test_bit(j, bit[i])) {
					DPRINT(INFO,"    Event code %d (%s)\n", j, names[i] ? (names[i][j] ? names[i][j] : "?") : "?");
					if (i == EV_ABS) {
						ioctl(fd, EVIOCGABS(j), abs);
						for (k = 0; k < 5; k++)
							if ((k < 3) || abs[k])
								DPRINT(INFO,"      %s %6d\n", absval[k], abs[k]);
					}
				}
		}
		

	DPRINT(INFO,"Testing ... (interrupt to exit)\n");
	
    
	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if (rd < (int) sizeof(struct input_event)) {
			DPRINT(ERROR,"yyy\n");
			perror("\nevtest: error reading");
			return 1;
		}

		for (i = 0; i < rd / sizeof(struct input_event); i++)

			if (ev[i].type == EV_SYN) {
#ifdef SENSE_LOGS
				DPRINT(INFO,"Event: time %ld.%06ld, -------------- %s ------------\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "Config Sync" : "Report Sync" );
#endif
			} else if (ev[i].type == EV_MSC && (ev[i].code == MSC_RAW || ev[i].code == MSC_SCAN)) {
#ifdef SENSE_LOGS
				DPRINT(INFO,"Event: time %ld.%06ld, type %d (%s), code %d (%s), value %02x\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
#endif
			} else {
#ifdef SENSE_LOGS
				DPRINT(INFO,"Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
					
				// Read Parameters and create text file of JSON Format
				// =================================================================================================================
				char temp_ts[30];
				sprintf(temp_ts, "%ld.%06ld", ev[i].time.tv_sec, ev[i].time.tv_usec);
				
				timestamp = strtod(temp_ts, (char **)NULL);
					
				js_type = ev[i].type;
				js_code = ev[i].code;
				js_value = ev[i].value;	
				
				// -----------------------------------------------------------------------------------------------------------------
				if ((js_flag == 1) && (js_type == 3)) {
					
					if ((timestamp != pre_t_val) && ((js_code == 0) || (js_code == 1) || (js_code == 24))) {
					    MAX++;
						
						if (MAX == 1){
							t_val = (double*)calloc(MAX, sizeof(double));
							x_val = (int*)calloc(MAX, sizeof(int));
							y_val = (int*)calloc(MAX, sizeof(int));
							p_val = (int*)calloc(MAX, sizeof(int));
						}else if (MAX > 1){
							// “realloc” or “re-allocation” method in C is used to dynamically change the memory allocation of a previously allocated memory. 
							// In other words, if the memory previously allocated with the help of malloc or calloc is insufficient, realloc can be used to dynamically re-allocate memory.
							t_val = realloc(t_val, MAX * sizeof(double));
							x_val = realloc(x_val, MAX * sizeof(int));
							y_val = realloc(y_val, MAX * sizeof(int));
							p_val = realloc(p_val, MAX * sizeof(int));
						}
						
					    t_val[MAX - 1] = timestamp;
					    pre_t_val = timestamp;
					}
					
					if(MAX == 1){
						
					    if ((timestamp == pre_t_val) && (js_code == 0) && (x_tkn_flg == 0)) {
						    x_val[MAX - 1] = js_value;
						    pre_x_val = js_value;
						    x_tkn_flg = 1;
					    }else if ((timestamp == pre_t_val) && (js_code == 1) && (y_tkn_flg == 0)) {
						    y_val[MAX - 1] = js_value;
						    pre_y_val = js_value;
						    y_tkn_flg = 1;
					    }else if ((timestamp == pre_t_val) && (js_code == 24) && (p_tkn_flg == 0)) {
						    p_val[MAX - 1] = js_value;
						    pre_p_val = js_value;
						    p_tkn_flg = 1;
						    if((p_val[MAX - 1] == 0) && (pre_p_val == 0) && (p_tkn_flg == 1)){
							    p_val[MAX - 1] = 1;
						    }
					    }
						
					}
					
					if(MAX > 1){ 
						
						if((array_ele_no < MAX) && ((x_tkn_flg == 0) || (y_tkn_flg == 0) || (p_tkn_flg == 0))){ 
						    
						    if (x_tkn_flg == 0) {
							    x_val[(MAX - 1) - 1] = pre_x_val;
							    x_tkn_flg = 1;
						    }
							if (y_tkn_flg == 0) {
							    y_val[(MAX - 1) - 1] = pre_y_val;
							    y_tkn_flg = 1;
						    }
							if (p_tkn_flg == 0) {
							    p_val[(MAX - 1) - 1] = pre_p_val;
							    p_tkn_flg = 1;
							    if((p_val[(MAX - 1) - 1] == 0) && (p_tkn_flg == 1)){
									p_val[(MAX - 1) - 1] = 1;
								}
							}
							array_ele_no = MAX;    // To fillup the backlog array elements of the previous step/loop, and balance
						}
						
						if ((array_ele_no == MAX) && (x_tkn_flg == 1) && (y_tkn_flg == 1) && (p_tkn_flg == 1)) {
						    x_tkn_flg = 0;
						    y_tkn_flg = 0;
						    p_tkn_flg = 0;
					    }
						
						if ((timestamp == pre_t_val) && (js_code == 0) && (x_tkn_flg == 0)) {
						    x_val[MAX - 1] = js_value;
						    pre_x_val = js_value;
						    x_tkn_flg = 1;
					    }else if ((timestamp == pre_t_val) && (js_code == 1) && (y_tkn_flg == 0)) {
						    y_val[MAX - 1] = js_value;
						    pre_y_val = js_value;
						    y_tkn_flg = 1;
					    }else if ((timestamp == pre_t_val) && (js_code == 24) && (p_tkn_flg == 0)) {
						    p_val[MAX - 1] = js_value;
						    pre_p_val = js_value;
						    p_tkn_flg = 1;
						    if((p_val[MAX - 1] == 0) && (pre_p_val == 0) && (p_tkn_flg == 1)){
							    p_val[MAX - 1] = 1;
						    }
					    }
					}
			    }
				
				if ((js_type == 1) && (js_code == 320)) {
					if ((js_flag == 1) && (js_value == 0)) {
						// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						if((x_tkn_flg == 0) || (y_tkn_flg == 0) || (p_tkn_flg == 0)){ 
						    
						    if (x_tkn_flg == 0) {
							    x_val[MAX - 1] = pre_x_val;
							    x_tkn_flg = 1;
						    }
							if (y_tkn_flg == 0) {
							    y_val[MAX - 1] = pre_y_val;
							    y_tkn_flg = 1;
						    }
							if (p_tkn_flg == 0) {
							    p_val[MAX - 1] = pre_p_val;
							    p_tkn_flg = 1;
							    if((p_val[MAX - 1] == 0) && (p_tkn_flg == 1)){
									p_val[MAX - 1] = 1;
								}
							}
						}
						// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						
						js_flag = 0;						
						x_tkn_flg = 0;
						y_tkn_flg = 0;
						p_tkn_flg = 0;
						
						pre_t_val = 0.0;
						pre_x_val = 0;
						pre_y_val = 0;
						pre_p_val = 0;
						
						//---------------------------------------------------
						//MAX--;
                        
						FILE *fp;
						
						char tsf[1024];
						sprintf(tsf, "./OTF_Files/OTF_%ld_%06ld.txt", ev[i].time.tv_sec, ev[i].time.tv_usec);
						
						fp = fopen(tsf, "a");
						
						fprintf(fp, "%s\n", "{");
						fprintf(fp, "%s\n", "  \"pointerType\": \"PEN\",");
						fprintf(fp, "%s%d%s\n", "  \"pointerId\": ", 1, ",");
						
						fprintf(fp, "%s\n%s", "  \"x\": [", "    ");
						int ii;
						for(ii = 0; ii < MAX; ii++)
						{
                            fprintf(fp,"%d", x_val[ii]);
                            if(ii < MAX - 1) {
                                fprintf(fp,"%s", ", ");
                            }
                            if((ii+1)%15 == 0 && ii < MAX - 1) {
                                fprintf(fp,"%s", "\n    ");
                            }
						}
						fprintf(fp, "\n%s\n", "  ],");
                        
						fprintf(fp, "%s\n%s", "  \"y\": [", "    ");
						int jj;
						for(jj = 0; jj < MAX; jj++)
						{
                            fprintf(fp,"%d", y_val[jj]);
                            if(jj < MAX - 1) {
                                fprintf(fp,"%s", ", ");
                            }
                            if((jj+1)%15 == 0 && jj < MAX - 1) {
                                fprintf(fp,"%s", "\n    ");
                            }
						}
						fprintf(fp, "\n%s\n", "  ],");
                        
						fprintf(fp, "%s\n%s", "  \"t\": [", "    ");
						int kk;
						for(kk = 0; kk < MAX; kk++)
						{
                            fprintf(fp,"%.6f", t_val[kk]);
                            if(kk < MAX - 1) {
                                fprintf(fp,"%s", ", ");
                            }
                            if((kk+1)%5 == 0 && kk < MAX - 1) {
                                fprintf(fp,"%s", "\n    ");
                            }
						}
						fprintf(fp, "\n%s\n", "  ],");
						
						fprintf(fp, "%s\n%s", "  \"p\": [", "    ");
						int ll;
						for(ll = 0; ll < MAX; ll++)
						{
                            fprintf(fp,"%d", p_val[ll]);
                            if(ll < MAX - 1) {
                                fprintf(fp,"%s", ", ");
                            }
                            if((ll+1)%30 == 0 && ll < MAX - 1) {
                                fprintf(fp,"%s", "\n    ");
                            }
						}
						fprintf(fp, "\n%s\n", "  ]");
						fprintf(fp, "%s", "}");
						
						fclose(fp);
						
						free(x_val);
						free(y_val);
						free(t_val);
						free(p_val);
						
						MAX = 0;
						array_ele_no = 0;
						//---------------------------------------------------
						
					}else if ((js_flag == 0) && (js_value == 1)) {
						js_flag = 1;
					}
				}
				
				// =================================================================================================================
#endif
#ifdef BLACKBOARD_CHANGES
				if((strcmp(names[ev[i].type][ev[i].code],"Touch")==0) && ev[i].value==1){
					if(ind==WRITING){
						toWrite=1;
					}
					else if(ind==MENUBOX){
						alreadySent=0;
					}
					else if(ind==HYBRID){
						toWrite=1;
						alreadySent=0;
					}
				}
				else if((strcmp(names[ev[i].type][ev[i].code],"Touch")==0) && ev[i].value==0){
					toWrite=0;
					pair=0;
					count=0;
					xF=0;yF=0;
					continue;
				}
				if((strcmp(names[ev[i].type][ev[i].code],"X")==0) || (strcmp(names[ev[i].type][ev[i].code],"Y")==0)){
					count++;
					if(((pair==1)&&(xF==1)&&((strcmp(names[ev[i].type][ev[i].code],"Y")==0))) ||
						((pair==1)&&(yF==1)&&((strcmp(names[ev[i].type][ev[i].code],"X")==0)))) {
						pair=2;
						count=0;
					}
					if(count==1){
						pair=1;
					}
					else if(count==2){
						pair=0;
						count=0;
						xF=0;yF=0;
						continue;
					}
				}
				if(strcmp(names[ev[i].type][ev[i].code],"X")==0){
					xF=1;
					xx=ev[i].value;
				}
				else
				if(strcmp(names[ev[i].type][ev[i].code],"Y")==0){
					yF=1;
					yy=ev[i].value;
				}
				if(pair==2 ){
					pair=0;
					xF=0;
					yF=0;
#ifdef SENSE_LOGS
					DPRINT(INFO,"Pen Sense Thread Mode=%d\n",ind);
#endif
					if(ind==WRITING){
						if(toWrite)
						{
							//xxx=round(xx/11.02);
							//yyy=round(yy/11.02);
							xxx=round(xx/11.2);
							yyy=round(yy/11.2);
							write_epd(1,1872-xxx,1404-yyy,be,bx,by,bw,bh);
						}
					}
					else if(ind==MENUBOX){
						//sending co-ordinates
						if(!alreadySent)
						{
							//xxx=round(xx/11.02);
							//yyy=round(yy/11.02);
							xxx=round(xx/11.2);
							yyy=round(yy/11.2);
							penTouchData(1872-xxx,1404-yyy);
							alreadySent=1;
						}
					}
					else if(ind==HYBRID){
						//xxx=round(xx/11.02);
						//yyy=round(yy/11.02);
						xxx=round(xx/11.2);
						yyy=round(yy/11.2);
						xxx=1872-xxx;
						yyy=1404-yyy;
#ifdef SENSE_LOGS
						DPRINT(INFO,"xxx=%d,yyy=%d,bx=%d,by=%d,bw=%d,bh=%d\n",
								xxx,yyy,bx,by,bw,bh);
#endif
						if(((xxx>bx)&&(xxx<(bx+bw))) &&
		    					((yyy>by)&&(yyy<(by+bh)))){
							alreadySent=1;
							if(toWrite)
							{
								write_epd(1,xxx,yyy);
							}
						}
						else{
							//sending co-ordinates
							if(!alreadySent)
							{
								penTouchData(xxx,yyy);
								alreadySent=1;
							}
						}
			
					}
				}
#endif
			}
        
	}
}

#ifndef MENU_H_
#define MENU_H_

#define hInfoCreative (*(int*)(0x0000213C))

#define AUTOREPEAT_DELAY_UNIT   50
#define AUTOREPEAT_DELAY_LONG   10
#define AUTOREPEAT_DELAY_SHORT   4

typedef enum {
	MENUITEM_AV_COMP,
	MENUITEM_FLASH_COMP,
	MENUITEM_AEB,
	MENUITEM_SAFETY_SHIFT,
	MENUITEM_RELEASE_COUNT,
	MENUITEM_ISO_VIEWFINDER,
	MENUITEM_WHITE_BALANCE,
	MENUITEM_EMIT_FLASH,
	MENUITEM_AF_FLASH,
	MENUITEM_DP_BUTTON,
	MENUITEM_WAVE,
	MENUITEM_EAEB,
	MENUITEM_INTERVAL,
	MENUITEM_REMOTE_DELAY,
	MENUITEM_TIMER,
	MENUITEM_COUNT,
	MENUITEM_FIRST = 0,
	MENUITEM_LAST  = MENUITEM_COUNT - 1
} type_MENUITEM;

typedef enum {
	MENUITEM_WAVE_DELAY,
	MENUITEM_WAVE_ACTION,
	MENUITEM_WAVE_COUNT,
	MENUITEM_WAVE_FIRST = 0,
	MENUITEM_WAVE_LAST  = MENUITEM_WAVE_COUNT - 1
} type_MENUITEM_WAVE;

typedef enum {
	MENUITEM_EAEB_FRAMES,
	MENUITEM_EAEB_EV,
	MENUITEM_EAEB_DELAY,
	MENUITEM_EAEB_M_MIN,
	MENUITEM_EAEB_M_MAX,
	MENUITEM_EAEB_COUNT,
	MENUITEM_EAEB_FIRST = 0,
	MENUITEM_EAEB_LAST  = MENUITEM_EAEB_COUNT - 1
} type_MENUITEM_EAEB;

typedef enum {
	MENUITEM_INTERVAL_DELAY,
	MENUITEM_INTERVAL_TIME,
	MENUITEM_INTERVAL_EAEB,
	MENUITEM_INTERVAL_SHOTS,
	MENUITEM_INTERVAL_COUNT,
	MENUITEM_INTERVAL_FIRST = 0,
	MENUITEM_INTERVAL_LAST  = MENUITEM_INTERVAL_COUNT - 1
} type_MENUITEM_INTERVAL;

typedef enum {
	MENUITEM_TIMER_DELAY,
	MENUITEM_TIMER_ACTION,
	MENUITEM_TIMER_COUNT,
	MENUITEM_TIMER_FIRST = 0,
	MENUITEM_TIMER_LAST  = MENUITEM_TIMER_COUNT - 1
} type_MENUITEM_TIMER;

extern void menu_initialize();

extern void menu_toggle();
extern void menu_cycle();
extern void menu_up();
extern void menu_down();
extern void menu_right();
extern void menu_left();

extern void menu_save();

extern char *menu_message();

#endif /* MENU_H_ */

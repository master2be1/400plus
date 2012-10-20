/**
 * $Revision$
 * $Date$
 * $Author$
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include "menu.h"

typedef struct {
	int  length;
	char **data;
} type_LIST;

typedef void(*type_MENUITEM_TASK)(type_MENUITEM *item);

typedef struct {
	int  *value;
	int   can_do_zero;
	int   zero_means_off;
} type_MENUITEM_EC;

typedef struct {
	int  *value;
} type_MENUITEM_AV;

typedef struct {
	int  *value;
	int   bulb;
} type_MENUITEM_TV;

typedef struct {
	int  *value;
	int   full;
} type_MENUITEM_ISO;

typedef struct {
	int   *value;
	int    min;
	int    max;
	int    small_step;
	int    big_step;
	int    zero_means_unlimited;
	char  *format;
} type_MENUITEM_INT;

typedef struct {
	int       *value;
	int        cycle;
	type_LIST *list;
} type_MENUITEM_ENUM;

typedef struct {
	char *value;
} type_MENUITEM_INFO;

typedef struct {
	type_MENUPAGE *page;
} type_MENUITEM_SUBMENU;

typedef union {
	type_MENUITEM_EC      menuitem_ec;
	type_MENUITEM_AV      menuitem_av;
	type_MENUITEM_TV      menuitem_tv;
	type_MENUITEM_ISO     menuitem_iso;
	type_MENUITEM_INT     menuitem_int;
	type_MENUITEM_ENUM    menuitem_enum;
	type_MENUITEM_INFO    menuitem_info;
	type_MENUITEM_SUBMENU menuitem_submenu;
} type_MENUITEM_PARM;

struct MENUITEM {
	char *name;
	int   readonly;
	type_MENUITEM_PARM  parm;
	void (*display)(const type_MENUITEM *item, char *buffer, const int length);
	void (*inc)    (const type_MENUITEM *item, const int repeating);
	void (*dec)    (const type_MENUITEM *item, const int repeating);
	void (*action) (const type_MENUITEM *item);
	void (*change) (const type_MENUITEM *item);
};

#define OPTIONLIST_DEC(NAME)      extern type_LIST _##NAME##_LIST_;
#define OPTIONLIST_REF(NAME)      (&_##NAME##_LIST_)
#define OPTIONLIST_DEF(NAME, ...) static char *_##NAME##_STRINGS_[]={__VA_ARGS__};  type_LIST _##NAME##_LIST_={length:LENGTH(_##NAME##_STRINGS_),data:_##NAME##_STRINGS_};

OPTIONLIST_DEC(bool)
OPTIONLIST_DEC(delay)
OPTIONLIST_DEC(flash)
OPTIONLIST_DEC(action)
OPTIONLIST_DEC(logfile)
OPTIONLIST_DEC(btnactn)
OPTIONLIST_DEC(direction)
OPTIONLIST_DEC(languages)
OPTIONLIST_DEC(vformat)
OPTIONLIST_DEC(scrind)
OPTIONLIST_DEC(scrlcd)

#define MENUITEM_EC(_NAME_, _VALUE_, _RO_, _CDZ_, _ZMO_, _CHANGE_) { \
	name     : _NAME_, \
	readonly : _RO_, \
	parm     : { menuitem_ec : { \
		value          : _VALUE_, \
		can_do_zero    : _CDZ_, \
		zero_means_off : _ZMO_, \
	}}, \
	display  : menuitem_display_ec, \
	inc      : menuitem_inc_ec, \
	dec      : menuitem_dec_ec, \
	change   : _CHANGE_ \
}

#define MENUITEM_AV(_NAME_, _VALUE_, _CHANGE_) { \
	name  : _NAME_, \
	parm  : { menuitem_av : { \
			value : _VALUE_, \
	}}, \
	display : menuitem_display_av, \
	inc     : menuitem_inc_av, \
	dec     : menuitem_dec_av, \
	change  : _CHANGE_ \
}

#define MENUITEM_TV(_NAME_, _VALUE_, _CHANGE_) { \
	name  : _NAME_, \
	parm  : { menuitem_tv : { \
		value : _VALUE_, \
	}}, \
	display : menuitem_display_tv, \
	inc     : menuitem_inc_tv, \
	dec     : menuitem_dec_tv, \
	change  : _CHANGE_ \
}

#define MENUITEM_BULB(_NAME_, _VALUE_, _CHANGE_) { \
	name  : _NAME_, \
	parm  : { menuitem_tv : { \
		value : _VALUE_, \
	}}, \
	display : menuitem_display_bulb,  \
	inc     : menuitem_inc_bulb, \
	dec     : menuitem_dec_bulb, \
	change  : _CHANGE_ \
}

#define MENUITEM_ISO(_NAME_, _VALUE_, _FULL_, _CHANGE_)  { \
	name  : _NAME_, \
	parm  : { menuitem_iso : { \
		value : _VALUE_, \
		full  : _FULL_, \
	}}, \
	display : menuitem_display_iso, \
	inc     : menuitem_inc_iso, \
	dec     : menuitem_dec_iso, \
	change  : _CHANGE_ \
}

#define MENUITEM_INT(_NAME_, _VALUE_, _RO_, _MIN_, _MAX_, _SMALL_, _BIG_, _ZMU_, _FORMAT_, _CHANGE_) { \
	name     : _NAME_, \
	readonly : _RO_, \
	parm     : {menuitem_int : { \
		value                : _VALUE_, \
		min                  : _MIN_, \
		max                  : _MAX_, \
		small_step           : _SMALL_, \
		big_step             : _BIG_, \
		zero_means_unlimited : _ZMU_, \
		format               : _FORMAT_, \
	}}, \
	display : menuitem_display_int, \
	inc     : menuitem_inc_int, \
	dec     : menuitem_dec_int, \
	change  : _CHANGE_ \
}

#define MENUITEM_TIME(_NAME_, _VALUE_, _RO_, _MIN_, _MAX_, _SMALL_, _BIG_, _CHANGE_) { \
	name     : _NAME_, \
	readonly : _RO_, \
	parm     : {menuitem_int : { \
		value                : _VALUE_, \
		min                  : _MIN_, \
		max                  : _MAX_, \
		small_step           : _SMALL_, \
		big_step             : _BIG_, \
	}}, \
	display : menuitem_display_time, \
	inc     : menuitem_inc_int, \
	dec     : menuitem_dec_int, \
	change  : _CHANGE_ \
}

#define MENUITEM_FLENGTH(_NAME_, _VALUE_, _CHANGE_) { \
	name     : _NAME_, \
	parm     : {menuitem_int : { \
		value                : _VALUE_, \
		format               : "%i", \
	}}, \
	display : menuitem_display_int, \
	inc     : menuitem_inc_flen, \
	dec     : menuitem_dec_flen, \
	change  : _CHANGE_ \
}

#define MENUITEM_ENUM(_NAME_, _VALUE_, _CYCLE_, _TEXTS_, _CHANGE_)  { \
	name  : _NAME_, \
	parm  : { menuitem_enum : { \
		value : _VALUE_, \
		cycle : _CYCLE_, \
		list  : _TEXTS_, \
	}}, \
	display : menuitem_display_enum, \
	inc     : menuitem_inc_enum, \
	dec     : menuitem_dec_enum, \
	change  : _CHANGE_ \
}

#define MENUITEM_LAUNCH(_NAME_, _ACTION_)  { \
	name  : _NAME_, \
	display : menuitem_display,  \
	action  : _ACTION_, \
}

#define MENUITEM_SUBMENU(_NAME_, _PAGE_, _ACTION_) { \
	name  : _NAME_, \
	parm  : { menuitem_submenu : { \
		page : _PAGE_, \
	}}, \
	display : menuitem_display_sub, \
	inc     : menuitem_inc_sub, \
	action  : _ACTION_, \
}

#define MENUITEM_PAGE(_NAME_) { \
	name    : _NAME_, \
	display : menuitem_display, \
}

#define MENUITEM_INFO(_NAME_, _VALUE_) { \
	name  : _NAME_, \
	parm  : { menuitem_info : { \
		value : _VALUE_, \
	}}, \
	display : menuitem_display_info, \
}

#define MENUITEM_EVCOMP(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_EC(_NAME_, _VALUE_, false, true,  false, _ON_CHANGE_)
#define MENUITEM_EVSEP( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_EC(_NAME_, _VALUE_, false, true,  true,  _ON_CHANGE_)
#define MENUITEM_EVEAEB(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_EC(_NAME_, _VALUE_, false, false, true,  _ON_CHANGE_)
#define MENUITEM_EVINFO(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_EC(_NAME_, _VALUE_, true,  true,  false, _ON_CHANGE_)

#define MENUITEM_BASEISO(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ISO(_NAME_, _VALUE_, true,  _ON_CHANGE_)
#define MENUITEM_FULLISO(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ISO(_NAME_, _VALUE_, false, _ON_CHANGE_)

#define MENUITEM_BOOLEAN( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(bool),      _ON_CHANGE_)
#define MENUITEM_AFFLASH( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(flash),     _ON_CHANGE_)
#define MENUITEM_ACTION(  _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(action),    _ON_CHANGE_)
#define MENUITEM_LOGFILE( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(logfile),   _ON_CHANGE_)
#define MENUITEM_BTNACTN( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(btnactn),   _ON_CHANGE_)
#define MENUITEM_EAEBDIR( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(direction), _ON_CHANGE_)
#define MENUITEM_LANG(    _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(languages), _ON_CHANGE_)
#define MENUITEM_VFORMAT( _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(vformat),   _ON_CHANGE_)
#define MENUITEM_SCRIND(  _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(scrind),    _ON_CHANGE_)
#define MENUITEM_SCRLCD(  _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_ENUM(_NAME_, _VALUE_, true, OPTIONLIST_REF(scrlcd),    _ON_CHANGE_)

#define MENUITEM_CLRTEMP(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_INT(_NAME_, _VALUE_, false, 1800, 11000, 100, 500, false, "%5u", _ON_CHANGE_)
#define MENUITEM_COUNTER(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_INT(_NAME_, _VALUE_, false,    0,   250,   1,  10, true,  "%3u", _ON_CHANGE_)
#define MENUITEM_BRACKET(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_INT(_NAME_, _VALUE_, false,    3,     9,   2,   2, false, "%1u", _ON_CHANGE_)
#define MENUITEM_FDIST(  _NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_INT(_NAME_, _VALUE_, false,    1,  1000,   1,  10, false, "%4u", _ON_CHANGE_)

#define MENUITEM_PARAM(_NAME_, _VALUE_) MENUITEM_INT(_NAME_, _VALUE_, true, 0, 0, 0, 0, false, "%u", NULL)

#define MENUITEM_TIMEOUT(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_TIME(_NAME_, _VALUE_, false,    1,   300,   1,  10, _ON_CHANGE_)
#define MENUITEM_LONGEXP(_NAME_, _VALUE_, _ON_CHANGE_) MENUITEM_TIME(_NAME_, _VALUE_, false,   15,  6000,  15,  60, _ON_CHANGE_)
#define MENUITEM_INFTIME(_NAME_, _VALUE_)              MENUITEM_TIME(_NAME_, _VALUE_, true,     1,   100,   1,   5, NULL)

extern void menuitem_display      (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_ec   (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_av   (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_tv   (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_bulb (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_iso  (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_int  (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_time (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_enum (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_info (const type_MENUITEM *item, char *buffer, const int length);
extern void menuitem_display_sub  (const type_MENUITEM *item, char *buffer, const int length);

extern void menuitem_inc_ec   (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_av   (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_tv   (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_bulb (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_iso  (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_int  (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_flen (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_enum (const type_MENUITEM *item, const int repeating);
extern void menuitem_inc_sub  (const type_MENUITEM *item, const int repeating);

extern void menuitem_dec_ec   (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_av   (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_tv   (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_bulb (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_iso  (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_int  (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_flen (const type_MENUITEM *item, const int repeating);
extern void menuitem_dec_enum (const type_MENUITEM *item, const int repeating);

#endif /* MENUITEM_H_ */

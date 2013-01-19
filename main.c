/**
 * $Revision$
 * $Date$
 * $Author$
 */

#include <stdio.h>
#include <stdbool.h>

#include "macros.h"
#include "firmware.h"

#include "actions.h"
#include "af_patterns.h"
#include "button.h"
#include "cmodes.h"
#include "display.h"
#include "languages.h"
#include "menu.h"
#include "menu_main.h"
#include "menu_rename.h"
#include "utils.h"
#include "viewfinder.h"

#include "main.h"

// Main message queue
int *action_queue;

// Global status
status_t status = {
	button_down       : BUTTON_NONE,
	script_running    : false,
	script_stopping   : false,
	iso_in_viewfinder : false,
	afp_dialog        : false,
	measuring         : false,
	ev_comp           : 0x00,
};

// Proxy listeners
int proxy_script_restore (char *message);
int proxy_script_shot    (char *message);
int proxy_script_stop    (char *message);
int proxy_set_language   (char *message);
int proxy_dialog_enter   (char *message);
int proxy_dialog_exit    (char *message);
int proxy_dialog_afoff   (char *message);
int proxy_measuring      (char *message);
int proxy_measurement    (char *message);
int proxy_settings0      (char *message);
int proxy_settings3      (char *message);
int proxy_button         (char *message);
int proxy_wheel          (char *message);
int proxy_initialize     (char *message);

proxy_t listeners_script[0x100] = {
	[IC_SHUTDOWN]  = proxy_script_restore,
	[IC_SHOOTING]  = proxy_script_shot,
	[IC_BUTTON_DP] = proxy_script_stop,
};

proxy_t listeners_menu[0x100] = {
	[IC_DIALOGOFF]     = proxy_dialog_exit,
	[IC_BUTTON_WHEEL]  = proxy_wheel,
	[IC_BUTTON_DISP]   = proxy_button,
	[IC_BUTTON_SET]    = proxy_button,
	[IC_BUTTON_RIGHT]  = proxy_button,
	[IC_BUTTON_LEFT]   = proxy_button,
	[IC_BUTTON_DP]     = proxy_button,
	[IC_BUTTON_AV]     = proxy_button,
};

proxy_t listeners_main[0x100] = {
	[IC_SET_LANGUAGE]  = proxy_set_language,
	[IC_DIALOGON]      = proxy_dialog_enter,
	[IC_MEASURING]     = proxy_measuring,
	[IC_MEASUREMENT]   = proxy_measurement,
	[IC_UNKNOWN_8D]    = proxy_initialize,
	[IC_SETTINGS_0]    = proxy_settings0,
	[IC_SETTINGS_3]    = proxy_settings3,
	[IC_AFPDLGOFF]     = proxy_dialog_afoff,
	[IC_BUTTON_WHEEL]  = proxy_wheel,
	[IC_BUTTON_DISP]   = proxy_button,
	[IC_BUTTON_SET]    = proxy_button,
	[IC_BUTTON_UP]     = proxy_button,
	[IC_BUTTON_DOWN]   = proxy_button,
	[IC_BUTTON_RIGHT]  = proxy_button,
	[IC_BUTTON_LEFT]   = proxy_button,
	[IC_BUTTON_DP]     = proxy_button,
	[IC_BUTTON_AV]     = proxy_button,
};

button_t message2button[0x100] = {
	[IC_BUTTON_DISP]   = BUTTON_DISP,
	[IC_BUTTON_SET]    = BUTTON_SET,
	[IC_BUTTON_UP]     = BUTTON_UP,
	[IC_BUTTON_DOWN]   = BUTTON_DOWN,
	[IC_BUTTON_RIGHT]  = BUTTON_RIGHT,
	[IC_BUTTON_LEFT]   = BUTTON_LEFT,
	[IC_BUTTON_DP]     = BUTTON_DP,
	[IC_BUTTON_AV]     = BUTTON_AV,
};

void action_dispatcher();
void message_logger (char *message);

void initialize() {
	action_queue = (int*)CreateMessageQueue("action_queue", 0x40);
	CreateTask("Action Dispatcher", 25, 0x2000, action_dispatcher, 0);
}

void initialize_display() {
	if (!status.script_running)
		enqueue_action(restore_display);
}

void intercom_proxy(const int handler, char *message) {
	proxy_t  listener;
	proxy_t *listeners;

#ifdef ENABLE_DEBUG
	message_logger(message);
#endif

	// Fast path for the case of a running script
	if (status.script_running)
		listeners = listeners_script;
	else if (status.menu_running)
		listeners = listeners_menu;
	else
		listeners = listeners_main;

	if ((listener = listeners[message[1]]) != NULL)
		if (listener(message))
			return;

	IntercomHandler(handler, message);
}

void action_dispatcher () {
	action_t action;

	// Loop while receiving messages
	for (;;) {
		ReceiveMessageQueue(action_queue, &action, false);
		action();
	}
}

void enqueue_action(action_t action) {
	TryPostMessageQueue(action_queue, (action), false);
}

void message_logger(char *message) {
	int i;
	char text[256];
	static int id = 0;

	for (i = 0; i < message[0]; i++)
		sprintf(text + 3 * i, "%02X ", message[i]);

	printf_log(8, 8, "[400plus-MSG%04d-%02X]: %s", id++, FLAG_GUI_MODE, text);
}

int proxy_script_restore(char *message) {
	script_restore();

	return false;
}

int proxy_script_shot(char *message) {
	status.last_shot_tv = message[2];
	status.last_shot_av = message[3];

	return false;
}

int proxy_script_stop(char *message) {
	status.script_stopping = true;

	return true;
}

int proxy_set_language(char *message) {
	enqueue_action(lang_pack_config);

	return false;
}

int proxy_dialog_enter(char *message) {
	status.afp_dialog = (message[2] == IC_SET_AF);

	return false;
}

int proxy_dialog_exit(char *message) {
	enqueue_action(menu_event_finish);

	return false;
}

int proxy_dialog_afoff(char *message) {
	if (status.afp_dialog) {
		// Open Extended AF-Point selection dialog
		message[1] = IC_AFPDLGON;
		status.afp_dialog = false;
		enqueue_action(afp_enter);
	}

	return false;
}

int proxy_measuring(char *message) {
	status.measuring = message[2];
	enqueue_action(display_overlay);

	return false;
}

int proxy_measurement(char *message) {
	if (status.measuring) {
		status.measured_tv = message[2];
		status.measured_av = message[3];
		status.measured_ec = message[4];

		if (settings.autoiso_enable)
			enqueue_action(autoiso);
	}

	enqueue_action(display_overlay);

	return false;
}

int proxy_initialize(char *message) {
	static int first = true;

	if (first) {
		first = false;

		enqueue_action(start_up);
		enqueue_action(cmode_recall);
	}

	return false;
}

int proxy_settings0(char *message) {
	static int first = true;

	if (status.ignore_ae_change) {
		status.ignore_ae_change = false;
	} else {
		status.main_dial_ae = message[2];

		if (first) {
			first = false;
		} else {
			enqueue_action(cmode_apply);
		}
	}

	return false;
}

int proxy_settings3(char *message) {
	enqueue_action(restore_display);

	return false;
}

int proxy_button(char *message) {
	return button_handler(message2button[message[1]], message[0] > 3 ? message[2] : true);
}

int proxy_wheel(char *message) {
	return button_handler((message[2] & 0x80) ? BUTTON_WHEEL_LEFT : BUTTON_WHEEL_RIGHT, true);
}

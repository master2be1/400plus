#include "main.h"
#include "utils.h"
#include "settings.h"
#include "firmware.h"

#include "viewfinder.h"

// Temporary storage while displaying ISO at viewfinder
struct {
	int TvVal;
	int CfNotEmitFlash;
} viewfinder_storage;

void viewfinder_change_iso(const int iso);

void viewfinder_right() {
	viewfinder_change_iso(iso_inc(cameraMode.iso));
}

void viewfinder_left() {
	viewfinder_change_iso(iso_dec(cameraMode.iso));
}

void viewfinder_end() {
	// Only if being displayed
	if (status.iso_in_viewfinder) {
		// Restore previous state
		SendToIntercom(EVENT_SET_CFN_NOFLASH, 1, viewfinder_storage.CfNotEmitFlash);
		SendToIntercom(EVENT_SET_TV,          1, viewfinder_storage.TvVal);

		// Reset flag, viewfinder restored
		status.iso_in_viewfinder = FALSE;
	}
}

void viewfinder_change_iso(const int iso) {
	// Only if active and mode is P, Tv, Av, M
	if (settings.iso_in_viewfinder) {
		if (cameraMode.ae == AE_MODE_M || cameraMode.ae == AE_MODE_TV) {
			// Save current state
			viewfinder_storage.TvVal          = cameraMode.tv_val;
			viewfinder_storage.CfNotEmitFlash = cameraMode.cf_emit_flash;

			// Change to Tv=ISO, no flash
			SendToIntercom(EVENT_SET_CFN_NOFLASH, 1, 1);
			SendToIntercom(EVENT_SET_TV,          1, iso + 0x25);

			// Set flag to restore viewfinder later
			status.iso_in_viewfinder = TRUE;
		}

		// Set new ISO
		SendToIntercom(EVENT_SET_ISO, 2, iso);
	}
}


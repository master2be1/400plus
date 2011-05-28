#include "main.h"
#include "firmware.h"
#include "debug.h"

int * mainctrl;

extern void my_task_MainCtrl(int * msg);

void __my_task_MainCtrl() {
	//(*((int*)0xC0220000)) = 0x46; // turn on the blue led
	//eventproc_EdLedOn(); // we cannot use this here, the tasks are not created yet...
	//task_MainCtrl();
	int msg;

	//SleepTask(5000);
	printf("\n\nAF: proxy started\n\n");
	//DeleteTask(mainctrl);
	//beep();
	while (1) {
		ReceiveMessageQueue(hMainMessQueue, &msg, 0);
		printf("\n\nMC: Message[0x%08X] Number: %02d, MC_State: 0x%04X, MC_T_Table: [t=%04d,act/sw=%08X,2=%08X]\n",
				&msg, msg, MC_State, MC_T_Table[msg*3], MC_T_Table[msg*3+1], MC_T_Table[msg*3+2]  );
		//blink_blue();
		my_task_MainCtrl(&msg);
		//blink_red();
	}

}


void my_MainCtrlInit() {
	hMainCtrlMonoSem = CreateBinarySemaphore("MonoSem", 0);

	MC_dword_2A520 = 0;
	MC_dword_259C0 = 0;
	MC_State = 1;
	MC_dword_26940 = 0;
	MC_dword_27BE0 = 0;

	PictureStyleInit();
	sub_FF825AE4();
	SetEmergencyProc(proc_CardDoor_Emergency, 0);
	SetCardDoorProc(proc_CardDoor_Emergency, 0);
	TryPostMessageQueueFds_7(SendToMC_T_28, 0);
	SetErrorDetectActSweepProc(ErrorDetectActSweep);
	hMainMessQueue = CreateMessageQueue("MainMessQueue", 0x64);
	hMainDataQueue = CreateMessageQueue("MainDataQueue", 0xC8);
	//mainctrl = CreateTask("MainCtrl", 0x15, 0x4000, my_task_MainCtrl, 0);
	CreateTask("MainCtr1", 0x15, 0x4000, __my_task_MainCtrl, 0);
	DebugProcsInit();
	CreateInterComQueue();
	MC_InitStart();
	IntercomHandlersInit();
	HandlersInit();
	ShootMainHandlerInit();
	sub_FF822DC0();
	sub_FF82296C();
	SetDprPrinterProperty();
	MC_RegisterEventProcedures();

	if (GetMainPreserveData_field_1C_LSR30()==0)
		goto loc_FF81BD8C;
	
	some_with_FixFacePos_0(0);

	unsigned char localvar1[0x198];
	unsigned char localvar2[0x194];

	memset(localvar1, 0, 0x198);
	localvar1[0x174] = 1;
	localvar2[0x174] = 1;

	SendMessageDprRequestChangeData(localvar1, localvar2);

loc_FF81BD8C:
	if (sub_FF823F34() != 0)
		goto out;

	TakeSemaphore(hMainCtrlMonoSem, 0);
	dword_1C78 = 1;

	if (get_0x1CCC() == 0)
		goto givesem;
	
	AbortDcfOperation();
	change_playback_file_id(1, 1);
	sub_FF823A44();

givesem:
	GiveSemaphore(hMainCtrlMonoSem);

out:
	return;
}

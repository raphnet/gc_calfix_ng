#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <dinputd.h>

LPDIRECTINPUT8          g_pDI = NULL;
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;

static int g_num_fixed = 0;

int fixJoystickCalibration(void)
{
	HRESULT hr;
	DIPROPCAL cpoints;
	DIDEVCAPS devcaps;
	int x;
	int axes[6] = { DIJOFS_X, DIJOFS_Y,
					DIJOFS_RX, DIJOFS_RY, 
					DIJOFS_RZ, DIJOFS_SLIDER(0) };

	if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick ) ) )
		return -1;
	
	//g_pJoystick->Acquire();

	devcaps.dwSize = sizeof(devcaps);
	hr = g_pJoystick->GetCapabilities(&devcaps);
	if (FAILED(hr)) {
		//MessageBox(NULL, _com_error(GetLastError()).ErrorMessage(), L"asd", MB_ICONWARNING | MB_OK);
		printf("*** ERROR ****\n");
	}
	else {
		printf("	Axes: %d, Buttons: %d\n", devcaps.dwAxes, devcaps.dwButtons);
	}	

	cpoints.diph.dwSize = sizeof(cpoints);
	cpoints.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	cpoints.diph.dwHow = DIPH_BYOFFSET;	

	for (x=0; x<6; x++) {
		//cpoints.diph.dwObj = x*4;
		cpoints.diph.dwObj = axes[x];
	
		hr = g_pJoystick->GetProperty(DIPROP_CALIBRATION, &cpoints.diph);
		if (FAILED(hr)) {
			continue;
		}
	
		printf("	%d: %04x %04x %04x\n", x, cpoints.lMin, cpoints.lCenter, cpoints.lMax);		

		if (x==4 || x==5) {
			cpoints.lCenter = cpoints.lMax;
			cpoints.lMax = 0xff;
		
			g_pJoystick->SetProperty(DIPROP_CALIBRATION, &cpoints.diph);
	
		}
		
	}
	
	g_num_fixed++;

	return 0;
}


BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*     
                                       pdidInstance, VOID* pContext)
{
    HRESULT hr;
	DIPROPDWORD vidpid;
	WORD vid, pid;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice(pdidInstance->guidInstance,  
                                &g_pJoystick, NULL);
    if(FAILED(hr)) 
        return DIENUM_CONTINUE;

	/* Test */
	vidpid.diph.dwSize = sizeof(vidpid);
	vidpid.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	vidpid.diph.dwHow = DIPH_DEVICE;
	vidpid.diph.dwObj = 0;
	hr = g_pJoystick->GetProperty(DIPROP_VIDPID, &vidpid.diph);
	if (FAILED(hr)) {
		return DIENUM_CONTINUE;
	}

	vid = vidpid.dwData & 0xffff;
	pid = vidpid.dwData >> 16;

	

	if (vid==0x1781 && pid==0x0a9a) {
		printf("Found Device ID : VID=%04x  PID=%04x (ADAP-GCN64 v1.x)\n", vid, pid);
		fixJoystickCalibration(); // uses global
	}
	else if (vid==0x1740 && pid==0x057f) {
		printf("Found Device ID : VID=%04x  PID=%04x (ADAP-GCN64 v2.1)\n", vid, pid);
		fixJoystickCalibration(); // uses global
	}
	else if (vid==0x289b && pid==0x0001) {
		printf("Found Device ID : VID=%04x  PID=%04x (ADAP-GCN64 v2.2)\n", vid, pid);
		fixJoystickCalibration(); // uses global
	}
	else {
		printf("Ignoring Device ID : VID=%04x  PID=%04x\n", vid, pid);
		
	}

	return DIENUM_CONTINUE;	
}

int main(int argc, const char * argv[])
{
	HRESULT res;

	cout << "raphnet.net Gamecube adapter L/R buttons calibration fixer v0.2" << endl;
	cout << "Copyright (C) 2009-2013, Raphael Assenat" << endl << endl;

	res = DirectInput8Create(GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, 
		(VOID **)&g_pDI, NULL);
	
	if (FAILED(res)) {
		printf("DirectInput8Create failed\n");
		return -1;
	}

	g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
						NULL, DIEDFL_ATTACHEDONLY);

	if (!g_num_fixed) {
		printf("Modified no joystick.");
	}
	else {
		printf("Modified %d joystick(s).\n", g_num_fixed);
		
		//g_pJoystick->RunControlPanel(0,0);
	}


	cout << endl << "-- Press ENTER to exit --" << endl;
	getchar();

	return 0;
}


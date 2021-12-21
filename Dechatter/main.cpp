//#include <iostream>
#include <windows.h>
#include <chrono>

DWORD lastPressedVirtKey;
int keyUpFlg = 0;
//uint64_t lastPressedTime;
auto timer = std::chrono::steady_clock::now();



LRESULT CALLBACK PressItHook(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
) {
		if (wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
			DWORD wVirtKey = kbdStruct->vkCode;
			auto timer2 = std::chrono::steady_clock::now();
			if (wVirtKey != lastPressedVirtKey) {
				if (wVirtKey != (DWORD)231) {
					lastPressedVirtKey = wVirtKey;
					keyUpFlg = 0;
					timer = timer2;
				}
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			uint64_t elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(timer2-timer).count();
			//std::cout << wVirtKey << std::endl;
			if (elapsedTime < 100 && elapsedTime > 10 && keyUpFlg) {
				//std::cout << "I did my job" << std::endl;
			}
			else {
				timer = timer2;
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
		}
		else if (wParam == WM_KEYUP) {
			KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
			DWORD wVirtKey = kbdStruct->vkCode;
			keyUpFlg = wVirtKey == lastPressedVirtKey;
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
		//else(wParam == WM_KEYDOWN) {

		//}

}

int main()
{
	HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, PressItHook, NULL, 0);

	if (hHook) {
		while (GetMessage(NULL, NULL, 0, 0));
	}

	return 0;
}
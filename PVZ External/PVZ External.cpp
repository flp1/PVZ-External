#include "pch.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

int main()
{
	HANDLE hProcess = 0;

	uintptr_t moduleBase = 0, sunAddress = 0, DynamicSunAddress = 0;
	bool bCooldown = false;

	//Get ProcId of the target process
	DWORD procId = GetProcId(L"popcapgame1.exe");
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "      Plants vs. Zombies - External Hack" << std::endl;
	std::cout << "      (NUMPAD-1) UNLIMITED SUN" << std::endl;
	std::cout << "      (NUMPAD-2) NO COOLDOWN" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl << std::endl;
	std::cout << "Getting stuff ready...";
	Sleep(1500);
	std::cout << " DONE!"<< std::endl;

	std::cout << "Process ID Identified: " << procId << std::endl;

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		moduleBase = GetModuleBaseAddress(procId, L"popcapgame1.exe");

		sunAddress = 0x00731C50;
		std::cout << "Press 'INSERT' to exit.\n" << std::endl;
	}
	else
	{
		std::cout << "Process not found, press enter to exit.\n";
		getchar();
		return 0;
	}


	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		// Get Unlimited SUN
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			int newSun = 9999;

			// Formar o Dynamic Point address da SUN
			std::vector<unsigned int> sunOffsets = { 0x868, 0x5578 };
			DynamicSunAddress = FindDMAAddy(hProcess, sunAddress, sunOffsets);
			std::cout << "DynamicSunAddress: " << "0x" << DynamicSunAddress << std::endl;

			//Read Sun value
			int sunValue = 0;

			ReadProcessMemory(hProcess, (BYTE*)DynamicSunAddress, &sunValue, sizeof(sunValue), nullptr);
			//std::cout << "Current SUN amount = " << std::dec << sunValue << std::endl;

			//Write to it
			WriteProcessMemory(hProcess, (BYTE*)DynamicSunAddress, &newSun, sizeof(newSun), nullptr);

			//Read out again
			ReadProcessMemory(hProcess, (BYTE*)DynamicSunAddress, &sunValue, sizeof(sunValue), nullptr);
			//std::cout << "New Sun amount = " << std::dec << sunValue << std::endl;

			std::cout << "1 - UNLIMITED SUN ACTIVATED" << std::endl;
			
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bCooldown = true;

			if (bCooldown)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x958C5), 2, hProcess);
				std::cout << "2 - NO COOLDOWN ACTIVATED" << std::endl;
			}
		
		}

		// exit hack
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return 0;
		}

		Sleep(10);
	}
	
}

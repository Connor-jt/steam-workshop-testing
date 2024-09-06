// SteamLiteshop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDK/public/steam/steam_api.h"
#include "SDK/public/steam/isteamugc.h"
#include "SDK/public/steam/isteamuser.h"

/*
105600  <- regular terraria steam app ID
1281930 <- tModLoader steam app ID
*/


//-----------------------------------------------------------------------------
// Purpose: callback hook for debug text emitted from the Steam API
//-----------------------------------------------------------------------------
extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText)
{
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
	//::OutputDebugString(pchDebugText);

	if (nSeverity >= 1)
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		(void)x;
	}
}

int main()
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
	{
		// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
		// local Steam client and also launches this game again.

		// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
		// removed steam_appid.txt from the game depot.

		return EXIT_FAILURE;
	}

	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	// You don't necessarily have to though if you write your code to check whether all the Steam
	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
	//
	// This will also load the in-game steam overlay dll into your process.  That dll is normally
	// injected by steam when it launches games, but by calling this you cause it to always load,
	// even when not launched via steam.
	SteamErrMsg errMsg = { 0 };
	if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
	{
		//OutputDebugString("SteamAPI_Init() failed: ");
		//OutputDebugString(errMsg);
		//OutputDebugString("\n");

		//Alert("Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n");
		return EXIT_FAILURE;
	}

	// set our debug handler
	//SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);

	// Ensure that the user has logged into Steam. This will always return true if the game is launched
	// from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
	// will return false.
	if (!SteamUser()->BLoggedOn())
	{
		//OutputDebugString("Steam user is not logged in\n");
		//Alert("Fatal Error", "Steam user must be logged in to play this game (SteamUser()->BLoggedOn() returned false).\n");
		return EXIT_FAILURE;
	}



    std::cout << "Hello World!\n";

    CSteamID m_SteamIDLocalUser = {};
    if (SteamUser()->BLoggedOn())
    {
        m_SteamIDLocalUser = SteamUser()->GetSteamID();
    }


    //SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(1281930u, k_EWorkshopFileTypeMicrotransaction);

    PublishedFileId_t vecSubscribedItems[16];

    int numSubscribedItems = SteamUGC()->GetSubscribedItems(vecSubscribedItems, 16);

    int test = 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

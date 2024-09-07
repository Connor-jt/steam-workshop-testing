// SteamLiteshop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDK/public/steam/steam_api.h"
#include "SDK/public/steam/isteamugc.h"
#include "SDK/public/steam/isteamuser.h"

#include <thread>
using namespace std;
using namespace std::this_thread;
/*
105600  <- regular terraria steam app ID
1281930 <- tModLoader steam app ID
*/


//-----------------------------------------------------------------------------
// Purpose: callback hook for debug text emitted from the Steam API
//-----------------------------------------------------------------------------
extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText){
	std::cout << pchDebugText;
}


// callback when new Workshop item was installed
//STEAM_CALLBACK(CSpaceWarClient, OnWorkshopItemInstalled, ItemInstalled_t);
//void OnUGCQueryCompleted(SteamUGCQueryCompleted_t* pParam, bool bIOFailure);
//CCallResult<CSpaceWarClient, SteamUGCQueryCompleted_t> m_SteamCallResultUGCQueryCompleted;


// a Steam Workshop item
class WorkshopResults{
public:
	int item_count;
	UGCQueryHandle_t ugc_handle;
	PublishedFileId_t* items;

	void OnUGCQueryComplete(SteamUGCQueryCompleted_t* pParam, bool bIOFailure){

		// iterate each mod
		for (int ugc_index = 0; ugc_index < item_count; ugc_index++) {

			// mod details
			SteamUGCDetails_t item_details = {};
			if (!SteamUGC()->GetQueryUGCResult(ugc_handle, ugc_index, &item_details))
				return;
			
			// mod metadata
			char pchMetadata[1024] = {};
			if (!SteamUGC()->GetQueryUGCMetadata(ugc_handle, ugc_index, pchMetadata, 1024))
				return;

			// mod main picture
			const int preview_url_length = 256;
			char preview_url_buffer[preview_url_length] = {};
			if (!SteamUGC()->GetQueryUGCPreviewURL(ugc_handle, ugc_index, preview_url_buffer, preview_url_length))
				return;

			
			// iterate pictures/previews
			uint32 preview_count = SteamUGC()->GetQueryUGCNumAdditionalPreviews(ugc_handle, ugc_index);
			for (int preview_index = 0; preview_index < preview_count; preview_index++) {

				const int url_length = 256;
				char url_buffer[url_length] = {};

				const int filename_length = 256;
				char filename[filename_length] = {};

				EItemPreviewType preview_type = {};

				if (SteamUGC()->GetQueryUGCAdditionalPreview(ugc_handle, ugc_index, preview_index, url_buffer, url_length, filename, filename_length, &preview_type)) {

					int test = 0;

				}
			}

			// mod statistics
			uint64 subscriptions;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumSubscriptions, &subscriptions)) 
				return;
			uint64 favorites;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumFavorites, &favorites)) 
				return;
			uint64 followers;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumFollowers, &followers)) 
				return;
			uint64 unique_subscriptions;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumUniqueSubscriptions, &unique_subscriptions)) 
				return;
			uint64 unique_favorites;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumUniqueFavorites, &unique_favorites)) 
				return;
			uint64 unique_followers;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumUniqueFollowers, &unique_followers)) 
				return;
			uint64 unique_views;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumUniqueWebsiteViews, &unique_views)) 
				return;
			uint64 report_score;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_ReportScore, &report_score)) 
				return;
			uint64 seconds;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumSecondsPlayed, &seconds)) 
				return;
			uint64 sessions;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumPlaytimeSessions, &sessions)) 
				return;
			uint64 comments;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumComments, &comments)) 
				return;
			uint64 seconds_recent;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumSecondsPlayedDuringTimePeriod, &seconds_recent)) 
				return;
			uint64 sessions_recent;
			if (!SteamUGC()->GetQueryUGCStatistic(ugc_handle, ugc_index, k_EItemStatistic_NumPlaytimeSessionsDuringTimePeriod, &sessions_recent)) 
				return;


			// mod children
			if (item_details.m_unNumChildren <= 256) {
				PublishedFileId_t mod_children[256] = {};
				if (!SteamUGC()->GetQueryUGCChildren(ugc_handle, ugc_index, mod_children, item_details.m_unNumChildren))
					return;
			}
		}
	}
	CCallResult<WorkshopResults, SteamUGCQueryCompleted_t> m_SteamCallResultUGCQueryCompleted;
};

void OnUGCQueryCompleted(SteamUGCQueryCompleted_t* pParam, bool bIOFailure);
CCallResult<CSpaceWarClient, SteamUGCQueryCompleted_t> m_SteamCallResultUGCQueryCompleted;


int main(){
	// steam init related stuff
	{
		// replace k_uAppIdInvalid with AppID + remove steam_appid.txt
		if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
			return EXIT_FAILURE;

		SteamErrMsg errMsg = { 0 };
		if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
			return EXIT_FAILURE;

		// debug handler
		SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);

		if (!SteamUser()->BLoggedOn())
			return EXIT_FAILURE;
	}

    CSteamID m_SteamIDLocalUser = SteamUser()->GetSteamID();

	// create workshop results object
	WorkshopResults* results_obj = new WorkshopResults;
	//{
		int subscribed_count = SteamUGC()->GetNumSubscribedItems();
		results_obj->items = new PublishedFileId_t[subscribed_count];
		results_obj->item_count = SteamUGC()->GetSubscribedItems(results_obj->items, subscribed_count);
	//}
	results_obj->ugc_handle = SteamUGC()->CreateQueryUGCDetailsRequest(results_obj->items, results_obj->item_count);
	// configure UGC request
	if (!SteamUGC()->SetReturnLongDescription(results_obj->ugc_handle, true)
	||  !SteamUGC()->SetReturnMetadata(results_obj->ugc_handle, true)
	||  !SteamUGC()->SetReturnAdditionalPreviews(results_obj->ugc_handle, true)
	||  !SteamUGC()->SetReturnChildren(results_obj->ugc_handle, true))
		std::cout << "debug: fail";
	// then send it off??
	SteamAPICall_t API_call = SteamUGC()->SendQueryUGCRequest(results_obj->ugc_handle);
	results_obj->m_SteamCallResultUGCQueryCompleted.Set(API_call, results_obj, &WorkshopResults::OnUGCQueryComplete);

	sleep_for(200ms);
	std::getchar();
	std::getchar();
}

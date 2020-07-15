#include "Discord_Handler.h"

Discord_Handler::Discord_Handler()
{
	discord::Result res = discord::Core::Create(732384839138803794, DiscordCreateFlags_Default, &disCore);

	if (res == discord::Result::Ok)
		discord_running = true;
}

void Discord_Handler::updateRPC(std::string s_state, std::string s_details, std::string s_imagekey, std::string s_imagetxt)
{
	if (discord_running)
	{
		activity.SetState(s_state.c_str());
		activity.SetDetails(s_details.c_str());
		activity.GetAssets().SetLargeImage(s_imagekey.c_str());
		activity.GetAssets().SetLargeText(s_imagetxt.c_str());

		disCore->ActivityManager().UpdateActivity(activity, [](discord::Result) {});
	}
}

void Discord_Handler::updateSDK()
{
	if (disCore->RunCallbacks() != discord::Result::Ok)
		discord_running = false;
	else
		discord_running = true;
}

discord::User Discord_Handler::getUserInfo()
{
	discord::User temp = {};
	if (discord_running)
	{
		disCore->UserManager().GetCurrentUser(&temp);
	}
	return temp;
}
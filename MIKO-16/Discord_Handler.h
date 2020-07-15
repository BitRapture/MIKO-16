#pragma once
#include <string>
#include "discord-files/discord.h"

class Discord_Handler
{
private:
	discord::Core* disCore = {};
	// Discord SDK
	
	discord::Activity activity = {};
	// Discord RPC

	bool discord_running = false;
	// Make sure that discord is fine and dandy

public:
	Discord_Handler();
	// Constructor

	void updateRPC(std::string s_state, std::string s_details, std::string s_imagekey, std::string s_imagetxt);
	// Update Rich Presence

	discord::User getUserInfo();
	// Get Information about the User

	void updateSDK();
	// Update Callbacks
};


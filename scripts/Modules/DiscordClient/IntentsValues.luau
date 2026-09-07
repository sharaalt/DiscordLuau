--[[
    @file IntentsValues.luau
	@author sharafzada
	@date 2026-09-07
	@brief Houses all intents and there related values.

    IntentsValues.luau simply just houses all the discord intents
    and there respectful values. I did not want DiscordClient.luau
    to get too big so I broke it down into this sub-module.
]]

local IntentsValues = {
	["Guilds"] = 1,
	["GuildMembers"] = 2,
	["GuildModeration"] = 4,
	["GuildExpressions"] = 8,
	["GuildIntegrations"] = 16,
	["GuildWebhooks"] = 32,
	["GuildInvites"] = 64,
	["GuildVoiceStates"] = 128,
	["GuildPresences"] = 256,
	["GuildMessages"] = 512,
	["GuildMessageReactions"] = 1024,
	["GuildMessageTyping"] = 2048,

	["DirectMessages"] = 4096,
	["DirectMessageReactions"] = 8192,
	["DirectMessageTyping"] = 16384,

	["MessageContent"] = 32768,

	["GuildScheduledEvents"] = 65536,

	["AutoModerationConfiguration"] = 1048576,
	["AutoModerationExecution"] = 2097152,

	["GuildMessagePolls"] = 16777216,
	["DirectMessagePolls"] = 33554432,
}

return IntentsValues

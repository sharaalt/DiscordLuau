--[[
	@file DiscordClient.luau
	@author sharafzada
	@date 2026-09-06
	@brief Handles basic client creation.

	The DiscordClient handles basic client creation and communicates
	with the C++ networking layer directly. It does NOT handle the 
	networking it only acts as a low-level script between the C++
	layer and luau layer.
]]

local Types = require("./types")

local native = __DiscordNative
local Discord: Types.__DiscordNative = {}

function Discord.connect(token: string)
	native.connect(token)
	print("[DiscordModule] - Successfully connected token to backend.")
end

function Discord.on(eventName: string, callback: Types.callback): ()
	native.on(eventName, callback)
end

return Discord

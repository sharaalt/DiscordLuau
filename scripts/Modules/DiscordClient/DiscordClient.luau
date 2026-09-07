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
local Discord = {}
Discord.__index = Discord

function Discord:setIntents(newIntent: number): ()
	-- Make sure types are in check.
	if typeof(newIntent) == "nil" then
		print("Typeof intent cannot be nil or false.")
		return
	end

	if self.intents then
		self.intents = self.intents + newIntent
	else
		self.intents = newIntent
	end
end

function Discord:connect(token: string): ()
	native.connect(token, self.intents)
	print(self.intents)
	print("Successfully connected token and intents to backend.")
end

function Discord:on(eventName: string, callback: Types.callback): ()
	native.on(eventName, callback)
end

return Discord

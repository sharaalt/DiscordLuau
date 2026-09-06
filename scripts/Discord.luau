local Types = require("./globals")

local native: Types.__DiscordNative = __DiscordNative
local Discord = {}

function Discord.connect(token: string)
	native.connect(token)
	print("[DiscordModule] - Successfully connected token to bacend.")
end

return Discord

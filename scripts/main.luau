local secerts = require("./secrets")
local discord = require("./Modules/DiscordClient/DiscordClient")

discord.connect(secerts.DISCORD_TOKEN)
discord.on("READY", function(data)
	print(data)
	print("c_str()")
end)

discord.on("INTERACTION_CREATE", function(data)
	print(data)
end)

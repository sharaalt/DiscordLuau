local secrets = require("./secrets")
local discord = require("./Modules/DiscordClient/DiscordClient")
local Intents = require("./Modules/DiscordClient/IntentsValues")

local intents = Intents.Guilds + Intents.MessageContent + Intents.GuildMessages

print("New intents: ", intents)

discord:setIntents(intents)
discord:connect(secrets.DISCORD_TOKEN)

discord:on("READY", function(data)
	print("Bot ready.")
end)

discord:on("MESSAGE_CREATE", function(data)
	print("Message recieved: ")

	print(data.content)
end)

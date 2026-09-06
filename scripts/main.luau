local secerts = require("./secrets")
local discord = require("./Discord")

discord.connect(secerts.DISCORD_TOKEN)

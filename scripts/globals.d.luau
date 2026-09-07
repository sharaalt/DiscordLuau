export type callback = (data: any) -> ()

--[[
	Exposes the C++ networking side to the luau side.
]]
declare __DiscordNative: {
	setIntents: (newIntent: number) -> (),
	connect: (token: string) -> (),
	on: (eventName: string, callback: callback) -> (),
}
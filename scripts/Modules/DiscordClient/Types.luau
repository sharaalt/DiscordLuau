local Types = {}

-- Variables
export type data = {
	t: string,
	s: number,
	op: number,
	d: {
		v: number,
		user: {
			id: string,
			username: string,
			discriminator: string,
			bot: boolean,
		},
		guilds: {
			id: string,
			unavailable: boolean,
		},
	},
	session_id: string,
	resume_gateway_url: string,
	application: {
		id: string,
		flags: number,
	},
}

-- Functions
export type callback = (data: data) -> ()

-- Classes
export type __DiscordNative = {
	connect: (token: string) -> (),
	on: (eventName: string, callback: callback) -> (),
}

return Types

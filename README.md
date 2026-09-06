# DiscordLuau
Discord Luau is a Discord API written in luau as well as C++

## Description:
Discord Luau is a translation of the Discord API into luau similar to projects such as [Discordia](https://github.com/SinisterRectus/Discordia), [Discord.js](https://github.com/discordjs/discord.js), etc. Discord Luau works by handling the back-end networking i.e. Interaction with the discord API [gateway](https://docs.discord.com/developers/events/gateway) using C++ and having luau interact with the discord API via communication with the C++ layer. I did networking this way as for me it was a bit more easier for me and, I'd like to learn more C++ along the way as I already have a respectable amount of experience with lua(u) I do recognize there are better alternatives such as [ByteNet Networking](https://ffrostfall.github.io/ByteNet/) however as stated before, I do prefer C++ as the back-end. 

## Dependencies:
- **[luau:](https://github.com/luau-lang/luau)** (it's kind of in the name): v.0.722
- **[Nlohmann JSON:](https://github.com/nlohmann/json)** v3.12.0 - Used to parse JSON for the C++ back-end.
- **[Beast:](https://github.com/boostorg/beast)** v1.19.0 - Used for HTTP requests and web-sockets.
- **[OpenSSL:](https://github.com/openssl/openssl)** v3.6.2 - Used for TLS/SSL encryption.

## To-do list:
### Back-end:
#### Networking:
- [x] Gateway Communication
- [ ] Expose networking functions to luau side
- [x] EventDispatcher for C++
   -  [ ] EventDispatcher integration for luau

### Front-end:
- [x] Basic bot connection
- [ ] Embed Class

## Contributions:
Feel free to contribute as much as you want however if you are making major changes please let me know and, I'll take a look at them before committing!
 
## Build Instructions
### Requirements:
- **CMake:** v3.2.0
- **Visual Studio:** 2022 or newer
- **C++ Standard:** C++20
- **Vcpkg:** Required for dependencies.

To build the repo simply open the repo in command prompt and type the following prompt: 

```cmd
mkdir build && cmake -B Build
```
> Note: The build is quite big ~10gb as of writing this, I didn't know builds could get so big but now I know xD

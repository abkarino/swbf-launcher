
# SWBF Launcher

<div align="center">

[![CC BY-NC 4.0][cc-by-nc-shield]][cc-by-nc]
![Issues](https://img.shields.io/github/issues/abkarino/swbf-launcher)
![Pull Requests](https://img.shields.io/github/issues-pr/abkarino/swbf-launcher)
[![Discord](https://img.shields.io/discord/916218353771741254?logo=discord&logoColor=white)](https://discord.gg/nbFRGGzdPz)

</div>

## Star Wars: Battlefront (2004) Memory Enhancement Launcher

It all started with a childhood love for the game, followed by a dream to keep playing the same game even after more than a decade of its release. Anstafer loved modding the game and pushing zero engine to the limit, but the old engine was designed to run on 64MB rams and in no way reached the maximum for an x32 process. No matter how much you would try to optimize it, it didn't have what it takes to run high resolution models.

The project took almost a year full of reverse engineering, debugging and maps creation specifically to test and identify each limit. It was painful journey in some times but the results came with satisfaction and the output was rewarding.

The final product is a launcher that allows a gamer to select the fixes he wants to apply, launch the game and enjoy it. The launcher was originally bundled with [Enhanced Gameplay and Graphics Mod - EGM](https://www.nexusmods.com/starwarsbattlefront/mods/402) which contained high res models and some gameplay changes.
## Authors

- **@Antstafer** The master mind behind this project
- **@Abkarino** The muscle



## Features

### Fixes

- **Fixed model memory limitation (Low res units fix)**

  This fixes the jarring switch out of hi-res unit models/animations to their low-res counterparts. 
  In vanilla, the max hi-res units you can have around you is 8 at a time. 
  This limitation is removed, allowing for hundreds of hi-res units on screen at once with hi-res animations. No more tumble-weed troops!
    
- **RedEngine Memory Increase**

  Along with playing a role in fixing the model memory, we have raised the memory allowed by RedEngine as a whole, which allows for over 500 million polygons on screen. We have tested this with High LOD BF2 2017 models with over 400 troops, and it doesn't break a sweat! This fix opens up the door for modders to go ham and create beautiful hi-poly assets and effects for the game.
    
- **Raised the total working AI count allowed on the maps**

  Though vanilla has no real limitation on the number of units that can spawn on a map, the max amount of active units total is capped at around 100, however. On larger maps, this causes 50 or so units on each team to stand around doing nothing until they reach their place in the queue. We have done away with this limitation, and now all units are able to be activated by the AI system!
    
- **Command more than 2 AI with the 'Follow Me' key**

  Ever wished you could command more than 2 units? Well we did, which is why you can now command up to 8 units! Now you can finally lead a decent sized raid-party to capture those enemy CPs.
    
- **Fixed Crouch glitch in steam version**

  Have you been playing the Steam version of Battlefront and noticed the crouch button sometimes making you go prone? This annoying bug is now fixed!
    
- **Fixed Effects Limitation (explosions, blaster fire, etc)**

  Raises the total number of allowed effects on screen (blaster fire, explosions, water splash impacts, etc).
    
- **Increased Vehicle limit**

  In vanilla you can have a max of around 20 vehicles on a map. We have removed this limitation, allowing for massive air and tank battles.
    
- **Fixed limitation on foliage distance/amount**

  Raises the distance at which foliage can appear on screen.
    
- **Fixed limitation on decal distance**

  Raises the distance at which decals ( like terrain burns ) can appear on screen, and the amount. Watch as the battlefield progressively gets more destroyed!
    
- **Fixed Sound Memory Limitation**

  Raises the number of SFX channels, reducing the chance of blaster audio cutting out on large battlefields, or mods with lots of sfx.
    
- **Increased AI Shooting distance / Aggro distance**

  Removes the limit of which an AI can detect other AI and the player. Instead of bee-lining towards each other across the map, they will use the "Max Range" values in their weapon ODFs accordingly to account for their aggro-distance and when they will open fire. This can be seen as a true "Hard" mode, where snipers can actually target you across the map, and return fire from the same distances you can. No more will they allow themselves to be picked-off like flies to your scope!
    
- **Removed Premature LOD switching for Vehicles and Buildings**

  Removes the premature LOD switching for vehicles and buildings based off the number of triangles in a scene.    

### Additions

- **PostHook Script**

  This provides a way for a user to customize any map he downloaded without having to rebuild it from source. This runs a lua 4 script just after map load and before the actual gameplay, allowing the user to override some of the setup scripts like unit count.

- **Plugins**

  This allows the game to load external DLLs and provide an interface for extending the game functionality by injecting new LUA commands or hooking existing functions. See [SWBF-BasePlugin](https://github.com/abkarino/SWBF-BasePlugin) and [SWBF-SetTeamColor](https://github.com/abkarino/SWBF-SetTeamColor).
## Support

The launcher supports 3 versions of the game.

- **SPTest** This is a developer centric build that contained logging commands that are very helpful for building new maps and tracing issues.
- **Steam** We support the latest version available on steam (1.3.7).
- **GOG** Unfortunately, at the time of development, we started with an old version of the GOG release namely 1.3.5 and it was the base foundation for the very initial testing since it was DRM-less. After a while, migrating the fixes would have required repeating the whole work on the latest version which was infeasible at that time, so we had to continue with that version. If you check the code, you will notice that there are huge differences between the three builds so it was very time consuming figuring out the patch and then redoing it from scratch for the other 2 versions. The good news is that the latest GOG release is very similar to the latest release of Steam, so someone could probably migrate the patches easily -though it would take some time- to the latest version.
## Future Plans
Our plan had some items that are not done yet, but we are not sure when we would get the chance to do them. Those include but not limited to:
- Fix multiplayer limitations
- Allow more soldiers than 200

Some of the modding teams, planned to port it into the CD version while others are looking to port it to SWBF II (2005). Since our pace slowed down a bit, we decided that it is time to release the source code so we would see people taking it into the next level.
## License

This work is licensed under a
[Creative Commons Attribution-NonCommercial 4.0 International License][cc-by-nc].

[![CC BY-NC 4.0][cc-by-nc-image]][cc-by-nc]


[cc-by-nc]: https://creativecommons.org/licenses/by-nc/4.0/
[cc-by-nc-image]: https://licensebuttons.net/l/by-nc/4.0/88x31.png
[cc-by-nc-shield]: https://img.shields.io/badge/License-CC%20BY--NC%204.0-lightgrey.svg
## Contributing

Contributions are always welcome!

We are looking forward to what the community will make out this release.


## Acknowledgements

The EGM Team would like to thank the following people over on swbfgamers.com and Discord for their experience and help with troubleshooting this mod as well as keeping modern Battlefront modding alive. None of this would have been possible without their help.

- DarkPhantom
- Giftheck
- Davewsa30h
- Surestsafe
- Sereja
- Fox


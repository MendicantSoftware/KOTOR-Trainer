# KOTOR Trainer

As the name suggests, the 'KOTOR Trainer', is a series of cheats for Star Wars: Knights of the Old Republic, specifically the Steam version. Many of the static offsets used by this trainer were found by the KOTOR
speedrunner "Lane" (LaneDibello on Github), who uses the GOG Version, so I would assume the GOG version is covered also.

## Current Features

* Edit attributes, modifiers, and experience.
* Increase damage, instant force regen.
* Access debug tools.
* Freecam (a work in process)
* Arbitrary value read/write.

## Using the Trainer

To attach the KOTOR process, just ensure the game and the trainer are open at the same time, and the trainer should identify the KOTOR process for you. The trainer is designed to be used on a second screen, or at
least without window focus, so KOTOR can be played fullscreen without needing to alt-tab to the trainer. Unfortunately for those without numpads, the trainer is hard coded to only work with numpads, all my TKL gang
homies hate me, but this brings us around to the next point.

## Installing the Trainer

The trainer is entirely open source, and is header only, so I would highly recommend building it yourself, or at least glancing in the general direction of the code - if not to change the keybindings, to at least
not risk performing a 2008 Frostwire-equse destruction of the family computer. Outside of the Windows api and the STL, it has no dependencies, so building it should not be as painful as the average C++ build
process (g++ -DUNICODE-D_UNICODE main.cpp -o main.exe). The main.exe is available in the repo for those who have great faith in technocommunism, I wish I were as brave as you wow.

## Future Updates

This is my first trainer, and first time using cheat engine so things are a bit "how ya going" right now, but I intend to:
* Refactor the code base to make the readwrite.h and similar headers a bit more useable/modular
* Add more offsets and functionality
* Improve the freecam
* Improve the memory scanning portion of the program

# PowerLEDColorU
This is a Wii U plugin for Aroma. This can let you change the Power LED Color while the system is on.

This can be useful for those wanting to customize the colors, or change the system's Power LED color back to blue for `isfshax` users.

## Usage
- Download the latest version from the releases page (Coming soon)
- Copy the `wiiu` folder to your SD Card
- When powering the console on, the LED will be set to blue by default

> To change this plugin's settings, go on the Aroma plugin menu,
> and head down to the `PowerLEDColorU` option and select it

## Plugin Options
Many options on the plugin can be set to your heart's content.

- Enable LED Light: If this is disabled, the Power LED light will not be on.
    - You can prank your friends with this!
- Color Choice: The choice of color to use for the system's power LED.
    - This is still being figured out at this time, so stick around!

## Building
To build this plugin, you'll need the following installed first:

- devKitPro (Contains the core Wii U libraries needed to build projects like these)
- Docker Engine (Used to build this project easier than just using `make`, by gathering other important libraries needed)

After those are installed, launch up your OS terminal (cmd.exe, Terminal, etc.) to perform the build:

1. Using `docker`, build the image. This is only needed once for every first-time build, so if you did this before, skip this step.
```
docker build . -t re_powerledcoloru_builder
```
2. After you set up the image with `docker`, use one of these commands (`make` for building & keeping the raw build files, `make clean` building & cleaning up the raw build files)
```
# make 
docker run -it --rm -v ${PWD}:/project re_powerledcoloru_builder make

# make clean
docker run -it --rm -v ${PWD}:/project re_powerledcoloru_builder make clean
```
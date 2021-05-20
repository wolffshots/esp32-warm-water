# Warm Water in esp-idf

ESP-IDF implementation of temperature controlled relay to switch a heating element to maintain water in a specific temperature range. 
A webserver is used to interface with the system and change the goal temperature, range from the goal and the relay state (forced on or off).

## How to use project

This project is built using the Visual Studio Code ESP-IDF plugin. 
Configure it from command line using `idf.py` or by using the cog in the plugin. 
Once it is configured, select the correct port of your ESP32 module and build and upload to it.
An explanation of the hardware setup will come at a later stage.

## Example folder contents

The project **esp32-warm-water** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are build using CMake. The project build configuration is contained in `CMakeLists.txt` files that provide set of directives and instructions describing the project's source files and targets (executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── components                  the extra components employed in this project
|   ├── esp32-ds18b20           used to interface with the temperature sensor
|   ├── esp32-owb               enables use of the one wire bus protocol
|   ├── esp32-utility           some extra helper functions for debugging and qol
|   └── esp32-wifi              some wrappers for the esp wifi functions
├── main                        main component
│   ├── CMakeLists.txt          specific component cmake file
│   ├── component.mk            component make file
│   ├── Kconfig.projbuild       kconfig description file to add build time vars
│   └── main.c                  core src file of the project (client code)
├── .gitignore                  describes what files and folders git should ignore
├── .gitmodules                 describes the sources and paths of the modules used in `/components`
├── CMakeLists.txt              base project cmake file
├── LICENSE.md                  MIT license file
├── Makefile                    makefile used by legacy GNU Make
└── README.md                   this file
```

For more information on structure and contents of ESP-IDF projects, please refer to Section [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) of the ESP-IDF Programming Guide.

## Troubleshooting

* Program upload failure

    * Hardware connection is not correct: run `idf.py -p PORT monitor`, and reboot your board to see if there are any output logs.
    * The baud rate for downloading is too high: lower your baud rate in the `menuconfig` menu, and try again.

# Helpful commands
- ```git submodule update --remote --recursive``` - updates the checked out modules to the most recent commit to their main branch

# Todo
 
- [ ] implement general gpio (built in led)
- [ ] implement owb
- [ ] implement ds18b20
- [ ] cleanup wifi component
- [ ] cleanup readmes
- [ ] spiffs!
- [ ] webserver/url handling

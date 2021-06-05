# warm water in esp-idf
[![Platform: ESP-IDF](https://img.shields.io/badge/ESP--IDF-v4.2%2B-blue.svg)](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/)
<!-- [![couldn't get build status](https://api.travis-ci.com/wolffshots/esp32-warm-water.svg?branch=main "Current doc build status")](https://wolffshots.github.io/esp32-warm-water/index.html) -->
![GitHub Docs Workflow Status](https://img.shields.io/github/workflow/status/wolffshots/esp32-warm-water/doxygen?label=doxygen)
![GitHub CodeQL Workflow Status](https://img.shields.io/github/workflow/status/wolffshots/esp32-warm-water/codeql?label=codeql/build)
![GitHub](https://img.shields.io/github/license/wolffshots/esp32-warm-water)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/wolffshots/esp32-warm-water)
![GitHub repo size](https://img.shields.io/github/repo-size/wolffshots/esp32-warm-water)
![Lines of code](https://img.shields.io/tokei/lines/github/wolffshots/esp32-warm-water)
![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/wolffshots/esp32-warm-water)

esp-idf implementation of temperature controlled relay to switch a heating element to maintain water in a specific temperature range.
A webserver is used to interface with the system and change the goal temperature, range from the goal and the relay state (forced on or off).

employs wifi, onewire and a relay to control the temperature of a body of water for the purpose of film development primarily. other uses include cooking and general chemistry.

the main motivation to do this project was to be able to better process c41 colour film.

## how to use project

this project is built using the Visual Studio Code esp-idf plugin.

configure it from command line using `idf.py` or by using the cog in the plugin.

once it is configured, select the correct port of your ESP32 module and build and upload to it.

an explanation of the hardware setup will come at a later stage.

## frontend files

the files in the `front/build` directory are for displaying via the webserver and are sort of a separate entity from the rest of the code. 

they automatically get made into an image but don't seem to flash with the vscode extension buttons (as a result you may need to run `idf.py -p PORT flash` for changes to persist).

the frontend can be coded in whatever framework you want as long as you can get it built into a flat file structure (all in one directory without subdirectories). i am partial to vanilla html, css and javascript but there are plenty of lightweight frameworks (like [svelte](https://svelte.dev/) and [preact](https://preactjs.com/)).

any file in `front/build` should get flashed to `/spiffs` but ensure that the `front/build` directory doens't exceed the size of the `/spiffs` partition (default 960KB).

- `/` - the root path which serves `index.html`
- @todo add more reserved locations here
- any other path should attempt to be read from spiffs and 404 if not found

## dependencies

- esp32-ds18b20 - [github](https://github.com/wolffshots/esp32-ds18b20) - [gitlab](https://gitlab.com/wolffshots/esp32-ds18b20) - [docs](https://wolffshots.github.io/esp32-ds18b20/index.html)
- esp32-gpio - [github](https://github.com/wolffshots/esp32-gpio) - [gitlab](https://gitlab.com/wolffshots/esp32-gpio) - [docs](https://wolffshots.github.io/esp32-gpio/index.html)
- esp32-owb - [github](https://github.com/wolffshots/esp32-owb) - [gitlab](https://gitlab.com/wolffshots/esp32-owb) - [docs](https://wolffshots.github.io/esp32-owb/index.html)
- esp32-ssd1306 - [github](https://github.com/wolffshots/esp32-ssd1306) - [gitlab](https://gitlab.com/wolffshots/esp32-ssd1306) - [docs](https://wolffshots.github.io/esp32-ssd1306/index.html)
- esp32-utility - [github](https://github.com/wolffshots/esp32-utility) - [gitlab](https://gitlab.com/wolffshots/esp32-utility) - [docs](https://wolffshots.github.io/esp32-utility/index.html)
- esp32-wifi - [github](https://github.com/wolffshots/esp32-wifi) - [gitlab](https://gitlab.com/wolffshots/esp32-wifi) - [docs](https://wolffshots.github.io/esp32-wifi/index.html)
- esp32-timer - [github](https://github.com/wolffshots/esp32-timer) - [gitlab](https://gitlab.com/wolffshots/esp32-timer) - [docs](https://wolffshots.github.io/esp32-timer/index.html)

## folder contents

the project **esp32-warm-water** contains one source file in C language [main.c](main/main.c). the file is located in folder [main](main).

esp-idf projects are build using cmake. the project build configuration is contained in `CMakeLists.txt` files that provide set of directives and instructions describing the project's source files and targets (executable, library, or both).

below is short explanation of remaining files in the project folder.

```
├── components                  the extra components employed in this project
|   ├── esp32-ds18b20           used to interface with the temperature sensor (includes wrapper)
|   ├── esp32-gpio              little gpio wrapper to simplify setup
|   ├── esp32-owb               enables use of the one wire bus protocol
|   ├── esp32-ssd1306           led interface library
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

for more information on structure and contents of esp-idf projects, please refer to Section [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) of the esp-idf programming guide.


## documentation

automatically generated API documentation (doxygen) is available [here](https://wolffshots.github.io/esp32-warm-water/index.html).

## license

the code in this project is licensed under the MIT license - see [LICENSE](LICENSE.md) for details.

## troubleshooting

- program upload failure

  - hardware connection is not correct: run `idf.py -p PORT monitor`, and reboot your board to see if there are any output logs.
  - the baud rate for downloading is too high: lower your baud rate in the `menuconfig` menu, and try again.

# helpful commands

- `git submodule update --remote --recursive` - updates the checked out modules to the most recent commit to their main branch
- `git push all` - as setup with `https://jigarius.com/blog/multiple-git-remote-repositories`

## acknowledgements

- a big thank you to [David Antliff](https://github.com/DavidAntliff/) for providing his fantastic onewire and ds18b20 libraries on github under the MIT license
- thanks to [Shirkybooi](https://github.com/ShirkyBooi) for pushing me to use esp-idf
- last but not least, thanks to [Grabt234](https://github.com/grabt234) for helping me get into this field
- "1-Wire" is a registered trademark of Maxim Integrated.

# todo

- [x] timer for temp reads
- [x] implement general gpio (built in led) - https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html
- [x] implement owb
- [x] implement ds18b20 - https://github.com/DavidAntliff/esp32-ds18b20-example
- [x] cleanup wifi component
- [x] cleanup readmes
- [x] goal temp
- [x] margins for goal temp
- [x] spiffs!
- [ ] webserver/url handling - https://github.com/espressif/esp-idf/tree/master/examples/protocols/http_server/file_serving
- [ ] component unit testing - https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/unit-tests.html
- [x] implement build testing - https://calinradoni.github.io/pages/200203-travisci-esp-idf.html
- [x] make frontend into own git module

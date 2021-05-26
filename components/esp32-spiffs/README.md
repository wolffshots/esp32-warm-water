# esp32-component

example template for

## how to use project

1. run `git submodule add git@github.com:wolffshots/esp32-spiffs.git components/esp32-spiffs` in your main project
2. configure project if needed
3. `#include "spiffs.h"` should give you access to this component.

if the above steps don't work then you may need to run `git submodule init components/esp32-spiffs`
and then `git submodule update --remote --recursive` in your main project

## folder contents

the component **esp32-spiffs** contains three source files in C lang: [spiffs.c](spiffs.c). this file is located in root folder.

esp-idf projects are build using cmake. the project build configuration is contained in `CMakeLists.txt` files that provide set of directives and instructions describing the project's source files and targets (executable, library, or both).

below is short explanation of remaining files in the project folder.

```
├── .github					    directory of templates used by github
├── doc                         documentation path
│   ├── .gitignore              ignore directive for generated docs
│   └── Doxyfile                instructions for doxygen to produce documentation
├── include                     header file directory
│   └── spiffs.h                the main header file for this component
├── .gitignore                  describes what files and folders git should ignore
├── CMakeLists.txt              base project cmake file (describes dependencies, include dir and src dir)
├── spiffs.c                    core src file of the component
├── component.mk                component make file
├── Kconfig.projbuild           kconfig description file to add build time vars
├── LICENSE.md                  MIT license file
└── README.md                   this file
```

for more information on structure and contents of esp-idf projects, please refer to Section [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) of the esp-idf programming guide.

<!--## documentation

automatically generated API documentation (doxygen) is available [here](https://wolffshots.github.io/esp32-ssd1306/index.html).-->

## license

the code in this project is licensed under the MIT license - see LICENSE for details.

# helpful commands

- `git submodule update --remote --recursive` - updates the checked out modules to the most recent commit to their main branch

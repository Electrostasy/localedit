## Localedit
### A graphical editor for `MissionTemplates` and `TaskObjectives` files for the video game [APB Reloaded](http://gamersfirst.com/apb/)

#### Purpose
This graphical tool is intended to make editing localization files easier for the game
[APB Reloaded](http://gamersfirst.com/apb/). At the moment, the scope of the project only allows editing files such as
`MissionTemplates` and `TaskObjectives` of any language, provided their internal structure matches the default template.

![](images/editor_preview.png)

#### Features:
* Syntax highlighting support of `<Col></Col>` tags;
* Searchable (by mission name and id) list of missions;
* Tabbed panel for editing opposing sides of a mission's objectives;
* Cross-platform (even though the game itself isn't, but that was not my goal)

You can import the entire contents of a folder and the program will find the required files, you don't have to look for
them yourself.

To export your edited objectives and missions, you need only select the folder you want them to be in, however
it WILL likely overwrite or append to existing files, so I recommend being careful with that!

#### Compiling from source
##### Windows (MSVC)
###### Requirements
You need working installations of:
* 'Visual Studio 16 2019' for the compiler ([official direct download](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16))
* Qt 5.15.2 with the 'msvc2019_64' target and CMake build system support
Make sure Qt 5.15.2 'msvc2019_64' compiler tools and CMake libs are present in
your PATH environment variable.

###### Compilation
To compile from source in a Windows environment, clone this project and execute these commands
in the project root directory:
```powershell
cmake . -B build -G"Visual Studio 16 2019"
cd build
cmake --build .
```
The compiled executable with debug symbols will be `.\Debug\Localedit.exe`,
with all required dependencies (Qt DLLs) copied over.

##### Linux and macOS
###### Requirements
Make sure Qt 5.15.2 and CMake are installed and present in your PATH environment variable.

All pinned dependencies can be fetched and present in a
[Nix](https://nixos.org/download.html#nix-quick-install)
[flake](https://github.com/mschwaig/howto-install-nix-with-flake-support)-based
development shell by running `nix develop` in the project root.

###### Compilation
To compile from source in a Linux environment, clone this project and execute these commands
in the project root directory:
```bash
cmake . -B build -G'Unix Makefiles'
cd build
cmake --build .
```
The compiled executable will be `./Localedit`. Note that depending on the display server (X/Wayland), you may
require the appropriate Qt libraries installed to run the compiled executable.

To compile from source in a macOS environment (Xcode), clone this project and execute these commands
in the project root directory:
```zsh
cmake . -B build -G'Xcode'
cd build
cmake --build .
```


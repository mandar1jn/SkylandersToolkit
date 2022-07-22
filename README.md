# Skylanders Editor

## What is this?
This is my work in progress skylanders editor. This accompanies my research into the skylanders figures and the portals, which can be found [here](https://medium.com/@marijnkneppers/list/dissecting-the-skylanders-portal-c4bd580dae2f)

## Wasn't this made in Raylib?
That is correct. This was originally made in raylib, but I decided to switch to QT to ease development. The original code can still be found [here](https://github.com/mandar1jn/SkylandersEditor/tree/old-raygui)

## Building

### Requirements
To build this project, you need to install [QT](https://www.qt.io/). You can download the open source version [here](https://www.qt.io/download-open-source). Make sure the bin folder from QT is also added to your PATH.

### Cloning the project
This project contains submodules and, therefor, needs an addition to the clone command. With this addition, the command becomes ``git clone https://github.com/mandar1jn/SkylandersEditor --recursive``.

If you cloned the repocitory without the recursive flag, you can run ``git submodule update --init`` inside the cloned folder instead.

### Generating project files
This project does not contain any traditional project files. It does, however, contain a `SkylandersEditor.pro` file that can be used to generate project files.

To begin generating a project files, create a folde which will contain your project files and build artifacts.

#### Visual Studio
To generate the visual studio project files, run ``qmake -tp vc <PATH TO SkylandersEditor.pro>`` inside the folder you just created. After this, you should have a vcxproj file you can open and build.

#### Makefile
Makefiles are a very well know way of building c/c++ project. So, of course, QT supports that too. You just need to run ``qmake <PATH TO SkylandersEditor.pro>`` inside the folder you created. With this makefile created, you can just run ``make <configuration>`` where configuration is either release or debug to build the editor.

## Support

Although I own quite a lot of figures, I don't own any from Skylanders Imaginators and Swap Force. There are a lot of kind people I have been in contact with who are willing to help me, but it's still likely that I will, at some point in the future, need to buy a few figures to do research on. If you want to help and financially support this project, you can do that using the button below or using the sponsor button at the top of this page.

<a href='https://ko-fi.com/mandar1jn' target='_blank'><img height='35' style='border:0px;height:46px;' src='https://az743702.vo.msecnd.net/cdn/kofi3.png?v=0' border='0' alt='Support me at ko-fi.com' />

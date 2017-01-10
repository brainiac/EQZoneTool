EQZoneTool
==========

**WIP** An attempt at making an EverQuest zone viewer tool using an amalgamation Urho3d and Qt. This is probably a bad idea...

This is an attempt at making something better than what I made for MQ2Nav's mesh generator. The goal is to be able to view everquest zones and provide a set of tools for viewing and modifying data associated with them.


Building
--------

To build, you need the following installed:
* Visual Studio 2015 (Update 3)
* Qt 5.7.1 w/ msvc2015 x86
* CMake
* Maybe more


Steps:

1. Build Urho3d

    cd contrib
    build_urho3d.bat

2. Open src\EQZoneTool.sln

3. Build - the outputs should be in src\Debug or src\Release if everything went ok.



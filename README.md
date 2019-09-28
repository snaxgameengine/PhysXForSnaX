# PhysX for SnaX
Copyright (c) 2013-2019, mCODE A/S
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Introduction
SnaX ([snaxgameengine.com](https://snaxgameengine.com/)) is a fast and lightweight development tool
for 3D-graphics applications based on a real-time, visual programming concept. _Real-time_ because
every update you make to your program can be viewed instantly as you develop - _Visual_ because 
programming happens in a graphical environment, where writing code is replaced by linking together 
small, precompiled blocks of functionality called _Chips_. 

This repository contains the Chips integrating the nVidia PhysX 4 library for SnaX. It enables you
to build dynamic scenes in SnaX to do real-time physics simulations with objects reacting to forces, 
collision detection and so on.

## Getting started
- Install [SnaX](https://snaxgameengine.com/downloads/) if you have not done so already!
- Get the SnaX SDK: `git clone https://github.com/snaxgameengine/snaxsdk.git`.
- Get PhysXForSnaX: `git clone https://github.com/snaxgameengine/PhysXForSnaX.git`.
- Get the PhysX 4 SDK. I recommend using [VCPKG](https://github.com/microsoft/vcpkg).  
  Alternatively, get it from https://github.com/NVIDIAGameWorks/PhysX.  
  if using VCPKG, install the PhysX SDK like this: `vcpkg -install physx:x64-windows`.
  Note that this plugin was built using __PhysX 4.1.1__.
- With PhysX SDK in place, you can generate the PhysXForSnaX Visual Studio projects using __CMake >= 3.15__.
  I recommend doing an out-of-source build. If using CMake GUI:
  - Select the __Visual Studio 15 (2017) or 16 (2019)__ generator. VS2015 is NOT supported.
  - Select x64 as the target platform.
  - Select "Specify toolchain file for cross-compiling" if using VCPKG.
  - Click _Next_.
  - For the toolchain file, specify <MY_VCPKG_INSTALL_DIR>/scripts/buildsystems/vcpkg.cmake.
  - Make sure SnaX, SnaX SDK and PhysX are found!
  - Generate!
- Open the project in Visual Studio 2017/2019. The _SnaX_ project is selected as startup-project pr default.
- Build the solution by pressing F5.
- If everything goes well, the newly built DLL-files (and PhysX-dependencis) are copied to the correct folders
  in the SnaX installation directory, and SnaXDeveloper is launched!
  
__IMPORTANT: You have to give the current user _write access_ to the _3rd_, _Chips_ and _Dialogs_ folders in the
SnaX installation directory. If not, Visual Studio will not be able to automatically copy the new files.__

__NOTE__: Because of a flaw with VCPKG/CMake, a library whose Debug and Release versions have the same name
(as is the case for the PhysX libraries), using the CMake GUI to generate both Debug and Release configurations
for Visual Studio, causes the Debug version of the libraries to be used also for the Release version. 
This is unfortunat, but little I can do about for the moment. It is a known problem, so hopefully there will
be a fix for it soon! __In the meantime, you wil have to manually go into the Release configurations in
Visual Studio, and change to the Release version of the PhysX libraries under "Additional Dependencies".__

## Examples
The _Examples_ folder contains a few example-projects that can be opened in SnaX Developer once the Chips
has been built. The examples shows you the very basic of how to use the PhysX Chips with SnaX.

Enjoy!



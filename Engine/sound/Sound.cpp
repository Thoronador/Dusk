/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 thoronador, walljumper, ssj5000

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Sound.h"
#include <algorithm>
#include <cmath> //needed for rotation calculations
#include <cstdio> //required for opening files for Ogg (since it's a C lib)
#include <fstream>
#include "../Messages.h"

namespace Dusk
{

//functions for class Sound:
//constructor
Sound::Sound()
{
  pDevice = NULL;
  pContext = NULL;
  AL_Ready = false;
  Vorbis_Ready = false;
  InitInProgress = false;
  libHandleAL = NULL;
  libHandleOV = NULL;
  pMediaList = NULL;
  pNoiseList = NULL;
  //init function pointers
  AllFuncPointersToNULL();
}

//destructor
Sound::~Sound()
{
  if (AL_Ready)
  {
    exit();
  }
}

Sound& Sound::get()
{
    static Sound Instance;
    return Instance;
}

//Initializes OpenAL, device and context for our application;
//returns: true, if initialization of OpenAL was successful; false otherwise
bool Sound::init(std::string PathToLib_AL, std::string PathToLib_Vorbisfile, const bool needVorbis)
{
  if (AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::Init: Warning: Cannot start initialization of OpenAL ";
    DuskLog() <<" since it is already initialized or init. is in progress.\n";
    return false;
  }
  InitInProgress = true;
  if ((PathToLib_AL== "") || (PathToLib_AL == "NULL"))
  {
    //if no path to the OpenAL library is given, try default value
    #if defined(_WIN32)
      PathToLib_AL = "C:\\Windows\\System32\\OpenAL32.dll";
    #else
      PathToLib_AL = "/usr/lib/libopenal.so";
    #endif
  }
  #if defined(_WIN32)
  //Windows
  libHandleAL = LoadLibrary(PathToLib_AL.c_str());
  #else
  //Linux goes here
  libHandleAL = dlopen(PathToLib_AL.c_str(), RTLD_LOCAL | RTLD_LAZY);
  #endif
  if (libHandleAL == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not open OpenAL dynamic library in \""
              << PathToLib_AL << "\". Exiting.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alcCreateContext = (LPALCCREATECONTEXT)GetProcAddress(libHandleAL, "alcCreateContext");
  alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT) GetProcAddress(libHandleAL, "alcMakeContextCurrent");
  alcProcessContext = (LPALCPROCESSCONTEXT) GetProcAddress(libHandleAL, "alcProcessContext");
  alcSuspendContext = (LPALCSUSPENDCONTEXT) GetProcAddress(libHandleAL, "alcSuspendContext");
  alcDestroyContext = (LPALCDESTROYCONTEXT) GetProcAddress(libHandleAL, "alcDestroyContext");
  alcGetCurrentContext = (LPALCGETCURRENTCONTEXT) GetProcAddress(libHandleAL, "alcGetCurrentContext");
  alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE) GetProcAddress(libHandleAL, "alcGetContextsDevice");
  #else
  //Linux
  alcCreateContext = (LPALCCREATECONTEXT) dlsym(libHandleAL, "alcCreateContext");
  alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT) dlsym(libHandleAL, "alcMakeContextCurrent");
  alcProcessContext = (LPALCPROCESSCONTEXT) dlsym(libHandleAL, "alcProcessContext");
  alcSuspendContext = (LPALCSUSPENDCONTEXT) dlsym(libHandleAL, "alcSuspendContext");
  alcDestroyContext = (LPALCDESTROYCONTEXT) dlsym(libHandleAL, "alcDestroyContext");
  alcGetCurrentContext = (LPALCGETCURRENTCONTEXT) dlsym(libHandleAL, "alcGetCurrentContext");
  alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE) dlsym(libHandleAL, "alcGetContextsDevice");
  #endif
  if (alcCreateContext == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcCreateContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcMakeContextCurrent == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcMakeContextCurrent\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcProcessContext == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcProcessContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcSuspendContext == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcSuspendContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcDestroyContext == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcDestroyContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetCurrentContext == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetCurrentContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetContextsDevice == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetContextsDevice\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alcOpenDevice = (LPALCOPENDEVICE) GetProcAddress(libHandleAL, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) GetProcAddress(libHandleAL, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) GetProcAddress(libHandleAL, "alcGetError");
  alcIsExtensionPresent = (LPALCISEXTENSIONPRESENT) GetProcAddress(libHandleAL, "alcIsExtensionPresent");
  alcGetEnumValue = (LPALCGETENUMVALUE) GetProcAddress(libHandleAL, "alcGetEnumValue");
  alcGetString = (LPALCGETSTRING) GetProcAddress(libHandleAL, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) GetProcAddress(libHandleAL, "alcGetIntegerv");
  #else
  //Linux
  alcOpenDevice = (LPALCOPENDEVICE) dlsym(libHandleAL, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) dlsym(libHandleAL, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) dlsym(libHandleAL, "alcGetError");
  alcIsExtensionPresent = (LPALCISEXTENSIONPRESENT) dlsym(libHandleAL, "alcIsExtensionPresent");
  alcGetEnumValue = (LPALCGETENUMVALUE) dlsym(libHandleAL, "alcGetEnumValue");
  alcGetString = (LPALCGETSTRING) dlsym(libHandleAL, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) dlsym(libHandleAL, "alcGetIntegerv");
  #endif

  if (alcOpenDevice == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcOpenDevice\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcCloseDevice == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcCloseDevice\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetError == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetError\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcIsExtensionPresent == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcIsExtensionPresent\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetEnumValue == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetEnumValue\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetString == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetString\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetIntegerv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alcGetIntegerv\" address.\n";
    InitInProgress = false;
    return false;
  }
  //al.h-function pointers
  #if defined(_WIN32)
  //Windows
  alEnable = (LPALENABLE) GetProcAddress(libHandleAL, "alEnable");
  alDisable = (LPALDISABLE) GetProcAddress(libHandleAL, "alDisable");
  alIsEnabled = (LPALISENABLED) GetProcAddress(libHandleAL, "alIsEnabled");
  #else
  //Linux
  alEnable = (LPALENABLE) dlsym(libHandleAL, "alEnable");
  alDisable = (LPALDISABLE) dlsym(libHandleAL, "alDisable");
  alIsEnabled = (LPALISENABLED) dlsym(libHandleAL, "alIsEnabled");
  #endif
  if (alEnable == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alEnable\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDisable == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alDisable\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsEnabled == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alIsEnabled\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alGetString = (LPALGETSTRING) GetProcAddress(libHandleAL, "alGetString");
  alGetBooleanv = (LPALGETBOOLEANV) GetProcAddress(libHandleAL, "alGetBooleanv");
  alGetIntegerv = (LPALGETINTEGERV) GetProcAddress(libHandleAL, "alGetIntegerv");
  alGetFloatv = (LPALGETFLOATV) GetProcAddress(libHandleAL, "alGetFloatv");
  alGetDoublev = (LPALGETDOUBLEV) GetProcAddress(libHandleAL, "alGetDoublev");
  alGetBoolean = (LPALGETBOOLEAN) GetProcAddress(libHandleAL, "alGetBoolean");
  alGetInteger = (LPALGETINTEGER) GetProcAddress(libHandleAL, "alGetInteger");
  alGetFloat = (LPALGETFLOAT) GetProcAddress(libHandleAL, "alGetFloat");
  alGetDouble = (LPALGETDOUBLE) GetProcAddress(libHandleAL, "alGetDouble");
  #else
  //Linux
  alGetString = (LPALGETSTRING) dlsym(libHandleAL, "alGetString");
  alGetBooleanv = (LPALGETBOOLEANV) dlsym(libHandleAL, "alGetBooleanv");
  alGetIntegerv = (LPALGETINTEGERV) dlsym(libHandleAL, "alGetIntegerv");
  alGetFloatv = (LPALGETFLOATV) dlsym(libHandleAL, "alGetFloatv");
  alGetDoublev = (LPALGETDOUBLEV) dlsym(libHandleAL, "alGetDoublev");
  alGetBoolean = (LPALGETBOOLEAN) dlsym(libHandleAL, "alGetBoolean");
  alGetInteger = (LPALGETINTEGER) dlsym(libHandleAL, "alGetInteger");
  alGetFloat = (LPALGETFLOAT) dlsym(libHandleAL, "alGetFloat");
  alGetDouble = (LPALGETDOUBLE) dlsym(libHandleAL, "alGetDouble");
  #endif
  if (alGetString == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetString\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBooleanv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBooleanv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetIntegerv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetIntegerv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetFloatv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetFloatv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetDoublev == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetDoublev\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBoolean == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBoolean\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetInteger == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetInteger\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetFloat == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetFloat\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetDouble == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetDouble\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alGetError = (LPALGETERROR) GetProcAddress(libHandleAL, "alGetError");
  #else
  //Linux
  alGetError = (LPALGETERROR) dlsym(libHandleAL, "alGetError");
  #endif
  if (alGetError == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetError\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alIsExtensionPresent = (LPALISEXTENSIONPRESENT) GetProcAddress(libHandleAL, "alIsExtensionPresent");
  alGetProcAddress = (LPALGETPROCADDRESS) GetProcAddress(libHandleAL, "alGetProcAddress");
  alGetEnumValue = (LPALGETENUMVALUE) GetProcAddress(libHandleAL, "alGetEnumValue");
  #else
  //Linux
  alIsExtensionPresent = (LPALISEXTENSIONPRESENT) dlsym(libHandleAL, "alIsExtensionPresent");
  alGetProcAddress = (LPALGETPROCADDRESS) dlsym(libHandleAL, "alGetProcAddress");
  alGetEnumValue = (LPALGETENUMVALUE) dlsym(libHandleAL, "alGetEnumValue");
  #endif
  if (alIsExtensionPresent == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alIsExtensionPresent\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetProcAddress == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetProcAddress\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetEnumValue == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetEnumValue\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alListenerf = (LPALLISTENERF) GetProcAddress(libHandleAL, "alListenerf");
  alListener3f = (LPALLISTENER3F) GetProcAddress(libHandleAL, "alListener3f");
  alListenerfv = (LPALLISTENERFV) GetProcAddress(libHandleAL, "alListenerfv");
  alListeneri = (LPALLISTENERI) GetProcAddress(libHandleAL, "alListeneri");
  alListener3i = (LPALLISTENER3I) GetProcAddress(libHandleAL, "alListener3i");
  alListeneriv = (LPALLISTENERIV) GetProcAddress(libHandleAL, "alListeneriv");
  #else
  //Linux
  alListenerf = (LPALLISTENERF) dlsym(libHandleAL, "alListenerf");
  alListener3f = (LPALLISTENER3F) dlsym(libHandleAL, "alListener3f");
  alListenerfv = (LPALLISTENERFV) dlsym(libHandleAL, "alListenerfv");
  alListeneri = (LPALLISTENERI) dlsym(libHandleAL, "alListeneri");
  alListener3i = (LPALLISTENER3I) dlsym(libHandleAL, "alListener3i");
  alListeneriv = (LPALLISTENERIV) dlsym(libHandleAL, "alListeneriv");
  #endif
  if (alListenerf == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListenerf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListener3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListener3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListenerfv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListenerfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListeneri == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListeneri\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListener3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListener3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListeneriv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alListeneriv\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alGetListenerf = (LPALGETLISTENERF) GetProcAddress(libHandleAL, "alGetListenerf");
  alGetListener3f = (LPALGETLISTENER3F) GetProcAddress(libHandleAL, "alGetListener3f");
  alGetListenerfv = (LPALGETLISTENERFV) GetProcAddress(libHandleAL, "alGetListenerfv");
  alGetListeneri = (LPALGETLISTENERI) GetProcAddress(libHandleAL, "alGetListeneri");
  alGetListener3i = (LPALGETLISTENER3I) GetProcAddress(libHandleAL, "alGetListener3i");
  alGetListeneriv = (LPALGETLISTENERIV) GetProcAddress(libHandleAL, "alGetListeneriv");
  #else
  //Linux
  alGetListenerf = (LPALGETLISTENERF) dlsym(libHandleAL, "alGetListenerf");
  alGetListener3f = (LPALGETLISTENER3F) dlsym(libHandleAL, "alGetListener3f");
  alGetListenerfv = (LPALGETLISTENERFV) dlsym(libHandleAL, "alGetListenerfv");
  alGetListeneri = (LPALGETLISTENERI) dlsym(libHandleAL, "alGetListeneri");
  alGetListener3i = (LPALGETLISTENER3I) dlsym(libHandleAL, "alGetListener3i");
  alGetListeneriv = (LPALGETLISTENERIV) dlsym(libHandleAL, "alGetListeneriv");
  #endif
  if (alGetListenerf == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListenerf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListener3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListener3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListenerfv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListenerfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListeneri == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListeneri\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListener3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListener3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListeneriv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetListeneriv\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alGenSources = (LPALGENSOURCES) GetProcAddress(libHandleAL, "alGenSources");
  alDeleteSources = (LPALDELETESOURCES) GetProcAddress(libHandleAL, "alDeleteSources");
  alIsSource = (LPALISSOURCE) GetProcAddress(libHandleAL, "alIsSource");
  #else
  //Linux
  alGenSources = (LPALGENSOURCES) dlsym(libHandleAL, "alGenSources");
  alDeleteSources = (LPALDELETESOURCES) dlsym(libHandleAL, "alDeleteSources");
  alIsSource = (LPALISSOURCE) dlsym(libHandleAL, "alIsSource");
  #endif
  if (alGenSources == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGenSources\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDeleteSources == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alDeleteSources\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsSource == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alIsSource\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alSourcef = (LPALSOURCEF) GetProcAddress(libHandleAL, "alSourcef");
  alSource3f = (LPALSOURCE3F) GetProcAddress(libHandleAL, "alSource3f");
  alSourcefv = (LPALSOURCEFV) GetProcAddress(libHandleAL, "alSourcefv");
  alSourcei = (LPALSOURCEI) GetProcAddress(libHandleAL, "alSourcei");
  alSource3i = (LPALSOURCE3I) GetProcAddress(libHandleAL, "alSource3i");
  alSourceiv = (LPALSOURCEIV) GetProcAddress(libHandleAL, "alSourceiv");
  #else
  //Linux
  alSourcef = (LPALSOURCEF) dlsym(libHandleAL, "alSourcef");
  alSource3f = (LPALSOURCE3F) dlsym(libHandleAL, "alSource3f");
  alSourcefv = (LPALSOURCEFV) dlsym(libHandleAL, "alSourcefv");
  alSourcei = (LPALSOURCEI) dlsym(libHandleAL, "alSourcei");
  alSource3i = (LPALSOURCE3I) dlsym(libHandleAL, "alSource3i");
  alSourceiv = (LPALSOURCEIV) dlsym(libHandleAL, "alSourceiv");
  #endif
  if (alSourcef == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcef\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSource3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSource3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcefv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcefv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcei == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcei\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSource3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSource3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceiv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceiv\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alGetSourcef = (LPALGETSOURCEF) GetProcAddress(libHandleAL, "alGetSourcef");
  alGetSource3f = (LPALGETSOURCE3F) GetProcAddress(libHandleAL, "alGetSource3f");
  alGetSourcefv = (LPALGETSOURCEFV) GetProcAddress(libHandleAL, "alGetSourcefv");
  alGetSourcei = (LPALGETSOURCEI) GetProcAddress(libHandleAL, "alGetSourcei");
  alGetSource3i = (LPALGETSOURCE3I) GetProcAddress(libHandleAL, "alGetSource3i");
  alGetSourceiv = (LPALGETSOURCEIV) GetProcAddress(libHandleAL, "alGetSourceiv");
  #else
  //Linux
  alGetSourcef = (LPALGETSOURCEF) dlsym(libHandleAL, "alGetSourcef");
  alGetSource3f = (LPALGETSOURCE3F) dlsym(libHandleAL, "alGetSource3f");
  alGetSourcefv = (LPALGETSOURCEFV) dlsym(libHandleAL, "alGetSourcefv");
  alGetSourcei = (LPALGETSOURCEI) dlsym(libHandleAL, "alGetSourcei");
  alGetSource3i = (LPALGETSOURCE3I) dlsym(libHandleAL, "alGetSource3i");
  alGetSourceiv = (LPALGETSOURCEIV) dlsym(libHandleAL, "alGetSourceiv");
  #endif
  if (alGetSourcef == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSourcef\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSource3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSource3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcefv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSourcefv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcei == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSourcei\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSource3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSource3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceiv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetSourceiv\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alSourcePlayv = (LPALSOURCEPLAYV) GetProcAddress(libHandleAL, "alSourcePlayv");
  alSourceStopv = (LPALSOURCESTOPV) GetProcAddress(libHandleAL, "alSourceStopv");
  alSourceRewindv = (LPALSOURCEREWINDV) GetProcAddress(libHandleAL, "alSourceRewindv");
  alSourcePausev = (LPALSOURCEPAUSEV) GetProcAddress(libHandleAL, "alSourcePausev");
  #else
  //Linux
  alSourcePlayv = (LPALSOURCEPLAYV) dlsym(libHandleAL, "alSourcePlayv");
  alSourceStopv = (LPALSOURCESTOPV) dlsym(libHandleAL, "alSourceStopv");
  alSourceRewindv = (LPALSOURCEREWINDV) dlsym(libHandleAL, "alSourceRewindv");
  alSourcePausev = (LPALSOURCEPAUSEV) dlsym(libHandleAL, "alSourcePausev");
  #endif
  if (alSourcePlayv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcePlayv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceStopv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceStopv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceRewindv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceRewindv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcePausev == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcePausev\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alSourcePlay = (LPALSOURCEPLAY) GetProcAddress(libHandleAL, "alSourcePlay");
  alSourceStop = (LPALSOURCESTOP) GetProcAddress(libHandleAL, "alSourceStop");
  alSourceRewind = (LPALSOURCEREWIND) GetProcAddress(libHandleAL, "alSourceRewind");
  alSourcePause = (LPALSOURCEPAUSE) GetProcAddress(libHandleAL, "alSourcePause");
  #else
  //Linux
  alSourcePlay = (LPALSOURCEPLAY) dlsym(libHandleAL, "alSourcePlay");
  alSourceStop = (LPALSOURCESTOP) dlsym(libHandleAL, "alSourceStop");
  alSourceRewind = (LPALSOURCEREWIND) dlsym(libHandleAL, "alSourceRewind");
  alSourcePause = (LPALSOURCEPAUSE) dlsym(libHandleAL, "alSourcePause");
  #endif
  if (alSourcePlay == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcePlay\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceStop == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceStop\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceRewind == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceRewind\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcePause == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourcePause\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alSourceQueueBuffers = (LPALSOURCEQUEUEBUFFERS) GetProcAddress(libHandleAL, "alSourceQueueBuffers");
  alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS) GetProcAddress(libHandleAL, "alSourceUnqueueBuffers");
  #else
  //Linux
  alSourceQueueBuffers = (LPALSOURCEQUEUEBUFFERS) dlsym(libHandleAL, "alSourceQueueBuffers");
  alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS) dlsym(libHandleAL, "alSourceUnqueueBuffers");
  #endif
  if (alSourceQueueBuffers == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceQueueBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceUnqueueBuffers == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSourceUnqueueBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alGenBuffers = (LPALGENBUFFERS) GetProcAddress(libHandleAL, "alGenBuffers");
  alDeleteBuffers = (LPALDELETEBUFFERS) GetProcAddress(libHandleAL, "alDeleteBuffers");
  alIsBuffer = (LPALISBUFFER) GetProcAddress(libHandleAL, "alIsBuffer");
  alBufferData = (LPALBUFFERDATA) GetProcAddress(libHandleAL, "alBufferData");
  #else
  //Linux
  alGenBuffers = (LPALGENBUFFERS) dlsym(libHandleAL, "alGenBuffers");
  alDeleteBuffers = (LPALDELETEBUFFERS) dlsym(libHandleAL, "alDeleteBuffers");
  alIsBuffer = (LPALISBUFFER) dlsym(libHandleAL, "alIsBuffer");
  alBufferData = (LPALBUFFERDATA) dlsym(libHandleAL, "alBufferData");
  #endif
  if (alGenBuffers == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGenBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDeleteBuffers == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alDeleteBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsBuffer == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alIsBuffer\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferData == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBufferData\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alBufferf = (LPALBUFFERF) GetProcAddress(libHandleAL, "alBufferf");
  alBuffer3f = (LPALBUFFER3F) GetProcAddress(libHandleAL, "alBuffer3f");
  alBufferfv = (LPALBUFFERFV) GetProcAddress(libHandleAL, "alBufferfv");
  alBufferi = (LPALBUFFERI) GetProcAddress(libHandleAL, "alBufferi");
  alBuffer3i = (LPALBUFFER3I) GetProcAddress(libHandleAL, "alBuffer3i");
  alBufferiv = (LPALBUFFERIV) GetProcAddress(libHandleAL, "alBufferiv");
  #else
  //Linux
  alBufferf = (LPALBUFFERF) dlsym(libHandleAL, "alBufferf");
  alBuffer3f = (LPALBUFFER3F) dlsym(libHandleAL, "alBuffer3f");
  alBufferfv = (LPALBUFFERFV) dlsym(libHandleAL, "alBufferfv");
  alBufferi = (LPALBUFFERI) dlsym(libHandleAL, "alBufferi");
  alBuffer3i = (LPALBUFFER3I) dlsym(libHandleAL, "alBuffer3i");
  alBufferiv = (LPALBUFFERIV) dlsym(libHandleAL, "alBufferiv");
  #endif
  if (alBufferf == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBufferf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBuffer3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBuffer3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferfv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBufferfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferi == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBufferi\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBuffer3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBuffer3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferiv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alBufferiv\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alGetBufferf = (LPALGETBUFFERF) GetProcAddress(libHandleAL, "alGetBufferf");
  alGetBuffer3f = (LPALGETBUFFER3F) GetProcAddress(libHandleAL, "alGetBuffer3f");
  alGetBufferfv = (LPALGETBUFFERFV) GetProcAddress(libHandleAL, "alGetBufferfv");
  alGetBufferi = (LPALGETBUFFERI) GetProcAddress(libHandleAL, "alGetBufferi");
  alGetBuffer3i = (LPALGETBUFFER3I) GetProcAddress(libHandleAL, "alGetBuffer3i");
  alGetBufferiv = (LPALGETBUFFERIV) GetProcAddress(libHandleAL, "alGetBufferiv");
  #else
  //Linux
  alGetBufferf = (LPALGETBUFFERF) dlsym(libHandleAL, "alGetBufferf");
  alGetBuffer3f = (LPALGETBUFFER3F) dlsym(libHandleAL, "alGetBuffer3f");
  alGetBufferfv = (LPALGETBUFFERFV) dlsym(libHandleAL, "alGetBufferfv");
  alGetBufferi = (LPALGETBUFFERI) dlsym(libHandleAL, "alGetBufferi");
  alGetBuffer3i = (LPALGETBUFFER3I) dlsym(libHandleAL, "alGetBuffer3i");
  alGetBufferiv = (LPALGETBUFFERIV) dlsym(libHandleAL, "alGetBufferiv");
  #endif
  if (alGetBufferf == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBufferf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBuffer3f == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBuffer3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferfv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBufferfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferi == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBufferi\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBuffer3i == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBuffer3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferiv == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alGetBufferiv\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alSpeedOfSound = (LPALSPEEDOFSOUND) GetProcAddress(libHandleAL, "alSpeedOfSound");
  #else
  //Linux
  alSpeedOfSound = (LPALSPEEDOFSOUND) dlsym(libHandleAL, "alSpeedOfSound");
  #endif
  if (alSpeedOfSound == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"alSpeedOfSound\" address.\n";
    InitInProgress = false;
    return false;
  }

  //Initialization of device (finally)
  pDevice = alcOpenDevice(NULL); //opens default device
  //later: should possibly be modified to open a selected device instead of default
  if (pDevice == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not open default device.\n";
    InitInProgress = false;
    return false;
  }
  //create context
  pContext = alcCreateContext(pDevice, NULL);
  if (pContext == NULL)
  {
    ALCenum error;
    DuskLog() << "Sound::Init: ERROR: ";
    error = alcGetError(pDevice);
    switch (error) {
      case ALC_INVALID_DEVICE:
             DuskLog() << "Specified device is not valid for output.\n";
             break;
      case ALC_INVALID_VALUE:
             DuskLog() << "Additional context cannot be created for this device.\n";
             break;
      case ALC_NO_ERROR: //should not happen here
             DuskLog() << "There is (or better should be) no error here.\n";
             break;
      default:
             DuskLog() << "Unknown error while creating context. Error code: " << error << "\n";
             break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //try to set current context
  if (alcMakeContextCurrent(pContext) == ALC_FALSE)
  {
    ALCenum error;
    DuskLog() << "Sound::Init: ERROR: Could not set current context.\n";
    error = alcGetError(pDevice);
    switch (error)
    {
      case ALC_NO_ERROR:
           DuskLog() << " There should be no error. (Funny thing, isn\'t it?)\n";
           break;
      case ALC_INVALID_CONTEXT:
           DuskLog() << " The given context pointer is invalid.\n";
           break;
      default:
           DuskLog() << " Unknown error. Error code: " << error << "\n";
           break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //the AL part is done here, so we can already set AL_Ready to true
  AL_Ready = true;

  DuskLog() << "Sound::Init: Info: OpenAL functions loaded, device opened, "
            << "and context created successfully.\n";

  //however, the OggVorbis part still needs to be done
  //now load the OggVorbis lib
  if ((PathToLib_Vorbisfile== "") || (PathToLib_Vorbisfile == "NULL"))
  {
    //if no path to the OpenAL library is given, try default value
    #if defined(_WIN32)
      PathToLib_Vorbisfile = "C:\\Windows\\System32\\vorbisfile.dll";
    #else
      PathToLib_Vorbisfile = "/usr/lib/libvorbisfile.so";
    #endif
  }

  //vorbisfile.dll (or libvorbisfile.so) tries to load ogg.dll and vorbis.dll
  //(or libogg.so and libvorbis.so), too. If these files are not found, then the
  // call to LoadLibrary/dlopen of vorbisfile will fail!
  #if defined(_WIN32)
  //Windows
  libHandleOV = LoadLibrary(PathToLib_Vorbisfile.c_str());
  #else
  //Linux goes here
  libHandleOV = dlopen(PathToLib_Vorbisfile.c_str(), RTLD_LOCAL | RTLD_LAZY);
  #endif
  if (libHandleOV == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not open OggVorbis dynamic library in \""
              << PathToLib_Vorbisfile << "\". Exiting.\n";
    InitInProgress = false;
    return (!needVorbis);
  }

  //get function addresses for OggVorbis (vorbisfile.dll/libvorbisfile.so)
  #if defined(_WIN32)
  //Windows
  ov_clear = (P_ov_clear) GetProcAddress(libHandleOV, "ov_clear");
  ov_comment = (P_ov_comment) GetProcAddress(libHandleOV, "ov_comment");
  ov_info = (P_ov_info) GetProcAddress(libHandleOV, "ov_info");
  // We should not use ov_open() on Windows systems. However, it's completely
  // fine for Linux.
  ov_open_callbacks = (P_ov_open_callbacks) GetProcAddress(libHandleOV, "ov_open_callbacks");
  ov_pcm_total = (P_ov_pcm_total) GetProcAddress(libHandleOV, "ov_pcm_total");
  ov_read = (P_ov_read) GetProcAddress(libHandleOV, "ov_read");
  ov_streams = (P_ov_streams) GetProcAddress(libHandleOV, "ov_streams");
  ov_time_total = (P_ov_time_total) GetProcAddress(libHandleOV, "ov_time_total");
  #else
  //Linux goes here
  ov_clear = (P_ov_clear) dlsym(libHandleOV, "ov_clear");
  ov_comment = (P_ov_comment) dlsym(libHandleOV, "ov_comment");
  ov_info = (P_ov_info) dlsym(libHandleOV, "ov_info");
  ov_open_callbacks = (P_ov_open_callbacks) dlsym(libHandleOV, "ov_open_callbacks");
  ov_pcm_total = (P_ov_pcm_total) dlsym(libHandleOV, "ov_pcm_total");
  ov_read = (P_ov_read) dlsym(libHandleOV, "ov_read");
  ov_streams = (P_ov_streams) dlsym(libHandleOV, "ov_streams");
  ov_time_total = (P_ov_time_total) dlsym(libHandleOV, "ov_time_total");
  #endif
  if (ov_clear == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_clear\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_comment == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_comment\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_info == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_info\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_open_callbacks == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_open_callbacks\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_pcm_total == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_pcm_total\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_read == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_read\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_streams == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_streams\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_time_total == NULL)
  {
    DuskLog() << "Sound::Init: ERROR: Could not retrieve \"ov_time_total\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  //just for curiosity/ debug reasons: get extension string
  DuskLog() << "Debug: Available AL extensions are:\n"
            << alGetString(AL_EXTENSIONS) << "\nEnd of extension list.\n"
            << "Available ALC extensions are:\n"
            << alcGetString(pDevice, ALC_EXTENSIONS) << "\nEnd of ALC extension list.\n"
            <<"alIsExtensionPresent(AL_EXT_VORBIS): "<<(int)alIsExtensionPresent("AL_EXT_VORBIS")
            << "\nEnum of AL_FORMAT_VORBIS_EXT: "<<alGetEnumValue("AL_FORMAT_VORBIS_EXT")
            << "\n";
  //all (up to this point) neccessary Vorbis functions are initialized
  Vorbis_Ready = true;
  //the basic initialization is done here, we can return true (for now,
  //  more will be done later)
  InitInProgress = false;
  return true; //this is the result we want
}

//returns true, if deinitialization of OpenAL was successful; false otherwise
bool Sound::exit()
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::Exit: Warning: OpenAL is not initialized, thus we can "
              << "not deinitialize it.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::Exit: Warning: (De-)Initialization of OpenAL is "
              << "already in progress, thus we quit here.\n";
    return false;
  }
  InitInProgress = true;

  unsigned int i;
  std::vector<std::string> object_list;
  //try to free all AL sources
  object_list = getNoiseList(false);
  for(i=0; i<object_list.size(); i++)
  {
    destroyNoise(object_list.at(i));
  }
  //try to free all file resources
  object_list = getMediaList(false);
  for(i=0; i<object_list.size(); i++)
  {
    //frees resources of given media
    destroyMedia(object_list.at(i));
  }


  //standard clean-up
  alcMakeContextCurrent(NULL); //NULL is valid for alcMakeContextCurrent, so we
                               //cannot get an error here
  alcDestroyContext(pContext);
  alcCloseDevice(pDevice);

  //release OpenAL library
  #if defined(_WIN32)
  if (libHandleAL != NULL)
  {
    if (!FreeLibrary(libHandleAL))
    {
      DuskLog() << "Sound::Exit: Error while closing OpenAL library object.\n"
                << "Error code is " << GetLastError() <<".\n";
    }
  }
  #else
  char * err_str;
  dlerror(); //clear error state
  if (libHandleAL != NULL)
  {
    if (dlclose(libHandleAL) != 0)
    {
      DuskLog() << "\nSound::Exit: Error while closing OpenAL library object.\n";
      err_str = dlerror();
      if (err_str!=NULL)
      {
        DuskLog() << "Error description: " << err_str <<"\n";
      }
    }
  }
  #endif
  libHandleAL = NULL;

  //release OggVorbis library
  #if defined(_WIN32)
  if (libHandleOV != NULL)
  {
    if (!FreeLibrary(libHandleOV))
    {
      DuskLog() << "Sound::Exit: Error while closing Vorbis library object.\n"
                << "Error code is " << GetLastError() <<".\n";
    }
  }
  #else
  dlerror(); //clear error state
  if (libHandleOV != NULL)
  {
    if (dlclose(libHandleOV) != 0)
    {
      DuskLog() << "\nSound::Exit: Error while closing Vorbis library object.\n";
      err_str = dlerror();
      if (err_str!=NULL)
      {
        DuskLog() << "Error description: " << err_str <<"\n";
      }
    }
  }
  #endif
  libHandleOV = NULL;

  //de-init pointers
  AllFuncPointersToNULL();//wrapped into private function

  AL_Ready = false;
  Vorbis_Ready = false;
  InitInProgress = false;
  return true;
}

bool Sound::isInitialized() const
{
  return AL_Ready;
}

bool Sound::hasVorbis() const
{
  return Vorbis_Ready;
}

bool Sound::isMediaPresent(const std::string& MediaIdentifier) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  TMediaRec * temp;
  temp = pMediaList;
  while (temp!=NULL)
  {
    if (temp->MediaName == MediaIdentifier)
    {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

bool Sound::isNoisePresent(const std::string& NoiseIdentifier) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL)
  {
    if (temp->NoiseName == NoiseIdentifier)
    {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

//returns list of present noise names as string vector.
// parameter:
//     -with_attached_media: if set to true, it also returns the names of
//           attached media for the noises, i.e. first element is noise name,
//           second element is media name of that noise. Third element is noise
//           name of second noise, fourth element is media name of 2nd noise,
//           and so on. Default value is false.
std::vector<std::string> Sound::getNoiseList(const bool with_attached_media) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  std::vector<std::string> result;
  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL)
  {
    result.push_back(temp->NoiseName);
    if (with_attached_media)
    {
      if (temp->attachedMedia != NULL)
      {
        result.push_back(temp->attachedMedia->MediaName);
      }//if
      else
      {
        result.push_back("");
      }//else
    }//if with media
    temp = temp->next;
  }//while
  return result;
}

//returns list of present media names as string vector.
// parameter:
//     -with_file_names: if set to true, it also returns the names of the files
//           the media was created from, i.e. first element is media name,
//           second element is file name of that media. Third element is media
//           name of second media, fourth element is file name of 2nd media, and
//           so on. Default value is false.
std::vector<std::string> Sound::getMediaList(const bool with_file_names) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class
  std::vector<std::string> result;
  TMediaRec * temp;
  temp = pMediaList;
  while(temp!=NULL)
  {
    result.push_back(temp->MediaName);
    if (with_file_names)
    {
      result.push_back(temp->FileName);
    }//if
    temp = temp->next;
  }//while
  return result;
}

bool Sound::createNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::CreateNoise: ERROR: OpenAL is not initialized, or"
              << "(de-)initialization is in progress; thus we can not create a "
              << "noise (yet).\n";
    return false;
  }

  if (isNoisePresent(NoiseIdentifier))
  {
    DuskLog() << "Sound::CreateNoise: ERROR: A noise named\""<<NoiseIdentifier
              <<"\" already exists. Creation stopped.\n";
    return false;
  }

  TNoiseRec * temp;
  ALenum error_state;
  temp = new TNoiseRec;
  temp->NoiseName = NoiseIdentifier;
  temp->attachedMedia = NULL;
  temp->next = NULL;
  temp->sourceID = 0;

  alGetError(); //clear error state
  alGenSources(1, &(temp->sourceID));
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::CreateNoise: Error while creating AL source.\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided array pointer is not valid.\n";
           break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the source.\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    delete temp;
    return false;
  }
  //insert into list
  temp->next = pNoiseList;
  pNoiseList = temp;
  return true;
}

bool Sound::destroyNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::DestroyNoise: ERROR: OpenAL is not initialized, thus "
              << "we can not destroy a noise (yet).\n";
    return false;
  }
  //no check for InitInProgress, since this should work in every state of the
  //class instance, where AL is ready; and it's called during Exit(), so a check
  //would prevent proper exit.
  if (!isNoisePresent(NoiseIdentifier))
  {
    DuskLog() << "Sound::DestroyNoise: ERROR: A noise named \""<<NoiseIdentifier
              <<"\" does not exist, hence we cannot destroy it.\n";
    return false;
  }
  ALenum error_state;
  TNoiseRec * temp;
  //remark: pNoiseList can't be NULL, since IsNoisePresent() returned true
  if (pNoiseList->NoiseName == NoiseIdentifier)
  {
    if (pNoiseList->attachedMedia!=NULL)
    {
      //detach media. Result does not matter, source will be deleted anyway
      detach(NoiseIdentifier);
    }
    alGetError(); //clear error state
    alDeleteSources(1, &(pNoiseList->sourceID));
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      DuskLog() << "Sound::DestroyNoise: ERROR: could not delete source.\n";
      switch(error_state)
      {
        case AL_INVALID_OPERATION:
             DuskLog() << "    There is no current context.\n"; break;
        case AL_INVALID_NAME:
             DuskLog() << "    Invalid source name. Corrupt structure?\n";
             break;
        default:
             DuskLog()<<"    Unknown error occured. Error code: "
                      <<(int)error_state<<".\n"; break;
      }//swi
      return false;
    }//if
    temp = pNoiseList;
    pNoiseList = pNoiseList->next;
    delete temp;
    return true;
  }//if
  //wanted source is second or later in list
  temp = pNoiseList;
  while ((temp->next!=NULL) && (temp->next->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  //check sanity
  if (temp->next==NULL)
  {
    DuskLog() << "Sound::DestroyNoise: ERROR: Could not find noise \""
              <<NoiseIdentifier<<"\", thus nothing's destroyed.\n";
    return false;
  }
  //check for attached media
  if (temp->next->attachedMedia!=NULL)
  {
    //detach media. Result does not matter, source will be deleted anyway
    detach(NoiseIdentifier);
  }//if
  //delete source
  alGetError(); //clear error state
  alDeleteSources(1, &(temp->next->sourceID));
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::DestroyNoise: ERROR: could not delete source.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt structure?\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    return false;
  }//if
  //delete noise record
  TNoiseRec * help_ptr;
  help_ptr = temp->next->next;
  delete (temp->next);
  temp->next = help_ptr;
  return true;
}


//media management routines

bool Sound::createMedia(const std::string& MediaIdentifier, const std::string& PathToMedia)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::CreateMedia: Warning: OpenAL is not initialized, thus "
              << "we cannot load a media file yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::CreateMedia: Warning: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot load a media file.\n";
    return false;
  }
  if (isMediaPresent(MediaIdentifier))
  {
    DuskLog() << "Sound::CreateMedia: ERROR: A media named\""<<MediaIdentifier
              <<"\" already exists. Creation stopped.\n";
    return false;
  }

  //check file for extension (and so for the implied file format)
  std::string ending;
  ending = PathToMedia.substr(PathToMedia.length()-4);
  std::transform(ending.begin(), ending.end(), ending.begin(), tolower);
  if (ending==".wav")
  {
    return createWAVMedia(MediaIdentifier, PathToMedia);
  }
  else if (ending==".ogg")
  {
    return createOggMedia(MediaIdentifier, PathToMedia);
  }
  else
  {
    DuskLog() << "Sound::CreateMedia: Error: File \""<<PathToMedia<<"\" does "
              << "not seem to be a Wave or a Ogg-Vorbis file. File cannot be "
              << "loaded.\n";
    return false;
  }
}

//function which actually loads the WAVE files into buffers to make the available
// to OpenAL
bool Sound::createWAVMedia(const std::string& MediaIdentifier, const std::string& PathToMedia)
{
  TRiffChunk riff_c;
  TFmtChunk fmt_c;
  TDataChunk data_c;
  std::ifstream dat;
  char * temp;

  dat.open(PathToMedia.c_str(), std::ios::in | std::ios::binary);
  if(!dat)
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: Unable to open stream for reading.\n"
              << "       File: \"" <<PathToMedia<<"\".\n\n";
    return false;
  }
  dat.read(riff_c.Riff, 4); // "RIFF"
  if ((riff_c.Riff[0]!='R') || (riff_c.Riff[1]!='I') || (riff_c.Riff[2]!='F')
       || (riff_c.Riff[3]!='F'))
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" RIFF header.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(riff_c.len), 4); //file size - 8 (in Bytes)
  dat.read(riff_c.Wave, 4); // "WAVE"
  if ((riff_c.Wave[0]!='W') || (riff_c.Wave[1]!='A') || (riff_c.Wave[2]!='V')
       || (riff_c.Wave[3]!='E'))
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" WAVE header.\n";
    dat.close();
    return false;
  }
  //Format chunk
  dat.read(fmt_c.fmt_, 4); // "fmt "
  if ((fmt_c.fmt_[0]!='f') || (fmt_c.fmt_[1]!='m') || (fmt_c.fmt_[2]!='t')
       || (fmt_c.fmt_[3]!=' '))
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" format chunk header signature.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(fmt_c.chunk_size), 4); //should have value of exactly 16
  //In case the format chunk is larger than that, everything after the 16th byte
  // will be ignored.
  if (fmt_c.chunk_size<16)
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: Format chunk of file \""
              <<PathToMedia<<"\" has incorrect size of "<<fmt_c.chunk_size
              <<" bytes. (Should be 16 instead.)\n";
    dat.close();
    return false;
  }
  else if (fmt_c.chunk_size>16)
  {
    DuskLog() << "Sound::CreateWAVMedia: Warning: Format chunk of file \""
              <<PathToMedia<<"\" is larger than 16 bytes. Actual size is "
              <<fmt_c.chunk_size<<" bytes. Everything after 16th byte will be "
              <<"ignored.\n";
  }
  dat.read((char*) &(fmt_c.FormatTag), 2); //should have value of 1 for PCM
                                        //(this is what we have for typical .wav)
  if (fmt_c.FormatTag!=1)
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: File \""<<PathToMedia<<"\" is "
              << "not of PCM format. Format index: " <<fmt_c.FormatTag<<".\n";
    dat.close();
  }
  dat.read((char*) &(fmt_c.Channels), 2);  // 1 for mono, 2 for stereo
  dat.read((char*) &(fmt_c.SamplesPerSecond), 4);
  dat.read((char*) &(fmt_c.BytesPerSecond), 4);
  dat.read((char*) &(fmt_c.BlockAlign), 2);
  dat.read((char*) &(fmt_c.BitsPerSample), 2);

  //for larger format chunks: read rest of chunk into temp buffer and discard it
  if (fmt_c.chunk_size > 16)
  {
    //check for size, again, to avoid to allocate immense amount of memory
    if (fmt_c.chunk_size <= 1024)
    {
      temp = (char*) malloc(fmt_c.chunk_size -16);
      dat.read(temp, fmt_c.chunk_size -16);
      free(temp);
    }
    else //chunk is larger than 1 KB; quite unnormal
    {
      DuskLog() << "Sound::CreateWAVMedia: ERROR: Format chunk is much too big "
                << "("<<fmt_c.chunk_size << " bytes). Exiting.\n";
      dat.close();
      return false;
    }
  }//if

  //read the data chunk
  dat.read(data_c.data, 4); // "data"
  if ((data_c.data[0]!='d') || (data_c.data[1]!='a') || (data_c.data[2]!='t')
       || (data_c.data[3]!='a'))
  {
    DuskLog() << "Sound::PlayWAV: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" data chunk header signature.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(data_c.length_of_data), 4); //Länge des folgenden Datenblocks
                                                 //bzw. der restlichen Datei

  //check if data length is valid
  if (data_c.length_of_data<fmt_c.BlockAlign)
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: Data chunk of file \""<<PathToMedia
              << "\" is to short to contain valid data. Exiting.\n";
    dat.close();
    return false;
  }

  //length check for high bound
  if (data_c.length_of_data>MaxMediaSize_MB*1024*1024)
  {
    DuskLog() << "Sound::CreateWAVMedia: ERROR: Size of PCM data from file \""
              <<PathToMedia<<"\" would be larger than "<< MaxMediaSize_MB
              << " MB. Aborting to avoid abusive memory allocation.\n";
    dat.close();
    return false;
  }//if

  //for calculations of number and size of buffers
  unsigned long buffer_size=0, buffer_num=0, i=0;
  unsigned long last_buffer_size=0;
  TMediaRec * buff_rec;
  ALenum error_state;//return value from alGetError();
  ALenum format_type;//format of data

  //Not sure about what is a good buffer size for WAVE/PCM file
  //Following line may need to be adjusted :?
  buffer_size =  32* fmt_c.BlockAlign *1024;
  //assure that buffer is not larger than amount of available data
  if (buffer_size>data_c.length_of_data)
  {
    buffer_size = data_c.length_of_data;
  }
  //buffer size needs to be an exact multiple of block align
  if ((buffer_size % fmt_c.BlockAlign)!=0)
  {
    //decrease buffer size to next lowest multiple of block align
    buffer_size = (buffer_size/fmt_c.BlockAlign) * fmt_c.BlockAlign;
    if (buffer_size==0)
    {
      buffer_size = fmt_c.BlockAlign;
    }
  }
  last_buffer_size = buffer_size;

  //determine number of buffers
  buffer_num = data_c.length_of_data/ buffer_size;
  if ((data_c.length_of_data % buffer_size)!=0)
  {
    buffer_num = buffer_num +1; //increase number of buffers
    last_buffer_size = data_c.length_of_data % buffer_size; //size of last buffer
                                                    //is diff. from regular size
  }
  //allocate memory for new record and initialise values
  buff_rec = new TMediaRec;
  buff_rec->MediaName = MediaIdentifier;
  buff_rec->FileName = PathToMedia;
  buff_rec->num_buffers = buffer_num;
  buff_rec->buffers = NULL;
  buff_rec->attached_to.clear();
  buff_rec->next = NULL;

  //allocate memory for buffer_num ALuint variables
  buff_rec->buffers = (ALuint*) malloc(sizeof(ALuint)*buffer_num);
  alGetError();//clear error state
  alGenBuffers(buffer_num, buff_rec->buffers);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "Sound::CreateWavMedia: ERROR while generating buffers for \""
              <<PathToMedia<<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided buffer array is not large enough to "
                     << "hold the requested number of buffers.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the buffers.\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    dat.close();
    alDeleteBuffers(buffer_num, buff_rec->buffers);
    delete buff_rec;
    return false;
  }

  //determine format
  if (fmt_c.BitsPerSample==16)
  {
    switch(fmt_c.Channels)
    {
      case 4:
        format_type = alGetEnumValue("AL_FORMAT_QUAD16"); break;
      case 2:
        format_type = AL_FORMAT_STEREO16; break;
      case 1:
        format_type = AL_FORMAT_MONO16; break;
      default:
        DuskLog() << "Sound::CreateWavMedia: ERROR: File \"" <<PathToMedia
                  <<"\" seems to have "<<fmt_c.Channels<<" channels. However, "
                  <<"only four, two (stereo) or one (mono) channels are "
                  <<"supported.\n";
        dat.close();
        alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
        delete buff_rec;
        return false;
        break; //pro forma :P
    }//swi
  }
  else if (fmt_c.BitsPerSample==8)
  {
    switch(fmt_c.Channels)
    {
      case 4:
        format_type = alGetEnumValue("AL_FORMAT_QUAD8"); break;
      case 2:
        format_type = AL_FORMAT_STEREO8; break;
      case 1:
        format_type = AL_FORMAT_MONO8;
      default:
        DuskLog() << "Sound::CreateWavMedia: ERROR: File \"" <<PathToMedia
                  <<"\" seems to have "<<fmt_c.Channels<<" channels. However, "
                  <<"only four, two (stereo) or one (mono) channels are "
                  <<"supported. Creation aborted.\n";
        dat.close();
        alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
        delete buff_rec;
        return false;
    }//swi
  }
  else //Bits per Sample neither 16 nor 8, thus unsupported by OpenAL
  {
    DuskLog() << "Sound::CreateWavMedia: ERROR: The sample rate of \""
              <<PathToMedia<<"\" ("<<fmt_c.BitsPerSample<< " bits per sample) "
              <<"is not supported. OpenAL supports only 8 and 16 bit samples.\n";
    dat.close();
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }
  //check for valid format enumeration value
  if (format_type == 0) //call to alGetEnumValue could not get a proper result
  {
    DuskLog() << "Sound::CreateWavMedia: ERROR: Could not find a valid OpenAL "
              << "format enumeration value. Most likely the format of \""
              <<PathToMedia<<"\" (channels: "<<fmt_c.Channels<<"; bits per sample: "
              <<fmt_c.BitsPerSample<<") is not supported.\n";
    dat.close();
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }

  //now read the data into OpenAL buffers
  temp = (char*) malloc(buffer_size);
  for (i=0; i<buffer_num-1; i=i+1)
  {
    dat.read(temp, buffer_size);
    alGetError(); //clear error state
    alBufferData(buff_rec->buffers[i], format_type, temp, buffer_size, fmt_c.SamplesPerSecond);
    error_state = alGetError();
    if (error_state!= AL_NO_ERROR)
    {
      DuskLog() << "Sound::CreateWavMedia: ERROR while buffering data.\n";
      switch (error_state)
      {
        case AL_INVALID_ENUM:
             DuskLog() <<"    The specified format does not exist.\n"; break;
        case AL_INVALID_VALUE:
             DuskLog() <<"    The size parameter is not valid for the given format"
                       <<" or the buffer is already in use.\n"; break;
        case AL_OUT_OF_MEMORY:
             DuskLog() <<"    Not enough memory to create the buffer.\n"; break;
        default:
             DuskLog() <<"    Unknown error. Error code: "<<(int)error_state
             <<".\n"; break;
      }//swi
      dat.close();
      free(temp);
      //delete all previously generated buffers
      alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
      delete buff_rec;
      return false;
    }//if
  }//for

  //read last buffer
  dat.read(temp, last_buffer_size);
  alGetError(); //clear error state
  alBufferData(buff_rec->buffers[buffer_num-1], format_type, temp, last_buffer_size, fmt_c.SamplesPerSecond);
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    DuskLog() << "Sound::CreateWavMedia: ERROR while buffering data.\n";
    switch (error_state)
    {
      case AL_INVALID_ENUM:
           DuskLog() <<"    The specified format does not exist.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() <<"    The size parameter is not valid for the given format"
                     <<" or the buffer is already in use.\n"; break;
      case AL_OUT_OF_MEMORY:
           DuskLog() <<"    Not enough memory to create the buffer.\n"; break;
      default:
           DuskLog() <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    dat.close();
    free(temp);
    //delete all previously generated buffers
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }//if
  else
  { //in case of no error we still need to close the stream and free the pointer
    dat.close();
    free(temp);
  }//else

  //we're finally done with reading the data, now put it into list
  buff_rec->next = pMediaList;
  pMediaList = buff_rec;
  return true;
}

bool Sound::createOggMedia(const std::string& MediaIdentifier, const std::string& PathToMedia)
{
  if (!Vorbis_Ready)
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: OggVorbis was not initialized, "
              << "so we cannot load an OggVorbis file here.\n";
    return false;
  }

  OggVorbis_File ov;
  vorbis_info * vinfo;
  FILE * dat;
  int section, ret;
  double time_total;
  ogg_int64_t pcm_samples;

  dat = fopen(PathToMedia.c_str(), "rb");
  if (dat==NULL)
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: Could not open file \""
              << PathToMedia << "\" via fopen properly.\n";
    return false;
  }//if

  ret = ov_open_callbacks(dat, &ov, NULL, 0, OV_CALLBACKS_DEFAULT);
  if (ret<0)
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: Could not open file \""
              << PathToMedia << "\" via callbacks properly.\n";
    switch(ret)
    {
      case OV_EREAD:
           DuskLog() << "    A read from the media returned an error.\n"; break;
      case OV_ENOTVORBIS:
           DuskLog() << "    Bitstream does not contain any Vorbis data.\n";
           break;
      case OV_EVERSION:
           DuskLog() << "    Version mismatch.\n"; break;
      case OV_EBADHEADER:
           DuskLog() << "    Invalid Vorbis header.\n"; break;
      case OV_EFAULT:
           DuskLog() << "    Internal logic error/ bug.\n"; break;
      default:
           DuskLog() << "    Unknown error. Code: " << ret <<".\n"; break;
    }//switch
    ov_clear(&ov);
    return false;
  }//if
  DuskLog() << "Sound::CreateOggMedia: Debug info: File \""<< PathToMedia
             << "\" opened properly.\n";

  vinfo = ov_info(&ov, -1);
  DuskLog() <<"Sound::CreateOggMedia: Information for \""<<PathToMedia<<"\":\n";
  if (vinfo == NULL)
  {
    DuskLog() << "Sound::CreateOggMedia: Warning: Could not get file "
              << "information for \"" << PathToMedia << "\".\n";
    ov_clear(&ov);
    return false;
  }

  DuskLog() << "    Vorbis encoder version: " << vinfo->version <<"\n"
            << "    Channels: " << vinfo->channels << "\n"
            << "    Sampling rate: " << vinfo->rate <<"\n    Bitrate:\n"
            << "    Nominal: " << vinfo->bitrate_nominal <<"\n"
            << "    Upper: " << vinfo->bitrate_upper <<"\n"
            << "    Lower: " << vinfo->bitrate_lower <<"\n";
  DuskLog() << "\n    Number of streams: "<< ov_streams(&ov) <<"\n";

  if ((vinfo->channels!=1) && (vinfo->channels!=2))
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: File \""<<PathToMedia<<"\" has "
              <<vinfo->channels<< " audio channels, however only one (mono) or "
              << "two (stereo) are supported.\n";
    ov_clear(&ov);
    return false;
  }
  time_total = ov_time_total(&ov, -1);
  if (time_total == OV_EINVAL)
  {
    DuskLog() << "Couldn't get total time for stream. Stream does not exist or "
              << "is unseekable.\n";
  }
  else
  {
    DuskLog() << "    Total time: "<<time_total<<" seconds.\n";
  }
  pcm_samples = ov_pcm_total(&ov, -1);
  if (pcm_samples == OV_EINVAL)
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: Couldn't get total sample count"
              <<" for stream. Stream does not exist or is unseekable.\n";
    ov_clear(&ov);
    return false;
  }
  DuskLog() << "    PCM samples: "<<pcm_samples<<" samples.\n";

  long int bytes_read, total_read;
  long int data_size;
  char * buffer;

  data_size = pcm_samples*vinfo->channels*2;

  if ((data_size>MaxMediaSize_MB*1024*1024)/*MaxMediaSize_MB MB (currently 30 MB)*/ || (data_size<=0))
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: Size of uncompressed stream "
              << "from file \""<<PathToMedia<<"\" would be larger than "
              << MaxMediaSize_MB << " MB. Aborting to avoid abusive memory "
              << "allocation.\n";
    ov_clear(&ov);
    return false;
  }

  buffer = new char[data_size];
  DuskLog() << "Sound::CreateOggMedia: Debug: "<<data_size<<" bytes allocated "
              <<"for uncompressed data from file \""<<PathToMedia<<"\".\n";
  section = 0;
  total_read = 0;

  do
  {
    bytes_read = ov_read(&ov, &buffer[total_read], data_size-total_read /*buffer lenght*/,
                         0 /*little endian*/, 2 /*16 bit data*/,
                         1 /*signed data*/, &section);
    if (bytes_read<0)
    {
      DuskLog() << "Sound::CreateOggMedia: ERROR while reading from file \""
                << PathToMedia << "\".\n";
      switch(bytes_read)
      {
        case OV_HOLE:
             DuskLog() << "    Interruption in data stream.\n"; break;
        case OV_EBADLINK:
             DuskLog() << "    Invalid stream section supplied.\n"; break;
        case OV_EINVAL:
             DuskLog() << "    File headers couldn't be read or are corrupt; or"
                       << " open call for supplied file failed.\n"; break;
        default:
             DuskLog() << "    Unknown error code: "<<bytes_read<<".\n"; break;
      }//switch
    }//if
    else
    {
      total_read = total_read + bytes_read;
    }//else
  } while (bytes_read>0);

  if (bytes_read<0)
  {
    delete buffer;
    ov_clear(&ov);
    return false;
  }
  DuskLog() << "Sound::CreateOggMedia: Debug: "<<total_read<<" bytes read, "
            <<data_size<<" bytes were assumed.\n";
  if (total_read!=data_size)
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR: Size miscalculations in file \""
                << PathToMedia << "\". Read "<<total_read<<" bytes, but assumed"
                <<" "<<data_size<<" bytes. Aborting.\n";
    delete buffer;
    ov_clear(&ov);
    return false;
  }//if

  //all is read, now pass it to OpenAL
  ALenum error_state, format;
  TMediaRec * buff_rec;
  if (vinfo->channels==2)
  {
    format = AL_FORMAT_STEREO16;
  }
  else
  {
    format = AL_FORMAT_MONO16;
  }

  //create new media and initialise its values
  buff_rec = new TMediaRec;
  buff_rec->MediaName = MediaIdentifier;
  buff_rec->FileName = PathToMedia;
  buff_rec->num_buffers = 1;
  buff_rec->buffers = NULL;
  buff_rec->attached_to.clear();
  buff_rec->next = NULL;

  //allocate memory for ALuint variable
  buff_rec->buffers = (ALuint*) malloc(sizeof(ALuint));
  alGetError();//clear error state
  alGenBuffers(1, buff_rec->buffers);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR while generating buffers for \""
              <<PathToMedia<<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided buffer array is not large enough to "
                     << "hold the requested number of buffers.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the buffers.\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    ov_clear(&ov);
    delete buffer;
    alDeleteBuffers(1, buff_rec->buffers);
    delete buff_rec;
    return false;
  }

  error_state = alGetError();//clear error state
  alBufferData(buff_rec->buffers[0], format, buffer, data_size, vinfo->rate);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "Sound::CreateOggMedia: ERROR while filling buffers for \""
              <<PathToMedia<<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided buffer array size is not valid for "
                     << "the given format, or the data pointer is NULL.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the buffers.\n";
           break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The specified format does not exist.\n"; break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    ov_clear(&ov);
    delete buffer;
    alDeleteBuffers(1, buff_rec->buffers);
    delete buff_rec;
    return false;
  }

  //finally, we are through
  delete buffer;
  ov_clear(&ov);
  //add media to list
  buff_rec->next = pMediaList;
  pMediaList = buff_rec;
  return true;
}

bool Sound::destroyMedia(const std::string& MediaIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::DestroyMedia: ERROR: OpenAL is not initialized, thus "
              << "we can not destroy any media (yet).\n";
    return false;
  }
  //no check for InitInProgress, since this should work in every state of the
  //class instance, where AL is ready; and it's called during Exit(), so a check
  //would prevent proper exit.
  if (!isMediaPresent(MediaIdentifier))
  {
    DuskLog() << "Sound::DestroyMedia: ERROR: A noise named \""<<MediaIdentifier
              <<"\" does not exist, hence we cannot destroy it.\n";
    return false;
  }
  ALenum error_state;
  TMediaRec * temp;
  std::vector<std::string> tempList;
  unsigned int i;

  //remark: pMediaList can't be NULL, since IsNoisePresent() returned true
  if (pMediaList->MediaName == MediaIdentifier)
  {
    if (pMediaList->attached_to.size()!=0)
    {
      //detach media. Result does not matter, source will be deleted anyway
      tempList.assign(pMediaList->attached_to.begin(), pMediaList->attached_to.end());
      for (i=0; i<tempList.size(); i=i+1)
      {
        detach(tempList[i]);
      }//for
    }
    //delete the buffers
    alGetError(); //clear error state
    alDeleteBuffers(pMediaList->num_buffers, pMediaList->buffers);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      DuskLog() << "Sound::DestroyMedia: ERROR: could not delete buffers.\n";
      switch(error_state)
      {
        case AL_INVALID_OPERATION:
             DuskLog() << "    At least one buffer is still in use and can't be"
                       <<" deleted.\n"; break;
        case AL_INVALID_NAME:
             DuskLog() << "    Invalid buffer name. Corrupt structure?\n";
             break;
        case AL_INVALID_VALUE:
             DuskLog() << "    The requested number of buffers cannot be "
                       <<"deleted.\n"; break;
        default:
             DuskLog()<<"    Unknown error occured. Error code: "
                      <<(int)error_state<<".\n"; break;
      }//swi
      return false;
    }//if
    temp = pMediaList;
    pMediaList = pMediaList->next;
    delete temp;
    return true;
  }//if

  //wanted media is second or later in list
  temp = pMediaList;
  while ((temp->next!=NULL) && (temp->next->MediaName!=MediaIdentifier))
  {
    temp = temp->next;
  }//while
  //check sanity
  if (temp->next==NULL)
  {
    DuskLog() << "Sound::DestroyMedia: ERROR: Could not find media \""
              <<MediaIdentifier<<"\", thus nothing's destroyed.\n";
    return false;
  }
  //check for attached media
  if (temp->next->attached_to.size()>0)
  {
    //detach media. Result does not matter, buffers will be deleted anyway
    tempList.assign(temp->next->attached_to.begin(), temp->next->attached_to.end());
    for (i=0; i<tempList.size(); i=i+1)
    {
      detach(tempList[i]);
    }//for
  }//if

  //delete buffers
  alGetError(); //clear error state
  alDeleteSources(temp->next->num_buffers, temp->next->buffers);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::DestroyMedia: ERROR: could not delete buffers.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    At least one buffer is still in use and can't be"
                     <<" deleted.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid buffer name. Corrupt structure?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The requested number of buffers cannot be "
                     << "deleted.\n"; break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    return false;
  }//if
  //delete media record
  TMediaRec * help_ptr;
  help_ptr = temp->next->next;
  delete (temp->next);
  temp->next = help_ptr;
  return true;
}

bool Sound::attach(const std::string& NoiseIdentifier, const std::string& MediaIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::Attach: Warning: OpenAL is not initialized, thus we "
              << "cannot attach media to noise yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::Attach: Warning: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot attach media to noise here.\n";
    return false;
  }
  if (!isNoisePresent(NoiseIdentifier))
  {
    DuskLog() << "Sound::Attach: ERROR: there is no noise named \""
              <<NoiseIdentifier<<"\", thus we cannot attach anything to it.\n";
    return false;
  }
  if (!isMediaPresent(MediaIdentifier))
  {
    DuskLog() << "Sound::Attach: ERROR: there is no media named \""
              <<MediaIdentifier<<"\", thus we cannot attach anything to it.\n";
    return false;
  }

  TNoiseRec * noise_ptr;
  TMediaRec * media_ptr;
  //get proper noise
  noise_ptr = pNoiseList;
  while ((noise_ptr!=NULL) && (noise_ptr->NoiseName!=NoiseIdentifier))
  {
    noise_ptr = noise_ptr->next;
  }
  //get proper media
  media_ptr = pMediaList;
  while ((media_ptr!=NULL) && (media_ptr->MediaName!=MediaIdentifier))
  {
    media_ptr = media_ptr->next;
  }
  //check for sanity
  if (noise_ptr==NULL || media_ptr==NULL)
  {
    DuskLog() << "Sound::Attach: ERROR: Noise or media not found. Corrupt list?\n";
    return false;
  }

  //check for attached media
  if (noise_ptr->attachedMedia!=NULL)
  {
    //there is already a attached media; detach it
    if (!detach(NoiseIdentifier))
    {
      DuskLog() << "Sound::Attach: ERROR: Noise \""<<NoiseIdentifier
                <<"\" already has a attached noise and Detach() failed.\n";
      return false;
    }
  }

  ALenum error_state;

  alGetError();//clear error state
  //Queue all of media's buffers to the source
  alSourceQueueBuffers(noise_ptr->sourceID, media_ptr->num_buffers, media_ptr->buffers);
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    DuskLog() << "Sound::Attach: ERROR while queueing buffers to source.\n";
    switch (error_state)
    {
      case AL_INVALID_NAME:
           DuskLog() <<"    The source name or one of the buffer names is not "
                     <<"valid.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() <<"    There is no context, the source has already a static"
                     <<" buffer attached or the new buffer has not the same "
                     <<"format as the other buffers in the queue.\n"; break;
      default:
           DuskLog() <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    return false;
  }//if
  noise_ptr->attachedMedia = media_ptr;
  media_ptr->attached_to.push_back(NoiseIdentifier);
  return true;
}

bool Sound::detach(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::Detach: Warning: OpenAL is not initialized, thus we "
              << "cannot detach media from noise yet.\n";
    return false;
  }
  //no check for InitInProgress, since this should work in every state if the
  //class instance, where AL is ready; and it's called during Exit(), so a check
  //would prevent proper exit.
  if (!isNoisePresent(NoiseIdentifier))
  {
    DuskLog() << "Sound::Detach: ERROR: there is no noise named \""
              <<NoiseIdentifier<<"\", thus we cannot detach anything from it.\n";
    return false;
  }

  TNoiseRec * noise_ptr;

  noise_ptr = pNoiseList;
  while ((noise_ptr!=NULL) && (noise_ptr->NoiseName!=NoiseIdentifier))
  {
    noise_ptr = noise_ptr->next;
  }
  //check for sanity
  if (noise_ptr==NULL)
  {
    DuskLog() << "Sound::Detach: ERROR: Noise \""<<NoiseIdentifier
              <<"\" not found. Corrupt list?\n";
    return false;
  }
  if (noise_ptr->attachedMedia==NULL)
  {
    //no attached media present, we don't need detach here and are done :)
    return true;
  }
  //stop playback of source, in case it is playing (or paused)
  alSourceStop(noise_ptr->sourceID);
  //actual detach
  ALenum error_state;

  alGetError();//clear error state
  alSourceUnqueueBuffers(noise_ptr->sourceID, noise_ptr->attachedMedia->num_buffers,
                         noise_ptr->attachedMedia->buffers);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::Detach: ERROR: couldn't detach buffers from source ("
              <<"noise: \""<<NoiseIdentifier<<"\", attached media: \""
              <<noise_ptr->attachedMedia->MediaName<<"\").\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name ("<<noise_ptr->sourceID
                     << "). Corrupt data structure?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    At least one buffer is still being processed and "
                     << "could not be detached.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  //adjust internal structures
  int i;
  //remove entry from media
  for(i= noise_ptr->attachedMedia->attached_to.size()-1; i>=0; i=i-1)
  {
    if (noise_ptr->attachedMedia->attached_to[i]==NoiseIdentifier)
    {
      noise_ptr->attachedMedia->attached_to[i] = noise_ptr->attachedMedia->attached_to[noise_ptr->attachedMedia->attached_to.size()-1];
      noise_ptr->attachedMedia->attached_to.erase(noise_ptr->attachedMedia->attached_to.end());
    }//if
  }//for
  //noise
  noise_ptr->attachedMedia = NULL;
  return true;
}

bool Sound::playNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::PlayNoise: Warning: OpenAL is not initialized, thus we"
              << "cannot play anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::PlayNoise: Warning: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot play anything here.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::PlayNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;

  alGetError(); //clear error state
  alSourcePlay(temp->sourceID);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::PlayNoise: ERROR: could not play source.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}


//Pauses a playing noise;
//    -pausing a noise that is either paused or stopped is a legal no-op
//    -trying to pause a non-existing noise is an no-op and will
//     return false and print a warning/hint
bool Sound::pauseNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::PauseNoise: Warning: OpenAL is not initialized, thus we"
              << "cannot pause anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::PauseNoise: Warning: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot pause anything here.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::PauseNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;

  alGetError();//clear error state
  alSourcePause(temp->sourceID);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::PauseNoise: could not pause source object.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//Resumes a previously paused noise.
//Unpausing a playing or stopped noise is legal no-op, which will return true.
bool Sound::unPauseNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::UnPauseNoise: Warning: OpenAL is not initialized, thus we"
              << "cannot unpause anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::UnPauseNoise: Warning: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot unpause anything here.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::UnPauseNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;
  ALint source_state;

  alGetError();//clear error state
  alGetSourcei(temp->sourceID, AL_SOURCE_STATE, &source_state);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::UnPauseNoise: ERROR unable to check source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should never occur, since AL_SOURCE_STATE is hardcoded
           DuskLog() << "    Specified parameter is not valid.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if

  //check state
  if (source_state == AL_PLAYING)
  {
    DuskLog() << "Sound::UnPauseNoise: Hint: Noise \""<<NoiseIdentifier
              <<"\" is already playing.\n";
  }
  else if (source_state == AL_PAUSED)
  {
    alSourcePlay(temp->sourceID);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      DuskLog() << "Sound::UnPauseNoise: ERROR: unable to play source.\n";
      switch(error_state)
      {
        case AL_INVALID_OPERATION:
             DuskLog() << "    There is no current context.\n"; break;
        case AL_INVALID_NAME:
             DuskLog() << "    Invalid source name. Data corruption?\n"; break;
        default:
             DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                       << ".\n"; break;
      }//swi
      return false;
    }//if
  }//else if AL_PAUSED
  else
  {
    DuskLog() << "Sound::UnPauseNoise: Hint: Noise \""<<NoiseIdentifier
              << "\" was not paused yet, thus we do nothing here.\n";
  }//else
  return true;
}

//Stops a playing noise and returns true on success, false on failure/error.
// Stopping an already stopped noise is legal no-op and will return true.
bool Sound::stopNoise(const std::string& NoiseIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::StopNoise: Warning: OpenAL is not initialized, thus we"
              << "cannot stop anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::StopNoise: Warning: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot stop anything here.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::StopNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;

  alGetError();//clear error state
  alSourceStop(temp->sourceID);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::StopNoise: could not stop source object.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//Sets a noise into looping mode if DoLoop==true, otherwise it gets the noise
// out of looping mode. Returns true on success, false otherwise.
bool Sound::loopNoise(const std::string& NoiseIdentifier, const bool DoLoop)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::LoopNoise: Warning: OpenAL is not initialized, thus we"
              << "cannot loop anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::LoopNoise: Warning: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot loop anything here.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::LoopNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;
  alGetError();//clear error state
  if (DoLoop)
  {
    alSourcei(temp->sourceID, AL_LOOPING, AL_TRUE);
  }
  else
  {
    alSourcei(temp->sourceID, AL_LOOPING, AL_FALSE);
  }
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::LoopNoise: ERROR: unable to set loop mode for noise \""
              << NoiseIdentifier << "\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_LOOPING is allowed enum type
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE: //should never occur, since AL_TRUE/AL_FALSE is valid
           DuskLog() << "    Given value is out of range.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

bool Sound::setNoiseOffset(const std::string& NoiseIdentifier, const float seconds)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetNoiseOffset: Warning: OpenAL is not initialized, "
              << "thus we cannot set anything yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetNoiseOffset: Warning: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot set anything here.\n";
    return false;
  }
  if (seconds<0.0f)
  {
    DuskLog() << "Sound::SetNoiseOffset: ERROR: Cannot set negative value for "
              << "offset. Aborting.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::SetNoiseOffset: ERROR: Noise \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;
  alGetError();//clear error state
  alSourcef(temp->sourceID, AL_SEC_OFFSET, seconds);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetNoiseOffset: ERROR: Unable to set offset for \""
              << NoiseIdentifier << "\" to "<<seconds<<" seconds.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //shouldn't happen, at least not with OpenAL 1.1
           DuskLog() << "    Invalid enumeration parameter. Make sure you have"
                     << " OpenAL 1.1 or higher installed.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The given offset value is out of range.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Internal data corruption?\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return false;
  }//if
  return true;
}//SetNoiseOffset

// retrieves noise offset in seconds. On error, -1.0 is returned
float Sound::getNoiseOffset(const std::string& NoiseIdentifier) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetNoiseOffset: Warning: OpenAL is not initialized, "
              << "thus we cannot get any values yet.\n";
    return -1.0f;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetNoiseOffset: Warning: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot get anything here.\n";
    return -1.0f;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::GetNoiseOffset: ERROR: Noise \""<<NoiseIdentifier
              << "\" was not found.\n";
    return -1.0f;
  }//if

  ALenum error_state;
  ALfloat seconds;
  alGetError();//clear error state
  alGetSourcef(temp->sourceID, AL_SEC_OFFSET, &seconds);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetNoiseOffset: ERROR: Unable to get offset for \""
              << NoiseIdentifier << "\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //shouldn't happen, at least not with OpenAL 1.1
           DuskLog() << "    Invalid enumeration parameter. Make sure you have"
                     << " OpenAL 1.1 or higher.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Internal data corruption?\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return -1.0;
  }//if
  return seconds;
}//GetNoiseOffset


//state retrieval
bool Sound::isPlayingNoise(const std::string& NoiseIdentifier) const
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::IsPlayingNoise: Warning: OpenAL is not initialized, or"
              << " (de-)initialisation is in progress. Thus we cannot check for"
              << " playback state.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::IsPlayingNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;
  ALint source_state;

  alGetError(); //clear error state
  alGetSourcei(temp->sourceID, AL_SOURCE_STATE, &source_state);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::IsPlayingNoise: ERROR: unable to retrieve source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_SOURCE_STATE is valid enum
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return (source_state==AL_PLAYING);
}//func

//Determines, whether a noise is in loop mode.
// -return value: true if noise is looping, false otherwise.
//                false is also returned on error.
bool Sound::isLoopingNoise(const std::string& NoiseIdentifier) const
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::IsLoopingNoise: Warning: OpenAL is not initialized, or"
              << " (de-)initialisation is in progress. Thus we cannot check for"
              << " looping state.\n";
    return false;
  }

  TNoiseRec * temp;

  temp = pNoiseList;
  while ((temp!=NULL) && (temp->NoiseName!=NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  if (temp==NULL)
  {
    DuskLog() << "Sound::IsLoopingNoise: ERROR: Noise named \""<<NoiseIdentifier
              << "\" was not found.\n";
    return false;
  }//if

  ALenum error_state;
  ALint loop_state;

  alGetError(); //clear error state
  alGetSourcei(temp->sourceID, AL_LOOPING, &loop_state);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::IsLoopingNoise: ERROR: unable to retrieve source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_LOOPING is valid enum
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return (loop_state==AL_TRUE);
}


// Sets the volume for a noise and returns true on success, false otherwise.
// Default volume for every noise is 1.0f, a value of zero means muted.
// Some implementations cut values >1.0f down to 1.0f, due to optimization.
bool Sound::setNoiseVolume(const std::string& NoiseIdentifier, const float volume)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetNoiseVolume: Warning: OpenAL is not initialized, "
              << "thus we cannot set any files volumes yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetNoiseVolume: ERROR: (De-)Initialization of OpenAL "
              << "is in progress, thus we cannot have a file here.\n";
    return false;
  }
  if (volume<0.0f)
  {
    DuskLog() << "Sound::SetNoiseVolume: ERROR: No volume values below zero "
              << "allowed. Skipping command.\n";
    return false;
  }

  ALenum error_state;
  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL)
  {
    if (temp->NoiseName == NoiseIdentifier)
    { //got it
      alGetError(); //clear error state
      alSourcef(temp->sourceID, AL_GAIN, volume);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        DuskLog() << "Sound::SetNoiseVolume: ERROR: Could not set volume for "
                  <<"noise \""<<NoiseIdentifier<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               DuskLog() << "    Value out of range.\n"; break;
          case AL_INVALID_ENUM: //should never occur, because AL_GAIN is valid
               DuskLog() << "    Invalid parameter.\n"; break;
          case AL_INVALID_NAME:
               DuskLog() << "    Invalid source("<<temp->sourceID<<"). Corrupt "
                         << "noise list?\n"; break;
          case AL_INVALID_OPERATION:
               DuskLog() << "    There is no current context.\n"; break;
          default:
               DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return false;
      }//if
      if (volume >1.0f)
      {
        DuskLog() << "Sound::SetNoiseVolume: Warning: Some OpenAL "
                  << "implementations cut volume values larger than 1.0 down to"
                  << " 1.0.\n";
      }//if
      return true;
    }//if
    temp = temp->next;
  }//while
  //file not found
  DuskLog() << "Sound::SetNoiseVolume: Warning: Noise \""<<NoiseIdentifier
            <<"\" was not found.\n";
  return false;
}//func

/*Determines the volume of a noise. A value of 1.0f is default volume, zero
  means muted. Values >1.0f can be clamped to 1.0f due to performance reasons
  by several implementations.
  Returns volume of noise. On error or if file isn't found, return value is zero
  as long as consider_MinMax == false. Otherwise, return value on error is un-
  defined.

  -parameter:
      bool consider_MinMax: if set to true, still checks for guaranteed minimum
                            and allowed maximum value and adjusts return value
                            accordingly
*/
float Sound::getNoiseVolume(const std::string& NoiseIdentifier, const bool consider_MinMax) const
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::GetNoiseVolume: Warning: OpenAL is not initialized, or"
              << "(de-)initialisation of OpenAL is in progress. Thus we cannot "
              << "retrieve any noise volumes yet.\n";
    return 0.0;
  }

  ALfloat volume_info;
  ALenum error_state;
  TNoiseRec * temp;
  temp = pNoiseList;
  while(temp!=NULL)
  {
    if (temp->NoiseName == NoiseIdentifier)
    { //noise found
      alGetError(); //clear error state
      alGetSourcef(temp->sourceID, AL_GAIN, &volume_info);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        DuskLog() << "Sound::GetNoiseVolume: ERROR: Could not retrieve source state "
                  << "for noise \""<<NoiseIdentifier<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               DuskLog() << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here, AL_GAIN is valid
               DuskLog() << "    Invalid enumeration parameter.\n"; break;
          case AL_INVALID_NAME:
               DuskLog() << "    The specified source ("<<temp->sourceID<<") is"
                         << " not valid. Corrupt noise list?\n"; break;
          case AL_INVALID_OPERATION:
               DuskLog() << "    There is no current context.\n"; break;
          default:
               DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return 0.0f; //assume something
      }//if
      //no error so far
      if (!consider_MinMax)
      {
        return volume_info;
      }
      ALfloat bound_on_gain =0.0f;
      alGetSourcef(temp->sourceID, AL_MIN_GAIN, &bound_on_gain);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        DuskLog() << "Sound::GetNoiseVolume: ERROR: Could not retrieve minimum "
                  << "bound on volume for noise \"" <<NoiseIdentifier<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               DuskLog() << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               DuskLog() << "    Invalid enumeration parameter.\n"; break;
          /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
            they would have already occured on the last call to alGetSourcef and
            we would not even get this far. ;)*/
          default:
               DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return volume_info; //return the gain value, though it might be beyond
                          // the minimum value. But we don't have a choice here.
      }//if
      //correct volume value, if below minimum
      if (bound_on_gain>volume_info)
      {
        volume_info = bound_on_gain;
      }//if min_gain > actual gain
      alGetSourcef(temp->sourceID, AL_MAX_GAIN, &bound_on_gain);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        DuskLog() << "Sound::GetNoiseVolume: ERROR: Could not retrieve maximum "
                  << "bound on volume for noise \"" <<NoiseIdentifier<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               DuskLog() << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               DuskLog() << "    Invalid enumeration parameter.\n"; break;
          /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
            they would have already occured on the last call to alGetSourcef and
            we would not even get this far. ;)*/
          default:
               DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return volume_info; //return the gain value, though it might be beyond
                          // the maximum value. But we don't have a choice here.
      }//if
      //correct volume value, if above maximum
      if (bound_on_gain<volume_info)
      {
        volume_info = bound_on_gain;
      }//if max < actual gain
      return volume_info;
    }//if
    temp = temp->next;
  }//while
  DuskLog() << "Sound::GetNoiseVolume: Warning: Noise \""<<NoiseIdentifier
           <<"\" was not found. Assuming zero volume.\n";
  return 0.0f; //no file found, hence it is "muted", i.e. volume zero
}

//sets the position of the noise in 3D. Returns true on success, false otherwise
bool Sound::setNoisePosition(const std::string& NoiseIdentifier, const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetNoisePosition: Warning: OpenAL is not initialized, "
              << "thus we cannot set the noise position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetNoisePosition: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot set a position here.\n";
    return false;
  }

  TNoiseRec * temp;
  temp = pNoiseList;
  while((temp!=NULL) && (temp->NoiseName != NoiseIdentifier))
  {
    temp = temp->next;
  }//while
  //check for sanity/ presence
  if (temp == NULL)
  {
    DuskLog() << "Sound::SetNoisePosition: ERROR: Could not find noise \""
              <<NoiseIdentifier<<"\".\n";
    return false;
  }

  ALenum error_state;
  alGetError(); //clear error state
  alSource3f(temp->sourceID, AL_POSITION, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetNoisePosition: ERROR: could not set position.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never happen, since AL_POSITION is valid
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Given values are out of range (x: "<<x<<"; y: "<<y
                     << "; z: "<<z<<").\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return false;
  }//if
  return true;
}

//Retrieves the position of a noise entity. Returns vector of zeroes on failure.
// However, a zero vector does not neccessarily indicate failure, because
// the zero vector is also a valid position for a noise placed in the origin.
std::vector<float> Sound::getNoisePosition(const std::string& NoiseIdentifier) const
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::GetNoisePosition: Warning: OpenAL is not initialized, "
              << "or (de-)initialisation is in progress. Thus we cannot get the"
              << " sound position yet.\n";
    return std::vector<float>(3, 0.0f);
  }

  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL && temp->NoiseName!=NoiseIdentifier)
  {
    temp = temp->next;
  }//while
  //check for presence
  if (temp==NULL)
  {
    DuskLog() << "Sound::GetNoisePosition: ERROR: There is no noise named \""
              <<NoiseIdentifier<<"\".\n";
    return std::vector<float>(3, 0.0f);
  }//if

  std::vector<float> result(3, 0.0f);
  ALenum error_state;

  alGetError(); //clear error state
  alGetSource3f(temp->sourceID, AL_POSITION, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetNoisePosition: ERROR: could not retrieve source "
              << "state of noise \""<<NoiseIdentifier<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_ENUM: //should never happen, AL_POSITION is valid
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    At least one value pointer is invalid.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(3, 0.0f);
  }//if
  return result;
}

//Sets velocity vector of noise and returns true on success.
//Noise position is not changed after certain time intveral; position changes
//are up to the application. Velocity is just needed for doppler calculations.
bool Sound::SetNoiseVelocity(const std::string& NoiseIdentifier, const float x, const float y, const float z)
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::SetNoiseVelocity: Warning: OpenAL is not initialized, "
              << "or (de-)initialisation is in progress. Thus we cannot set the"
              << " noise position yet.\n";
    return false;
  }

  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL && temp->NoiseName!=NoiseIdentifier)
  {
    temp = temp->next;
  }//while
  //check for NULL
  if (temp==NULL)
  {
    DuskLog() << "Sound::SetNoiseVelocity: ERROR: could not find noise \""
              <<NoiseIdentifier<<"\".\n";
    return false;
  }//if

  ALenum error_state;
  alGetError(); //clear error state
  alSource3f(temp->sourceID, AL_VELOCITY, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetNoiseVelocity: ERROR: could not set position for "
              << "noise \""<<NoiseIdentifier<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corruption of data?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_VELOCITY is valid enum
           DuskLog() << "    Invalid enumeration value given.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Given values are out of range (x: "<<x<<"; y: "<<y
                     << "; z: "<<z<<").\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//retrieve current velocity of a noise source;
// returns (0.0, 0.0, 0.0), if velocity could not be determined. However, a return
// value of (0.0, 0.0, 0.0) does not necessarily indicate an error, since this
// is also a legal return value for a non-moving source.
std::vector<float> Sound::GetNoiseVelocity(const std::string& NoiseIdentifier) const
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::GetNoiseVelocity: Warning: OpenAL is not initialized, "
              << "or (de-)initialisation is in progress. Thus we cannot set the"
              << " noise position yet.\n";
    return std::vector<float>(3, 0.0);
  }

  TNoiseRec * temp;
  temp = pNoiseList;
  while (temp!=NULL && temp->NoiseName!=NoiseIdentifier)
  {
    temp = temp->next;
  }//while
  //check for presence/ sanity
  if (temp==NULL)
  {
    DuskLog() << "Sound::GetNoiseVelocity: ERROR: could not find noise \""
              <<NoiseIdentifier<<"\". Returning zero vector.\n";
    return std::vector<float>(3, 0.0);
  }//if

  ALenum error_state;
  std::vector<float> result(3, 0.0);

  alGetError(); //clear error state
  alGetSource3f(temp->sourceID, AL_VELOCITY, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetNoiseVelocity: ERROR: could not retrieve source "
              << "velocity for noise \""<<NoiseIdentifier<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_VELOCITY is valid enum
           DuskLog() << "    Invalid enumeration value.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values supplied.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return std::vector<float>(3, 0.0);
  }//if
  return result;
}

//returns speed of sound (for doppler and such stuff)
//returns 0.0 on error
float Sound::getSpeedOfSound() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetSpeedOfSound: Warning: OpenAL is not initialized "
              << "yet, hence we cannot query anything here.\n";
    return 0.0f;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetSpeedOfSound: ERROR: (De-)Initialization of OpenAL "
              << "is in progress. No state query possbile.\n";
    return 0.0f;
  }
  ALenum error_state;
  ALfloat result=0.0f;
  alGetError();//clear error state
  result = alGetFloat(AL_SPEED_OF_SOUND);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetSpeedOfSound: ERROR: Could not query state var.\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely (as in impossible) to happen
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION: //shouldn't happen, since we always have a valid
                                 // context after initialization of Sound class
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return 0.0f;
  }//if
  return result;
}

//sets speed of sound (for doppler and such stuff)
// -returns: false on error, true otherwise
bool Sound::setSpeedOfSound(const float new_value)
{
  //values equal to or lesser than zero are rated "not funny"
  if (new_value<=0.0f)
  {
    DuskLog()<<"Sound::SetSpeedOfSound: ERROR: Only positive values allowed!\n";
    return false;
  }
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetSpeedOfSound: Warning: OpenAL is not initialized "
              << "yet, hence we cannot set any values here.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetSpeedOfSound: ERROR: (De-)Initialization of OpenAL "
              << "is in progress. No state changes possbile.\n";
    return false;
  }

  ALenum error_state;
  alGetError();//clear error state
  alSpeedOfSound(new_value);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetSpeedOfSound: ERROR: Could not set new value.\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value given ("<<new_value<<").\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}//function SetSpeedOfSound


//****listener functions****

//Sets position of the listener
bool Sound::setListenerPostion(const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetListenerPostion: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot set position here.\n";
    return false;
  }

  ALenum error_state;
  alGetError(); //clear error state
  alListener3f(AL_POSITION, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetListenerPosition: ERROR: Could not set listener's "
              << "position!\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should never happen here, since param is constant
           DuskLog() << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value, possible NaN or Inf?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//retrieves listener's position, if possible
//otherwise, return value is vector of zeroes
std::vector<float> Sound::getListenerPosition() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot get the listener's position yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerPostion: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot get a position here.\n";
    return std::vector<float>(3, 0.0f);
  }

  std::vector<float> result(3, 0.0f); //declare vector and initialize it with
                                      // three zeros (better than thrice push_b)
  ALenum error_state;

  alGetError(); //clear error state
  alGetListener3f(AL_POSITION, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerPosition: ERROR: Could not get listener's "
              << "position.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should not happen here
           DuskLog() << "    Invalid enumeration value.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values.\n";
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    //Normally we would return "false" here, but since there is no bool, we can
    //use the content of the result vector as well.
  }//if
  return result;
}

//changes listener's position relative to current listener's postion, i.e.
// simple vector addition of current pos. and parameter vector
bool Sound::translateListenerPostion(const float delta_x, const float delta_y, const float delta_z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::ListenerTranslatePostion: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::ListenerTranslatePostion: ERROR: (De-)Initialization "
              << "of OpenAL is in progress, thus we cannot set position here.\n";
    return false;
  }

  ALenum error_state;
  ALfloat current_x, current_y, current_z; //will hold current listener pos.
  alGetError(); //clear error state
  //get current position of listener
  alGetListener3f(AL_POSITION, &current_x, &current_y, &current_z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerTranslatePosition: ERROR: Could not get the "
              << "current listener position.\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //should never happen here
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context. :(\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  //now set the new position
  alListener3f(AL_POSITION, current_x+delta_x, current_y+delta_y, current_z+delta_z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerTranslatePosition: ERROR: Could not set new "
              << "listener position. :(\n";
    switch/*reloaded*/(error_state)
    {
      case AL_INVALID_ENUM: //should not happen
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context, so what?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid position value, maybe NaN or Inf?"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//sets listener's velocity and returns true on success, false on error
bool Sound::setListenerVelocity(const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetListenerVelocity: Warning: OpenAL is not "
              << "initialized, we cannot set the listener velocity yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetListenerVelocity: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot set a velocity here.\n";
    return false;
  }

  ALenum error_state;

  alGetError(); //clear error state
  alListener3f(AL_VELOCITY, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetListenerVelocity: ERROR: Could not set velocity!\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely to happen
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value given, possibly NaN? (Values: x: "<<x
                     << "; y: "<<y<<"; z: "<<z<<")\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}//function SetListenerVelocity

/*retrieves listener's velocity,
  returns (0.0, 0.0, 0.0), if velocity could not be determined. However, a return
  value of (0.0, 0.0, 0.0) does not necessarily indicate an error, since this
  is also a legal return value for a non-moving listener.

  Note to self: Maybe we should use NaN instead of (0.0, 0.0, 0.0) in case of
                error. However, this might cause problems if retrieved values
                are not checked for possible occurence of NaN. */
std::vector<float> Sound::getListenerVelocity() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerVelocity: Warning: OpenAL is not "
              << "initialized, we cannot have a listener velocity yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerVelocity: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot get a velocity here.\n";
    return std::vector<float>(3, 0.0f);
  }

  ALenum error_state;
  std::vector<float> result(3, 0.0f);

  alGetError();//clears error state
  alGetListener3f(AL_VELOCITY, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerVelocity: ERROR: Could not retrieve "
              << "velocity value of listener!\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value(s) given.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(3, 0.0f);
  }//if
  return result;
}//function GetListenerVelocity

//Gets the direction and the "up" vector of the Listener as a pair of 3-tuples
//returns two 3-dimensional zero vectors on failure
std::vector<float> Sound::getListenerOrientation() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerOrientation: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return std::vector<float>(6, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerOrientation: ERROR: (De-)Initialization "
              << "of OpenAL is in progress, thus we cannot set position here.\n";
    return std::vector<float>(6, 0.0f);
  }

  ALenum error_state;
  ALfloat orientation[6];
  std::vector<float> result;

  alGetError(); //clear error state
  alGetListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerOrientation: ERROR: Could not get listener "
              << "orientation.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The enumeration parameter is not valid.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The value pointer is invalid.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(6, 0.0f); //"return false"
  }//if
  //fill vector (ALenum directly converts to int)
  for (error_state=0; error_state<6; error_state++)
  {
    result.push_back(orientation[error_state]);
  }
  return result;
}

//rotate Listener orientation around x-, y- or z-axis... ONLY ONE OF THEM
// parameters: values (in radiant) indicating how far listener should rotate
bool Sound::rotateListener(const float x_axis, const float y_axis, const float z_axis)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::ListenerRotate: Warning: OpenAL is not initialized, "
              << "thus we cannot rotate the listener position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot rotate the listener here.\n";
    return false;
  }

  ALenum error_state;
  ALfloat orientation[6];
  ALfloat new_orientation[6];

  alGetError(); //clear error state
  alGetListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: Could not get listener's "
              << "orientation, thus we cannot rotate.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The enum parameter is invalid.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n";
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if

  //initialisation of new values with old values
  //note: error_state is of ALenum, but ALenum directly converts to int
  for (error_state=0; error_state<6; error_state++)
  {
    new_orientation[error_state] = orientation[error_state];
  }//for


  //do the rotation here
  /* Rotation matrix (2D):
            / cos x    -sin x \
     R(x) = |                 |  where x is (likely to be) in [0, 2*Pi]
            \ sin x     cos x /
  */
  float sinus, cosinus;
  if (x_axis != 0.0)
  {
    sinus = sin(x_axis);
    cosinus = cos(x_axis);
    //rotate around x axis (x-coords remain)
    new_orientation[1] = orientation[1]*cosinus - orientation[2]*sinus;//at-vector,y
    new_orientation[2] = orientation[1]*sinus + orientation[2]*cosinus;//at-vector,z
    new_orientation[4] = orientation[4]*cosinus - orientation[5]*sinus;//up-vector,y
    new_orientation[5] = orientation[4]*sinus + orientation[5]*cosinus;//up-vector,z
  }//if x_axis
  else if (y_axis != 0.0)
  {
    sinus = sin(y_axis);
    cosinus = cos(y_axis);
    //rotate around y axis (y-coords remain untouched)
    new_orientation[2] = orientation[2]*cosinus - orientation[0]*sinus;//at-vector,z
    new_orientation[0] = orientation[2]*sinus + orientation[0]*cosinus;//at-vector,x
    new_orientation[5] = orientation[5]*cosinus - orientation[3]*sinus;//up-vector,z
    new_orientation[3] = orientation[5]*sinus + orientation[3]*cosinus;//up-vector,x
  }//if y_axis
  else if (z_axis != 0.0)
  {
    sinus = sin(z_axis);
    cosinus = cos(z_axis);
    //rotate around z axis (z-coords remain untouched)
    new_orientation[0] = orientation[0]*cosinus - orientation[1]*sinus;//at_vector,x
    new_orientation[1] = orientation[0]*sinus + orientation[1]*cosinus;//at-vector,y
    new_orientation[3] = orientation[3]*cosinus - orientation[4]*sinus;//up-vector,x
    new_orientation[4] = orientation[3]*sinus + orientation[4]*cosinus;//up-vector,y
  }//if z_axis
  //set new values for at- & up-vector
  alListenerfv(AL_ORIENTATION, &new_orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: Could not set new orientation "
              << "of listener.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           DuskLog() << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value pointer given.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true; //seems like wie made it :)
}

bool Sound::getCurrentDeviceName(std::string& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getCurrentDeviceName: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getCurrentDeviceName: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }
  if (pDevice==NULL)
  {
    DuskLog() << "Sound::getCurrentDeviceName: ERROR: there is no current device.\n";
    return false;
  }

  alcGetError(pDevice); //clear error state
  const ALCchar * ptrName = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);
  const ALCenum error_state = alcGetError(pDevice);
  if (error_state==ALC_NO_ERROR)
  {
    result = ptrName;
    return true;
  }
  //error occured
  DuskLog() << "Sound::getCurrentDeviceName: ERROR: Could not get device name.\n";
  switch(error_state)
  {
    case ALC_INVALID_ENUM:
         DuskLog() << "    Invalid enum parameter.\n"; break;
    default:
         DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                   << ".\n"; break;
  }//swi
  return false;
}

bool Sound::getDefaultDeviceName(std::string& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getDefaultDeviceName: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getDefaultDeviceName: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }

  const ALCchar * ptrName;
  //check for extension ALC_ENUMERATE_ALL_EXT, that might give different devices
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
  {
    //use extension-provided name
    ptrName = alcGetString(pDevice, alcGetEnumValue(pDevice, "ALC_DEFAULT_ALL_DEVICES_SPECIFIER"));
  }
  else
  {
    //use standard
    ptrName = alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER);
  }
  if (ptrName==NULL)
  {
    //system has no output device
    DuskLog() << "Sound::getDefaultDeviceName: ERROR: There is no audio output"
              << " device on that system.\n";
    return false;
  }
  result = ptrName;
  return true;
}

bool Sound::getAvailableDevices(std::vector<std::string>& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getAvailableDevices: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getAvailableDevices: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }

  const ALCchar * ptrName;
  //check for extension ALC_ENUMERATE_ALL_EXT, that might give more devices
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
  {
    //use extension
    ptrName = alcGetString(NULL, alcGetEnumValue(pDevice, "ALC_ALL_DEVICES_SPECIFIER"));
  }
  else if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
  {
    //use standard extension
    ptrName = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  }
  else
  {
    //no suitable extension present
    DuskLog() << "Sound::getAvailableDevices: ERROR: No known enumeration "
              << " extension is available, thus we cannot retrieve a list of "
              << "device names.\n";
    return false;
  }
  result.clear();
  unsigned int offset = 0;
  std::string last;
  do
  {
    last = &ptrName[offset];
    if (not last.empty())
    {
      result.push_back(last);
    }
    offset = offset + last.length()+1;
  } while (not last.empty());
  return true;
}

//sets all AL function pointers to NULL
void Sound::AllFuncPointersToNULL(void)
{
  alcCreateContext = NULL;
  alcMakeContextCurrent = NULL;
  alcProcessContext = NULL;
  alcSuspendContext = NULL;
  alcDestroyContext = NULL;
  alcGetCurrentContext = NULL;
  alcGetContextsDevice = NULL;
  //****Device Management
  alcOpenDevice = NULL;
  alcCloseDevice = NULL;
  //****Error handling
  alcGetError = NULL;
  //****Extension handling functions (ALC)
  alcIsExtensionPresent = NULL;
  /* Disabled for now
  alcGetProcAddress = NULL;  */
  alcGetEnumValue = NULL;
  //****Query functions
  alcGetString = NULL;
  alcGetIntegerv = NULL;

  //**** Renderer State management
  alEnable = NULL;
  alDisable = NULL;
  alIsEnabled = NULL;
  //**** State retrieval
  alGetString = NULL;
  alGetBooleanv = NULL;
  alGetIntegerv = NULL;
  alGetFloatv = NULL;
  alGetDoublev = NULL;
  alGetBoolean = NULL;
  alGetInteger = NULL;
  alGetFloat = NULL;
  alGetDouble = NULL;
  //***** Error handling
  alGetError = NULL;

  //**** Extension handling func (AL)
  alIsExtensionPresent = NULL;
  alGetProcAddress = NULL;
  alGetEnumValue = NULL;

  //**** Set Listener parameters
  alListenerf = NULL;
  alListener3f = NULL;
  alListenerfv = NULL;
  alListeneri = NULL;
  alListener3i = NULL;
  alListeneriv = NULL;

  //**** Get Listener parameters
  alGetListenerf = NULL;
  alGetListener3f = NULL;
  alGetListenerfv = NULL;
  alGetListeneri = NULL;
  alGetListener3i = NULL;
  alGetListeneriv = NULL;

  //**** Create Source objects
  alGenSources = NULL;
  //**** Delete Source objects
  alDeleteSources = NULL;
  //***Verify a handle is a valid source
  alIsSource = NULL;

  //**** Set Source parameters
  alSourcef = NULL;
  alSource3f = NULL;
  alSourcefv = NULL;
  alSourcei = NULL;
  alSource3i = NULL;
  alSourceiv = NULL;

  //**** Get Source parameters
  alGetSourcef = NULL;
  alGetSource3f = NULL;
  alGetSourcefv = NULL;
  alGetSourcei = NULL;
  alGetSource3i = NULL;
  alGetSourceiv = NULL;

  //**** Source vector based playback calls
  alSourcePlayv = NULL;
  alSourceStopv = NULL;
  alSourceRewindv = NULL;
  alSourcePausev = NULL;

  //**** Source based playback calls
  alSourcePlay = NULL;
  alSourceStop = NULL;
  alSourceRewind = NULL;
  alSourcePause = NULL;

  //**** Source Queuing
  alSourceQueueBuffers = NULL;
  alSourceUnqueueBuffers = NULL;

  //**** Create Buffer objects
  alGenBuffers = NULL;
  //**** Delete Buffer objects
  alDeleteBuffers = NULL;
  //**** Verify a handle is a valid Buffer
  alIsBuffer = NULL;
  //**** Specify the data to be copied into a buffer
  alBufferData = NULL;

  //**** Set Buffer parameters
  alBufferf = NULL;
  alBuffer3f = NULL;
  alBufferfv = NULL;
  alBufferi = NULL;
  alBuffer3i = NULL;
  alBufferiv = NULL;

  //**** Get Buffer parameters
  alGetBufferf = NULL;
  alGetBuffer3f = NULL;
  alGetBufferfv = NULL;
  alGetBufferi = NULL;
  alGetBuffer3i = NULL;
  alGetBufferiv = NULL;

  //**** Global Parameters
  /* Disabled (partially) for now
  alDopplerFactor = NULL;
  alDopplerVelocity = NULL;*/
  alSpeedOfSound = NULL;
  /*alDistanceModel = NULL;
  */

  //**** OggVorbis function pointers
  ov_clear = NULL;
  ov_comment = NULL;
  ov_info = NULL;
  ov_open_callbacks = NULL;
  ov_pcm_total = NULL;
  ov_read = NULL;
  ov_streams = NULL;
  ov_time_total = NULL;
}

} //namespace

#include "Sound.h"
#include <cmath> //needed for rotation calculations

//vorbisfile callback functions for streams
size_t stream_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
int stream_seek_func(void *datasource, ogg_int64_t offset, int whence);
int stream_close_func(void *datasource);
long stream_tell_func(void *datasource);

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
  pFileList = NULL;
  //init function pointers
  AllFuncPointersToNULL();
}

//destructor
Sound::~Sound()
{
  if (AL_Ready)
  {
    Exit();
  }
}

Sound& Sound::get()
{
    static Sound Instance;
    return Instance;
}

//Initializes OpenAL, device and context for our application;
//returns: true, if initialization of OpenAL was successful; false otherwise
bool Sound::Init(std::string PathToLib_AL, std::string PathToLib_Vorbisfile, bool needVorbis)
{
  if (AL_Ready || InitInProgress)
  {
    std::cout << "Sound::Init: Warning: Cannot start initialization of OpenAL ";
    std::cout <<" since it is already initialized or init. is in progress.\n";
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
    std::cout << "Sound::Init: ERROR: Could not open OpenAL dynamic library in \""
              << PathToLib_AL << "\". Exiting.";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcCreateContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcMakeContextCurrent == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcMakeContextCurrent\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcProcessContext == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcProcessContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcSuspendContext == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcSuspendContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcDestroyContext == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcDestroyContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetCurrentContext == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcGetCurrentContext\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetContextsDevice == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcGetContextsDevice\" address.\n";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alcOpenDevice = (LPALCOPENDEVICE) GetProcAddress(libHandleAL, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) GetProcAddress(libHandleAL, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) GetProcAddress(libHandleAL, "alcGetError");
  alcGetString = (LPALCGETSTRING) GetProcAddress(libHandleAL, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) GetProcAddress(libHandleAL, "alcGetIntegerv");
  #else
  //Linux
  alcOpenDevice = (LPALCOPENDEVICE) dlsym(libHandleAL, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) dlsym(libHandleAL, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) dlsym(libHandleAL, "alcGetError");
  alcGetString = (LPALCGETSTRING) dlsym(libHandleAL, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) dlsym(libHandleAL, "alcGetIntegerv");
  #endif

  if (alcOpenDevice == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcOpenDevice\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcCloseDevice == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcCloseDevice\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetError == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcGetError\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetString == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcGetString\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alcGetIntegerv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alcGetIntegerv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alEnable\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDisable == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alDisable\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsEnabled == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alIsEnabled\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetString\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBooleanv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBooleanv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetIntegerv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetIntegerv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetFloatv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetFloatv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetDoublev == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetDoublev\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBoolean == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBoolean\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetInteger == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetInteger\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetFloat == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetFloat\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetDouble == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetDouble\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetError\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alIsExtensionPresent\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetProcAddress == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetProcAddress\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetEnumValue == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetEnumValue\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListenerf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListener3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListener3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListenerfv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListenerfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListeneri == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListeneri\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListener3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListener3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alListeneriv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alListeneriv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListenerf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListener3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListener3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListenerfv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListenerfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListeneri == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListeneri\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListener3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListener3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetListeneriv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetListeneriv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGenSources\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDeleteSources == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alDeleteSources\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsSource == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alIsSource\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcef\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSource3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSource3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcefv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcefv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcei == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcei\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSource3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSource3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceiv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceiv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcef\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSource3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSource3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcefv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcefv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcei == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcei\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSource3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSource3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceiv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourceiv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcePlayv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceStopv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceStopv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceRewindv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceRewindv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcePausev == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcePausev\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcePlay\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceStop == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceStop\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceRewind == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceRewind\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourcePause == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourcePause\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceQueueBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alSourceUnqueueBuffers == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alSourceUnqueueBuffers\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGenBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alDeleteBuffers == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alDeleteBuffers\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alIsBuffer == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alIsBuffer\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferData == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBufferData\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBufferf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBuffer3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBuffer3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferfv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBufferfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferi == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBufferi\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBuffer3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBuffer3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alBufferiv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alBufferiv\" address.\n";
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
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBufferf\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBuffer3f == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBuffer3f\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferfv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBufferfv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferi == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBufferi\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBuffer3i == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBuffer3i\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetBufferiv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetBufferiv\" address.\n";
    InitInProgress = false;
    return false;
  }

  //Initialization of device (finally)
  pDevice = alcOpenDevice(NULL); //opens default device
  //later: should possibly be modified to open a selected device instead of default
  if (pDevice == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not open default device.\n";
    InitInProgress = false;
    return false;
  }
  //create context
  pContext = alcCreateContext(pDevice, NULL);
  if (pContext == NULL)
  {
    ALCenum error;
    std::cout << "Sound::Init: ERROR: ";
    error = alcGetError(pDevice);
    switch (error) {
      case ALC_INVALID_DEVICE:
             std::cout << "Specified device is not valid for output.\n";
             break;
      case ALC_INVALID_VALUE:
             std::cout << "Additional context cannot be created for this device.\n";
             break;
      case ALC_NO_ERROR: //should not happen here
             std::cout << "There is (or better should be) no error here.\n";
             break;
      default:
             std::cout << "Unknown error while creating context. Error code: " << error << "\n";
             break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //try to set current context
  if (alcMakeContextCurrent(pContext) == ALC_FALSE)
  {
    ALCenum error;
    std::cout << "Sound::Init: ERROR: Could not set current context.\n";
    error = alcGetError(pDevice);
    switch (error)
    {
      case ALC_NO_ERROR:
           std::cout << " There should be no error. (Funny thing, isn\'t it?)\n";
           break;
      case ALC_INVALID_CONTEXT:
           std::cout << " The given context pointer is invalid.\n";
           break;
      default:
           std::cout << " Unknown error. Error code: " << error << "\n";
           break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //the AL part is done here, so we can already set AL_Ready to true
  AL_Ready = true;

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
    std::cout << "Sound::Init: ERROR: Could not open OggVorbis dynamic library in \""
              << PathToLib_Vorbisfile << "\". Exiting.";
    InitInProgress = false;
    return (!needVorbis);
  }

  //get function addresses for OggVorbis (vorbisfile.dll/libvorbisfile.so)
  #if defined(_WIN32)
  //Windows
  ov_clear = (P_ov_clear) GetProcAddress(libHandleOV, "ov_clear");
  ov_comment = (P_ov_comment) GetProcAddress(libHandleOV, "ov_comment");
  //ov_fopen = (P_ov_fopen) GetProcAddress(libHandleOV, "ov_fopen");
  ov_info = (P_ov_info) GetProcAddress(libHandleOV, "ov_info");
  // We should not use ov_open() on Windows systems. However, it's completely
  // fine for Linux.
  ov_open = (P_ov_open) GetProcAddress(libHandleOV, "ov_open");
  ov_open_callbacks = (P_ov_open_callbacks) GetProcAddress(libHandleOV, "ov_open_callbacks");
  ov_pcm_total = (P_ov_pcm_total) GetProcAddress(libHandleOV, "ov_pcm_total");
  ov_read = (P_ov_read) GetProcAddress(libHandleOV, "ov_read");
  ov_test = (P_ov_test) GetProcAddress(libHandleOV, "ov_test");
  #else
  //Linux goes here
  ov_clear = (P_ov_clear) dlsym(libHandleOV, "ov_clear");
  ov_comment = (P_ov_comment) dlsym(libHandleOV, "ov_comment");
  //ov_fopen = (P_ov_fopen) dlsym(libHandleOV, "ov_fopen");
  ov_info = (P_ov_info) dlsym(libHandleOV, "ov_info");
  ov_open = (P_ov_open) dlsym(libHandleOV, "ov_open");
  ov_open_callbacks = (P_ov_open_callbacks) dlsym(libHandleOV, "ov_open_callbacks");
  ov_pcm_total = (P_ov_pcm_total) dlsym(libHandleOV, "ov_pcm_total");
  ov_read = (P_ov_read) dlsym(libHandleOV, "ov_read");
  ov_test = (P_ov_test) dlsym(libHandleOV, "ov_test");
  #endif
  if (ov_clear == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_clear\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_comment == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_comment\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  /*if (ov_fopen == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_fopen\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }*/
  if (ov_info == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_info\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_open == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_open\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_open_callbacks == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_open_callbacks\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_pcm_total == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_pcm_total\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_read == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_read\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  if (ov_test == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"ov_test\" address.\n";
    InitInProgress = false;
    return (!needVorbis);
  }
  //just for curiosity/ debug reasons: get extension string
  std::cout << "Debug:\n  Available AL extensions are:\n"
            << alGetString(AL_EXTENSIONS) << "\nEnd of extension list.\n"
            << "Enum of AL_FORMAT_VORBIS_EXT: "<<alGetEnumValue("AL_FORMAT_VORBIS_EXT")
            <<"\nIsExtPresent(AL_EXT_vorbis): "<<(int)alIsExtensionPresent("AL_EXT_vorbis")
            <<"\nIsExtPresent(AL_EXT_VORBIS): "<<(int)alIsExtensionPresent("AL_EXT_VORBIS")
            << "\nend.\n";
  //all (up to this point) neccessary Vorbis functions are initialized
  Vorbis_Ready = true;
  //the basic initialization is done here, we can return true (for now,
  //  more will be done later)
  InitInProgress = false;
  return true; //this is the result we want
}

//returns true, if deinitialization of OpenAL was successful; false otherwise
bool Sound::Exit()
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Exit: Warning: OpenAL is not initialized, thus we can "
              << "not deinitialize it.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Exit: Warning: (De-)Initialization of OpenAL is "
              << "already in progress, thus we quit here.\n";
    return false;
  }
  InitInProgress = true;
  //try to free all file resources
  while (pFileList != NULL)
  {
    //frees resources of first file and sets pointer pFileList to next file,
    //  so we will reach NULL (end of list) sooner or later
    FreeFileResources(pFileList->FileName);
  }//while

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
      std::cout << "Sound::Exit: Error while closing OpenAL library object.\n"
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
      std::cout << "\nSound::Exit: Error while closing OpenAL library object.\n";
      err_str = dlerror();
      if (err_str!=NULL)
      {
        std::cout << "Error description: " << err_str <<"\n";
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
      std::cout << "Sound::Exit: Error while closing Vorbis library object.\n"
                << "Error code is " << GetLastError() <<".\n";
    }
  }
  #else
  dlerror(); //clear error state
  if (libHandleOV != NULL)
  {
    if (dlclose(libHandleOV) != 0)
    {
      std::cout << "\nSound::Exit: Error while closing Vorbis library object.\n";
      err_str = dlerror();
      if (err_str!=NULL)
      {
        std::cout << "Error description: " << err_str <<"\n";
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

bool Sound::Play(std::string FileName)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Play: Warning: OpenAL is not initialized, thus we can "
              << "not play a file yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Play: Warning: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot play a file and quit here.\n";
    return false;
  }

  //check whether file is already buffered
  TBufSrcRecord * temp;
  temp = pFileList;
  while (temp != NULL)
  {
    if (temp->FileName == FileName)
    {
      std::cout << "Sound::Play: Hint: File \"" <<FileName<< "\" is already "
                << "buffered. Hence we try to replay it.\n";
      return Replay(FileName);
    }//if
    temp = temp ->next;
  }//while

  //check file for extension (and so for the implied file format)
  if (FileName.substr(FileName.length()-4)==".wav")
  {
    return PlayWAV(FileName);
  }
  else if (FileName.substr(FileName.length()-4)==".ogg")
  {
    return PlayOgg(FileName);
  }
  else
  {
    std::cout << "Sound::Play: Error: File \""<<FileName<<"\" does not seem to "
              << "be a Wave or a Ogg-Vorbis file. File cannot be played.\n";
    return false;
  }
}

bool Sound::PlayWAV(std::string WAV_FileName)
{
  TRiffChunk riff_c;
  TFmtChunk fmt_c;
  TDataChunk data_c;
  std::ifstream dat;
  char * temp;

  dat.open(WAV_FileName.c_str(), std::ios::in | std::ios::binary);
  if(!dat)
  {
    std::cout << "Sound::PlayWAV: ERROR: Unable to open stream for reading.\n"
              << "       File: \"" <<WAV_FileName<<"\".\n\n";
    return false;
  }
  dat.read(riff_c.Riff, 4); // "RIFF"
  if ((riff_c.Riff[0]!='R') || (riff_c.Riff[1]!='I') || (riff_c.Riff[2]!='F')
       || (riff_c.Riff[3]!='F'))
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" has incorrect"
              <<" RIFF header.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(riff_c.len), 4); //file size - 8 (in Bytes)
  dat.read(riff_c.Wave, 4); // "WAVE"
  if ((riff_c.Wave[0]!='W') || (riff_c.Wave[1]!='A') || (riff_c.Wave[2]!='V')
       || (riff_c.Wave[3]!='E'))
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" has incorrect"
              <<" WAVE header.\n";
    dat.close();
    return false;
  }
  //Format chunk
  dat.read(fmt_c.fmt_, 4); // "fmt "
  if ((fmt_c.fmt_[0]!='f') || (fmt_c.fmt_[1]!='m') || (fmt_c.fmt_[2]!='t')
       || (fmt_c.fmt_[3]!=' '))
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" has incorrect"
              <<" format chunk header signature.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(fmt_c.chunk_size), 4); //should have value of exactly 16
  //In case the format chunk is larger than that, everything after the 16th byte
  // will be ignored.
  if (fmt_c.chunk_size<16)
  {
    std::cout << "Sound::PlayWAV: ERROR: Format chunk of file \""<<WAV_FileName
              <<"\" has incorrect size of "<<fmt_c.chunk_size
              <<" bytes. (Should be 16 instead.)\n";
    dat.close();
    return false;
  }
  else if (fmt_c.chunk_size>16)
  {
    std::cout << "Sound::PlayWAV: Warning: Format chunk of file \""<<WAV_FileName
              <<"\" is larger than 16 bytes. Actual size is "<<fmt_c.chunk_size
              <<" bytes. Everything after 16th byte will be ignored.\n";
  }
  dat.read((char*) &(fmt_c.FormatTag), 2); //should have value of 1 for PCM
                                        //(this is what we have for typical .wav)
  if (fmt_c.FormatTag!=1)
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" is not of "
              << "PCM format. Format index: " <<fmt_c.FormatTag<<".\n";
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
      std::cout << "Sound::PlayWAV: ERROR: Format chunk is much too big ("
                << fmt_c.chunk_size << " bytes). Exiting.\n";
      dat.close();
      return false;
    }
  }//if

  //read the data chunk
  dat.read(data_c.data, 4); // "data"
  if ((data_c.data[0]!='d') || (data_c.data[1]!='a') || (data_c.data[2]!='t')
       || (data_c.data[3]!='a'))
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" has incorrect"
              <<" data chunk header signature.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(data_c.length_of_data), 4); //Länge des folgenden Datenblocks
                                                 //bzw. der restlichen Datei

  unsigned long buffer_size=0, buffer_num=0, i=0;
  unsigned long last_buffer_size=0;
  TBufSrcRecord * buff_rec;
  ALenum error_state;
  //format of data
  ALenum format_type;

  //Not sure about what is a good buffer size for WAVE/PCM file
  //Following line may need to be adjusted :?
  buffer_size =  32* fmt_c.BlockAlign *1024;
  //aassure that buffer is not larger than amount of available data
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

  //check if data length is valid
  if (data_c.length_of_data<fmt_c.BlockAlign)
  {
     std::cout << "Sound::PlayWAV: ERROR: Data chunk is to short to contain "
               << "valid data. Exiting.\n";
     dat.close();
     return false;
  }

  //determine number of buffers
  buffer_num = data_c.length_of_data/ buffer_size;
  if ((data_c.length_of_data % buffer_size)!=0)
  {
    buffer_num = buffer_num +1; //increase number of buffers
    last_buffer_size = data_c.length_of_data % buffer_size; //size of last buffer
                                                    //is diff. from regular size
  }
  //allocate memory for new record
  buff_rec = new TBufSrcRecord;
  buff_rec->FileName = WAV_FileName;
  buff_rec->num_buffers = buffer_num;
  //allocate memory for buffer_num ALuint variables
  buff_rec->buffers = (ALuint*) malloc(sizeof(ALuint)*buffer_num);
  alGetError();//clear error state
  alGenBuffers(buffer_num, buff_rec->buffers);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    std::cout << "Sound::Play: ERROR while generating buffers for \""
              <<WAV_FileName <<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           std::cout << "    The provided buffer array is not large enough to "
                     << "hold the requested number of buffers.\n";
           break;
      case AL_OUT_OF_MEMORY:
           std::cout << "    Not enough memory to generate the buffers.\n";
           break;
      default:
           std::cout<<"    Unknown error occured. Error code: "
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
    if (fmt_c.Channels==4)
    {
      format_type = alGetEnumValue("AL_FORMAT_QUAD16");
    }
    if (fmt_c.Channels==2)
    {
      format_type = AL_FORMAT_STEREO16;
    }
    else if (fmt_c.Channels == 1)
    {
      format_type = AL_FORMAT_MONO16;
    }
    else
    {
      std::cout << "Sound::Play: ERROR: File \"" <<WAV_FileName<<"\" seems to "
                << "have "<<fmt_c.Channels<<" channels. However, only four, two"
                << " (stereo) or one (mono) channels are supported.\n";
      dat.close();
      alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
      delete buff_rec;
      return false;
    }
  }
  else if (fmt_c.BitsPerSample==8)
  {
    if (fmt_c.Channels==4)
    {
      format_type = alGetEnumValue("AL_FORMAT_QUAD8");
    }
    else if (fmt_c.Channels==2)
    {
      format_type = AL_FORMAT_STEREO8;
    }
    else if (fmt_c.Channels == 1)
    {
      format_type = AL_FORMAT_MONO8;
    }
    else
    {
      std::cout << "Sound::Play: ERROR: File \"" <<WAV_FileName<<"\" seems to "
                << "have "<<fmt_c.Channels<<" channels. However, only four, two"
                << " (stereo) or one (mono) channels are supported.\n";
      dat.close();
      alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
      delete buff_rec;
      return false;
    }
  }
  else //Bits per Sample neither 16 nor 8, thus unsupported by OpenAL
  {
    std::cout << "Sound::Play: ERROR: The sample rate of \"" <<WAV_FileName
              <<"\" (" << fmt_c.BitsPerSample << " bits per sample) is not "
              <<"supported. OpenAL supports only 8 and 16 bit samples.\n";
    dat.close();
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }
  //check for valid format enumeration value
  if (format_type == 0) //call to alGetEnumValue could not get a proper result
  {
    std::cout << "Sound::Play: ERROR: Could not find a valid OpenAL format "
              << "enumeration value. Most likely the format of \""<<WAV_FileName
              << "\" (channels: "<<fmt_c.Channels<<"; bits per sample: "
              <<fmt_c.BitsPerSample<<") is not supported.\n";
    dat.close();
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }
  temp = (char*) malloc(buffer_size);
  for (i=0; i<buffer_num-1; i=i+1)
  {
    dat.read(temp, buffer_size);
    alGetError(); //clear error state
    alBufferData(buff_rec->buffers[i], format_type, temp, buffer_size, fmt_c.SamplesPerSecond);
    error_state = alGetError();
    if (error_state!= AL_NO_ERROR)
    {
      std::cout << "Sound::Play: ERROR while buffering data.\n";
      switch (error_state)
      {
        case AL_INVALID_ENUM:
             std::cout <<"    The specified format does not exist.\n"; break;
        case AL_INVALID_VALUE:
             std::cout <<"    The sie parameter is not valid for the given format"
                       <<" or the buffer is already in use.\n"; break;
        case AL_OUT_OF_MEMORY:
             std::cout <<"    Not enough memory to create the buffer.\n"; break;
        default:
             std::cout <<"    Unknown error. Error code: "<<(int)error_state
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
    std::cout << "Sound::Play: ERROR while buffering data.\n";
    switch (error_state)
    {
      case AL_INVALID_ENUM:
           std::cout <<"    The specified format does not exist.\n"; break;
      case AL_INVALID_VALUE:
           std::cout <<"    The size parameter is not valid for the given format"
                     <<" or the buffer is already in use.\n"; break;
      case AL_OUT_OF_MEMORY:
           std::cout <<"    Not enough memory to create the buffer.\n"; break;
      default:
           std::cout <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
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

  //Source generation
  alGetError();//clear error state
  alGenSources(1, &(buff_rec->sourceID));
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    std::cout << "Sound::Play: ERROR while generating source.\n";
    switch (error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout <<"    There is no context to create a source in.\n"; break;
      case AL_INVALID_VALUE:
           std::cout <<"    The array pointer is not valid or there are not "
                     <<"enough resources to create a source.\n"; break;
      case AL_OUT_OF_MEMORY:
           std::cout <<"    Not enough memory to create the source.\n"; break;
      default:
           std::cout <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    //should delete previously generated buffers here
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    delete buff_rec;
    return false;
  }//if

  //Queue all buffers to the source
  alSourceQueueBuffers(buff_rec->sourceID, buff_rec->num_buffers, buff_rec->buffers);
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    std::cout << "Sound::Play: ERROR while queueing buffers to source.\n";
    switch (error_state)
    {
      case AL_INVALID_NAME:
           std::cout <<"    The source name or one of the buffer names is not "
                     <<"valid.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout <<"    There is no context, the source has already a static"
                     <<" buffer attached or the new buffer has not the same "
                     <<"format as the other buffers in the queue.\n"; break;
      default:
           std::cout <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    //should delete previously generated (and now not needed) buffers here
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    //same with source here
    alDeleteSources(1, &(buff_rec->sourceID));
    delete buff_rec;
    return false;
  }//if
  alSourcePlay(buff_rec->sourceID); //finally play it
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    std::cout << "Sound::Play: ERROR: Could not play source.\n";
    switch(error_state)
    {
      case AL_INVALID_NAME: //should not occur
           std::cout << "    The source name is not valid.\n"; break;
      case AL_INVALID_OPERATION: //should not occur either
           std::cout << "    There is no current context."; break;
      default:
           std::cout << "    Unknown error. Error code: "<<error_state<<".\n";
           break;
    }//swi
    /*We should delete previously generated (and now not needed) buffers and
     source here. I'm not completely sure whether buffers need to be unqueued
     first before being deleted or not. So we just do it.*/
    alSourceUnqueueBuffers(buff_rec->sourceID, buff_rec->num_buffers, buff_rec->buffers);
    alDeleteBuffers(buff_rec->num_buffers, buff_rec->buffers);
    alDeleteSources(1, &(buff_rec->sourceID));
    delete buff_rec;
    return false;
  }//if
  //add file to list of playing files
  buff_rec->next = pFileList;
  pFileList = buff_rec;
  return true; //this is what we want :)
}

bool Sound::PlayOgg(std::string Ogg_FileName)
{
  if (!Vorbis_Ready)
  {
    std::cout << "Sound::PlayOgg: ERROR: OggVorbis was not initialized, so we "
              << "cannot play an OggVorbis file here.\n";
    return false;
  }

  OggVorbis_File * pOggFile;
  std::ifstream* pFileStream;
  vorbis_info * pVorbisInfo;
  int result;

  pFileStream = new std::ifstream;
  pFileStream->open(Ogg_FileName.c_str(), std::ios::in|std::ios::binary);
  if (!(*pFileStream))
  {
    std::cout << "Sound::PlayOgg: ERROR: Could not open file \""<<Ogg_FileName
              << "\" for reading via stream.\n";
    return false;
  }
  std::cout << "Sound::PlayOgg: Debug: file \""<<Ogg_FileName<< "\" opened for "
            <<"reading with stream->open().\n";

  //prepare callbacks
  ov_callbacks custom_cb;
  custom_cb.read_func = stream_read_func;
  custom_cb.seek_func = stream_seek_func;
  custom_cb.tell_func = stream_tell_func;
  custom_cb.close_func = stream_close_func;

  pOggFile = new OggVorbis_File;

  //open it!
  std::cout << "Sound::PlayOgg: Debug: calling ov_open_callbacks.\n";
  result = ov_open_callbacks((void*)pFileStream, pOggFile, NULL, 0, custom_cb);
  std::cout << "Sound::PlayOgg: Debug: call of ov_open_callbacks done.\n";
  if (result !=0)
  {
    std::cout << "Sound::PlayOgg: Error while opening file \"" <<Ogg_FileName
              << "\".\n";
    switch (result)
    {
      case OV_EREAD:
           std::cout << "    Could not read from file.\n"; break;
      case OV_ENOTVORBIS:
           std::cout << "    Bitstream does not contain any Vorbis data.\n";
           break;
      case OV_EVERSION:
           std::cout << "    Version mismatch.\n"; break;
      case OV_EBADHEADER:
           std::cout << "    Invalid Vorbis header.\n"; break;
      case OV_EFAULT:
           std::cout << "    Internal logic error/ bug.\n"; break;
      default:
           std::cout << "    Unknown error. Code: " << result <<".\n"; break;
    }//swi
    ov_clear(pOggFile);
    if (*pFileStream)
    {
      pFileStream->close();
    }
    return false;
  }//if

  std::cout << "Sound::PlayOgg: Debug: file \""<<Ogg_FileName<< "\" opened for "
            <<"reading with ov_open_callbacks.\n";

  pVorbisInfo = ov_info(pOggFile, -1);
  std::cout << "Sound::PlayOgg: Information for \"" << Ogg_FileName <<"\":\n";
  if (pVorbisInfo == NULL)
  {
    std::cout << "Sound::PlayOgg: Warning: Could not get file information for "
              << "\"" << Ogg_FileName << "\".\n";
  }
  else
  {
    std::cout << "    Vorbis encoder version: " << pVorbisInfo->version <<"\n"
              << "    Channels: " << pVorbisInfo->channels << "\n"
              << "    Sampling rate: " << pVorbisInfo->rate <<"\n    Bitrate:\n"
              << "    Nominal: " << pVorbisInfo->bitrate_nominal <<"\n"
              << "    Upper: " << pVorbisInfo->bitrate_upper <<"\n"
              << "    Lower: " << pVorbisInfo->bitrate_lower <<"\n";
  }
  
  char * temp_buff; //temporary buffer
  long int buff_size; //size of buffer in bytes
  ogg_int64_t pcm_samples; //number of samples (when file is completely decoded)
  int section; //current section
  long int bytes_read=0; //number of bytes read so far
  long read_by_func; //number of bytes that were read during last ov_read-call
  
  pcm_samples = ov_pcm_total(pOggFile, -1);
  std::cout << "    Total number of samples: " << (long int)pcm_samples <<"\n";
  //assume, we get 16-bit samples, thus set buffer size accordingly
  buff_size = 2*pVorbisInfo->channels*pcm_samples;
  temp_buff = new char[buff_size];
  
  //read loop
  do
  {
    read_by_func = ov_read(pOggFile, &(temp_buff[bytes_read]), buff_size-bytes_read, 0, 2, 1, &section);
    if (read_by_func<0) //indicates error
    {
      std::cout << "Sound::PlayOgg: Error while reading/ decoding file \""
                << Ogg_FileName << "\".\n";
      switch(read_by_func)
      {
        case OV_HOLE:
             std::cout << "    There was a data interruption (one of: corrupt "
                       << "page, garbage between pages or loss of sync.\n";
             break;
        case OV_EBADLINK:
             std::cout << "    Invalid stream section was supplied, or the "
                       << "requested link is corrupt."; break;
        default:
             std::cout << "    Unknown error. Error code: " << read_by_func
                       <<".\n"; break;
      }//swi
      /*not sure whether we should jump out of the loop here either by break; or
      by setting read_by_func = 0; */
    }//if
    else //read was successfull or end of file was reached
    {
      bytes_read = bytes_read + read_by_func;
    }//else
  } while (read_by_func!=0); //a value of 0 indicates end of file, and we stop
  
  //If we were successful, all the decoded PCM data of the Ogg file should now
  //be in temp_buff. Since we did not(!) care about sample rate changes, we are
  //only able to play ogg files with constant sample rate so far. :(
  
  ALenum error_state;
  ALenum format;
  TBufSrcRecord * buff_rec;
  
  //allocate memory for new record
  buff_rec = new TBufSrcRecord;
  buff_rec->FileName = Ogg_FileName;
  buff_rec->num_buffers = 1;
  //allocate memory for buffer_num ALuint variables
  buff_rec->buffers = (ALuint*) malloc(sizeof(ALuint));
  
  
  alGetError(); //clear error state
  alGenBuffers(1, buff_rec->buffers);
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    std::cout << "Sound::PlayOgg: ERROR: Could not generate buffer for file \""
              << Ogg_FileName << "\".\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           std::cout << "    The buffer array is too small!\n"; break;
      case AL_OUT_OF_MEMORY:
           std::cout << "    Not enough memory to create the buffer.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    delete buff_rec;
    ov_clear(pOggFile);
    if (*pFileStream)
    {
      pFileStream->close();
    }
    return false;
  }//if
  
  //determine audio format of buffer
  if (pVorbisInfo->channels == 2)
  {
    format = AL_FORMAT_STEREO16;
  }
  else if (pVorbisInfo->channels == 1)
  {
    format = AL_FORMAT_MONO16;
  }
  else  //number of channels not supported (by OpenAL)
  {
    std::cout << "Sound::PlayOgg: ERROR: Format of file \""<<Ogg_FileName
              << "\" is not supported. OpenAL supports only channel numbers of "
              << "1 (mono) or 2 (stereo), but this file seems to have "
              << pVorbisInfo->channels << " channels.\n";
    delete[] temp_buff;
    //delete buffer(s) (currently only one, might change later)
    alDeleteBuffers(1, buff_rec->buffers);
    delete buff_rec;
    ov_clear(pOggFile);
    if (*pFileStream)
    {
      pFileStream->close();
    }
    if (pOggFile) { delete pOggFile; }
    if (pVorbisInfo) { delete pVorbisInfo; }
    return false;
  }//else
  
  //fill buffer with data
  alGetError(); //clear error state
  alBufferData(buff_rec->buffers[0], format, (ALvoid*)temp_buff, buff_size, pVorbisInfo->rate);
  error_state = alGetError(); //will be examined later
  //whether or not alBufferData() was successful: we can release other temp
  // buffers now. (Avoids to do it twice and saves a few lines of coding later.)
  delete[] temp_buff;
  ov_clear(pOggFile);
  if (*pFileStream)
  {
    pFileStream->close();
  }
  if (pOggFile)
  {
    delete pOggFile;
  }
  if (pVorbisInfo)
  {
    delete pVorbisInfo;
  }
  //now check for/ examine alBufferData errors
  if (error_state!=AL_NO_ERROR)
  {
    std::cout<<"Sound::PlayOgg: ERROR: Couldn't fill buffer with audio data.\n";
    switch(error_state)
    {
      case AL_OUT_OF_MEMORY:
           std::cout << "    There is not enough memory to create the buffer.\n";
           break;
      case AL_INVALID_VALUE:
           std::cout << "    The size parameter is not valid for the specified "
                     << "format, the buffer is already in use or data is a NULL"
                     << " pointer.\n"; break;
      case AL_INVALID_ENUM:
           std::cout << "    The specified format does not exist.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    alDeleteBuffers(1, buff_rec->buffers);
    delete buff_rec;
    return false;
  }//if
  
  
  //Gererate source
  alGetError(); //clear error state
  alGenSources(1, &(buff_rec->sourceID));
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    std::cout << "Sound::PlayOgg: ERROR: Could not generate source for file \""
              << Ogg_FileName << "\".\n";
    switch(error_state)
    {
      case AL_OUT_OF_MEMORY:
           std::cout << "    Not enough memory to generate the source.\n";
           break;
      case AL_INVALID_VALUE:
           std::cout << "    The array pointer is not valid or there are not "
                     <<"enough resources to create a source.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout << "    There is no context to create sources in.\n";
           break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    //delete buffer(s) (currently only one, might change later)
    alDeleteBuffers(1, buff_rec->buffers);
    delete buff_rec;
    return false;
  }//if
  
  //queue buffer to the source
  alSourceQueueBuffers(buff_rec->sourceID, 1, buff_rec->buffers);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    std::cout << "Sound::PlayOgg: ERROR: Could not queue buffer to source.\n";
    switch(error_state)
    {
      case AL_INVALID_NAME:
           std::cout << "    The buffer name or the specified source name is "
                     << "not valid.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout << "    There is no context, the source has already a static"
                     << " buffer attached or the new buffer has not the same "
                     << "format as the other buffers in the queue.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    //delete buffer(s) (currently only one, might change later) and source
    alDeleteBuffers(1, buff_rec->buffers);
    alDeleteSources(1, &(buff_rec->sourceID));
    delete buff_rec;
    return false;
  }//if
  //finally play it :)
  alSourcePlay(buff_rec->sourceID);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    std::cout << "Sound::PlayOgg: ERROR: Could not play source.\n";
    switch(error_state)
    {
      case AL_INVALID_NAME: //should not happen
           std::cout << "    The source name is not valid.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout << "    There is no context.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    //unqueue and delete buffer(s) (currently only one, might change later)
    alSourceUnqueueBuffers(buff_rec->sourceID, 1, buff_rec->buffers);
    alDeleteBuffers(1, buff_rec->buffers);
    //delete source
    alDeleteSources(1, &(buff_rec->sourceID));
    delete buff_rec;
    return false;
  }//if
  
  //we finally made it :)
  buff_rec->next = pFileList;
  pFileList = buff_rec;
  return true;
}

//returns true if the specified file is currently playing
bool Sound::IsPlaying(std::string FileName) const
{
  TBufSrcRecord * pTemp;
  ALint source_state;
  ALenum error_state;

  if (!AL_Ready)
  {
    std::cout << "Sound::IsPlaying: Warning: OpenAL is not initialized, thus we"
              << " can not have a playing file yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::IsPlaying: ERROR: (De-)Initialization of OpenAL is "
              << "in progress, thus we cannot determine file state here.\n";
    return false;
  }

  //search file list for matching file name
  pTemp = pFileList;
  while (pTemp!=NULL)
  {
    if (pTemp->FileName == FileName)
    {
      alGetError();//clear error state
      alGetSourcei(pTemp->sourceID, AL_SOURCE_STATE, &source_state);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout <<"Sound::IsPlaying: ERROR while querying source state.\n";
        switch (error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    The given pointer to ALint is not valid.\n";
               break;
          case AL_INVALID_ENUM:
               //this one should never occur here, since AL_SOURCE_STATE is valid
               std::cout << "    The source parameter is not valid.\n"; break;
          case AL_INVALID_NAME:
               std::cout << "    The specified source name is not vailid. "
                         << "Corrupt file list?\n"; break;
          case AL_INVALID_OPERATION:
               //should normally never occur
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<error_state<<".\n";
               break;
        }//swi
        return false;
      }
      else if (source_state == AL_PLAYING)
      {
        return true;
      }
    }
    pTemp = pTemp->next;
  };
  return false;
}

//Pauses a playing file;
//    -pausing a file that is either paused or stopped is a legal no-op
//    -trying to pause a non-existing file is an "expensive no-op" and will
//     result in false and a warning/hint
bool Sound::Pause(std::string FileName)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Pause: Warning: OpenAL is not initialized, thus we can"
              << " not have a playing file to pause yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Pause: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot pause a file here.\n";
    return false;
  }

  TBufSrcRecord * temp;
  ALenum error_state;

  temp = pFileList;
  while (temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //we found the appropriate file and it's source
      alGetError();//clear error state
      alSourcePause(temp->sourceID);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::Pause: ERROR while trying to pause file.\n";
        switch (error_state)
        {
          case AL_INVALID_NAME:
               std::cout << "    The specified source name is not valid.\n";
               break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error occured. Error code: "
                         <<error_state<<"\n."; break;
        }//swi
        return false; //shit happens, source was not paused
      }//if
      else
      { //no error occured
        return true; //what we want :)
      }
    }//if
    temp = temp->next;
  }//while
  std::cout << "Sound::Pause: Hint: Could not pause \""<<FileName<<"\". There "
            << "is no such file.\n";
  return false;
}

//Resumes a previously; unpausing a playing or stopped file is legal no-op
bool Sound::UnPause(std::string FileName)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::UnPause: Warning: OpenAL is not initialized, thus we "
              << "can not have a paused file to resume to playing yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::UnPause: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot resume a file to playing here.\n";
    return false;
  }

  TBufSrcRecord * temp;
  ALint source_state;
  ALenum error_state;

  temp = pFileList;
  while (temp != NULL)
  {
    if (temp->FileName == FileName)
    { //we found the appropriate file
      alGetError();//clear error state
      alGetSourcei(temp->sourceID, AL_SOURCE_STATE, &source_state);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::UnPause: ERROR: Could not check for source state.\n";
        switch (error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    The pointer to ALint is invalid.\n"; break;
          case AL_INVALID_ENUM://shouldn't occur, since AL_SOURCE STATE is valid
               std::cout << "    Invalid parameter given.\n"; break;
          case AL_INVALID_NAME: //shouldn't occur, if pFileList is not corrupt
               std::cout << "    Invalid source name("<< temp->sourceID
                         <<").\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error occured. Error code: "
                         <<error_state<<"\n."; break;
        }//swi
        return false;
      }//if
      else
      { //no error so far
        if (source_state == AL_PLAYING)
        { //legal no-op
          std::cout << "Sound::UnPause: Hint: File \""<<FileName<<"\"is already"
                    << " playing. No need to UnPause.\n";
          return true;
        }
        else if (source_state == AL_STOPPED || source_state == AL_INITIAL)
        { //legal no-op
          return true;
        }
        else
        { //state is AL_PAUSED
          alGetError();//clear error state
          alSourcePlay(temp->sourceID);//resume to playing
          error_state = alGetError();
          if (error_state != AL_NO_ERROR)
          {
            std::cout << "Sound::UnPause: ERROR: Couldn't unpause file \""
                      << FileName << "\".\n";
            switch (error_state)
            {
              case AL_INVALID_NAME: //shouldn't happen
                   std::cout << "    The source name("<< temp->sourceID
                             <<") is invalid.\n"; break;
              case AL_INVALID_OPERATION:
                   std::cout << "    There is no current context.\n"; break;
              default:
                   std::cout << "    Unknown error occured. Error code: "
                             <<error_state<<"\n."; break;
            }//swi
            return false;
          }
          return true;//luckily we managed to unpause
        }//else
      }//else
    }//if
    temp = temp->next;
  }//while
  std::cout << "Sound::UnPause: Hint: Could not resume \""<<FileName<<"\" to "
            << "playing. There is no such file.\n";
  return false;
}

//Stops a playing file; stopping an already stopped file is legal no-op
bool Sound::Stop(std::string FileName)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Stop: Warning: OpenAL is not initialized, thus we can"
              << " not have a playing file to stop yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Stop: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot stop a file here.\n";
    return false;
  }

  TBufSrcRecord * temp;
  ALenum error_state;

  temp = pFileList;
  while (temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //we found the appropriate file and it's source
      alGetError();//clear error state
      alSourceStop(temp->sourceID);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::Stop: ERROR while trying to stop file.\n";
        switch (error_state)
        {
          case AL_INVALID_NAME:
               std::cout << "    The specified source name is not valid.\n";
               break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error occured. Error code: "
                         <<error_state<<"\n."; break;
        }//swi
        return false; //shit happens, source was not stopped
      }//if
      else
      { //no error occured
        return true; //what we want :)
      }
    }//if
    temp = temp->next;
  }//while
  std::cout << "Sound::Stop: Hint: Could not stop \""<<FileName<<"\". There "
            << "is no such file.\n";
  return false;
}

//Rewinds a file and starts to play it again from the beginning
bool Sound::Replay(std::string FileName)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Replay: Warning: OpenAL is not initialized, thus we "
              << "can not replay file yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Replay: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot replay a file here.\n";
    return false;
  }

  TBufSrcRecord * temp;
  ALenum error_state;
  ALint source_state;

  temp = pFileList;
  while (temp != NULL)
  {
    if (temp->FileName == FileName)
    { //got it
      alGetError(); //clear error state
      alGetSourcei(temp->sourceID, AL_SOURCE_STATE, &source_state);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::Replay: ERROR: Could not retrieve source state.\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    The pointer to ALint is invalid.\n"; break;
          case AL_INVALID_ENUM://shouldn't occur, since AL_SOURCE STATE is valid
               std::cout << "    Invalid parameter given.\n"; break;
          case AL_INVALID_NAME: //shouldn't occur, if pFileList is not corrupted
               std::cout << "    Invalid source name("<< temp->sourceID
                         <<").\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error occured. Error code: "
                         <<error_state<<"\n."; break;
        }//swi
        return false;
      }//if
      else
      {
        //we got the state, so go on
        if (source_state != AL_INITIAL) //a AL_INITIAL source is already rewound
        {
          alGetError();
          alSourceRewind(temp->sourceID);
          error_state = alGetError();
          if (error_state != AL_NO_ERROR)
          {
            std::cout << "Sound::Replay: ERROR: Could not rewind source.\n";
            switch(error_state)
            {
              case AL_INVALID_OPERATION:
                   std::cout << "    There is no current context.\n"; break;
              case AL_INVALID_NAME: //shouldn't happen, if pFileList is not corrupted
                   std::cout << "    Invalid source name(" << temp->sourceID
                             << ").\n"; break;
              default:
                   std::cout << "    Unknown error. Error code: " << error_state
                             << ".\n"; break;
            }//swi
            return false;
          }//if
        }//if
        //source state is now AL_INITIAL
        alGetError();
        alSourcePlay(temp->sourceID);
        error_state = alGetError();
        if (error_state != AL_NO_ERROR)
        {
          std::cout << "Sound::Replay: ERROR: Could not play source.\n";
          switch(error_state)
          {
            case AL_INVALID_OPERATION:
                 std::cout << "    There is no current context.\n"; break;
            case AL_INVALID_NAME:
                 std::cout << "    Invalid source name(" << temp->sourceID
                           << ").\n"; break;
            default:
                 std::cout << "    Unknown error. Error code: " << error_state
                           << ".\n"; break;
          }//swi
          return false;
        }//if
        return true;
      }//else
    }//if
    temp = temp->next;
  }//while
  std::cout << "Sound::Replay: Hint: Could not replay \""<<FileName<<"\". There"
            << " is no such file.\n";
  return false;
}

/*Sets an audio file into looping mode if DoLoop==true, otherwise it gets the
   file out of looping mode. Returns true on success, false otherwise.*/
bool Sound::Loop(std::string FileName, bool DoLoop)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::Loop: Warning: OpenAL is not initialized, thus we "
              << "can not have any (looping) files yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::Loop: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot have a looping file here.\n";
    return false;
  }
  ALenum error_state;
  TBufSrcRecord * temp;
  temp = pFileList;
  while (temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //found it
      alGetError();
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
        std::cout << "Sound::Loop: ERROR: Could not set loop mode for file \""
                  << FileName << "\".\n";
        switch (error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    The given value is out of range.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               std::cout << "    The specified parameter is not valid.\n";
               break;
          case AL_INVALID_NAME:
               std::cout << "    The source name("<<temp->sourceID<<") is not "
                         << "valid. Corrupt internal file list?\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return false;
      }//if
      return true;
    }//if
    temp = temp->next;
  }//while
  //there is no such file
  return false;
}

/*Determines, whether a sound file is in loop mode
   return value: true if file is looping, false otherwise*/
bool Sound::IsLooping(std::string FileName) const
{ 
  if (!AL_Ready)
  {
    std::cout << "Sound::IsLooping: Warning: OpenAL is not initialized, thus we "
              << "can not have any looping files yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::IsLooping: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot have a looping file here.\n";
    return false;
  }
  TBufSrcRecord * temp;
  temp = pFileList;
  
  ALint loop_state;
  ALenum error_state;
  
  while (temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //found file
      alGetError(); //clear error state
      alGetSourcei(temp->sourceID, AL_LOOPING, &loop_state);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::IsLooping: ERROR: Could not retrieve source state "
                  << "for file \""<<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    The pointer to ALint is invalid.\n"; break;
          case AL_INVALID_ENUM://shouldn't occur, since AL_SOURCE STATE is valid
               std::cout << "    Invalid parameter given.\n"; break;
          case AL_INVALID_NAME: //shouldn't occur, if pFileList is not corrupted
               std::cout << "    Invalid source name("<< temp->sourceID
                         <<"). Corrupt file list?\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error occured. Error code: "
                         <<error_state<<"\n."; break;
        }//swi
        return false; //assume source is not in loop mode, though we can't know
      }//if
      return (loop_state == AL_TRUE);
    }//if
    temp = temp->next;
  }//while
  return false;
}

/* Sets the volume for a file and returns true on success, false otherwise.
   Default volume for every file is 1.0f, a value of zero means muted.
   Some implementations cut values >1.0f down to 1.0f, due to optimization. */
bool Sound::SetVolume(std::string FileName, const float volume)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::SetVolume: Warning: OpenAL is not initialized, thus we"
              << "cannot set any files volumes yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::SetVolume: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot have a file here.\n";
    return false;
  }
  if (volume<0.0f)
  {
    std::cout << "Sound::SetVolume: ERROR: No volume values below zero allowed."
              << "Skipping command.\n";
    return false;
  }
  ALenum error_state;
  TBufSrcRecord * temp;
  temp = pFileList;
  while (temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //got it
      alGetError(); //clear error state
      alSourcef(temp->sourceID, AL_GAIN, volume);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::SetVolume: ERROR: Could not set volume for file \""
                  <<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    Value out of range.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               std::cout << "    Invalid parameter.\n"; break;
          case AL_INVALID_NAME:
               std::cout << "    Invalid source("<<temp->sourceID<<"). Corrupt "
                         << "file list?\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return false;
      }//if
      if (volume >1.0f)
      {
        std::cout << "Sound::SetVolume: Warning: Some OpenAL implementations"
                  << " cut volume values larger than 1.0 down to 1.0.\n";
      }//if
      return true;
    }//if
    temp = temp->next;
  }//while
  //file not found
  std::cout << "Sound::SetVolume: Warning: File \""<<FileName<<"\" was not "
            <<"found.\n";
  return false;
}

/*Determines the volume of a file. A value of 1.0f is default volume, zero means
  muted. Values >1.0f can be clamped to 1.0f due to performance reasons by
  several implementations.
  Returns volume of file. On error or if file isn't found, return value is zero
  as long as consider_MinMax == false. Otherwise, return value on error is un-
  specified.
  
  -parameter:
      bool consider_MinMax: if set to true, still checks for guaranteed minimum
                            and allowed maximum value and adjusts return value
                            accordingly
*/
float Sound::GetVolume(std::string FileName, bool consider_MinMax) const
{
  if (!AL_Ready)
  {
    std::cout << "Sound::GetVolume: Warning: OpenAL is not initialized, thus we"
              << "cannot retrieve any files volumes yet.\n";
    return 0.0;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::GetVolume: ERROR: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot have a file here.\n";
    return 0.0;
  }
  
  ALfloat volume_info;
  ALenum error_state;
  TBufSrcRecord * temp;
  temp = pFileList;
  while(temp!=NULL)
  {
    if (temp->FileName == FileName)
    { //file found
      alGetError(); //clear error state
      alGetSourcef(temp->sourceID, AL_GAIN, &volume_info);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::GetVolume: ERROR: Could not retrieve source state "
                  << "for file \""<<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               std::cout << "    Invalid enumeration parameter.\n"; break;
          case AL_INVALID_NAME:
               std::cout << "    The specified source ("<<temp->sourceID<<") is"
                         << " not valid. Corrupt file list?\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
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
        std::cout << "Sound::GetVolume: ERROR: Could not retrieve minimum bound"
                  << " on volume for file \"" <<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               std::cout << "    Invalid enumeration parameter.\n"; break;
          /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
            they would have already occured on the last call to alGetSourcef and
            we would not even get this far. ;)*/
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
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
        std::cout << "Sound::GetVolume: ERROR: Could not retrieve maximum bound"
                  << " on volume for file \"" <<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    Invalid value pointer.\n"; break;
          case AL_INVALID_ENUM: //should never occur here
               std::cout << "    Invalid enumeration parameter.\n"; break;
          /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
            they would have already occured on the last call to alGetSourcef and
            we would not even get this far. ;)*/
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
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
  std::cout << "Sound::GetVolume: Warning: File \""<<FileName<<"\" was not "
            <<"found. Assuming zero volume.\n";
  return 0.0f; //no file found, hence it is "muted", i.e. volume zero
}

//Sets position of the listener
bool Sound::SetListenerPostion(const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::SetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::SetListenerPostion: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot set position here.\n";
    return false;
  }
  
  ALenum error_state;
  alGetError(); //clear error state
  alListener3f(AL_POSITION, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    std::cout << "Sound::SetListenerPosition: ERROR: Could not set listener's "
              << "position!\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should never happen here, since param is constant
           std::cout << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid value, possible NaN or Inf?\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

std::vector<float> Sound::GetListenerPosition() const
{
  if (!AL_Ready)
  {
    std::cout << "Sound::GetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot get the listener's position yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    std::cout << "Sound::GetListenerPostion: ERROR: (De-)Initialization of "
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
    std::cout << "Sound::GetListenerPosition: ERROR: Could not get listener's "
              << "position.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should not happen here
           std::cout << "    Invalid enumeration value.\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid pointer values.\n";
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    //Normally we would return "false" here, but since there is no bool, we can
    //use the content of the result vector as well.
  }//if
  return result;
}

//changes listener's position relative to current listener's postion, i.e.
// simple vector addition of current pos. and parameter vector
bool Sound::ListenerTranslatePostion(const float delta_x, const float delta_y, const float delta_z)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::ListenerTranslatePostion: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::ListenerTranslatePostion: ERROR: (De-)Initialization "
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
    std::cout << "Sound::ListenerTranslatePosition: ERROR: Could not get the "
              << "current listener position.\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //should never happen here
           std::cout << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context. :(\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid pointer values.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  //now set the new position
  alListener3f(AL_POSITION, current_x+delta_x, current_y+delta_y, current_z+delta_z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    std::cout << "Sound::ListenerTranslatePosition: ERROR: Could not set new "
              << "listener position. :(\n";
    switch/*reloaded*/(error_state)
    {
      case AL_INVALID_ENUM: //should not happen
           std::cout << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context, so what?\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid position value, maybe NaN or Inf?"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//Gets the direction and the "up" vector of the Listener as a pair of 3-tuples
//returns to zero vectors on failure
std::vector<float> Sound::GetListenerOrientation() const
{
  if (!AL_Ready)
  {
    std::cout << "Sound::GetListenerOrientation: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return std::vector<float>(6, 0.0f);
  }
  if (InitInProgress)
  {
    std::cout << "Sound::GetListenerOrientation: ERROR: (De-)Initialization "
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
    std::cout << "Sound::GetListenerOrientation: ERROR: Could not get listener "
              << "orientation.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           std::cout << "    The enumeration parameter is not valid.\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    The value pointer is invalid.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
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

//rotate Listener orientation around x-, y- or z-axis... or all of them
// parameters: values (in radiant) indicating how far listener should rotate
bool Sound::ListenerRotate(const float x_axis, const float y_axis, const float z_axis)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::ListenerRotate: Warning: OpenAL is not initialized, "
              << "thus we cannot rotate the listener position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::ListenerRotate: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot rotate the listener here.\n";
    return false;
  }
  
  ALenum error_state;
  ALfloat orientation[6];
  
  alGetError(); //clear error state
  alGetListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    std::cout << "Sound::ListenerRotate: ERROR: Could not get listener's "
              << "orientation, thus we cannot rotate.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           std::cout << "    The enum parameter is invalid.\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid pointer value.\n";
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
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
    orientation[1] = orientation[1]*cosinus - orientation[2]*sinus;//at-vector,y
    orientation[2] = orientation[1]*sinus + orientation[2]*cosinus;//at-vector,z
    orientation[4] = orientation[4]*cosinus - orientation[5]*sinus;//up-vector,y
    orientation[5] = orientation[4]*sinus + orientation[5]*cosinus;//up-vector,z
  }//if x_axis
  if (y_axis != 0.0)
  {
    sinus = sin(y_axis);
    cosinus = cos(y_axis);
    //rotate around y axis (y-coords remain untouched)
    orientation[2] = orientation[2]*cosinus - orientation[0]*sinus;//at-vector,z
    orientation[0] = orientation[2]*sinus + orientation[0]*cosinus;//at-vector,x
    orientation[5] = orientation[5]*cosinus - orientation[3]*sinus;//up-vector,z
    orientation[3] = orientation[5]*sinus + orientation[3]*cosinus;//up-vector,x
  }//if y_axis
  if (z_axis != 0.0)
  {
    sinus = sin(z_axis);
    cosinus = cos(z_axis);
    //rotate around z axis (z-coords remain untouched)
    orientation[0] = orientation[0]*cosinus - orientation[1]*sinus;//at_vector,x
    orientation[1] = orientation[0]*sinus + orientation[1]*cosinus;//at-vector,y
    orientation[3] = orientation[3]*cosinus - orientation[4]*sinus;//up-vector,x
    orientation[4] = orientation[3]*sinus + orientation[4]*cosinus;//up-vector,y
  }//if z_axis
  //set new values for at- & up-vector
  alListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    std::cout << "Sound::ListenerRotate: ERROR: Could not set new orientation "
              << "of listener.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           std::cout << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           std::cout << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           std::cout << "    Invalid value pointer given.\n"; break;
      default:
           std::cout << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true; //seems like wie made it :)
}

//sets the postion of a sound source
bool Sound::SetSoundPosition(const std::string FileName, const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    std::cout << "Sound::SetSoundPosition: Warning: OpenAL is not initialized, "
              << "thus we cannot set the sound position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    std::cout << "Sound::SetSoundPosition: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot set a position here.\n";
    return false;
  }
  
  ALenum error_state;
  TBufSrcRecord * temp;
  
  temp = pFileList;
  while (temp != NULL)
  {
    if (temp->FileName == FileName)
    { //found it!
      alGetError(); //clear error state
      alSource3f(temp->sourceID, AL_POSITION, x, y, z);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::SetSoundPosition: ERROR: Could not set source "
                  << "position for file \""<<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          case AL_INVALID_VALUE:
               std::cout << "    The given value is out of range.\n"; break;
          case AL_INVALID_ENUM: //should not happen
               std::cout << "    The specified parameter is invalid.\n"; break;
          case AL_INVALID_NAME:
               std::cout << "    The source name("<<temp->sourceID<<") is not "
                         << "valid. Corrupt file list?\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        return false;
      }//if
      return true;
    }//if
    temp = temp->next;
  }//while
  //file does not exist in pFileList
  std::cout << "Sound::SetSoundPosition: ERROR: Couldn't find file \""<<FileName
            << "\" to set position.\n";
  return false;
}

//determines the position of a sound source
std::vector<float> Sound::GetSoundPosition(const std::string FileName) const
{
  if (!AL_Ready)
  {
    std::cout << "Sound::GetSoundPosition: Warning: OpenAL is not initialized, "
              << "thus we cannot get the sound position yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    std::cout << "Sound::GetSoundPosition: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot get a position here.\n";
    return std::vector<float>(3, 0.0f);
  }
  
  ALenum error_state;
  TBufSrcRecord * temp;
  std::vector<float> result(3, 0.0f);
  
  temp = pFileList;
  while (temp != NULL)
  {
    if (temp->FileName == FileName)
    { //got it
      alGetError(); //clear error state
      alGetSource3f(temp->sourceID, AL_POSITION, &result[0], &result[1], &result[2]);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::GetSoundPosition: ERROR: Could not get sound "
                  << "position for file \""<<FileName<<"\".\n";
        switch(error_state)
        {
          case AL_INVALID_VALUE:
               std::cout << "    Invalid pointer values.\n"; break;
          case AL_INVALID_ENUM: //shouldn't happen
               std::cout << "    Invalid enumeration value.\n"; break;
          case AL_INVALID_OPERATION:
               std::cout << "    There is no current context.\n"; break;
          case AL_INVALID_NAME:
               std::cout << "    The source name ("<<temp->sourceID<<") is not "
                         << "valid. Corrupt file list?\n"; break;
          default:
               std::cout << "    Unknown error. Error code: "<<(int)error_state
                         << ".\n"; break;
        }//swi
        //normally we should return "false" here, but since we pass a vector, we
        //can use the other value as well.
      }//if
      return result;
    }//if
  }//while
  //file not found
  std::cout << "Sound::GetSoundPosition: ERROR: Couldn't find file \""<<FileName
            << "\" in file list.\n";
  return result;
}

//Frees all buffers of a file - if present.
//  freeing a not buffered file is a legal no-op, but should result in false
bool Sound::FreeFileResources(std::string FileName)
{
  TBufSrcRecord * temp;
  TBufSrcRecord * temp2;
  ALenum error_state;

  if (pFileList == NULL)
  {
    std::cout << "Sound::FreeFileResources: Hint: Couldn't free resources for "
              << "file \""<<FileName<<"\". There are no resources at all.\n";
    return false;
  }
  if (pFileList->FileName == FileName)
  { //first entry is to be removed
    alGetError();//clear error state
    alSourceStop(pFileList->sourceID);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      std::cout << "Sound::FreeFileResources: Error: Could not stop source for"
                << " file \""<<FileName<<"\". Aborting.\n";
      return false;
    }
    alSourceUnqueueBuffers(pFileList->sourceID, pFileList->num_buffers,
                           pFileList->buffers);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      std::cout << "Sound::FreeFileResources: Error: Could not unqueue buffers"
                << " of file \""<<FileName<<"\". Aborting.\n";
      return false;
    }
    alDeleteBuffers(pFileList->num_buffers, pFileList->buffers);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      std::cout << "Sound::FreeFileResources: Error: Could not delete buffers"
                << " of file \""<<FileName<<"\". Aborting.\n";
      return false;
    }
    temp = pFileList;
    pFileList = pFileList->next;
    delete temp;
    return true;
  }
  temp = pFileList;
  while (temp->next != NULL)
  {
    //here needs work to be done
    if (temp->next->FileName == FileName)
    {
      alSourceStop(temp->next->sourceID);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::FreeFileResources: Error: Could not stop source "
                  << " for file \""<<FileName<<"\". Aborting.\n";
        return false;
      }
      alSourceUnqueueBuffers(temp->next->sourceID, temp->next->num_buffers,
                             temp->next->buffers);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::FreeFileResources: Error: Could not unqueue "
                  << "buffers of file \""<<FileName<<"\". Aborting.\n";
        return false;
      }
      alDeleteBuffers(temp->next->num_buffers, temp->next->buffers);
      error_state = alGetError();
      if (error_state != AL_NO_ERROR)
      {
        std::cout << "Sound::FreeFileResources: Error: Could not delete buffers"
                  << " of file \""<<FileName<<"\". Aborting.\n";
        return false;
      }
      temp2 = temp->next;
      temp->next = temp->next->next;
      delete temp2;
      return true;
    }
    temp = temp->next;
  }//while
  std::cout << "Sound::FreeFileResources: Hint: Couldn't free resources for \""
            << FileName << "\". There are no resources for such a file.\n";
  return false;
}

//retrieves vector of all currently buffered files
std::vector<std::string> Sound::GetBufferedFiles() const
{
  std::vector<std::string> result;
  TBufSrcRecord * temp;

  temp = pFileList;
  while (temp != NULL)
  {
    result.push_back(temp->FileName);
    temp = temp->next;
  }//while
  return result;
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
  /* Disabled for now
  alcIsExtensionPresent = NULL;
  alcGetProcAddress = NULL;
  alcGetEnumValue = NULL;  */
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
  /* Disabled for now
  alDopplerFactor = NULL;
  alDopplerVelocity = NULL;
  alSpeedOfSound = NULL;
  alDistanceModel = NULL;
  */

  //**** OggVorbis function pointers
  ov_clear = NULL;
  ov_comment = NULL;
  //ov_fopen = NULL;
  ov_info = NULL;
  ov_open = NULL;
  ov_open_callbacks = NULL;
  ov_pcm_total = NULL;
  ov_read = NULL;
  ov_test = NULL;
}

//vorbisfile callback functions for streams
size_t stream_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
  std::ifstream* ms;
  if (size==0 || nmemb==0)
  {
    return 0;
  }
  ms = static_cast<std::ifstream*> (datasource);
  ms->read((char*) ptr, size*nmemb);
  return (ms->gcount() / size);
}//read_func

/* Returns zero on success, returns a non-zero value on error, result is -1
  when device is unseekable.*/
int stream_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
  std::ifstream* ms;
  ms = static_cast<std::ifstream*> (datasource);
  switch (whence)
  {
    case SEEK_CUR:
         ms->seekg(offset, std::ios::cur);
         break;
    case SEEK_END:
         ms->seekg(offset, std::ios::end);
         break;
    case SEEK_SET:
         ms->seekg(offset, std::ios::beg);
         break;
    default:
         return -1;
  }//swi
  return 0;
}//seek_func

/* Should return zero on success and EOF on failure. However, libvorbisfile does
   not check return value, since the function is supposed to succeed.*/
int stream_close_func(void *datasource)
{
  std::ifstream* ms;
  ms = static_cast<std::ifstream*> (datasource);
  ms->close();
  return 0;
}//close_func

long stream_tell_func(void *datasource)
{
  std::ifstream* ms;
  ms = static_cast<std::ifstream*> (datasource);
  return (ms->tellg());
}//tell_func

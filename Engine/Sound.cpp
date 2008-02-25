#include "Sound.h"

//functions for class Sound:
//constructor
Sound::Sound()
{
  pDevice = NULL;
  pContext = NULL;
  AL_Ready = false;
  InitInProgress = false;
  libHandle = NULL;
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
bool Sound::Init(std::string PathToLibrary)
{
  if (AL_Ready || InitInProgress)
  {
    std::cout << "Sound::Init: Warning: Cannot start initialization of OpenAL ";
    std::cout <<" since it is already initialized or init. is in progress.\n";
    return false;
  }
  InitInProgress = true;
  if ((PathToLibrary== "") || (PathToLibrary == "NULL"))
  {
    //if no path to the OpenAL library is given, try default value
    #if defined(_WIN32)
      PathToLibrary = "C:\\Windows\\System32\\OpenAL32.dll";
    #else
      PathToLibrary = "/usr/lib/libopenal.so";
    #endif
  }
  #if defined(_WIN32)
  //Windows
  libHandle = LoadLibrary(PathToLibrary.c_str());
  #else
  //Linux goes here
  libHandle = dlopen(PathToLibrary.c_str(), RTLD_LOCAL | RTLD_LAZY);
  #endif
  if (libHandle == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not open library in \""
              << PathToLibrary << "\". Exiting.";
    InitInProgress = false;
    return false;
  }

  #if defined(_WIN32)
  //Windows
  alcCreateContext = (LPALCCREATECONTEXT)GetProcAddress(libHandle, "alcCreateContext");
  alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT) GetProcAddress(libHandle, "alcMakeContextCurrent");
  alcProcessContext = (LPALCPROCESSCONTEXT) GetProcAddress(libHandle, "alcProcessContext");
  alcSuspendContext = (LPALCSUSPENDCONTEXT) GetProcAddress(libHandle, "alcSuspendContext");
  alcDestroyContext = (LPALCDESTROYCONTEXT) GetProcAddress(libHandle, "alcDestroyContext");
  alcGetCurrentContext = (LPALCGETCURRENTCONTEXT) GetProcAddress(libHandle, "alcGetCurrentContext");
  alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE) GetProcAddress(libHandle, "alcFetContextsDevice");
  #else
  //Linux
  alcCreateContext = (LPALCCREATECONTEXT) dlsym(libHandle, "alcCreateContext");
  alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT) dlsym(libHandle, "alcMakeContextCurrent");
  alcProcessContext = (LPALCPROCESSCONTEXT) dlsym(libHandle, "alcProcessContext");
  alcSuspendContext = (LPALCSUSPENDCONTEXT) dlsym(libHandle, "alcSuspendContext");
  alcDestroyContext = (LPALCDESTROYCONTEXT) dlsym(libHandle, "alcDestroyContext");
  alcGetCurrentContext = (LPALCGETCURRENTCONTEXT) dlsym(libHandle, "alcGetCurrentContext");
  alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE) dlsym(libHandle, "alcFetContextsDevice");
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
  alcOpenDevice = (LPALCOPENDEVICE) GetProcAddress(libHandle, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) GetProcAddress(libHandle, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) GetProcAddress(libHandle, "alcGetError");
  alcGetString = (LPALCGETSTRING) GetProcAddress(libHandle, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) GetProcAddress(libHandle, "alcGetIntegerv");
  #else
  //Linux
  alcOpenDevice = (LPALCOPENDEVICE) dlsym(libHandle, "alcOpenDevice");
  alcCloseDevice = (LPALCCLOSEDEVICE) dlsym(libHandle, "alcCloseDevice");
  alcGetError = (LPALCGETERROR) dlsym(libHandle, "alcGetError");
  alcGetString = (LPALCGETSTRING) dlsym(libHandle, "alcGetString");
  alcGetIntegerv = (LPALCGETINTEGERV) dlsym(libHandle, "alcGetIntegerv");
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
  alEnable = (LPALENABLE) GetProcAddress(libHandle, "alEnable");
  alDisable = (LPALDISABLE) GetProcAddress(libHandle, "alDisable");
  alIsEnabled = (LPALISENABLED) GetProcAddress(libHandle, "alIsEnabled");
  #else
  //Linux
  alEnable = (LPALENABLE) dlsym(libHandle, "alEnable");
  alDisable = (LPALDISABLE) dlsym(libHandle, "alDisable");
  alIsEnabled = (LPALISENABLED) dlsym(libHandle, "alIsEnabled");
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
  alGetString = (LPALGETSTRING) GetProcAddress(libHandle, "alGetString");
  alGetBooleanv = (LPALGETBOOLEANV) GetProcAddress(libHandle, "alGetBooleanv");
  alGetIntegerv = (LPALGETINTEGERV) GetProcAddress(libHandle, "alGetIntegerv");
  alGetFloatv = (LPALGETFLOATV) GetProcAddress(libHandle, "alGetFloatv");
  alGetDoublev = (LPALGETDOUBLEV) GetProcAddress(libHandle, "alGetDoublev");
  alGetBoolean = (LPALGETBOOLEAN) GetProcAddress(libHandle, "alGetBoolean");
  alGetInteger = (LPALGETINTEGER) GetProcAddress(libHandle, "alGetInteger");
  alGetFloat = (LPALGETFLOAT) GetProcAddress(libHandle, "alGetFloat");
  alGetDouble = (LPALGETDOUBLE) GetProcAddress(libHandle, "alGetDouble");
  #else
  //Linux
  alGetString = (LPALGETSTRING) dlsym(libHandle, "alGetString");
  alGetBooleanv = (LPALGETBOOLEANV) dlsym(libHandle, "alGetBooleanv");
  alGetIntegerv = (LPALGETINTEGERV) dlsym(libHandle, "alGetIntegerv");
  alGetFloatv = (LPALGETFLOATV) dlsym(libHandle, "alGetFloatv");
  alGetDoublev = (LPALGETDOUBLEV) dlsym(libHandle, "alGetDoublev");
  alGetBoolean = (LPALGETBOOLEAN) dlsym(libHandle, "alGetBoolean");
  alGetInteger = (LPALGETINTEGER) dlsym(libHandle, "alGetInteger");
  alGetFloat = (LPALGETFLOAT) dlsym(libHandle, "alGetFloat");
  alGetDouble = (LPALGETDOUBLE) dlsym(libHandle, "alGetDouble");
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
  alGetError = (LPALGETERROR) GetProcAddress(libHandle, "alGetError");
  #else
  //Linux
  alGetError = (LPALGETERROR) dlsym(libHandle, "alGetError");
  #endif
  if (alGetError == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetError\" address.\n";
    InitInProgress = false;
    return false;
  }
  #if defined(_WIN32)
  //Windows
  alListenerf = (LPALLISTENERF) GetProcAddress(libHandle, "alListenerf");
  alListener3f = (LPALLISTENER3F) GetProcAddress(libHandle, "alListener3f");
  alListenerfv = (LPALLISTENERFV) GetProcAddress(libHandle, "alListenerfv");
  alListeneri = (LPALLISTENERI) GetProcAddress(libHandle, "alListeneri");
  alListener3i = (LPALLISTENER3I) GetProcAddress(libHandle, "alListener3i");
  alListeneriv = (LPALLISTENERIV) GetProcAddress(libHandle, "alListeneriv");
  #else
  //Linux
  alListenerf = (LPALLISTENERF) dlsym(libHandle, "alListenerf");
  alListener3f = (LPALLISTENER3F) dlsym(libHandle, "alListener3f");
  alListenerfv = (LPALLISTENERFV) dlsym(libHandle, "alListenerfv");
  alListeneri = (LPALLISTENERI) dlsym(libHandle, "alListeneri");
  alListener3i = (LPALLISTENER3I) dlsym(libHandle, "alListener3i");
  alListeneriv = (LPALLISTENERIV) dlsym(libHandle, "alListeneriv");
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
  alGetListenerf = (LPALGETLISTENERF) GetProcAddress(libHandle, "alGetListenerf");
  alGetListener3f = (LPALGETLISTENER3F) GetProcAddress(libHandle, "alGetListener3f");
  alGetListenerfv = (LPALGETLISTENERFV) GetProcAddress(libHandle, "alGetListenerfv");
  alGetListeneri = (LPALGETLISTENERI) GetProcAddress(libHandle, "alGetListeneri");
  alGetListener3i = (LPALGETLISTENER3I) GetProcAddress(libHandle, "alGetListener3i");
  alGetListeneriv = (LPALGETLISTENERIV) GetProcAddress(libHandle, "alGetListeneriv");
  #else
  //Linux
  alGetListenerf = (LPALGETLISTENERF) dlsym(libHandle, "alGetListenerf");
  alGetListener3f = (LPALGETLISTENER3F) dlsym(libHandle, "alGetListener3f");
  alGetListenerfv = (LPALGETLISTENERFV) dlsym(libHandle, "alGetListenerfv");
  alGetListeneri = (LPALGETLISTENERI) dlsym(libHandle, "alGetListeneri");
  alGetListener3i = (LPALGETLISTENER3I) dlsym(libHandle, "alGetListener3i");
  alGetListeneriv = (LPALGETLISTENERIV) dlsym(libHandle, "alGetListeneriv");
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
  alGenSources = (LPALGENSOURCES) GetProcAddress(libHandle, "alGenSources");
  alDeleteSources = (LPALDELETESOURCES) GetProcAddress(libHandle, "alDeleteSources");
  alIsSource = (LPALISSOURCE) GetProcAddress(libHandle, "alIsSource");
  #else
  //Linux
  alGenSources = (LPALGENSOURCES) dlsym(libHandle, "alGenSources");
  alDeleteSources = (LPALDELETESOURCES) dlsym(libHandle, "alDeleteSources");
  alIsSource = (LPALISSOURCE) dlsym(libHandle, "alIsSource");
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
  alSourcef = (LPALSOURCEF) GetProcAddress(libHandle, "alSourcef");
  alSource3f = (LPALSOURCE3F) GetProcAddress(libHandle, "alSource3f");
  alSourcefv = (LPALSOURCEFV) GetProcAddress(libHandle, "alSourcefv");
  alSourcei = (LPALSOURCEI) GetProcAddress(libHandle, "alSourcei");
  alSource3i = (LPALSOURCE3I) GetProcAddress(libHandle, "alSource3i");
  alSourceiv = (LPALSOURCEIV) GetProcAddress(libHandle, "alSourceiv");
  #else
  //Linux
  alSourcef = (LPALSOURCEF) dlsym(libHandle, "alSourcef");
  alSource3f = (LPALSOURCE3F) dlsym(libHandle, "alSource3f");
  alSourcefv = (LPALSOURCEFV) dlsym(libHandle, "alSourcefv");
  alSourcei = (LPALSOURCEI) dlsym(libHandle, "alSourcei");
  alSource3i = (LPALSOURCE3I) dlsym(libHandle, "alSource3i");
  alSourceiv = (LPALSOURCEIV) dlsym(libHandle, "alSourceiv");
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
  alGetSourcef = (LPALGETSOURCEF) GetProcAddress(libHandle, "alGetSourcef");
  alGetSource3f = (LPALGETSOURCE3F) GetProcAddress(libHandle, "alGetSource3f");
  alGetSourcefv = (LPALGETSOURCEFV) GetProcAddress(libHandle, "alGetSourcefv");
  alGetSourcei = (LPALGETSOURCEI) GetProcAddress(libHandle, "alGetSourcei");
  alGetSource3i = (LPALGETSOURCE3I) GetProcAddress(libHandle, "alGetSource3i");
  alGetSourceiv = (LPALGETSOURCEIV) GetProcAddress(libHandle, "alGetSourceiv");
  #else
  //Linux
  alGetSourcef = (LPALGETSOURCEF) dlsym(libHandle, "alGetSourcef");
  alGetSource3f = (LPALGETSOURCE3F) dlsym(libHandle, "alGetSource3f");
  alGetSourcefv = (LPALGETSOURCEFV) dlsym(libHandle, "alGetSourcefv");
  alGetSourcei = (LPALGETSOURCEI) dlsym(libHandle, "alGetSourcei");
  alGetSource3i = (LPALGETSOURCE3I) dlsym(libHandle, "alGetSource3i");
  alGetSourceiv = (LPALGETSOURCEIV) dlsym(libHandle, "alGetSourceiv");
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
  alSourcePlayv = (LPALSOURCEPLAYV) GetProcAddress(libHandle, "alSourcePlayv");
  alSourceStopv = (LPALSOURCESTOPV) GetProcAddress(libHandle, "alSourceStopv");
  alSourceRewindv = (LPALSOURCEREWINDV) GetProcAddress(libHandle, "alSourceRewindv");
  alSourcePausev = (LPALSOURCEPAUSEV) GetProcAddress(libHandle, "alSourcePausev");
  #else
  //Linux
  alSourcePlayv = (LPALSOURCEPLAYV) dlysm(libHandle, "alSourcePlayv");
  alSourceStopv = (LPALSOURCESTOPV) dlysm(libHandle, "alSourceStopv");
  alSourceRewindv = (LPALSOURCEREWINDV) dlysm(libHandle, "alSourceRewindv");
  alSourcePausev = (LPALSOURCEPAUSEV) dlysm(libHandle, "alSourcePausev");
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
  alSourcePlay = (LPALSOURCEPLAY) GetProcAddress(libHandle, "alSourcePlay");
  alSourceStop = (LPALSOURCESTOP) GetProcAddress(libHandle, "alSourceStop");
  alSourceRewind = (LPALSOURCEREWIND) GetProcAddress(libHandle, "alSourceRewind");
  alSourcePause = (LPALSOURCEPAUSE) GetProcAddress(libHandle, "alSourcePause");
  #else
  //Linux
  alSourcePlay = (LPALSOURCEPLAY) dlysm(libHandle, "alSourcePlay");
  alSourceStop = (LPALSOURCESTOP) dlysm(libHandle, "alSourceStop");
  alSourceRewind = (LPALSOURCEREWIND) dlysm(libHandle, "alSourceRewind");
  alSourcePause = (LPALSOURCEPAUSE) dlysm(libHandle, "alSourcePause");
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
  alSourceQueueBuffers = (LPALSOURCEQUEUEBUFFERS) GetProcAddress(libHandle, "alSourceQueueBuffers");
  alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS) GetProcAddress(libHandle, "alSourceUnqueueBuffers");
  #else
  //Linux
  alSourceQueueBuffers = (LPALSOURCEQUEUEBUFFERS) dlsym(libHandle, "alSourceQueueBuffers");
  alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS) dlsym(libHandle, "alSourceUnqueueBuffers");
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
  alGenBuffers = (LPALGENBUFFERS) GetProcAddress(libHandle, "alGenBuffers");
  alDeleteBuffers = (LPALDELETEBUFFERS) GetProcAddress(libHandle, "alDeleteBuffers");
  alIsBuffer = (LPALISBUFFER) GetProcAddress(libHandle, "alIsBuffer");
  alBufferData = (LPALBUFFERDATA) GetProcAddress(libHandle, "alBufferData");
  #else
  //Linux
  alGenBuffers = (LPALGENBUFFERS) dlsym(libHandle, "alGenBuffers");
  alDeleteBuffers = (LPALDELETEBUFFERS) dlsym(libHandle, "alDeleteBuffers");
  alIsBuffer = (LPALISBUFFER) dlsym(libHandle, "alIsBuffer");
  alBufferData = (LPBUFFERDATA) dlsym(libHandle, "alBufferData");
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
  alBufferf = (LPALBUFFERF) GetProcAddress(libHandle, "alBufferf");
  alBuffer3f = (LPALBUFFER3F) GetProcAddress(libHandle, "alBuffer3f");
  alBufferfv = (LPALBUFFERFV) GetProcAddress(libHandle, "alBufferfv");
  alBufferi = (LPALBUFFERI) GetProcAddress(libHandle, "alBufferi");
  alBuffer3i = (LPALBUFFER3I) GetProcAddress(libHandle, "alBuffer3i");
  alBufferiv = (LPALBUFFERIV) GetProcAddress(libHandle, "alBufferiv");
  #else
  //Linux
  alBufferf = (LPALBUFFERF) dlsym(libHandle, "alBufferf");
  alBuffer3f = (LPALBUFFER3F) dlsym(libHandle, "alBuffer3f");
  alBufferfv = (LPALBUFFERFV) dlsym(libHandle, "alBufferfv");
  alBufferi = (LPALBUFFERI) dlsym(libHandle, "alBufferi");
  alBuffer3i = (LPALBUFFER3I) dlsym(libHandle, "alBuffer3i");
  alBufferiv = (LPALBUFFERIV) dlsym(libHandle, "alBufferiv");
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
  alGetBufferf = (LPALGETBUFFERF) GetProcAddress(libHandle, "alGetBufferf");
  alGetBuffer3f = (LPALGETBUFFER3F) GetProcAddress(libHandle, "alGetBuffer3f");
  alGetBufferfv = (LPALGETBUFFERFV) GetProcAddress(libHandle, "alGetBufferfv");
  alGetBufferi = (LPALGETBUFFERI) GetProcAddress(libHandle, "alGetBufferi");
  alGetBuffer3i = (LPALGETBUFFER3I) GetProcAddress(libHandle, "alGetBuffer3i");
  alGetBufferiv = (LPALGETBUFFERIV) GetProcAddress(libHandle, "alGetBufferiv");
  #else
  //Linux
  alGetBufferf = (LPALGETBUFFERF) dlsym(libHandle, "alGetBufferf");
  alGetBuffer3f = (LPALGETBUFFER3F) dlsym(libHandle, "alGetBuffer3f");
  alGetBufferfv = (LPALGETBUFFERFV) dlsym(libHandle, "alGetBufferfv");
  alGetBufferi = (LPALGETBUFFERI) dlsym(libHandle, "alGetBufferi");
  alGetBuffer3i = (LPGETALBUFFER3I) dlsym(libHandle, "alGetBuffer3i");
  alGetBufferiv = (LPGETALBUFFERIV) dlsym(libHandle, "alGetBufferiv");
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
  //the basic initialization is done here, we can return true (for now,
  //  more will be done later)
  AL_Ready = true;
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

  //release library
  #if defined(_WIN32)
  if (libHandle != NULL)
  {
    if (!FreeLibrary(libHandle))
    {
      std::cout << "Sound::Exit: Error while closing library object.\n"
                << "Error code is " << GetLastError() <<".\n";
    }
  }
  #else
  char * err_str;
  dlerror(); //clear error state
  if (libHandle != NULL)
  {
    if (dlclose(libHandle) != 0)
    {
      std::cout << "\nSound::Exit: Error while closing library object.\n";
      err_str = dlerror();
      if (err_str!=NULL)
      {
        std::cout << "Error description: " << err_str <<"\n";
      }
    }
  }
  #endif
  libHandle = NULL;

  //de-init pointers
  AllFuncPointersToNULL();//wrapped into private function

  AL_Ready = false;
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
       || (fmt_c.fmt_[3]!='_'))
  {
    std::cout << "Sound::PlayWAV: ERROR: File \""<<WAV_FileName<<"\" has incorrect"
              <<" format chunk header signature.\n";
    dat.close();
    return false;
  }
  dat.read((char*) &(fmt_c.chunk_size), 4); //should have value of 16
  if (fmt_c.chunk_size!=16)
  {
    std::cout << "Sound::PlayWAV: ERROR: Format chunk of file \""<<WAV_FileName
              <<"\" has incorrect size of "<<fmt_c.chunk_size
              <<" bytes. (Should be 16 instead.)\n";
    dat.close();
    return false;
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
  //data chunk
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
           std::cout<<"    Unknown error occured. Error code: "<<error_state<<".\n";
           break;
    }//swi
    dat.close();
    return false;
  }
  //determine format, (unsecure yet, since it does not check for channel values
  //   unequal to 1 ot 2 and for sample sizes unequal to 8 or 16)
  if (fmt_c.BitsPerSample==16)
  {
    if (fmt_c.Channels==2)
    {
      format_type = AL_FORMAT_STEREO16;
    }
    else
    {
      format_type = AL_FORMAT_MONO16;
    }
  }
  else if (fmt_c.BitsPerSample==8)
  {
    if (fmt_c.Channels==2)
    {
      format_type = AL_FORMAT_STEREO8;
    }
    else
    {
      format_type = AL_FORMAT_MONO8;
    }
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
             std::cout <<"    Unknown error. Error code: "<<error_state<<".\n";
             break;
      }//swi
      dat.close();
      free(temp);
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
           std::cout <<"    Unknown error. Error code: "<<error_state<<".\n";
           break;
    }//swi
    dat.close();
    free(temp);
    return false;
  }//if
  else
  { //in case of no error we still need to close the stream and free the pointer
    dat.close();
    free(temp);
  }//else

  //Source generation
  alGetError();
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
           std::cout <<"    Unknown error. Error code: "<<error_state<<".\n";
           break;
    }//swi
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
           std::cout <<"    Unknown error. Error code: "<<error_state<<".\n";
           break;
    }//swi
    return false;
  }//if
  alSourcePlay(buff_rec->sourceID); //finally play it
  //add file to list of playing files
  buff_rec->next = pFileList;
  pFileList = buff_rec;

  return true; //this is what we want :)
}

bool Sound::PlayOgg(std::string Ogg_FileName)
{
  return false;
}

//returns true if the specified file is currently playing
bool Sound::IsPlaying(std::string FileName)
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
    alSourceStop(pFileList->sourceID);
    alSourceUnqueueBuffers(pFileList->sourceID, pFileList->num_buffers,
                           pFileList->buffers);
    alDeleteBuffers(pFileList->num_buffers, pFileList->buffers);
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
      alSourceUnqueueBuffers(temp->next->sourceID, temp->next->num_buffers,
                             temp->next->buffers);
      alDeleteBuffers(temp->next->num_buffers, temp->next->buffers);
      temp2 = temp->next;
      temp->next = temp->next->next;
      delete temp2;
      return true;
    }
    temp = temp->next;
  }
  std::cout << "Sound::FreeFileResources: Hint: Couldn't free resources for \""
            << FileName << "\". There are no resources for such a file.\n";
  return false;
}

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
  /* Disable for now
  alIsExtensionPresent = NULL;
  alGetProcAddress = NULL;
  alGetEnumValue = NULL;
  */

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
}

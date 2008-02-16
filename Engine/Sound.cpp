#include "Sound.h"


/*//Hilfsfunktion
bool ReadChunksFromFile(std::string FileName, TRiffChunk * rc, TFmtChunk * fc, TDataChunk *dc)
{
  ifstream dat;
  dat.open(FileName.c_str(), ios::in | ios::binary);
  if(!dat)
  {
   std::cout << "ReadCunksFromFile: Error opening input stream.\n";
   return false;
  }
  dat.read(rc->Riff, 4);
  dat.read((char*) &(rc->len), 4);
  dat.read(rc->Wave, 4);
  dat.read(fc->fmt_, 4);
  dat.read((char*) &(fc->chunk_size), 4);
  dat.read((char*) &(fc->FormatTag), 2);
  dat.read((char*) &(fc->Channels), 2);
  dat.read((char*) &(fc->SamplesPerSecond), 4);
  dat.read((char*) &(fc->BytesPerSecond), 4);
  dat.read((char*) &(fc->BlockAlign), 2);
  dat.read((char*) &(fc->BitsPerSample), 2);
  dat.read(dc->data, 4);
  dat.read((char*) &(dc->length_of_data), 4);
  dat.close();
  return true;
}*/

//functions for class Sound:
//constructor
Sound::Sound()
{
  pDevice = NULL;
  pContext = NULL;
  AL_Ready = false;
  InitInProgress = false;
  libHandle = NULL;
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
      PathToLibrary = "C:\\Windows\\System\\OpenAL32.dll";
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
  alcGetIntegerv = (LPALCGETINTEGERV) GetProcAdress(libHandle, "alcGetIntegerv");
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
  if (alGetSourcePlayv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcePlayv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceStopv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourceStopv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceRewindv == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourceRewindv\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcePausev == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcePausev\" address.\n";
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
  if (alGetSourcePlay == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcePlay\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceStop == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourceStop\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourceRewind == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourceRewind\" address.\n";
    InitInProgress = false;
    return false;
  }
  if (alGetSourcePause == NULL)
  {
    std::cout << "Sound::Init: ERROR: Could not retrieve \"alGetSourcePause\" address.\n";
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
  alBufferData = (LPBUFFERDATA) GetProcAddress(libHandle, "alBufferData");
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
    InitInProgess = false;
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
    std::cout << "Sound::Exit: Warning: (De-)Initialization of OpenAL is ";
              << "already in progress, thus we quit here.\n";
    return false;
  }
  InitInProgress = true;
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
  if (FileName.substr(FileName.length()-4)==".wav")
  {
    return PlayWAV(FileName);
  }
  else if (FileName.substr(FileName.length()-4)==".ogg")
  {
    return PlayOgg(FileName);
  }
  else return false;
}

bool Sound::PlayWAV(std::string WAV_FileName)
{

  return false;
}

bool Sound::PlayOgg(std::string Ogg_FileName)
{
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

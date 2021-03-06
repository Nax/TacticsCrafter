return {
  sceKernelRegisterSubIntrHandler = 0x08a4af14,
  sceKernelReleaseSubIntrHandler = 0x08a4af1c,
  sceKernelEnableSubIntr = 0x08a4af24,
  sceIoWrite = 0x08a4af2c,
  sceIoChdir = 0x08a4af34,
  sceIoRead = 0x08a4af3c,
  sceIoRename = 0x08a4af44,
  sceIoClose = 0x08a4af4c,
  sceIoReadAsync = 0x08a4af54,
  sceIoDopen = 0x08a4af5c,
  sceIoDclose = 0x08a4af64,
  sceIoOpen = 0x08a4af6c,
  sceIoLseek = 0x08a4af74,
  sceIoWaitAsyncCB = 0x08a4af7c,
  sceKernelCpuSuspendIntr = 0x08a4af84,
  sceKernelCpuResumeIntr = 0x08a4af8c,
  sceKernelExitGame = 0x08a4af94,
  sceKernelRegisterExitCallback = 0x08a4af9c,
  sceKernelUnloadModule = 0x08a4afa4,
  sceKernelGetModuleIdByAddress = 0x08a4afac,
  sceKernelGetModuleId = 0x08a4afb4,
  sceKernelStopUnloadSelfModuleWithStatus = 0x08a4afbc,
  sceKernelStopModule = 0x08a4afc4,
  sceKernelStdin = 0x08a4afcc,
  sceKernelStdout = 0x08a4afd4,
  sceKernelStderr = 0x08a4afdc,
  sceKernelPrintf = 0x08a4afe4,
  sceKernelAllocPartitionMemory = 0x08a4afec,
  sceKernelSetCompiledSdkVersion = 0x08a4aff4,
  sceKernelGetBlockHeadAddr = 0x08a4affc,
  sceKernelFreePartitionMemory = 0x08a4b004,
  sceKernelSetCompilerVersion = 0x08a4b00c,
  sceKernelDelayThread = 0x08a4b014,
  sceKernelSetEventFlag = 0x08a4b01c,
  sceKernelCreateSema = 0x08a4b024,
  sceKernelCreateCallback = 0x08a4b02c,
  sceKernelDeleteFpl = 0x08a4b034,
  sceKernelDeleteEventFlag = 0x08a4b03c,
  sceKernelDeleteMsgPipe = 0x08a4b044,
  sceKernelStartThread = 0x08a4b04c,
  sceKernelFreeFpl = 0x08a4b054,
  sceKernelWaitThreadEnd = 0x08a4b05c,
  sceKernelDeleteSema = 0x08a4b064,
  sceKernelPollEventFlag = 0x08a4b06c,
  sceKernelWaitEventFlagCB = 0x08a4b074,
  sceKernelCancelMsgPipe = 0x08a4b07c,
  sceKernelTerminateDeleteThread = 0x08a4b084,
  sceKernelSignalSema = 0x08a4b08c,
  sceKernelWaitEventFlag = 0x08a4b094,
  sceKernelCreateThread = 0x08a4b09c,
  sceKernelWaitSema = 0x08a4b0a4,
  sceKernelCreateEventFlag = 0x08a4b0ac,
  sceKernelTryAllocateFpl = 0x08a4b0b4,
  sceKernelSetAlarm = 0x08a4b0bc,
  sceKernelReceiveMsgPipe = 0x08a4b0c4,
  sceKernelCreateMsgPipe = 0x08a4b0cc,
  sceKernelCancelAlarm = 0x08a4b0d4,
  sceKernelExitDeleteThread = 0x08a4b0dc,
  sceKernelClearEventFlag = 0x08a4b0e4,
  sceKernelGetSystemTimeWide = 0x08a4b0ec,
  sceKernelTrySendMsgPipe = 0x08a4b0f4,
  sceKernelSuspendThread = 0x08a4b0fc,
  sceKernelDeleteThread = 0x08a4b104,
  sceKernelExitThread = 0x08a4b10c,
  sceKernelCreateFpl = 0x08a4b114,
  sceKernelLibcGettimeofday = 0x08a4b11c,
  sceKernelLibcClock = 0x08a4b124,
  sceKernelLibcTime = 0x08a4b12c,
  sceKernelDcacheWritebackRange = 0x08a4b134,
  sceAudioOutputBlocking = 0x08a4b13c,
  sceAudioOutputPannedBlocking = 0x08a4b144,
  sceAudioChReserve = 0x08a4b14c,
  sceAudioChRelease = 0x08a4b154,
  sceAudioChangeChannelConfig = 0x08a4b15c,
  sceAudioGetChannelRestLength = 0x08a4b164,
  sceAudioChangeChannelVolume = 0x08a4b16c,
  sceAudioSetChannelDataLen = 0x08a4b174,
  sceAudioOutputPanned = 0x08a4b17c,
  sceCtrlSetSamplingMode = 0x08a4b184,
  sceCtrlPeekBufferPositive = 0x08a4b18c,
  sceCtrlSetSamplingCycle = 0x08a4b194,
  sceDisplaySetMode = 0x08a4b19c,
  sceDisplayGetAccumulatedHcount = 0x08a4b1a4,
  sceDisplaySetFrameBuf = 0x08a4b1ac,
  sceDisplayWaitVblankStartCB = 0x08a4b1b4,
  sceDisplayWaitVblankStart = 0x08a4b1bc,
  sceDisplayGetVcount = 0x08a4b1c4,
  sceDmacMemcpy = 0x08a4b1cc,
  sceGeListSync = 0x08a4b1d4,
  sceGeUnsetCallback = 0x08a4b1dc,
  sceGeListEnQueueHead = 0x08a4b1e4,
  sceGeContinue = 0x08a4b1ec,
  sceGeSetCallback = 0x08a4b1f4,
  sceGeListEnQueue = 0x08a4b1fc,
  sceGeDrawSync = 0x08a4b204,
  sceGeBreak = 0x08a4b20c,
  sceGeListUpdateStallAddr = 0x08a4b214,
  sceGeEdramGetAddr = 0x08a4b21c,
  sceMpegAvcDecode = 0x08a4b224,
  sceMpegRingbufferDestruct = 0x08a4b22c,
  sceMpegInitAu = 0x08a4b234,
  sceMpegQueryStreamOffset = 0x08a4b23c,
  sceMpegRingbufferConstruct = 0x08a4b244,
  sceMpegRegistStream = 0x08a4b24c,
  sceMpegAvcDecodeFlush = 0x08a4b254,
  sceMpegUnRegistStream = 0x08a4b25c,
  sceMpegDelete = 0x08a4b264,
  sceMpegQueryStreamSize = 0x08a4b26c,
  sceMpegInit = 0x08a4b274,
  sceMpegFlushAllStream = 0x08a4b27c,
  sceMpegAvcDecodeStop = 0x08a4b284,
  sceMpegAtracDecode = 0x08a4b28c,
  sceMpegFinish = 0x08a4b294,
  sceMpegMallocAvcEsBuf = 0x08a4b29c,
  sceMpegRingbufferPut = 0x08a4b2a4,
  sceMpegRingbufferAvailableSize = 0x08a4b2ac,
  sceMpegQueryMemSize = 0x08a4b2b4,
  sceMpegFreeAvcEsBuf = 0x08a4b2bc,
  sceMpegRingbufferQueryMemSize = 0x08a4b2c4,
  sceMpegCreate = 0x08a4b2cc,
  sceMpegGetAtracAu = 0x08a4b2d4,
  sceMpegQueryAtracEsSize = 0x08a4b2dc,
  sceMpegGetAvcAu = 0x08a4b2e4,
  sceNetAdhocPtpClose = 0x08a4b2ec,
  sceNetAdhocPtpSend = 0x08a4b2f4,
  sceNetAdhocPtpOpen = 0x08a4b2fc,
  sceNetAdhocPtpRecv = 0x08a4b304,
  sceNetAdhocPtpFlush = 0x08a4b30c,
  sceNetAdhocPtpAccept = 0x08a4b314,
  sceNetAdhocTerm = 0x08a4b31c,
  sceNetAdhocPtpListen = 0x08a4b324,
  sceNetAdhocInit = 0x08a4b32c,
  sceNetAdhocPtpConnect = 0x08a4b334,
  sceNetAdhocctlScan = 0x08a4b33c,
  sceNetAdhocctlAddHandler = 0x08a4b344,
  sceNetAdhocctlDisconnect = 0x08a4b34c,
  sceNetAdhocctlJoin = 0x08a4b354,
  sceNetAdhocctlDelHandler = 0x08a4b35c,
  sceNetAdhocctlGetScanInfo = 0x08a4b364,
  sceNetAdhocctlTerm = 0x08a4b36c,
  sceNetAdhocctlGetPeerList = 0x08a4b374,
  sceNetAdhocctlInit = 0x08a4b37c,
  sceNetAdhocctlCreate = 0x08a4b384,
  sceNetGetLocalEtherAddr = 0x08a4b38c,
  sceNetTerm = 0x08a4b394,
  sceNetInit = 0x08a4b39c,
  sceNetEtherNtostr = 0x08a4b3a4,
  __sceSasSetADSR = 0x08a4b3ac,
  __sceSasRevParam = 0x08a4b3b4,
  __sceSasGetPauseFlag = 0x08a4b3bc,
  __sceSasRevType = 0x08a4b3c4,
  __sceSasInit = 0x08a4b3cc,
  __sceSasSetVolume = 0x08a4b3d4,
  __sceSasCoreWithMix = 0x08a4b3dc,
  __sceSasSetSL = 0x08a4b3e4,
  __sceSasGetEndFlag = 0x08a4b3ec,
  __sceSasGetEnvelopeHeight = 0x08a4b3f4,
  __sceSasSetKeyOn = 0x08a4b3fc,
  __sceSasSetPause = 0x08a4b404,
  __sceSasSetVoice = 0x08a4b40c,
  __sceSasSetADSRmode = 0x08a4b414,
  __sceSasSetKeyOff = 0x08a4b41c,
  __sceSasCore = 0x08a4b424,
  __sceSasSetPitch = 0x08a4b42c,
  __sceSasSetNoise = 0x08a4b434,
  __sceSasGetGrain = 0x08a4b43c,
  __sceSasSetSimpleADSR = 0x08a4b444,
  __sceSasSetGrain = 0x08a4b44c,
  __sceSasRevEVOL = 0x08a4b454,
  __sceSasGetOutputmode = 0x08a4b45c,
  __sceSasSetOutputmode = 0x08a4b464,
  __sceSasRevVON = 0x08a4b46c,
  sceKernelPowerTick = 0x08a4b474,
  sceUmdWaitDriveStatCB = 0x08a4b47c,
  sceUmdCancelWaitDriveStat = 0x08a4b484,
  sceUmdGetDriveStat = 0x08a4b48c,
  sceUmdWaitDriveStat = 0x08a4b494,
  sceUmdActivate = 0x08a4b49c,
  sceUmdDeactivate = 0x08a4b4a4,
  sceUtilityLoadModule = 0x08a4b4ac,
  sceUtilitySavedataInitStart = 0x08a4b4b4,
  sceUtilitySavedataGetStatus = 0x08a4b4bc,
  sceUtilitySavedataShutdownStart = 0x08a4b4c4,
  sceUtilitySavedataUpdate = 0x08a4b4cc,
  sceUtilityUnloadModule = 0x08a4b4d4,
  sceWlanGetEtherAddr = 0x08a4b4dc,
  sceImposeSetLanguageMode = 0x08a4b4e4,
}

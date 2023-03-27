#pragma once

#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
//#define DEVICE_FORMAT       ma_format_f32
//#define DEVICE_CHANNELS     2
//#define DEVICE_SAMPLE_RATE  48000
#include <stdio.h>
#include <iostream>
#include <string>
#include <array>

namespace me {

   class MeAudio {
   public:
      MeAudio(const std::string& filepath) {
         ma_result result;
         ma_device_config deviceConfig;

         result = ma_decoder_init_file(filepath.c_str(), NULL, &decoder);
         if (result != MA_SUCCESS)  assert(false && "Failed to open music file.\n");
         deviceConfig = ma_device_config_init(ma_device_type_playback);
         deviceConfig.playback.format = decoder.outputFormat;
         deviceConfig.playback.channels = decoder.outputChannels;
         deviceConfig.sampleRate = decoder.outputSampleRate;
         deviceConfig.dataCallback = AudioCallback;
         deviceConfig.pUserData = &decoder;

         if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
            assert(false && "Failed to open playback device.\n");
         }
         printf("Device Name: %s\n", device.playback.name);
      }
      ~MeAudio()
      {
         ma_device_uninit(&device);
         ma_decoder_uninit(&decoder);
      }
      void Start() {
         ma_decoder_seek_to_pcm_frame(&decoder, 0);
         if (ma_device_start(&device) != MA_SUCCESS) {
            ma_device_uninit(&device);
            assert(false && "Failed to start playback device.\n");
         }
      }
      void Stop() {
         if (ma_device_stop(&device) != MA_SUCCESS) {
            ma_device_uninit(&device);
            assert(false && "Failed to start playback device.\n");
         }
      }
private:
   static void AudioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
   {
      ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
      if (pDecoder == NULL) {
         return;
      }

      ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

      (void)pInput;
   }

   ma_device device;
   ma_decoder decoder;
};

} // namespace me
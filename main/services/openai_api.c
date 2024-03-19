#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "OpenAI.h"
#include "openai_api.h"

const char *TAG = "OpenAI";

uint8_t openai_service_type = 0; // 0: chat, 1: image

static OpenAI_t *openai = NULL;
static OpenAI_AudioTranscription_t *audioTranscription = NULL;
static OpenAI_ChatCompletion_t *chatCompletion = NULL;
static OpenAI_ImageGeneration_t *imageGeneration = NULL;

OpenAI_StringResponse_t *chat_result;
OpenAI_ImageResponse_t *image_response;

void openai_init(char *api_key)
{
    if (openai == NULL)
    {
        openai = OpenAICreate(api_key);
        audioTranscription = openai->audioTranscriptionCreate(openai);
        chatCompletion = openai->chatCreate(openai);
        imageGeneration = openai->imageGenerationCreate(openai);

        audioTranscription->setResponseFormat(audioTranscription, OPENAI_AUDIO_RESPONSE_FORMAT_JSON);
        audioTranscription->setLanguage(audioTranscription, "en");
        audioTranscription->setTemperature(audioTranscription, 0.2);

        chatCompletion->setModel(chatCompletion, CONFIG_OPENAI_MODEL);
        chatCompletion->setSystem(chatCompletion, "user");
        chatCompletion->setMaxTokens(chatCompletion, CONFIG_MAX_TOKEN);
        chatCompletion->setTemperature(chatCompletion, 0.2);
        chatCompletion->setStop(chatCompletion, "\r");
        chatCompletion->setPresencePenalty(chatCompletion, 0);
        chatCompletion->setFrequencyPenalty(chatCompletion, 0);
        chatCompletion->setUser(chatCompletion, "OpenAI-ESP32");

        imageGeneration->setResponseFormat(imageGeneration, OPENAI_IMAGE_RESPONSE_FORMAT_URL);
        imageGeneration->setSize(imageGeneration, OPENAI_IMAGE_SIZE_256x256);
        imageGeneration->setN(imageGeneration, 1);

        ESP_LOGI(TAG, "OpenAI init success");
    }
}

char *openai_transcribe_audio(uint8_t *audio, int audio_len)
{
    return audioTranscription->file(audioTranscription, (uint8_t *)audio, audio_len, OPENAI_AUDIO_INPUT_FORMAT_WAV);
}

char *openai_chat(char *prompt)
{
    chat_result = chatCompletion->message(chatCompletion, prompt, false); // Calling Chat completion api
    return chat_result->getData(chat_result, 0);
}

void openai_chat_cleanup(void)
{
    chat_result->delete (chat_result);
}

char *openai_create_image_url(char *prompt)
{
    OpenAI_ImageResponse_t *image_response = imageGeneration->prompt(imageGeneration, prompt);
    return image_response->getData(image_response, 0);
}

void openai_create_image_url_cleanup(void)
{
    image_response->delete (image_response);
}

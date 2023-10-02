#pragma once

#include <inttypes.h>

extern uint8_t openai_service_type; // 0: chat, 1: image

enum ServiceType
{
    SERVICE_TYPE_CHAT,
    SERVICE_TYPE_IMAGE,
};

/**
 * Initialize the OpenAI API
 * @param api_key The OpenAI API key to use
 */
void openai_init(char *api_key);

/**
 * Transcribe audio using the OpenAI API
 * @param audio The audio data to transcribe
 * @param audio_len The length of the audio data
 * @return char* The text response from the API
 */
char *openai_transcribe_audio(uint8_t *audio, int audio_len);

/**
 * Chat with the OpenAI API (ChatGPT)
 * @param prompt The prompt to use for the chat
 * @return char* The text response from the API
 */
char *openai_chat(char *prompt);

/**
 *  Clean up the memory allocated by the OpenAI API
 */
void openai_chat_cleanup(void);

/**
 *  DALL-E image generation
 *  @param prompt The prompt to use for the image generation
 *  @return The URL of the image
 */
char *openai_create_image_url(char *prompt);

/**
 *  Clean up the memory allocated by the OpenAI API
 */
void openai_create_image_url_cleanup(void);
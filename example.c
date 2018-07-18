#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bing_rs.h"

#define CUSTOM_SPEECH

#if defined(CUSTOM_SPEECH)
static const char SUBSCRIPTION_KEY[] = "SUBSCRIPTION_KEY";
static const char ENDPOINT_ID[] = "ENDPOINT_ID";
#else
static const char SUBSCRIPTION_KEY[] = "SUBSCRIPTION_KEY";
#endif

static void on_turn_start()
{
    fprintf(stdout, "Turn Start\n");
}

static void on_turn_end()
{
    fprintf(stdout, "Turn End\n");
}

static void on_speech_start_detected()
{
    fprintf(stdout, "Speech Start Detected\n");
}

static void on_speech_end_detected()
{
    fprintf(stdout, "Speech End Detected\n");
}

static void on_speech_hypothesis(BingSpeechHypothesis hypothesis)
{
    fprintf(stdout, "Speech Hypothesis: %s\n", hypothesis.text);
}

static void on_speech_phrase(BingSpeechPhrase phrase)
{
    fprintf(stdout, "Speech Phrase\n");
    fprintf(stdout, "  Recognition Status: %s\n", phrase.recognition_status);
    fprintf(stdout, "  Offset: %.0lf\n", phrase.offset);
    fprintf(stdout, "  Duration: %.0lf\n", phrase.duration);
    fprintf(stdout, "  NBest:\n");
    for (int i = 0; i < phrase.nbest_count; i++) {
        BingSpeechResult item = phrase.nbest[i];
        fprintf(stdout, "  #%d\n", i);
        fprintf(stdout, "    Confidence: %lf\n", item.confidence);
        fprintf(stdout, "    Lexical: %s\n", item.lexical);
        fprintf(stdout, "    ITN: %s\n", item.itn);
        fprintf(stdout, "    MaskedITN: %s\n", item.masked_itn);
        fprintf(stdout, "    Display: %s\n", item.display);
    }

    free(phrase.nbest);
}

int main()
{
    static const size_t BUF_SIZE = 4096;
    BingSpeech *bing_speech;
    BingSpeechWebsocket *websocket;
    BingSpeechWebsocketHandler handler;
    char *subscription_key;
    FILE *file;
    struct stat file_stat;
    int i;

    subscription_key = malloc(33); // Freed by Rust
    strcpy(subscription_key, SUBSCRIPTION_KEY);

    // Initialize Bing Speech
    bing_speech = bing_speech_new(subscription_key);

    #if defined(CUSTOM_SPEECH)
    char *endpoint_id = malloc(sizeof(ENDPOINT_ID));
    strcpy(endpoint_id, ENDPOINT_ID);
    bing_speech_set_custom_speech(bing_speech, 1);
    bing_speech_set_endpoint_id(bing_speech, endpoint_id);
    #endif

    // Fetch Token
    char *token = bing_speech_fetch_token(bing_speech);
    fprintf(stdout, "Got token: %s\n", token);

    // Get Websocket Handle
    memset(&handler, 0, sizeof(handler));
    handler.on_turn_start = on_turn_start;
    handler.on_turn_end = on_turn_end;
    handler.on_speech_start_detected = on_speech_start_detected;
    handler.on_speech_start_detected = on_speech_start_detected;
    handler.on_speech_hypothesis = on_speech_hypothesis;
    handler.on_speech_phrase = on_speech_phrase;
    websocket = bing_speech_websocket(bing_speech, handler);

    // Read audio data
    file = fopen("assets/audio.raw", "r");
    if (!file) {
        perror("main:");
        goto out;
    }

    // Get file stat
    fstat(fileno(file), &file_stat);

    // Send the audio to Bing
    for (i = 0; i < file_stat.st_size; i += BUF_SIZE) {
        char *buf = malloc(BUF_SIZE); // Freed by Rust
        int size_to_read = BUF_SIZE;

        if (file_stat.st_size - i < BUF_SIZE) {
            size_to_read = file_stat.st_size - i;
        }

        int nread = fread(buf, 1, size_to_read, file);
        bing_speech_websocket_audio(websocket, buf, nread);

        usleep(256 * 1000);
    }

    // Send silence to Bing
    for (i = 0; i < 20; i++) {
        char *buf = malloc(BUF_SIZE); // Freed by Rust

        memset(buf, 0, BUF_SIZE);
        bing_speech_websocket_audio(websocket, buf, BUF_SIZE);
        usleep(256 * 1000);
    }

out:
    fclose(file);
    bing_speech_websocket_close(websocket);
    bing_speech_free(bing_speech);
}

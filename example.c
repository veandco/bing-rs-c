#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bing_rs.h"

//#define CUSTOM_SPEECH

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
    BingSpeechPhrase phrase;
    char *recognize_buf;
    char *subscription_key;
    char *token;
    FILE *file;
    FILE *synthesize_file;
    void *synthesize_data;
    int synthesize_data_len;
    struct stat file_stat;
    int i;

    // Initialize Bing Speech
    bing_speech = bing_speech_new(getenv("SUBSCRIPTION_KEY"));

#if defined(CUSTOM_SPEECH)
    bing_speech_set_custom_speech(bing_speech, 1);
    bing_speech_set_endpoint_id(bing_speech, getenv("ENDPOINT_ID"));
#endif

    // Read audio data
    file = fopen("assets/audio.raw", "r");
    if (!file) {
        perror("main:");
        goto out;
    }

    // Get file stat
    fstat(fileno(file), &file_stat);

    // Fetch Token
    token = bing_speech_fetch_token(bing_speech);
    fprintf(stdout, "Got token: %s\n", token);
    bing_speech_auto_fetch_token(bing_speech);

    // Recognize
    {
        recognize_buf = (char *) malloc(file_stat.st_size);
        fread(recognize_buf, 1, file_stat.st_size, file);
        fprintf(stdout, "Begin\n");
        bing_speech_recognize(bing_speech, recognize_buf, file_stat.st_size, MODE_INTERACTIVE, LANGUAGE_ENGLISH_UNITED_STATES, FORMAT_DETAILED, &phrase);
        fprintf(stdout, "End\n");
        fprintf(stdout, "%s\n", phrase.nbest[0].itn);
    }

    // Synthesize
    synthesize_data = NULL;
    synthesize_data_len = 0;
    bing_speech_synthesize(bing_speech, "Hello, Jacky!", VOICE_FONT_EN_US_JESSA_RUS, &synthesize_data, &synthesize_data_len);
    fprintf(stdout, "Synthesized audio with size of %d\n", synthesize_data_len);
    synthesize_file = fopen("synthesize.bin", "w");
    fwrite(synthesize_data, 1, synthesize_data_len, synthesize_file);
    fclose(synthesize_file);

    // Get Websocket Handle
    memset(&handler, 0, sizeof(handler));
    handler.on_turn_start = on_turn_start;
    handler.on_turn_end = on_turn_end;
    handler.on_speech_start_detected = on_speech_start_detected;
    handler.on_speech_end_detected = on_speech_end_detected;
    handler.on_speech_hypothesis = on_speech_hypothesis;
    handler.on_speech_phrase = on_speech_phrase;
    websocket = bing_speech_websocket_new();

#if defined(CUSTOM_SPEECH)
    bing_speech_websocket_connect(bing_speech, websocket, MODE_INTERACTIVE, LANGUAGE_ENGLISH_UNITED_STATES, FORMAT_DETAILED, 0, getenv("ENDPOINT_ID"), handler);
#else
    bing_speech_websocket_connect(bing_speech, websocket, MODE_INTERACTIVE, LANGUAGE_ENGLISH_UNITED_STATES, FORMAT_DETAILED, 0, NULL, handler);
#endif

    // Read audio data again
    fclose(file);
    file = fopen("assets/audio.raw", "r");
    if (!file) {
        perror("main:");
        goto out;
    }

    // Send the audio to Bing
    for (i = 0; i < file_stat.st_size; i += BUF_SIZE) {
        char buf[BUF_SIZE] = { 0 };
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
        char buf[BUF_SIZE] = { 0 };

        bing_speech_websocket_audio(websocket, buf, BUF_SIZE);
        usleep(256 * 1000);
    }

out:
    fclose(file);
    bing_speech_websocket_disconnect(websocket);
    bing_speech_websocket_free(websocket);
    bing_speech_free(bing_speech);
}

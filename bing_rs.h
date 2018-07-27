#pragma once

typedef struct BingSpeech BingSpeech;
typedef struct BingSpeechWebsocket BingSpeechWebsocket;

typedef struct BingSpeechResult {
    double confidence;
    char *lexical;
    char *itn;
    char *masked_itn;
    char *display;
} BingSpeechResult;

typedef struct BingSpeechPhrase {
    char *recognition_status;
    char *display_text;
    double offset;
    double duration;
    BingSpeechResult *nbest;
    int nbest_count;
} BingSpeechPhrase;

typedef struct BingSpeechHypothesis {
    char *text;
    double offset;
    double duration;
} BingSpeechHypothesis;

typedef struct BingSpeechWebsocketHandler {
    void (*on_turn_start)();
    void (*on_turn_end)();
    void (*on_speech_start_detected)();
    void (*on_speech_end_detected)();
    void (*on_speech_hypothesis)(BingSpeechHypothesis);
    void (*on_speech_phrase)(BingSpeechPhrase);
} BingSpeechWebsocketHandler;

extern "C"
{
    extern BingSpeech * bing_speech_new(const char *subscription_key);
    extern void bing_speech_free(BingSpeech *);
    extern void bing_speech_set_custom_speech(BingSpeech *, int is_custom_speech);
    extern void bing_speech_set_endpoint_id(BingSpeech *, const char *endpoint_id);
    extern char * bing_speech_fetch_token(BingSpeech *);
    extern void bing_speech_auto_fetch_token(BingSpeech *);

    extern BingSpeechWebsocket * bing_speech_websocket_new();
    extern BingSpeechWebsocket * bing_speech_websocket_connect(BingSpeech *, BingSpeechWebsocket *, int mode, int language, int format, int is_custom_speech, char *endpoint_id, BingSpeechWebsocketHandler handler);
    extern int bing_speech_websocket_audio(BingSpeechWebsocket *, const char *audio, int audio_size);
    extern int bing_speech_websocket_disconnect(BingSpeechWebsocket *);
}

// Modes
#define MODE_INTERACTIVE (0)
#define MODE_DICTATION (1)
#define MODE_CONVERSATION (2)

// Languages
#define LANGUAGE_ARABIC_EGYPT (0)
#define LANGUAGE_CATALAN_SPAIN (1)
#define LANGUAGE_CHINESE_CHINA (2)
#define LANGUAGE_CHINESE_HONG_KONG (3)
#define LANGUAGE_CHINESE_TAIWAN (4)
#define LANGUAGE_DANISH_DENMARK (5)
#define LANGUAGE_DUTCH_NETHERLANDS (6)
#define LANGUAGE_ENGLISH_AUSTRALIA (7)
#define LANGUAGE_ENGLISH_CANADA (8)
#define LANGUAGE_ENGLISH_INDIA (9)
#define LANGUAGE_ENGLISH_NEW_ZEALAND (10)
#define LANGUAGE_ENGLISH_UNITED_KINDOM (11)
#define LANGUAGE_ENGLISH_UNITED_STATES (12)
#define LANGUAGE_FINNISH_FINLAND (13)
#define LANGUAGE_FRENCH_CANADA (14)
#define LANGUAGE_FRENCH_FRANCE (15)
#define LANGUAGE_GERMAN_GERMANY (16)
#define LANGUAGE_HINDI_INDIA (17)
#define LANGUAGE_ITALIAN_ITALY (18)
#define LANGUAGE_JAPANESE_JAPAN (19)
#define LANGUAGE_KOREAN_KOREA (20)
#define LANGUAGE_NORWEGIAN_NORWAY (21)
#define LANGUAGE_POLISH_POLAND (22)
#define LANGUAGE_PORTUGUESE_BRAZIL (23)
#define LANGUAGE_PORTUGUESE_PORTUGAL (24)
#define LANGUAGE_RUSSIAN_RUSSIA (25)
#define LANGUAGE_SPANISH_MEXICO (26)
#define LANGUAGE_SPANISH_SPAIN (27)
#define LANGUAGE_SWEDISH_SWEDEN (28)

// Format
#define FORMAT_SIMPLE (0)
#define FORMAT_DETAILED (1)
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
    extern void bing_speech_synthesize(BingSpeech *, const char *text, int font, void **synthesize_data, int *synthesize_data_len);

    extern BingSpeechWebsocket * bing_speech_websocket_new();
    extern BingSpeechWebsocket * bing_speech_websocket_connect(BingSpeech *, BingSpeechWebsocket *, int mode, int language, int format, int is_custom_speech, char *endpoint_id, BingSpeechWebsocketHandler handler);
    extern int bing_speech_websocket_audio(BingSpeechWebsocket *, const char *audio, int audio_size);
    extern int bing_speech_websocket_disconnect(BingSpeechWebsocket *);
    extern void bing_speech_websocket_free(BingSpeechWebsocket *);
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
#define LANGUAGE_ENGLISH_UNITED_KINGDOM (11)
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

// Voice Font
#define VOICE_FONT_AR_EG_HODA            (0)
#define VOICE_FONT_AR_SA_NAAYF           (1)
#define VOICE_FONT_BG_BG_IVAN            (2)
#define VOICE_FONT_CA_ES_HERENA_RUS      (3)
#define VOICE_FONT_CA_CZ_JAKUB           (4)
#define VOICE_FONT_DA_DK_HELLE_RUS       (5)
#define VOICE_FONT_DE_AT_MICHAEL         (6)
#define VOICE_FONT_DE_CH_KARSTEN         (7)
#define VOICE_FONT_DE_DE_HEDDA           (8)
#define VOICE_FONT_DE_DE_HEDDA_RUS       (9)
#define VOICE_FONT_DE_DE_STEFAN_APOLLO   (10)
#define VOICE_FONT_EL_GR_STEFANOS        (11)
#define VOICE_FONT_EN_AU_CATHERINE       (12)
#define VOICE_FONT_EN_AU_HAYLEY_RUS      (13)
#define VOICE_FONT_EN_CA_LINDA           (14)
#define VOICE_FONT_EN_CA_HEATHER_RUS     (15)
#define VOICE_FONT_EN_GB_SUSAN_APOLLO    (16)
#define VOICE_FONT_EN_GB_HAZEL_RUS       (17)
#define VOICE_FONT_EN_GB_GEORGE_APOLLO   (18)
#define VOICE_FONT_EN_IE_SEAN            (19)
#define VOICE_FONT_EN_IN_HEERA_APOLLO    (20)
#define VOICE_FONT_EN_IN_PRIYA_RUS       (21)
#define VOICE_FONT_EN_IN_RAVI_APOLLO     (22)
#define VOICE_FONT_EN_US_ZIRA_RUS        (23)
#define VOICE_FONT_EN_US_JESSA_RUS       (24)
#define VOICE_FONT_EN_US_BENJAMIN_RUS    (25)
#define VOICE_FONT_ES_ES_LAURA_APOLLO    (26)
#define VOICE_FONT_ES_ES_HELENA_RUS      (27)
#define VOICE_FONT_ES_ES_PABLO_APOLLO    (28)
#define VOICE_FONT_ES_MX_HILDA_RUS       (29)
#define VOICE_FONT_ES_MX_RAUL_APOLLO     (30)
#define VOICE_FONT_FI_FI_HEIDI_RUS       (31)
#define VOICE_FONT_FR_CA_CAROLINE        (32)
#define VOICE_FONT_FR_CA_HARMONIE_RUS    (33)
#define VOICE_FONT_FR_CH_GUILLAUME       (34)
#define VOICE_FONT_FR_FR_JULIE_APOLLO    (35)
#define VOICE_FONT_FR_FR_HORTENSE_RUS    (36)
#define VOICE_FONT_FR_FR_PAUL_APOLLO     (37)
#define VOICE_FONT_HE_IL_ASAF            (38)
#define VOICE_FONT_HI_IN_KALPANA_APOLLO  (39)
#define VOICE_FONT_HI_IN_KALPANA         (40)
#define VOICE_FONT_HI_IN_HEMANT          (41)
#define VOICE_FONT_HR_HR_MATEJ           (42)
#define VOICE_FONT_HU_HU_SZABOLCS        (43)
#define VOICE_FONT_ID_ID_ANDIKA          (44)
#define VOICE_FONT_IT_IT_COSIMA_APOLLO   (45)
#define VOICE_FONT_JA_JP_AYUMI_APOLLO    (46)
#define VOICE_FONT_JA_JP_ICHIRO_APOLLO   (47)
#define VOICE_FONT_JA_JP_HARUKA_RUS      (48)
#define VOICE_FONT_JA_JP_LUCIA_RUS       (49)
#define VOICE_FONT_JA_JP_EKATERINA_RUS   (50)
#define VOICE_FONT_KO_KR_HEAMI_RUS       (51)
#define VOICE_FONT_MS_MY_RIZWAN          (52)
#define VOICE_FONT_NB_NO_HULDA_RUS       (53)
#define VOICE_FONT_NL_NL_HANNA_RUS       (54)
#define VOICE_FONT_PT_BR_HELOISA_RUS     (55)
#define VOICE_FONT_PT_BR_DANIEL_APOLLO   (56)
#define VOICE_FONT_RO_RO_ANDREI          (57)
#define VOICE_FONT_RU_RU_IRINA_APOLLO    (58)
#define VOICE_FONT_RU_RU_PAVEL_APOLLO    (59)
#define VOICE_FONT_SK_SK_FILIP           (60)
#define VOICE_FONT_SV_SE_HEDVIG_RUS      (61)
#define VOICE_FONT_TA_IN_VALLUVAR        (62)
#define VOICE_FONT_TH_TH_PATTARA         (63)
#define VOICE_FONT_TR_TR_SEDA_RUS        (64)
#define VOICE_FONT_VI_VN_AN              (65)
#define VOICE_FONT_ZH_CN_HUIHUI_RUS      (66)
#define VOICE_FONT_ZH_CN_YAOYAO_APOLLO   (67)
#define VOICE_FONT_ZH_CN_KANGKANG_APOLLO (68)
#define VOICE_FONT_ZH_HK_TRACY_APOLLO    (69)
#define VOICE_FONT_ZH_HK_TRACY_RUS       (70)
#define VOICE_FONT_ZH_HK_DANNY_APOLLO    (71)
#define VOICE_FONT_ZH_TW_YATING_APOLLO   (72)
#define VOICE_FONT_ZH_TW_HANHAN_RUS      (73)
#define VOICE_FONT_ZH_TW_ZHIWEI_APOLLO   (74)
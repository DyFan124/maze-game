#include "game.h"

static const char* SOUND_PATHS[SFX_COUNT] = {
    "../assets/sfx/start.wav",       // SFX_START
    "../assets/sfx/point.wav",       // SFX_POINT  
    "../assets/sfx/win.wav",         // SFX_WIN
    "../assets/sfx/gameover.wav",    // SFX_GAMEOVER（先重命名！）
    "../assets/sfx/damage.wav",      // SFX_DAMAGE
    "../assets/sfx/score.wav"        // SFX_SCORE
};

static Sound sounds[SFX_COUNT];
static Music musicStart;
static float sfxVolume = 0.7f;
static float musicVolume = 0.8f;

// 辅助函数：限制值范围
static float Audio_Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void Audio_Init(void) {
    // 先重命名文件（去掉空格）
    if (FileExists("../assets/sfx/game over.wav")) {
        rename("../assets/sfx/game over.wav", "../assets/sfx/gameover.wav");
    }
    
    // 初始化音频
    InitAudioDevice();
    
    if (!IsAudioDeviceReady()) {
        printf("音频设备初始化失败\n");
        return;
    }
    
    printf("加载音频文件...\n");
    
    // 加载所有音效
    for (int i = 0; i < SFX_COUNT; i++) {
        printf("  %s ... ", SOUND_PATHS[i]);
        
        if (FileExists(SOUND_PATHS[i])) {
            sounds[i] = LoadSound(SOUND_PATHS[i]);
            if (sounds[i].frameCount > 0) {
                printf("成功\n");
                SetSoundVolume(sounds[i], sfxVolume);
            } else {
                printf("失败（格式可能不支持）\n");
            }
        } else {
            printf("文件不存在\n");
        }
    }
    
    // 加载背景音乐
    if (FileExists(SOUND_PATHS[SFX_START])) {
        musicStart = LoadMusicStream(SOUND_PATHS[SFX_START]);
        SetMusicVolume(musicStart, musicVolume);
        PlayMusicStream(musicStart);
    }
}

void Audio_PlaySound(SoundType type) {
    if (type < 0 || type >= SFX_COUNT) return;
    
    // 如果音频加载成功就播放
    if (sounds[type].frameCount > 0) {
        PlaySound(sounds[type]);
    } else {
        printf("音频 %d 未加载，跳过播放\n", type);
    }
}

void Audio_SetSfxVolume(float volume) {
    sfxVolume = Audio_Clamp(volume, 0.0f, 1.0f);
    for (int i = 0; i < SFX_COUNT; i++) {
        if (sounds[i].frameCount > 0) {
            SetSoundVolume(sounds[i], sfxVolume);
        }
    }
}

void Audio_SetMusicVolume(float volume) {
    musicVolume = Audio_Clamp(volume, 0.0f, 1.0f);
    if (musicStart.frameCount > 0) {
        SetMusicVolume(musicStart, musicVolume);
    }
}

float Audio_GetSfxVolume(void) { return sfxVolume; }
float Audio_GetMusicVolume(void) { return musicVolume; }

void Audio_UpdateMusic(void) {
    if (musicStart.frameCount > 0) {
        UpdateMusicStream(musicStart);
    }
}

void Audio_Unload(void) {
    for (int i = 0; i < SFX_COUNT; i++) {
        if (sounds[i].frameCount > 0) {
            UnloadSound(sounds[i]);
        }
    }
    
    if (musicStart.frameCount > 0) {
        UnloadMusicStream(musicStart);
    }
    
    CloseAudioDevice();
}
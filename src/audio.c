#include "game.h"  // 复用全局配置（如SCREEN_WIDTH）
#include "raymath.h" 


// ========== 私有变量（仅音频模块内部使用） ==========
static const char* SOUND_PATHS[SFX_COUNT] = {
    "assets/sfx/start.wav",       // SFX_START
    "assets/sfx/point.wav",       // SFX_POINT
    "assets/sfx/win.wav",         // SFX_WIN
    "assets/sfx/game over.wav",   // SFX_GAMEOVER
    "assets/sfx/damage.wav",      // SFX_DAMAGE
    "assets/sfx/score.wav"        // SFX_SCORE
};

static Sound sounds[SFX_COUNT];    // 存储所有音效
static Music musicStart;           // 菜单/背景音乐
static float sfxVolume = 0.7f;     // 音效默认音量（0.0~1.0）
static float musicVolume = 0.8f;   // 音乐默认音量（0.0~1.0）

// ========== 接口实现 ==========
void Audio_Init(void) {
    // 初始化音频设备（Raylib要求）
    InitAudioDevice();
    
    // 加载所有音效
    for (int i = 0; i < SFX_COUNT; i++) {
        sounds[i] = LoadSound(SOUND_PATHS[i]);
        SetSoundVolume(sounds[i], sfxVolume); // 应用初始音量
    }
    
    // 加载背景音乐并播放
    musicStart = LoadMusicStream(SOUND_PATHS[SFX_START]);
    SetMusicVolume(musicStart, musicVolume);
    PlayMusicStream(musicStart);
}

void Audio_PlaySound(SoundType type) {
    // 防越界
    if (type < 0 || type >= SFX_COUNT) return;
    PlaySound(sounds[type]);
}

void Audio_SetSfxVolume(float volume) {
    // 限制音量范围（0.0~1.0）
    sfxVolume = Clamp(volume, 0.0f, 1.0f);
    // 同步所有音效的音量
    for (int i = 0; i < SFX_COUNT; i++) {
        SetSoundVolume(sounds[i], sfxVolume);
    }
}

void Audio_SetMusicVolume(float volume) {
    musicVolume = Clamp(volume, 0.0f, 1.0f);
    SetMusicVolume(musicStart, musicVolume);
}

float Audio_GetSfxVolume(void) {
    return sfxVolume;
}

float Audio_GetMusicVolume(void) {
    return musicVolume;
}

void Audio_UpdateMusic(void) {
    // 每帧更新音乐流（否则背景音乐播放异常）
    UpdateMusicStream(musicStart);
}

void Audio_Unload(void) {
    // 释放所有音效
    for (int i = 0; i < SFX_COUNT; i++) {
        UnloadSound(sounds[i]);
    }
    // 释放背景音乐
    UnloadMusicStream(musicStart);
    // 关闭音频设备
    CloseAudioDevice();
}

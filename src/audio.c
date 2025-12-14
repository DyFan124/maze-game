#include "game.h"  // 复用全局配置（如SCREEN_WIDTH）
#include "raymath.h" 
// audio.c - 游戏音频管理代码（负责背景音乐、音效的播放和音量调节）
#include "game.h"  // 引入游戏全局配置
#include "raymath.h" // 引入数学工具


// 私有变量（仅音频模块内部使用）
// 私有变量（音频模块自用）
// 所有音效文件的路径
static const char* SOUND_PATHS[SFX_COUNT] = {
    "assets/sfx/start.wav",       // SFX_START
    "assets/sfx/point.wav",       // SFX_POINT
    "assets/sfx/win.wav",         // SFX_WIN
    "assets/sfx/game over.wav",   // SFX_GAMEOVER
    "assets/sfx/damage.wav",      // SFX_DAMAGE
    "assets/sfx/score.wav"        // SFX_SCORE
    "assets/sfx/start.wav",       // 对应SFX_START：菜单/开始音效
    "assets/sfx/point.wav",       // 对应SFX_POINT：得分音效
    "assets/sfx/win.wav",         // 对应SFX_WIN：胜利音效
    "assets/sfx/game over.wav",   // 对应SFX_GAMEOVER：游戏失败音效
    "assets/sfx/damage.wav",      // 对应SFX_DAMAGE：受伤音效
    "assets/sfx/score.wav"        // 对应SFX_SCORE：收集道具加分音效
};

static Sound sounds[SFX_COUNT];    // 存储所有音效
static Music musicStart;           // 菜单/背景音乐
static float sfxVolume = 0.7f;     // 音效默认音量（0.0~1.0）
static float musicVolume = 0.8f;   // 音乐默认音量（0.0~1.0）
static Sound sounds[SFX_COUNT];    // 存储加载后的所有音效（数组）
static Music musicStart;           // 存储背景音乐（菜单/游戏中播放）
static float sfxVolume = 0.7f;     // 音效默认音量（0.0=静音，1.0=最大）
static float musicVolume = 0.8f;   // 背景音乐默认音量（0.0=静音，1.0=最大）

// 音频模块核心函数（外部调用接口）

// 接口实现 
// 功能：初始化音频系统（游戏启动时只调用一次）
void Audio_Init(void) {
    // 初始化音频设备（Raylib要求）
    // 第一步：初始化音频设备（Raylib库要求必须先初始化才能用音频）
    InitAudioDevice();

    // 加载所有音效
    // 第二步：加载所有音效文件到内存
    // 遍历所有音效路径，逐个加载
    for (int i = 0; i < SFX_COUNT; i++) {
        // 加载单个音效文件
        sounds[i] = LoadSound(SOUND_PATHS[i]);
        SetSoundVolume(sounds[i], sfxVolume); // 应用初始音量
        // 给加载好的音效设置初始音量
        SetSoundVolume(sounds[i], sfxVolume);
    }

    // 加载背景音乐并播放
    // 第三步：加载并播放背景音乐
    // 加载背景音乐文件
    musicStart = LoadMusicStream(SOUND_PATHS[SFX_START]);
    // 设置背景音乐的初始音量
    SetMusicVolume(musicStart, musicVolume);
    // 开始循环播放背景音乐
    PlayMusicStream(musicStart);
}

// 功能：播放指定类型的音效
// 参数：type - 要播放的音效类型（比如SFX_WIN、SFX_DAMAGE）
void Audio_PlaySound(SoundType type) {
    // 防越界
    // 安全检查：防止传入的音效类型超出范围（比如传了一个不存在的类型）
    if (type < 0 || type >= SFX_COUNT) return;
    // 播放对应类型的音效
    PlaySound(sounds[type]);
}

// 功能：设置所有音效的音量
// 参数：volume音量值（0.0~1.0，超出范围会被自动限制）
void Audio_SetSfxVolume(float volume) {
    // 限制音量范围（0.0~1.0）
    // 限制音量范围：确保volume在0.0（静音）到1.0（最大）之间
    sfxVolume = Clamp(volume, 0.0f, 1.0f);
    // 同步所有音效的音量
    // 把新音量同步到所有已加载的音效
    for (int i = 0; i < SFX_COUNT; i++) {
        SetSoundVolume(sounds[i], sfxVolume);
    }
}

// 功能：设置背景音乐的音量
// 参数：volume音量值（0.0~1.0，超出范围会被自动限制）
void Audio_SetMusicVolume(float volume) {
    // 限制音量范围：确保volume在0.0（静音）到1.0（最大）之间
    musicVolume = Clamp(volume, 0.0f, 1.0f);
    // 把新音量应用到背景音乐
    SetMusicVolume(musicStart, musicVolume);
}

// 功能：获取当前音效的音量
// 返回值：当前音效音量（0.0~1.0）
float Audio_GetSfxVolume(void) {
    return sfxVolume;
}

// 功能：获取当前背景音乐的音量
// 返回值：当前背景音乐音量（0.0~1.0）
float Audio_GetMusicVolume(void) {
    return musicVolume;
}

// 功能：更新背景音乐流（每帧调用，音乐播放防卡顿/停止）
void Audio_UpdateMusic(void) {
    // 每帧更新音乐流（否则背景音乐播放异常）
    // 刷新背景音乐的播放状态（Raylib每帧调用）
    UpdateMusicStream(musicStart);
}

// 功能：释放音频资源（游戏退出时调用，防止内存泄漏）
void Audio_Unload(void) {
    // 释放所有音效
    // 第一步：释放所有加载的音效
    for (int i = 0; i < SFX_COUNT; i++) {
        UnloadSound(sounds[i]);
    }
    // 释放背景音乐
    // 第二步：释放背景音乐
    UnloadMusicStream(musicStart);
    // 关闭音频设备
    // 第三步：关闭音频设备
    CloseAudioDevice();
}
#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>

// Âm thanh trong mixer là phát duy nhất âm thanh được lưu cuối cùng trong channel , các channel có thể được phát song song
// Việc sử dụng việc gắn channel =-1 là để hệ thống tự chọn kênh trống để phát từ đó tránh xung đột

/*SDL Mixer phân chia rõ ràng hai hệ thống phát âm thanh:

1. Kênh âm thanh ngắn (Mix_Chunk):
SDL Mixer sử dụng kênh từ 0 đến số lượng kênh bạn đã cấp phát (mặc định là 8, từ 0 đến 7) để phát âm thanh ngắn (Mix_Chunk) qua Mix_PlayChannel().

Bạn có thể điều chỉnh số lượng kênh này bằng Mix_AllocateChannels(). Ví dụ, tăng số lượng kênh lên 16 sẽ mở rộng phạm vi từ 0 đến 15.

2. Nhạc nền (Mix_Music):
Nhạc nền (Mix_Music) không sử dụng kênh trong hệ thống kênh của Mix_Chunk. Nó được phát độc lập qua hàm Mix_PlayMusic().

SDL Mixer chỉ cho phép phát một file nhạc nền tại một thời điểm. Bạn có thể kiểm soát nhạc nền bằng các hàm như:

Mix_PlayMusic() để phát.

Mix_PauseMusic() để tạm dừng.

Mix_HaltMusic() để dừng hoàn toàn.

Mix_PlayingMusic() để kiểm tra trạng thái.  */


class Sound {
// loại 1 : âm thanh ngắn
// loại 2 : âm thanh nền
private:
    Mix_Chunk* shortSound = nullptr;  // Âm thanh ngắn (tiếng đạn/nổ)
    Mix_Music* longSound = nullptr;   // Âm thanh dài (nhạc nền)
    int soundType;                    // Loại âm thanh: 1 (ngắn) hoặc 2 (dài)
    bool isPaused = false;            // Trạng thái tạm dừng nhạc nền
    int channel   = -1;               // kênh âm thanh nếu nó là âm thanh loại 1 : trunk

public:
    Sound(const std::string& filePath, int type) : soundType(type) {
        if (soundType == 1) {
            shortSound = Mix_LoadWAV(filePath.c_str());
            if (!shortSound) {
                printf("Error loading short sound: %s\n", Mix_GetError());
            }
        } else if (soundType == 2) {
            longSound = Mix_LoadMUS(filePath.c_str());
            if (!longSound) {
                printf("Error loading long sound: %s\n", Mix_GetError());
            }
        }
    }

    ~Sound() {
        if (shortSound) Mix_FreeChunk(shortSound);
        if (longSound) Mix_FreeMusic(longSound);
    }

    void play() {
        if (soundType == 1 && shortSound) {
            channel = Mix_PlayChannel(-1, shortSound, 0); // Phát âm thanh ngắn một lần trên kênh tự chọn và ghi lại kênh đó
        } else if (soundType == 2 && longSound) {
            if (isPaused) {
                Mix_ResumeMusic(); // Tiếp tục phát từ trạng thái tạm dừng
                isPaused = false;
            }

            else if(!Mix_PlayingMusic()) {
                Mix_PlayMusic(longSound, -1); // Phát nhạc nền từ đầu và liên tục
            }
        }
    }

    void pause() {
        if (soundType == 2 && longSound) {
            Mix_PauseMusic(); // Tạm dừng nhạc nền
            isPaused = true;
        }

    }

    void stop() {
        if (soundType == 2 && longSound) {
            Mix_HaltMusic(); // Dừng nhạc nền hoàn toàn
            isPaused = false; // Đặt trạng thái về mặc định
        }
        else if( channel != -1){
            //channel != -1 tức nó đã được gắn vào một kênh để pphats và chắc chắn đây là âm thanh ngắn

              Mix_HaltChannel(channel);// dừng âm thanh trên kênh lần cuối cùng mà trunk phát

              // cần lưu ý có thể hiện tại channel đang pahst âm khác , việc dừng này sẽ làm mất âm thanh đang đang phát

              channel = -1; // Đặt lại trạng thái kênh sau khi dừng

        }
    }
    void set_volume (int volume){
        if(soundType == 2){
            Mix_VolumeMusic(volume); // Thiết lập âm lượng nhạc nền
        }
    }
};



#endif // MUSIC_H

#include "FPS.h"
#include <thread>
#include "Common/Assertion.h"

namespace Altseed {

FPS::FPS() {
    previousTime_ = std::chrono::system_clock::now();
    SetTarget(60);
}

void FPS::Update() {
    auto currentTime = std::chrono::system_clock::now();
    auto deltans = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime_);

    // 更新時間に余裕がある時は待機処理をする
    if (deltans < framens_) {
        auto sleepns = ((previousTime_ + framens_) - currentTime);

        // milliseconds単位でsleepする
        std::this_thread::sleep_for(std::chrono::milliseconds((sleepns.count() / 1000000) - 1));

        // busy loopで調整
        do {
            currentTime = std::chrono::system_clock::now();
            deltans = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime_);
        } while (deltans < framens_);
    }

    // 計測処理を行う
    currentFPS_ = 1.0f / deltaSecond_;

    if (framerateMode_ == FramerateMode::Constant) {
        // 固定FPSでは経過時間は一定として扱う
        deltaSecond_ = 1.0f / targetFPS_;
    } else {
        deltaSecond_ = static_cast<float>(deltans.count()) / nano;
    }

    previousTime_ = currentTime;
}

float FPS::GetDeltaSecond() const { return deltaSecond_; }
float FPS::GetCurrentFPS() const { return currentFPS_; }

int32_t FPS::GetTargetFPS() const { return targetFPS_; }
void FPS::SetTarget(int32_t fps) {
    targetFPS_ = fps;
    framens_ = std::chrono::nanoseconds(nano / fps);
}

FramerateMode FPS::GetFramerateMode() const { return framerateMode_; }
void FPS::SetFramerateMode(FramerateMode framerateMode) { framerateMode_ = framerateMode; }

}  // namespace Altseed
#include <Sound/SoundMixer.h>
#include <Sound/Sound.h>
#include <gtest/gtest.h>

namespace asd = altseed;

TEST(Sound, SoundPlay)
{
    char16_t s16[] = u"test";
    EXPECT_TRUE(asd::Core::Initialize(s16, 640, 480, asd::CoreOption()));

    auto mixer = asd::SoundMixer::GetInstance();
    auto bgm = mixer->CreateSound(u"TestData/Sound/bgm1.ogg", false);
    auto se = mixer->CreateSound(u"TestData/Sound/se1.wav", true);

    int id_bgm = mixer->Play(bgm);
    int id_se = mixer->Play(se);

    while(asd::Core::GetInstance() -> DoEvent() && (mixer->GetIsPlaying(id_bgm) || mixer->GetIsPlaying(id_se))) { }

    asd::Core::Terminate();
}
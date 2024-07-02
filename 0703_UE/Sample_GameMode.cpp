
// インクルード部
#include <Sample_GameMode.h>

// クラス実装
void ASample_GameMode::StartPlay()
{
    Super::StartPlay();

    // 出力テスト
    check(GEngine != nullptr);

    // 5 秒間デバッグメッセージを表示します。
    // 引数-1の "Key "値によって、メッセージの更新やリフレッシュができなくなります。
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, L"Hello World, this is Sample_GameMode!");
}

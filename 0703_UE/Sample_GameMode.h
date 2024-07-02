
// インクルード部
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

// インクルード終了
#include <Sample_GameMode.generated.h>

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_GameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	// 関数宣言
	virtual void StartPlay() override;
};

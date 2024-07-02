
// インクルード部
#pragma once
#include <GameFramework/Actor.h>

// インクルード終了
#include <Sample_CameraDirector.generated.h>

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_CameraDirector : public AActor
{
	GENERATED_BODY()

public:

	// 関数宣言
	ASample_CameraDirector();                    // コンストラクタ
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// 関数宣言
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== カメラ仮実装 =====

public:

	//変数宣言
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> CameraOne;	// Camera 1

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> CameraTwo;	// Camera 2

private:

	//変数宣言
	float CameraTimer = 0.0f;		// カメラ遷移用タイマ
	float CameraStayTime = 2.0f;	// カメラの維持時間
	float CameraBlendTime = 0.75f;	// カメラの遷移時間
};

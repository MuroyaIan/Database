
// インクルード部
#include <Sample_CameraDirector.h>
#include <Kismet/GameplayStatics.h> // PlayerController

// クラス実装
ASample_CameraDirector::ASample_CameraDirector()
{
	PrimaryActorTick.bCanEverTick = true;

	// カメラ1は自分自身
	CameraOne = this;
}

void ASample_CameraDirector::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// カメラの繊維処理
	CameraTimer -= DeltaTime;
	if (CameraTimer <= 0.0f)
	{
		CameraTimer += CameraStayTime;

		//  PlayerController取得
		if (const TObjectPtr<APlayerController> pOurPlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			if ((pOurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				pOurPlayerController->SetViewTarget(CameraOne);								// カメラ1に瞬時に切り替わる。
			}
			else if ((pOurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				pOurPlayerController->SetViewTargetWithBlend(CameraTwo, CameraBlendTime);	// カメラ2へスムーズにブレンドする。
			}
		}
	}
}

void ASample_CameraDirector::BeginPlay()
{
	Super::BeginPlay();
}

void ASample_CameraDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

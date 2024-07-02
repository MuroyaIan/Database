
// �C���N���[�h��
#include <Sample_CameraDirector.h>
#include <Kismet/GameplayStatics.h> // PlayerController

// �N���X����
ASample_CameraDirector::ASample_CameraDirector()
{
	PrimaryActorTick.bCanEverTick = true;

	// �J����1�͎������g
	CameraOne = this;
}

void ASample_CameraDirector::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �J�����̑@�ۏ���
	CameraTimer -= DeltaTime;
	if (CameraTimer <= 0.0f)
	{
		CameraTimer += CameraStayTime;

		//  PlayerController�擾
		if (const TObjectPtr<APlayerController> pOurPlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			if ((pOurPlayerController->GetViewTarget() != CameraOne) && (CameraOne != nullptr))
			{
				pOurPlayerController->SetViewTarget(CameraOne);								// �J����1�ɏu���ɐ؂�ւ��B
			}
			else if ((pOurPlayerController->GetViewTarget() != CameraTwo) && (CameraTwo != nullptr))
			{
				pOurPlayerController->SetViewTargetWithBlend(CameraTwo, CameraBlendTime);	// �J����2�փX���[�Y�Ƀu�����h����B
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


// �C���N���[�h��
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

// �C���N���[�h�I��
#include <Sample_GameMode.generated.h>

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_GameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	// �֐��錾
	virtual void StartPlay() override;
};

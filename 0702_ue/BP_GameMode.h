
#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>
#include <BP_GameMode.generated.h>

/**
 *
 */
UCLASS()
class PROJECTC_5_4_API ABP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	// �֐��錾
	virtual void StartPlay() override;
};


#pragma once

// include
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// include end
#include <GgDataTableTest.generated.h>

// forward declare
class UNiagaraSystem;
class UDataTable;

// struct
USTRUCT(BlueprintType, Blueprintable)
struct FGgDebrisAbility : public FTableRowBase
{
	GENERATED_BODY()

	// variable

	/* Debris Number */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DebrisNumber = 0;

	/* Debris Effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> DebrisEffect = nullptr;
};

// class
UCLASS()
class PRJ_NIAGARADI_API AGgDataTableTest : public AActor
{
	GENERATED_BODY()


	//===== Base =====

public:

	AGgDataTableTest();                          // Constructor
	virtual void Tick(float DeltaTime) override; // Update

protected:

	virtual void BeginPlay() override;                                       // Begin
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // End


	//===== Base =====

private:

	/* DT Debris Ability 2 */
	UPROPERTY(EditDefaultsOnly, Category = "Debris Ability")
	TObjectPtr<UDataTable> DT_DebrisAbility2;

	/* DT Debris Ability 3 */
	UPROPERTY(EditDefaultsOnly, Category = "Debris Ability")
	TSoftObjectPtr<UDataTable> DT_DebrisAbility3;
};

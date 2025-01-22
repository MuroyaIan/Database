
#pragma once

// include
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// include end
#include <GgDataTableTest.generated.h>

// forward declare
class UNiagaraSystem;
class UDataTable;
class USceneComponent;
class USkeletalMeshComponent;
class USkeletalMesh;

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


	//===== DataTable Test =====

private:

	/* DT Debris Ability 2 */
	UPROPERTY(EditDefaultsOnly, Category = "DataTable Test")
	TObjectPtr<UDataTable> DT_DebrisAbility2;

	/* DT Debris Ability 3 */
	UPROPERTY(EditDefaultsOnly, Category = "DataTable Test")
	TSoftObjectPtr<UDataTable> DT_DebrisAbility3;


	//===== Component Test =====

	/* Scene Root */
	UPROPERTY(EditDefaultsOnly, Category = "Component Test")
	TObjectPtr<USceneComponent> SceneRoot = nullptr;

	/* Skeletal Mesh Component 1 */
	UPROPERTY(EditDefaultsOnly, Category = "Component Test")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent1 = nullptr;

	/* Skeletal Mesh Component 2 */
	UPROPERTY(EditDefaultsOnly, Category = "Component Test")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent2 = nullptr;

	/* Skeletal Mesh Template */
	UPROPERTY(EditDefaultsOnly, Category = "Component Test")
	TObjectPtr<USkeletalMesh> SkeletalMeshTemplate = nullptr;

	/* Skeletal Mesh Component Template */
	UPROPERTY(EditDefaultsOnly, Category = "Component Test")
	TSubclassOf<USkeletalMeshComponent> SkeletalMeshComponentTemplate = nullptr;

	/* Add Skeletal Mesh Component */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "ComponentTest"), Category = "Component Test")
	void AddSkeletalMeshComponent();
};

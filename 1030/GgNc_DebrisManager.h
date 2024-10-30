
// include
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <GgNDI_CustomExport.h>

// include end
#include <GgNc_DebrisManager.generated.h>

// class
UCLASS()
class PROJECTC_5_4_API AGgNc_DebrisManager : public AActor
{
	GENERATED_BODY()

	//===== Base =====

public:

	// function
	AGgNc_DebrisManager();                       // Constructor
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// function
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

private:

	//===== Get Particle =====

	/* Get Particle From Distance */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static TArray<int32> GetParticleFromDistance(UPARAM(ref) const TArray<FGgNDI_BasicParticleData>& ParticleArray, UPARAM(ref) const FVector& CenterPosition,
		UPARAM(ref) const float& MaxDistance, UPARAM(ref) const int32& MaxNumber) noexcept;
};

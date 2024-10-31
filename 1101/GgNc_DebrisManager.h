
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


	//===== Particle =====

public:

	// function

protected:

	// variable

	/* Particle Usage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gg Particle")
	TArray<bool> ParticleUsage;

	/* Particle Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gg Particle")
	TArray<FGgNDI_BasicParticleData> ParticleData;

	/* Particle Number */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000"), Category = "Gg Particle")
	int32 ParticleNumber = 0;

private:

	// function

	/* Use Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	void UseParticle(UPARAM(ref) const TArray<int32>& IndexArray, TArray<FGgNDI_BasicParticleData>& ParticleArray) noexcept;

	/* Free Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	void FreeParticle(UPARAM(ref) const TArray<FGgNDI_BasicParticleData>& ParticleArray) noexcept;

	/* Get Particle From Distance */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FuncLib"), Category = "GgNc | FuncLib")
	static void GetParticleFromDistance(UPARAM(ref) const TArray<FGgNDI_BasicParticleData>& ParticleArray, UPARAM(ref) const FVector& CenterPosition,
										UPARAM(ref) const float& MaxDistance, UPARAM(ref) const int32& MaxNumber, TArray<int32>& IndexArray) noexcept;
};


// include
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <GgNcDi_ParticleExport.h>

// include end
#include <GgNc_DebrisManager.generated.h>

// forward declaration
class UNiagaraComponent;

// class
UCLASS()
class PROJECTC_5_4_API AGgNc_DebrisManager : public AActor, public IGgNcDi_NiagaraParticleCallbackHandler
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


	//===== Particle Export =====

private:

	// function
	virtual void GgNcDi_ReceiveParticleData_Implementation(const TArray<FGgNcDi_ParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;


	//===== Particle =====

public:

	// function

	/* Request Use Particle */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void RequestUseParticle();

	/* Request Free Particle */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void RequestFreeParticle();

protected:

	// variable

	/* Particle Number */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000"), Category = "Gg Particle")
	int32 ParticleNumber = 0;

	/* Particle Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gg Particle")
	TArray<FGgNcDi_ParticleData> ParticleData;

	/* Particle Usage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gg Particle")
	TArray<bool> ParticleUsage;

private:

	// function

	/* Init Niagara */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | System")
	void InitNiagara(UNiagaraComponent* EffectIn) noexcept;

	/* Switch Update Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	static void SwitchUpdateParticle(UNiagaraComponent* EffectIn, const bool bUpdate) noexcept;

	/* Update Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void UpdateParticle(UPARAM(ref) const TArray<FGgNcDi_ParticleData>& ParticleArrayIn) noexcept;

	/* Use Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void UseParticle(UNiagaraComponent* EffectIn, UPARAM(ref) const TArray<int32>& IndexArrayIn, TArray<FGgNcDi_ParticleData>& ParticleArrayOut) noexcept;

	/* Free Particle */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void FreeParticle(UNiagaraComponent* EffectIn, UPARAM(ref) const TArray<FGgNcDi_ParticleData>& ParticleArrayIn, TArray<int32>& IndexArrayOut) noexcept;

	/* End Reset Particle */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	void EndResetParticle(UNiagaraComponent* EffectIn, UPARAM(ref) const TArray<int32>& IndexArrayIn) const noexcept;

	/* Get Particle From Distance */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "GgDebris"), Category = "GgNc | Particle")
	static void GetParticleFromDistance(UPARAM(ref) const TArray<FGgNcDi_ParticleData>& ParticleArray, const FVector CenterPosition, const float MaxDistance, const int32 MaxNumber,
										TArray<int32>& IndexArray) noexcept;
};

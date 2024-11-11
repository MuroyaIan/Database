
// include
#pragma once
#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <GgNcDi_ParticleExport.h>
#include <GgNc_DebrisManager.h>

// include end
#include <GgNc_DebrisComponent.generated.h>

// class
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTC_5_4_API UGgNc_DebrisComponent : public UActorComponent, public IGgNcDi_NiagaraParticleCallbackHandler
{
	GENERATED_BODY()


	//===== Base =====

public:

	// function
	UGgNc_DebrisComponent();                                                                                                  // Constructor
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // Update

protected:

	// function
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit


	//===== Particle Export =====

private:

	// function
	virtual void GgNcDi_ReceiveParticleData_Implementation(const TArray<FGgNcDi_ParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;


	//===== Particle =====

protected:

	// variable

	/* Debris Manager */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GgNc | Debris")
	TObjectPtr<AGgNc_DebrisManager> DebrisManager;

	/* Particle Number */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2000"), Category = "GgNc | Particle")
	int32 ParticleNumber = 0;

	/* Particle Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GgNc | Particle")
	TArray<FGgNcDi_ParticleData> ParticleData;

	/* Particle Usage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GgNc | Particle")
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
};

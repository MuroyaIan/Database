
// include
#include <GgNc_DebrisComponent.h>
#include <NiagaraComponent.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>

// local namespace
namespace GgNcDebrisComponent
{
	static const FName StrExportActor = FName(L"RefActor");
	static const FName StrExportParticle = FName(L"bExport");
	static const FName StrSpawnCount = FName(L"SpawnCount");
	static const FName StrUseParticle = FName(L"UseParticle");
	static const FName StrResetParticle = FName(L"ResetParticle");
	static const FName StrInitPos = FName(L"InitPos");
	static const FName StrInitVel = FName(L"InitVel");
	static const FName StrInitRot = FName(L"InitRot");
	static const FName StrInitID = FName(L"InitID");
	static const FName StrInitMeshIndex = FName(L"InitMeshIndex");
	static const FName StrInitMass = FName(L"InitMass");
}

// class
UGgNc_DebrisComponent::UGgNc_DebrisComponent()
{
	if (ParticleNumber > 0)
	{
		ParticleData.Init(FGgNcDi_ParticleData(), ParticleNumber);
		ParticleUsage.Init(false, ParticleNumber);
	}
	else
	{
		ensureAlways(false);
	}

	PrimaryComponentTick.bCanEverTick = true;
}

void UGgNc_DebrisComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGgNc_DebrisComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGgNc_DebrisComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UGgNc_DebrisComponent::GgNcDi_ReceiveParticleData_Implementation(const TArray<FGgNcDi_ParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	ensureAlways(NiagaraSystem);
	UpdateParticle(Data);
}

void UGgNc_DebrisComponent::InitNiagara(UNiagaraComponent* EffectIn) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	EffectIn->SetVariableObject(GgNcDebrisComponent::StrExportActor, this);
	EffectIn->SetVariableBool(GgNcDebrisComponent::StrExportParticle, false);
	EffectIn->SetVariableInt(GgNcDebrisComponent::StrSpawnCount, ParticleNumber);

	TArray<bool> TempArrayBool;
	TempArrayBool.Init(true, ParticleNumber);
	TArray<float> TempArrayFloat;
	TempArrayFloat.Init(0.0f, ParticleNumber);
	TArray<FVector> TempArrayVector;
	TempArrayVector.Init(FVector(0.0, 0.0, 0.0), ParticleNumber);
	TArray<FQuat> TempArrayQuat;
	TempArrayQuat.Init(FQuat(0.0, 0.0, 0.0, 0.0), ParticleNumber);

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisComponent::StrUseParticle, TempArrayBool);
	TempArrayBool.Init(false, ParticleNumber);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisComponent::StrResetParticle, TempArrayBool);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloat(EffectIn, GgNcDebrisComponent::StrInitID, TempArrayFloat);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloat(EffectIn, GgNcDebrisComponent::StrInitMeshIndex, TempArrayFloat);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloat(EffectIn, GgNcDebrisComponent::StrInitMass, TempArrayFloat);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(EffectIn, GgNcDebrisComponent::StrInitPos, TempArrayVector);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(EffectIn, GgNcDebrisComponent::StrInitVel, TempArrayVector);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayQuat(EffectIn, GgNcDebrisComponent::StrInitRot, TempArrayQuat);
}

void UGgNc_DebrisComponent::SwitchUpdateParticle(UNiagaraComponent* EffectIn, const bool bUpdate) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	EffectIn->SetVariableBool(GgNcDebrisComponent::StrExportParticle, bUpdate);
}

void UGgNc_DebrisComponent::UpdateParticle(const TArray<FGgNcDi_ParticleData>& ParticleArrayIn) noexcept
{
	ParticleData.Empty();

	for (const auto& Particle : ParticleArrayIn)
	{
		ParticleData.Emplace(Particle);
	}
}

void UGgNc_DebrisComponent::UseParticle(UNiagaraComponent* EffectIn, const TArray<int32>& IndexArrayIn, TArray<FGgNcDi_ParticleData>& ParticleArrayOut) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	for (const auto& Index : IndexArrayIn)
	{
		if (Index >= ParticleNumber)
		{
			ensureAlways(false);
			continue;
		}
		ParticleArrayOut.Emplace(ParticleData[Index]);

		ParticleUsage[Index] = true;
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisComponent::StrUseParticle, Index, true, false);
	}
}

void UGgNc_DebrisComponent::FreeParticle(UNiagaraComponent* EffectIn, const TArray<FGgNcDi_ParticleData>& ParticleArrayIn, TArray<int32>& IndexArrayOut) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	ParticleData.Empty();

	int32 Index = 0;
	for (const auto& Particle : ParticleArrayIn)
	{
		if (Index >= ParticleNumber)
		{
			ensureAlways(false);
			continue;
		}
		ParticleData.Emplace(Particle);

		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloatValue(EffectIn, GgNcDebrisComponent::StrInitID, Index, Particle.UniqueID, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloatValue(EffectIn, GgNcDebrisComponent::StrInitMeshIndex, Index, Particle.MeshIndex, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloatValue(EffectIn, GgNcDebrisComponent::StrInitMass, Index, Particle.Mass, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPositionValue(EffectIn, GgNcDebrisComponent::StrInitPos, Index, Particle.Position, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(EffectIn, GgNcDebrisComponent::StrInitVel, Index, Particle.Velocity, false);
		FQuat TempQuat = FQuat(Particle.Rotation.X, Particle.Rotation.Y, Particle.Rotation.Z, Particle.Rotation.W);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayQuatValue(EffectIn, GgNcDebrisComponent::StrInitRot, Index, TempQuat, false);

		IndexArrayOut.Emplace(Index);

		ParticleUsage[Index] = false;
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisComponent::StrUseParticle, Index, false, false);

		Index++;
	}

	TArray<bool> TempArrayBool;
	TempArrayBool.Init(true, ParticleNumber);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisComponent::StrResetParticle, TempArrayBool);
}

void UGgNc_DebrisComponent::EndResetParticle(UNiagaraComponent* EffectIn, const TArray<int32>& IndexArrayIn) const noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	TArray<bool> TempArrayBool;
	TempArrayBool.Init(false, ParticleNumber);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisComponent::StrResetParticle, TempArrayBool);
}

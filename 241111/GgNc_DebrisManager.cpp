
// include
#include <GgNc_DebrisManager.h>
#include <NiagaraComponent.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>
#include <Kismet/KismetMathLibrary.h>

// local namespace
namespace GgNcDebrisManager
{
	static const FName StrExportActor = FName(L"RefActor");
	static const FName StrExportParticle = FName(L"bExport");
	static const FName StrSpawnCount = FName(L"SpawnCount");
	static const FName StrUseParticle = FName(L"UseParticle");
	static const FName StrResetParticle = FName(L"ResetParticle");
	static const FName StrInitPos = FName(L"InitPos");
	static const FName StrInitVel = FName(L"InitVel");
	static const FName StrInitRot = FName(L"InitRot");
}

// class
AGgNc_DebrisManager::AGgNc_DebrisManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGgNc_DebrisManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGgNc_DebrisManager::BeginPlay()
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

	Super::BeginPlay();

	//ensureAlways(true);
	//ensureAlwaysMsgf(false, L"%s->BeginPlay():Test Debug Log.", *GetName());
}

void AGgNc_DebrisManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGgNc_DebrisManager::GgNcDi_ReceiveParticleData_Implementation(const TArray<FGgNcDi_ParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	// super func is empty
	//IGgNcDi_NiagaraParticleCallbackHandler::GgNcDi_ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	ensureAlways(NiagaraSystem);
	UpdateParticle(Data);
}

void AGgNc_DebrisManager::InitNiagara(UNiagaraComponent* EffectIn) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	EffectIn->SetVariableObject(GgNcDebrisManager::StrExportActor, this);
	EffectIn->SetVariableBool(GgNcDebrisManager::StrExportParticle, false);
	EffectIn->SetVariableInt(GgNcDebrisManager::StrSpawnCount, ParticleNumber);

	TArray<bool> TempArrayBool;
	TempArrayBool.Init(false, ParticleNumber);
	TArray<FVector> TempArrayVector;
	TempArrayVector.Init(FVector(0.0, 0.0, 0.0), ParticleNumber);
	TArray<FQuat> TempArrayQuat;
	TempArrayQuat.Init(FQuat(0.0, 0.0, 0.0, 0.0), ParticleNumber);

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisManager::StrUseParticle, TempArrayBool);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBool(EffectIn, GgNcDebrisManager::StrResetParticle, TempArrayBool);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(EffectIn, GgNcDebrisManager::StrInitPos, TempArrayVector);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(EffectIn, GgNcDebrisManager::StrInitVel, TempArrayVector);
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayQuat(EffectIn, GgNcDebrisManager::StrInitRot, TempArrayQuat);
}

void AGgNc_DebrisManager::SwitchUpdateParticle(UNiagaraComponent* EffectIn, const bool bUpdate) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	EffectIn->SetVariableBool(GgNcDebrisManager::StrExportParticle, bUpdate);
}

void AGgNc_DebrisManager::UpdateParticle(const TArray<FGgNcDi_ParticleData>& ParticleArrayIn) noexcept
{
	for (const auto& Particle : ParticleArrayIn)
	{
		const int32 Index = Particle.UniqueID;
		if (Index >= ParticleNumber)
		{
			ensureAlways(false);
			continue;
		}

		ParticleData[Index].UniqueID = Particle.UniqueID;
		ParticleData[Index].MeshIndex = Particle.MeshIndex;
		ParticleData[Index].Mass = Particle.Mass;
		ParticleData[Index].Position = Particle.Position;
		ParticleData[Index].Velocity = Particle.Velocity;
		ParticleData[Index].Rotation = Particle.Rotation;
	}
}

void AGgNc_DebrisManager::UseParticle(UNiagaraComponent* EffectIn, const TArray<int32>& IndexArrayIn, TArray<FGgNcDi_ParticleData>& ParticleArrayOut) noexcept
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
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisManager::StrUseParticle, Index, true, false);
	}
}

void AGgNc_DebrisManager::FreeParticle(UNiagaraComponent* EffectIn, const TArray<FGgNcDi_ParticleData>& ParticleArrayIn, TArray<int32>& IndexArrayOut) noexcept
{
	if (EffectIn == nullptr)
	{
		ensureAlways(false);
		return;
	}

	for (const auto& Particle : ParticleArrayIn)
	{
		const int32 Index = Particle.UniqueID;
		if (Index >= ParticleNumber)
		{
			ensureAlways(false);
			continue;
		}

		ParticleData[Index].Position = Particle.Position;
		ParticleData[Index].Velocity = Particle.Velocity;
		ParticleData[Index].Rotation = Particle.Rotation;

		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPositionValue(EffectIn, GgNcDebrisManager::StrInitPos, Index, Particle.Position, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(EffectIn, GgNcDebrisManager::StrInitVel, Index, Particle.Velocity, false);
		FQuat TempQuat = FQuat(Particle.Rotation.X, Particle.Rotation.Y, Particle.Rotation.Z, Particle.Rotation.W);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayQuatValue(EffectIn, GgNcDebrisManager::StrInitRot, Index, TempQuat, false);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisManager::StrResetParticle, Index, true, false);

		IndexArrayOut.Emplace(Index);

		ParticleUsage[Index] = false;
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisManager::StrUseParticle, Index, false, false);
	}
}

void AGgNc_DebrisManager::EndResetParticle(UNiagaraComponent* EffectIn, UPARAM(ref) const TArray<int32>& IndexArrayIn) const noexcept
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

		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayBoolValue(EffectIn, GgNcDebrisManager::StrResetParticle, Index, false, false);
	}
}

void AGgNc_DebrisManager::GetParticleFromDistance(const TArray<FGgNcDi_ParticleData>& ParticleArray, const FVector CenterPosition, const float MaxDistance, const int32 MaxNumber, TArray<int32>& IndexArray) noexcept
{
	TMap<int32, double> DistanceMap;

	// get particles suitable for conditions
	for (const auto& Particle : ParticleArray)
	{
		double Distance = UKismetMathLibrary::Vector_Distance(Particle.Position, CenterPosition);
		if (Distance < MaxDistance)
		{
			DistanceMap.Emplace(Particle.UniqueID, Distance);
		}
	}

	// sort
	if (DistanceMap.Num() > MaxNumber)
	{
		DistanceMap.ValueSort(
			[](const double& A, const double& B)
			{
				return A < B;
			}
		);
	}

	// add
	for (const TPair<int32, double>& Particle : DistanceMap)
	{
		if (IndexArray.Num() == MaxNumber)
		{
			break;
		}
		IndexArray.Emplace(Particle.Key);
	}
}

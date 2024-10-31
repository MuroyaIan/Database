
// include
#include <GgNc_DebrisManager.h>
#include <Kismet/KismetMathLibrary.h>

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
	Super::BeginPlay();

	if (ParticleNumber > 0)
	{
		ParticleUsage.Init(false, ParticleNumber);
		ParticleData.Init(FGgNDI_BasicParticleData{}, ParticleNumber);
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"Particle Number is 0");
	}
}

void AGgNc_DebrisManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGgNc_DebrisManager::UseParticle(const TArray<int32>& IndexArray, TArray<FGgNDI_BasicParticleData>& ParticleArray) noexcept
{
	for (const auto& Index : IndexArray)
	{
		ParticleUsage[Index] = true;
		ParticleArray.Add(ParticleData[Index]);
	}
}

void AGgNc_DebrisManager::FreeParticle(const TArray<FGgNDI_BasicParticleData>& ParticleArray) noexcept
{
	for (const auto& Particle : ParticleArray)
	{
		ParticleUsage[Particle.UniqueID] = false;
	}
}

void AGgNc_DebrisManager::GetParticleFromDistance(const TArray<FGgNDI_BasicParticleData>& ParticleArray, const FVector& CenterPosition, const float& MaxDistance, const int32& MaxNumber, TArray<int32>& IndexArray) noexcept
{
	// get particles suitable for conditions
	int32 ParticleIndex{0};
	for (auto& Particle : ParticleArray)
	{
		if (UKismetMathLibrary::Vector_Distance(Particle.Position, CenterPosition) < MaxDistance && IndexArray.Num() <= MaxNumber)
		{
			IndexArray.Add(ParticleIndex);
		}

		ParticleIndex++;
	}

	// sort
	if (IndexArray.Num() <= MaxNumber)
	{
		IndexArray.Sort();
	}
}

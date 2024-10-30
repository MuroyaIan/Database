
// include
#include <GgNc_DebrisManager.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>

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
}

void AGgNc_DebrisManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

TArray<int32> AGgNc_DebrisManager::GetParticleFromDistance(const TArray<FGgNDI_BasicParticleData>& ParticleArray, const FVector& CenterPosition, const float& MaxDistance, const int32& MaxNumber) noexcept
{
	TArray<int32> IndexArray{};

	// get particles suitable for conditions
	int32 ParticleIndex{0};
	for (auto& Particle : ParticleArray)
	{
		if (UKismetMathLibrary::Vector_Distance(Particle.Position, CenterPosition) < MaxDistance && UKismetArrayLibrary::Array_Length(IndexArray) <= MaxNumber)
		{
			IndexArray.Add(ParticleIndex);
		}

		ParticleIndex++;
	}

	// sort
	if (UKismetArrayLibrary::Array_Length(IndexArray) <= MaxNumber)
	{
		UKismetArrayLibrary::SortIntArray(IndexArray, false, EArraySortOrder::Descending);
	}

	return IndexArray;
}

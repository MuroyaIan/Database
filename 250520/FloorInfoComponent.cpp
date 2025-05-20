
// include
#include <PRJ_TPS/FloorInfoComponent.h>
#include <Kismet/KismetSystemLibrary.h> // ray cast

#include "GameFramework/Character.h"

//===== Base =====

UFloorInfoComponent::UFloorInfoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFloorInfoComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// BP Tick
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ray cast test
	ExecRayCast(1000.0);
}

void UFloorInfoComponent::BeginPlay()
{
	// BP BeginPlay
	Super::BeginPlay();
}

void UFloorInfoComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// BP EndPlay
	Super::EndPlay(EndPlayReason);
}


//===== Ray Cast =====

void UFloorInfoComponent::ExecRayCast(const double RayLength) const noexcept
{
	// ray cast
	const FVector StartPos = GetOwner()->GetActorLocation();
	const FVector EndPos = StartPos + GetOwner()->GetActorUpVector() * (RayLength * -1.0);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Empty();
	FHitResult OutHit;
	const bool bResult = UKismetSystemLibrary::LineTraceSingle(
		this, StartPos, EndPos, UEngineTypes::ConvertToTraceType(ECC_Visibility),false,
		ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f
	);

	// check result
	if (bResult)
	{
		const FString PosText = FString::Printf(L"%.2f, %.2f, %.2f", OutHit.Location.X, OutHit.Location.Y, OutHit.Location.Z);

		// debug log
		UKismetSystemLibrary::PrintString(this, PosText, true, false, FColor::Cyan, 0.0f);
	}
}


//===== Foot Info =====

TArray<FName> UFloorInfoComponent::GetSocketNameList() const
{
	TArray<FName> SocketNames;
	SocketNames.Empty();

#if WITH_EDITOR

	// get skeletal mesh component
	if (!IsValid(OwnerClass))
	{
		return SocketNames;
	}
	const ACharacter* Character = OwnerClass.GetDefaultObject();
	if (!IsValid(Character))
	{
		return SocketNames;
	}
	const USkeletalMeshComponent* MeshComp = Character->GetMesh();
	if (!IsValid(MeshComp))
	{
		return SocketNames;
	}

	// get all socket names
	TArray<FComponentSocketDescription> Sockets;
	Sockets.Empty();
	MeshComp->QuerySupportedSockets(Sockets);
	for (const auto& Socket : Sockets)
	{
		SocketNames.Add(Socket.Name);
	}

#endif

	return SocketNames;
}

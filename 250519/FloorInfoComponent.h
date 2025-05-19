
// include
#pragma once
#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

// include end
#include <FloorInfoComponent.generated.h>

// class
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRJ_TPS_API UFloorInfoComponent : public UActorComponent
{
	GENERATED_BODY()


	//===== Base =====

public:

	//== function ==
	UFloorInfoComponent();                                                                                                    // Constructor
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // Tick

protected:

	//== function ==
	virtual void BeginPlay() override;                                       // Begin Play
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // End Play


	//===== Ray Cast =====

private:

	//== function ==
	void ExecRayCast(const double RayLength) const noexcept; // Exec Ray Cast
};

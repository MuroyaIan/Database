
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


	//===== Foot Info =====

private:

	//== variable ==

	// Owner Class �iSocket���擾�p�j
	UPROPERTY(EditDefaultsOnly, Category = "Foot Info")
	TSubclassOf<ACharacter> OwnerClass = nullptr;

	// Socket���F����
	UPROPERTY(EditDefaultsOnly, meta = (GetOptions = "GetSocketNameList"), Category = "Foot Info")
	FName SocketName_FootL = L"None";

	// Socket���F�E��
	UPROPERTY(EditDefaultsOnly, meta = (GetOptions = "GetSocketNameList"), Category = "Foot Info")
	FName SocketName_FootR = L"None";

	//== function ==

	// Socket���̔z����擾
	UFUNCTION()
	TArray<FName> GetSocketNameList() const;
};

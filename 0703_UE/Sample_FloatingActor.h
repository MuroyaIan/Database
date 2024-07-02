
// �C���N���[�h��
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// �C���N���[�h�I��
#include <Sample_FloatingActor.generated.h>

// �O���錾
class UStaticMeshComponent;

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_FloatingActor : public AActor
{
	GENERATED_BODY()

public:

	// �֐��錾
	ASample_FloatingActor();                     // �R���X�g���N�^
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// �֐��錾
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== SM������ =====

public:

	//�ϐ��錾
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshSample;	// SM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_FloatingActor")
	float FloatSpeed = 80.0f;						// Up Down Speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_FloatingActor")
	float RotationSpeed = 40.0f;					// Rotate Speed
};


// �C���N���[�h��
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// �C���N���[�h�I��
#include <Sample_ActorWithComp.generated.h>

// �O���錾
class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UBoxComponent;

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_ActorWithComp : public AActor
{
	GENERATED_BODY()

public:

	// �֐��錾
	ASample_ActorWithComp();                     // �R���X�g���N�^
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// �֐��錾
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== �R���|�[�l���g������ =====

public:

	//�ϐ��錾
	TObjectPtr<USceneComponent> SceneSample;					// SceneComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UStaticMeshComponent> StaticMeshSample;			// StaticMeshComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UBoxComponent> ColBoxSample;						// BoxComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UParticleSystemComponent> ParticleSystemSample;	// ParticleSystemComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UAudioComponent> AudioSample;					// AudioComponent
};

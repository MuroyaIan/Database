
// �C���N���[�h��
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>			// ����

// �C���N���[�h�I��
#include <Sample_Character.generated.h>

// �O���錾
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USkeletalMeshComponent;

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_Character : public ACharacter
{
	GENERATED_BODY()

public:

	// �֐��錾
	ASample_Character();                         // �R���X�g���N�^
	virtual void Tick(float DeltaTime) override; // Update

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// �֐��錾
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== EnhancedInputComponent������ =====

	// �ϐ��錾
	UPROPERTY(EditAnywhere, Category = "ASample_Character | Input")
	TObjectPtr<UInputAction> JumpAction;		// SampleAction

	UPROPERTY(EditAnywhere, Category = "ASample_Character | Input")
	TObjectPtr<UInputAction> MoveAction;		// SampleAxis

	UPROPERTY(EditAnywhere, Category = "ASample_Character | Input")
	TObjectPtr<UInputAction> LookAction;		// SampleAxis

	UPROPERTY(EditAnywhere, Category = "ASample_Character | Input")
	TObjectPtr<UInputAction> FireAction;		// SampleAction

	UPROPERTY(EditAnywhere, Category = "ASample_Character | Input")
	TObjectPtr<UInputMappingContext> SampleImc; // SampleIMC

private:

	// �ϐ��錾
	FVector2D m_Move2D; // �ړ���
	FVector2D m_Look2D; // �J������]��

	// �֐��錾
	void InitInput();                                          // Input������
	void JumpAction_Started();                                 // EventHandle(Started)
	void JumpAction_Completed();                               // EventHandle(Completed)
	void MoveAction_Triggered(const FInputActionValue& Value); // EventHandle(Triggered)
	void LookAction_Triggered(const FInputActionValue& Value); // EventHandle(Triggered)
	void FireAction_Started();								   // EventHandle(Started)

	UFUNCTION()
	void MoveForward(float Value);	// Move Forward And Back

	UFUNCTION()
	void MoveRight(float Value);	// Move Left And Right

	//===== UCameraComponent������ =====

public:

	// �ϐ��錾
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraSample;	// CameraComponent

    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Character | Mesh")
    TObjectPtr<USkeletalMeshComponent> MeshArm;	// Arm Mesh

	//===== AFPSProjectile������ =====

public:

	// �ϐ��錾
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_Character | Gameplay")
    FVector MuzzleOffset;									// Offset From Camera Pos To Muzzle

protected:

	// �ϐ��錾
	UPROPERTY(EditDefaultsOnly, Category = "ASample_Character | Projectile")
	TSubclassOf<class ASample_Projectile> ClassProjectile;	// Sample_Projectile For Spawn
};

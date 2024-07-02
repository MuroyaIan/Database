
// インクルード部
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>			// 入力

// インクルード終了
#include <Sample_Character.generated.h>

// 前方宣言
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USkeletalMeshComponent;

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_Character : public ACharacter
{
	GENERATED_BODY()

public:

	// 関数宣言
	ASample_Character();                         // コンストラクタ
	virtual void Tick(float DeltaTime) override; // Update

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// 関数宣言
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== EnhancedInputComponent仮実装 =====

	// 変数宣言
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

	// 変数宣言
	FVector2D m_Move2D; // 移動量
	FVector2D m_Look2D; // カメラ回転量

	// 関数宣言
	void InitInput();                                          // Input初期化
	void JumpAction_Started();                                 // EventHandle(Started)
	void JumpAction_Completed();                               // EventHandle(Completed)
	void MoveAction_Triggered(const FInputActionValue& Value); // EventHandle(Triggered)
	void LookAction_Triggered(const FInputActionValue& Value); // EventHandle(Triggered)
	void FireAction_Started();								   // EventHandle(Started)

	UFUNCTION()
	void MoveForward(float Value);	// Move Forward And Back

	UFUNCTION()
	void MoveRight(float Value);	// Move Left And Right

	//===== UCameraComponent仮実装 =====

public:

	// 変数宣言
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraSample;	// CameraComponent

    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Character | Mesh")
    TObjectPtr<USkeletalMeshComponent> MeshArm;	// Arm Mesh

	//===== AFPSProjectile仮実装 =====

public:

	// 変数宣言
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_Character | Gameplay")
    FVector MuzzleOffset;									// Offset From Camera Pos To Muzzle

protected:

	// 変数宣言
	UPROPERTY(EditDefaultsOnly, Category = "ASample_Character | Projectile")
	TSubclassOf<class ASample_Projectile> ClassProjectile;	// Sample_Projectile For Spawn
};

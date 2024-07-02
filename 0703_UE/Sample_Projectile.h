
// �C���N���[�h��
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// �C���N���[�h�I��
#include "Sample_Projectile.generated.h"

// �O���錾
class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_Projectile : public AActor
{
	GENERATED_BODY()

public:

	// �֐��錾
	ASample_Projectile();                        // �R���X�g���N�^
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// �֐��錾
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== UProjectileMovementComponent������ =====

public:

	// �ϐ��錾
    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Projectile | Projectile")
    TObjectPtr<USphereComponent> ColSphereSample;						// SphereComponent

    UPROPERTY(VisibleAnywhere, Category = "ASample_Projectile | Movement")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementSample;	// ProjectileMovementComponent

    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Projectile | Projectile")
    TObjectPtr<UStaticMeshComponent> ProjectileMeshSample;				// StaticMeshComponent

	UPROPERTY(EditAnywhere, Category = "ASample_Projectile | Movement")
	TObjectPtr<UMaterial> ProjectileMaterial;							// Material For Projectile

	TObjectPtr<UMaterialInstanceDynamic> m_pProjectileMID;				// ���˕��pMID

	// �֐��錾
    void FireInDirection(const FVector& ShootDirection) const;	// ���˕��̔��˕����̑��x������������֐��B

	//===== OnHit Event =====

public:

	// �֐��錾
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};


// インクルード部
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// インクルード終了
#include "Sample_Projectile.generated.h"

// 前方宣言
class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_Projectile : public AActor
{
	GENERATED_BODY()

public:

	// 関数宣言
	ASample_Projectile();                        // コンストラクタ
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// 関数宣言
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== UProjectileMovementComponent仮実装 =====

public:

	// 変数宣言
    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Projectile | Projectile")
    TObjectPtr<USphereComponent> ColSphereSample;						// SphereComponent

    UPROPERTY(VisibleAnywhere, Category = "ASample_Projectile | Movement")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementSample;	// ProjectileMovementComponent

    UPROPERTY(VisibleDefaultsOnly, Category = "ASample_Projectile | Projectile")
    TObjectPtr<UStaticMeshComponent> ProjectileMeshSample;				// StaticMeshComponent

	UPROPERTY(EditAnywhere, Category = "ASample_Projectile | Movement")
	TObjectPtr<UMaterial> ProjectileMaterial;							// Material For Projectile

	TObjectPtr<UMaterialInstanceDynamic> m_pProjectileMID;				// 発射物用MID

	// 関数宣言
    void FireInDirection(const FVector& ShootDirection) const;	// 発射物の発射方向の速度を初期化する関数。

	//===== OnHit Event =====

public:

	// 関数宣言
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};

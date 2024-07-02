
// インクルード部
#include <Sample_Projectile.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// クラス実装
ASample_Projectile::ASample_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// USceneComponent コンポーネント生成
	TObjectPtr<USceneComponent> pScene = CreateDefaultSubobject<USceneComponent>(L"RootScene");
	check(pScene);
	RootComponent = pScene;

	// USphereComponent コンポーネント生成
	ColSphereSample = CreateDefaultSubobject<USphereComponent>(L"SphereComponent");
	check(ColSphereSample);
	ColSphereSample->SetupAttachment(RootComponent);

	// USphereComponent 初期化
    ColSphereSample->BodyInstance.SetCollisionProfileName(L"BlockAllDynamic");
    ColSphereSample->OnComponentHit.AddDynamic(this, &ASample_Projectile::OnHit);

	// 球体のコリジョン半径を設定します。
	ColSphereSample->InitSphereRadius(15.0f);	// 半径を設定

	// UProjectileMovementComponent コンポーネント生成
	ProjectileMovementSample = CreateDefaultSubobject<UProjectileMovementComponent>(L"ProjectileMovementComponent");
	check(ProjectileMovementSample);

	// ProjectileMovementComponent初期化
	ProjectileMovementSample->SetUpdatedComponent(ColSphereSample);
	ProjectileMovementSample->InitialSpeed = 3000.0f;
	ProjectileMovementSample->MaxSpeed = 3000.0f;
	ProjectileMovementSample->bRotationFollowsVelocity = true;
	ProjectileMovementSample->bShouldBounce = true;
	ProjectileMovementSample->Bounciness = 0.3f;
	ProjectileMovementSample->ProjectileGravityScale = 0.0f;

	// UStaticMeshComponent コンポーネント生成
	ProjectileMeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"ProjectileMeshComponent");
	check(ProjectileMeshSample);
	ProjectileMeshSample->SetupAttachment(CastChecked<USceneComponent>(ColSphereSample));

	// ProjectileMeshSample初期化
	ProjectileMeshSample->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));

	// Actor Param 初期化
    InitialLifeSpan = 3.0f;
}

void ASample_Projectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASample_Projectile::BeginPlay()
{
	Super::BeginPlay();

	// 発射物用MIDの初期化
	if (ProjectileMaterial)
	{
		m_pProjectileMID = UMaterialInstanceDynamic::Create(ProjectileMaterial, ProjectileMeshSample);
		if (m_pProjectileMID)
		{
			ProjectileMeshSample->SetMaterial(0, m_pProjectileMID);
		}
	}
}

void ASample_Projectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASample_Projectile::FireInDirection(const FVector& ShootDirection) const
{
    ProjectileMovementSample->Velocity = ShootDirection * ProjectileMovementSample->InitialSpeed;
}

void ASample_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        OtherComponent->AddImpulseAtLocation(ProjectileMovementSample->Velocity * 100.0f, Hit.ImpactPoint);
    }

	// アクタを破棄
    //Destroy();
}

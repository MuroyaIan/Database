
// �C���N���[�h��
#include <Sample_Projectile.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// �N���X����
ASample_Projectile::ASample_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// USceneComponent �R���|�[�l���g����
	TObjectPtr<USceneComponent> pScene = CreateDefaultSubobject<USceneComponent>(L"RootScene");
	check(pScene);
	RootComponent = pScene;

	// USphereComponent �R���|�[�l���g����
	ColSphereSample = CreateDefaultSubobject<USphereComponent>(L"SphereComponent");
	check(ColSphereSample);
	ColSphereSample->SetupAttachment(RootComponent);

	// USphereComponent ������
    ColSphereSample->BodyInstance.SetCollisionProfileName(L"BlockAllDynamic");
    ColSphereSample->OnComponentHit.AddDynamic(this, &ASample_Projectile::OnHit);

	// ���̂̃R���W�������a��ݒ肵�܂��B
	ColSphereSample->InitSphereRadius(15.0f);	// ���a��ݒ�

	// UProjectileMovementComponent �R���|�[�l���g����
	ProjectileMovementSample = CreateDefaultSubobject<UProjectileMovementComponent>(L"ProjectileMovementComponent");
	check(ProjectileMovementSample);

	// ProjectileMovementComponent������
	ProjectileMovementSample->SetUpdatedComponent(ColSphereSample);
	ProjectileMovementSample->InitialSpeed = 3000.0f;
	ProjectileMovementSample->MaxSpeed = 3000.0f;
	ProjectileMovementSample->bRotationFollowsVelocity = true;
	ProjectileMovementSample->bShouldBounce = true;
	ProjectileMovementSample->Bounciness = 0.3f;
	ProjectileMovementSample->ProjectileGravityScale = 0.0f;

	// UStaticMeshComponent �R���|�[�l���g����
	ProjectileMeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"ProjectileMeshComponent");
	check(ProjectileMeshSample);
	ProjectileMeshSample->SetupAttachment(CastChecked<USceneComponent>(ColSphereSample));

	// ProjectileMeshSample������
	ProjectileMeshSample->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));

	// Actor Param ������
    InitialLifeSpan = 3.0f;
}

void ASample_Projectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASample_Projectile::BeginPlay()
{
	Super::BeginPlay();

	// ���˕��pMID�̏�����
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

	// �A�N�^��j��
    //Destroy();
}

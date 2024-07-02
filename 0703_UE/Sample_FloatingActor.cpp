
// �C���N���[�h��
#include <Sample_FloatingActor.h>
#include <Components/StaticMeshComponent.h>	// UStaticMeshComponent
//#include <UObject/ConstructorHelpers.h>		// �A�Z�b�g�w��

// �N���X����
ASample_FloatingActor::ASample_FloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// �R���|�[�l���g����
	MeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"Mesh");
	check(MeshSample);
	MeshSample->SetupAttachment(RootComponent);

	// SM�w��
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(L"/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube");
	//if (Asset.Succeeded())
	//{
	//	MeshSample->SetStaticMesh(Asset.Object);
	//	MeshSample->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//}
}

void ASample_FloatingActor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �㉺����]����
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	const float RunningTime = GetGameTimeSinceCreation();	// ���s����

	const float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatSpeed;				// ������ 80 �{�Ɋg�債�܂�
	const float DeltaRotation = DeltaTime * RotationSpeed;
	NewRotation.Yaw += DeltaRotation;						// ���b 40 �x��]�����܂�

	SetActorLocationAndRotation(NewLocation, NewRotation);	// ���X�V
}

void ASample_FloatingActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASample_FloatingActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

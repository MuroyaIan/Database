
// インクルード部
#include <Sample_FloatingActor.h>
#include <Components/StaticMeshComponent.h>	// UStaticMeshComponent
//#include <UObject/ConstructorHelpers.h>		// アセット指定

// クラス実装
ASample_FloatingActor::ASample_FloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// コンポーネント生成
	MeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"Mesh");
	check(MeshSample);
	MeshSample->SetupAttachment(RootComponent);

	// SM指定
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

	// 上下＆回転処理
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	const float RunningTime = GetGameTimeSinceCreation();	// 実行時間

	const float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatSpeed;				// 高さを 80 倍に拡大します
	const float DeltaRotation = DeltaTime * RotationSpeed;
	NewRotation.Yaw += DeltaRotation;						// 毎秒 40 度回転させます

	SetActorLocationAndRotation(NewLocation, NewRotation);	// 情報更新
}

void ASample_FloatingActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASample_FloatingActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

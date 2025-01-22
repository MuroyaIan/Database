
// include
#include <PRJ_NiagaraDI/DT/GgDataTableTest.h>
//#include <Components/SkeletalMeshComponent.h>


//===== Base =====

AGgDataTableTest::AGgDataTableTest()
{
	PrimaryActorTick.bCanEverTick = true;

    // Component Test

	SceneRoot = CreateDefaultSubobject<USceneComponent>(L"SceneRoot");
	RootComponent = SceneRoot;

	SkeletalMeshComponent1 = CreateDefaultSubobject<USkeletalMeshComponent>(L"SkeletalMeshComponent1");
	SkeletalMeshComponent1->SetupAttachment(SceneRoot);

}

void AGgDataTableTest::Tick(const float DeltaTime)
{
	// BP Tick
	Super::Tick(DeltaTime);
}

void AGgDataTableTest::BeginPlay()
{
	// BP BeginPlay
	Super::BeginPlay();

	// Debug Data Table Ptr
	if (IsValid(DT_DebrisAbility2))
    {
        // データテーブルから全データを取得する.
        TArray<FName> RowNames = DT_DebrisAbility2->GetRowNames();
        for (const auto& Name : RowNames)
        {
            const FGgDebrisAbility* DataPtr = DT_DebrisAbility2->FindRow<FGgDebrisAbility>(Name, FString());
            if (DataPtr == nullptr)
            {
                continue;
            }

            UE_LOG(LogTemp, Warning, TEXT("[ %s : Num( %d )  FX( %s )"), *Name.ToString(), DataPtr->DebrisNumber, *DataPtr->DebrisEffect.GetName());
        }
    }

	// Debug Data Table Soft Ptr
    const UDataTable* Ptr = DT_DebrisAbility3.Get();
    if (!IsValid(Ptr))
    {
        Ptr = DT_DebrisAbility3.LoadSynchronous();
    }

	if (IsValid(Ptr))
    {
        // データテーブルから全データを取得する.
        TArray<FName> RowNames = Ptr->GetRowNames();
        for (const auto& Name : RowNames)
        {
            const FGgDebrisAbility* DataPtr = Ptr->FindRow<FGgDebrisAbility>(Name, FString());
            if (DataPtr == nullptr)
            {
                continue;
            }

            UE_LOG(LogTemp, Error, TEXT("[ %s : Num( %d )  FX( %s )"), *Name.ToString(), DataPtr->DebrisNumber, *DataPtr->DebrisEffect.GetName());
        }
    }
}

void AGgDataTableTest::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// BP EndPlay
	Super::EndPlay(EndPlayReason);
}

void AGgDataTableTest::AddSkeletalMeshComponent()
{
    // method 1
	//SkeletalMeshComponent2 = CreateDefaultSubobject<USkeletalMeshComponent>(L"SkeletalMeshComponent2");
	//SkeletalMeshComponent2->SetupAttachment(SceneRoot);

	// method 2
    const FTransform Transform = FTransform();
	UActorComponent* ActorComponent = AddComponentByClass(SkeletalMeshComponentTemplate, false, Transform, false);
    SkeletalMeshComponent2 = CastChecked<USkeletalMeshComponent>(ActorComponent);

    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
    SkeletalMeshComponent2->AttachToComponent(SceneRoot, AttachmentTransformRules);

    SkeletalMeshComponent2->SetSkeletalMeshAsset(SkeletalMeshTemplate);
    const FVector Location = FVector(500.0, 0.0, 0.0);
    SkeletalMeshComponent2->AddLocalOffset(Location);
}

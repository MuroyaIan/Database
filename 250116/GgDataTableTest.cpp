
// include
#include <PRJ_NiagaraDI/DT/GgDataTableTest.h>


//===== Base =====

AGgDataTableTest::AGgDataTableTest()
{
	PrimaryActorTick.bCanEverTick = true;
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

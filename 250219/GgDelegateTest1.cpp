// Fill out your copyright notice in the Description page of Project Settings.

#include <PRJ_NiagaraDI/Delegate/GgDelegateTest1.h>


// Sets default values
AGgDelegateTest1::AGgDelegateTest1()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGgDelegateTest1::BeginPlay()
{
	DelegateTest1.BindLambda([]
	{
	    UE_LOG(LogTemp, Error, L"call delegate test 1. (lamda)");
	});
	bool Result = DelegateTest1.ExecuteIfBound();

	DelegateTest2.BindUObject(this, &AGgDelegateTest1::DebugDelegateTest2);
	int32 a = 111;
	float b = 222.2f;
	Result = DelegateTest2.ExecuteIfBound(a, b);

	Super::BeginPlay();
}

// Called every frame
void AGgDelegateTest1::Tick(const float DeltaTime)
{
	static int32 Count = 0;
	DelegateTest3.ExecuteIfBound(Count);
	Count++;

	static float Count2 = 0;
	DelegateTest4.ExecuteIfBound(Count2);
	Count2 += 1.0f;

	Super::Tick(DeltaTime);
}


//===== Delegate Test =====

AGgDelegateTest1::FDelegateTest4& AGgDelegateTest1::GetDelegateTest4()
{
	return DelegateTest4;
}

void AGgDelegateTest1::DebugDelegateTest2(int32& a, float& b)
{
	UE_LOG(LogTemp, Error, L"call delegate test 2 : a(%d) b(%f). (func)", a, b);
}

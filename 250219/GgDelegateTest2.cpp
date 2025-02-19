// Fill out your copyright notice in the Description page of Project Settings.

#include <PRJ_NiagaraDI/Delegate/GgDelegateTest2.h>
#include <PRJ_NiagaraDI/Delegate/GgDelegateTest1.h>

// Sets default values
AGgDelegateTest2::AGgDelegateTest2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGgDelegateTest2::BeginPlay()
{
	GgDelegateTest->DelegateTest3.BindUObject(this, &AGgDelegateTest2::DebugDelegateTest3);

	AGgDelegateTest1::FDelegateTest4& DelegateTest4 = GgDelegateTest->GetDelegateTest4();
	DelegateTest4.BindUObject(this, &AGgDelegateTest2::DebugDelegateTest4);

	Super::BeginPlay();
}

// Called every frame
void AGgDelegateTest2::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}


//===== Delegate Test =====

void AGgDelegateTest2::DebugDelegateTest3(int32& a)
{
	UE_LOG(LogTemp, Error, L"call delegate test 3 : c(%d). (object)", a);
}

void AGgDelegateTest2::DebugDelegateTest4(float& a)
{
	UE_LOG(LogTemp, Warning, L"call delegate test 3 : c(%f). (object)", a);
}

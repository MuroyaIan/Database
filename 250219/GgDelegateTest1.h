// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GgDelegateTest1.generated.h"

UCLASS()
class PRJ_NIAGARADI_API AGgDelegateTest1 : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AGgDelegateTest1();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//===== Delegate Test =====

public:

	DECLARE_DELEGATE_OneParam(FDelegateTest3, int32&);
	FDelegateTest3 DelegateTest3;

	DECLARE_DELEGATE_OneParam(FDelegateTest4, float&);
	FDelegateTest4& GetDelegateTest4();

private:

	DECLARE_DELEGATE(FDelegateTest1);
	FDelegateTest1 DelegateTest1;

	DECLARE_DELEGATE_TwoParams(FDelegateTest2, int32&, float&);
	FDelegateTest2 DelegateTest2;

	FDelegateTest4 DelegateTest4;

	void DebugDelegateTest2(int32& a, float& b);
};

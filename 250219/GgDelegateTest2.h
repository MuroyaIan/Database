// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GgDelegateTest2.generated.h"

class AGgDelegateTest1;

UCLASS()
class PRJ_NIAGARADI_API AGgDelegateTest2 : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AGgDelegateTest2();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//===== Delegate Test =====

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<AGgDelegateTest1> GgDelegateTest;

	void DebugDelegateTest3(int32& a);
	void DebugDelegateTest4(float& a);
};

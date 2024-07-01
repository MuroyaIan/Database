
#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>

#include <Sample_Character.generated.h>

// 前方宣言
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTC_5_4_API ASample_Character : public ACharacter
{
	GENERATED_BODY()

public:

	// コンストラクタ
	ASample_Character();

	// 関数宣言
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// 関数宣言
	virtual void BeginPlay() override;
	//void EventIA_Action(const FInputActionValue& Value);	//IA_SampleAction
	//void EventIA_Axis(const FInputActionValue& Value);		//IA_SampleAxis

	// 変数宣言
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> m_SampleIA_Action;		//IA_SampleAction

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> m_SampleIA_Axis;		//IA_SampleAxis

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> m_SampleIMC;	//IMC_Sample

private:

	// 関数宣言
	void InitInput();									// Input初期化
	void PressedAction();								// EventHandle(KeyQ Pressed)
	void ReleasedAction();								// EventHandle(KeyQ Released)
	void PressedAxis(const FInputActionValue& Value);	// EventHandle(Axis)

    UFUNCTION()
    void MoveForward(float Value);	//前後の移動

    UFUNCTION()
    void MoveRight(float Value);	//左右の移動

	// 変数宣言
	bool m_IsPressed_KeyQ = false;	// Qが押された
	FVector2D m_Move2D;				// 移動量
};

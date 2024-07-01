
#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>

#include <Sample_Character.generated.h>

// �O���錾
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTC_5_4_API ASample_Character : public ACharacter
{
	GENERATED_BODY()

public:

	// �R���X�g���N�^
	ASample_Character();

	// �֐��錾
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// �֐��錾
	virtual void BeginPlay() override;
	//void EventIA_Action(const FInputActionValue& Value);	//IA_SampleAction
	//void EventIA_Axis(const FInputActionValue& Value);		//IA_SampleAxis

	// �ϐ��錾
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> m_SampleIA_Action;		//IA_SampleAction

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> m_SampleIA_Axis;		//IA_SampleAxis

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> m_SampleIMC;	//IMC_Sample

private:

	// �֐��錾
	void InitInput();									// Input������
	void PressedAction();								// EventHandle(KeyQ Pressed)
	void ReleasedAction();								// EventHandle(KeyQ Released)
	void PressedAxis(const FInputActionValue& Value);	// EventHandle(Axis)

    UFUNCTION()
    void MoveForward(float Value);	//�O��̈ړ�

    UFUNCTION()
    void MoveRight(float Value);	//���E�̈ړ�

	// �ϐ��錾
	bool m_IsPressed_KeyQ = false;	// Q�������ꂽ
	FVector2D m_Move2D;				// �ړ���
};

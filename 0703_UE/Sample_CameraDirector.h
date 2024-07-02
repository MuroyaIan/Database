
// �C���N���[�h��
#pragma once
#include <GameFramework/Actor.h>

// �C���N���[�h�I��
#include <Sample_CameraDirector.generated.h>

// �N���X�錾
UCLASS()
class PROJECTC_5_4_API ASample_CameraDirector : public AActor
{
	GENERATED_BODY()

public:

	// �֐��錾
	ASample_CameraDirector();                    // �R���X�g���N�^
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// �֐��錾
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== �J���������� =====

public:

	//�ϐ��錾
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> CameraOne;	// Camera 1

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> CameraTwo;	// Camera 2

private:

	//�ϐ��錾
	float CameraTimer = 0.0f;		// �J�����J�ڗp�^�C�}
	float CameraStayTime = 2.0f;	// �J�����̈ێ�����
	float CameraBlendTime = 0.75f;	// �J�����̑J�ڎ���
};


// �C���N���[�h��
#include <Sample_Character.h>
//#include <Components/InputComponent.h>	// Actor����

//#include <Kismet/GameplayStatics.h>		// ���L1�����ς�
#include <Kismet/KismetSystemLibrary.h>

#include <InputMappingContext.h>			// ���L2�����ς�
//#include <InputAction.h>
//#include <InputActionValue.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include <Camera/CameraComponent.h>			// �J����
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>

#include <Sample_Projectile.h>				// ���˕�

// �N���X����
ASample_Character::ASample_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// Input Action��Ǎ�
	JumpAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Jump", nullptr, LOAD_None, nullptr);
	MoveAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Move", nullptr, LOAD_None, nullptr);
	LookAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Look", nullptr, LOAD_None, nullptr);
	FireAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Fire", nullptr, LOAD_None, nullptr);

	// Input Mapping Context��Ǎ�
	SampleImc = LoadObject<UInputMappingContext>(nullptr, L"/Game/PRJ/Input/IMC_Player", nullptr, LOAD_None, nullptr);

	// UCameraComponent �R���|�[�l���g����
	CameraSample = CreateDefaultSubobject<UCameraComponent>(L"CameraSample");
	check(CameraSample != nullptr);
	CameraSample->SetupAttachment(CastChecked<USceneComponent>(GetCapsuleComponent()));

	// �J����������
	CameraSample->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));	// �J������ڂ̏������
	CameraSample->bUsePawnControlRotation = true;												// �|�[�����J�����̉�]�𐧌�

	// USkeletalMeshComponent �R���|�[�l���g����
    MeshArm = CreateDefaultSubobject<USkeletalMeshComponent>(L"FirstPersonMesh");
    check(MeshArm != nullptr);
	MeshArm->SetupAttachment(CameraSample);

	// �r���b�V��������
    MeshArm->SetOnlyOwnerSee(true);			// ���L�v���C���[�̂݌�����
	MeshArm->bCastDynamicShadow = false;	// �P��̃��b�V�������邩�̂悤�ȍ��o���ێ�����ɂ́A�ꕔ�̊��V���h�E�𖳌��ɂ���
    MeshArm->CastShadow = false;

	// SKM��\��
    GetMesh()->SetOwnerNoSee(true);
}

void ASample_Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �ړ��ʃf�o�b�O
	UKismetSystemLibrary::PrintString(this, FString::Printf(L"Move : X(%.1f) Y(%.1f)", m_Move2D.X, m_Move2D.Y), true, true, FColor::Cyan, 0.0f, L"None");
}

// Called to bind functionality to input
void ASample_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASample_Character::BeginPlay()
{
	Super::BeginPlay();

	// �o�̓e�X�g
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, L"We are using Sample_Character.");

	// Input������
	InitInput();
}

void ASample_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//===== EnhancedInputComponent������ =====

void ASample_Character::InitInput()
{
	// PlayerController���擾
	//const TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const TObjectPtr<APlayerController> pPlayerController = CastChecked<APlayerController>(GetController());
	if (!pPlayerController)
	{
		return;
	}

	// ���͗L����
	EnableInput(pPlayerController);

	if (InputComponent)
	{
		// ���̓C�x���g��o�^
		if (const TObjectPtr<UEnhancedInputComponent> pEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			// Input Action
			pEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASample_Character::JumpAction_Started);
			pEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASample_Character::JumpAction_Completed);
			pEnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASample_Character::FireAction_Started);

			// Input Axis
			pEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASample_Character::MoveAction_Triggered);
			pEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASample_Character::LookAction_Triggered);
		}

		// Input Mapping Context��o�^
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> pSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pPlayerController->GetLocalPlayer()))
		{
			pSubsystem->AddMappingContext(SampleImc, 0);
		}
	}

	// �p�����[�^������
	m_Move2D.X = 0.0f;
	m_Move2D.Y = 0.0f;
	m_Look2D.X = 0.0f;
	m_Look2D.Y = 0.0f;
}

void ASample_Character::JumpAction_Started()
{
	bPressedJump = true;
	UKismetSystemLibrary::PrintString(this, L"JumpAction Started", true, true, FColor::Cyan, 2.0f, L"None");
}

void ASample_Character::JumpAction_Completed()
{
	bPressedJump = false;
	UKismetSystemLibrary::PrintString(this, L"JumpAction Completed", true, true, FColor::Cyan, 2.0f, L"None");
}

void ASample_Character::MoveAction_Triggered(const FInputActionValue& Value)
{
	// �������擾
	m_Move2D = Value.Get<FVector2D>();

	// �ړ��ʍX�V
	MoveForward(m_Move2D.X);
	MoveRight(m_Move2D.Y);
}

void ASample_Character::LookAction_Triggered(const FInputActionValue& Value)
{
	// �������擾
	m_Look2D = Value.Get<FVector2D>();

	// �ړ��ʍX�V
	AddControllerPitchInput(m_Look2D.Y);
	AddControllerYawInput(m_Look2D.X);
}

void ASample_Character::FireAction_Started()
{
	// ���˕��𔭎�
	if (ClassProjectile)
	{
	    // �J�����̃g�����X�t�H�[�����擾
	    FVector CameraLocation;
	    FRotator CameraRotation;
	    GetActorEyesViewPoint(CameraLocation, CameraRotation);

	    // MuzzleOffset ���킸���ɃJ�������ʂɑ΂��Ĕ��˕����X�|�[������悤�ɐݒ�
	    MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	    // MuzzleOffset ���J������Ԃ��烏�[���h��ԂɃg�����X�t�H�[�����܂��B
		const FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	    // �Ə����킸���ɏ���ɘc�߂܂��B
	    FRotator MuzzleRotation = CameraRotation;
	    MuzzleRotation.Pitch += 10.0f;

		if (const TObjectPtr<UWorld> pWorld = GetWorld())
	    {
	        FActorSpawnParameters SpawnParams;
	        SpawnParams.Owner = this;
	        SpawnParams.Instigator = GetInstigator();

	        // ���˕����e���ɃX�|�[�����܂��B
			if (const TObjectPtr<ASample_Projectile> pProjectile = pWorld->SpawnActor<ASample_Projectile>(ClassProjectile, MuzzleLocation, MuzzleRotation, SpawnParams))
	        {
	            // ���˕��̏����O����ݒ肵�܂��B
				const FVector LaunchDirection = MuzzleRotation.Vector();
	            pProjectile->FireInDirection(LaunchDirection);
	        }
	    }
	}

	UKismetSystemLibrary::PrintString(this, L"FireAction Started", true, true, FColor::Cyan, 2.0f, L"None");
}

void ASample_Character::MoveForward(const float Value)
{
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ASample_Character::MoveRight(const float Value)
{
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

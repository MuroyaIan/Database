
#include <Sample_Character.h>
//#include <Components/InputComponent.h>	// Actor����

//#include <Kismet/GameplayStatics.h>	// ���L1�����ς�
#include <Kismet/KismetSystemLibrary.h>

#include <InputMappingContext.h>	// ���L2�����ς�
//#include <InputAction.h>
//#include <InputActionValue.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

ASample_Character::ASample_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// Input Action��Ǎ�
	m_SampleIA_Action = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Action", nullptr, LOAD_None, nullptr);
	m_SampleIA_Axis = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Axis", nullptr, LOAD_None, nullptr);

	// Input Mapping Context��Ǎ�
	m_SampleIMC = LoadObject<UInputMappingContext>(nullptr, L"/Game/PRJ/Input/IMC_Sample", nullptr, LOAD_None, nullptr);
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

void ASample_Character::InitInput()
{
	// PlayerController���擾
	//const TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const TObjectPtr<APlayerController> pPlayerController = CastChecked<APlayerController>(GetController());
	if (!pPlayerController)
		return;

	// ���͗L����
	EnableInput(pPlayerController);

	if (InputComponent)
	{
		// ���̓C�x���g��o�^
		if (const TObjectPtr<UEnhancedInputComponent> pEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

			// Input Action
			pEnhancedInputComponent->BindAction(m_SampleIA_Action, ETriggerEvent::Triggered, this, &ASample_Character::PressedAction);
			pEnhancedInputComponent->BindAction(m_SampleIA_Action, ETriggerEvent::Completed, this, &ASample_Character::ReleasedAction);

			// Input Axis
			pEnhancedInputComponent->BindAction(m_SampleIA_Axis, ETriggerEvent::Triggered, this, &ASample_Character::PressedAxis);
		}

		// Input Mapping Context��o�^
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> pSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pPlayerController->GetLocalPlayer()))
			pSubsystem->AddMappingContext(m_SampleIMC, 0);
	}

	// �p�����[�^������
	m_IsPressed_KeyQ = false;
	m_Move2D.X = 0.0f;
	m_Move2D.Y = 0.0f;
}

void ASample_Character::PressedAction()
{
	if (!m_IsPressed_KeyQ)
	{
		// Pressed
		UKismetSystemLibrary::PrintString(this, L"Pressed", true, true, FColor::Cyan, 2.0f, L"None");

		// Press��Ԃɐݒ�
		m_IsPressed_KeyQ = true;
	}
}

void ASample_Character::ReleasedAction()
{
	// Released
	UKismetSystemLibrary::PrintString(this, L"Released", true, true, FColor::Cyan, 2.0f, L"None");

	// Press��Ԃ�����
	m_IsPressed_KeyQ = false;
}

void ASample_Character::PressedAxis(const FInputActionValue& Value)
{
	// �������擾
	m_Move2D = Value.Get<FVector2D>();

	// �ړ��ʍX�V
	MoveForward(m_Move2D.X);
	MoveRight(m_Move2D.Y);
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

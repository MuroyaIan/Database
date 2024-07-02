
// インクルード部
#include <Sample_Character.h>
//#include <Components/InputComponent.h>	// Actor内包

//#include <Kismet/GameplayStatics.h>		// 下記1を内包済み
#include <Kismet/KismetSystemLibrary.h>

#include <InputMappingContext.h>			// 下記2を内包済み
//#include <InputAction.h>
//#include <InputActionValue.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include <Camera/CameraComponent.h>			// カメラ
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>

#include <Sample_Projectile.h>				// 発射物

// クラス実装
ASample_Character::ASample_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// Input Actionを読込
	JumpAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Jump", nullptr, LOAD_None, nullptr);
	MoveAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Move", nullptr, LOAD_None, nullptr);
	LookAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Look", nullptr, LOAD_None, nullptr);
	FireAction = LoadObject<UInputAction>(nullptr, L"/Game/PRJ/Input/Actions/IA_Fire", nullptr, LOAD_None, nullptr);

	// Input Mapping Contextを読込
	SampleImc = LoadObject<UInputMappingContext>(nullptr, L"/Game/PRJ/Input/IMC_Player", nullptr, LOAD_None, nullptr);

	// UCameraComponent コンポーネント生成
	CameraSample = CreateDefaultSubobject<UCameraComponent>(L"CameraSample");
	check(CameraSample != nullptr);
	CameraSample->SetupAttachment(CastChecked<USceneComponent>(GetCapsuleComponent()));

	// カメラ初期化
	CameraSample->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));	// カメラを目の少し上に
	CameraSample->bUsePawnControlRotation = true;												// ポーンがカメラの回転を制御

	// USkeletalMeshComponent コンポーネント生成
    MeshArm = CreateDefaultSubobject<USkeletalMeshComponent>(L"FirstPersonMesh");
    check(MeshArm != nullptr);
	MeshArm->SetupAttachment(CameraSample);

	// 腕メッシュ初期化
    MeshArm->SetOnlyOwnerSee(true);			// 所有プレイヤーのみ見える
	MeshArm->bCastDynamicShadow = false;	// 単一のメッシュがあるかのような錯覚を維持するには、一部の環境シャドウを無効にする
    MeshArm->CastShadow = false;

	// SKM非表示
    GetMesh()->SetOwnerNoSee(true);
}

void ASample_Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移動量デバッグ
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

	// 出力テスト
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, L"We are using Sample_Character.");

	// Input初期化
	InitInput();
}

void ASample_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//===== EnhancedInputComponent仮実装 =====

void ASample_Character::InitInput()
{
	// PlayerControllerを取得
	//const TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const TObjectPtr<APlayerController> pPlayerController = CastChecked<APlayerController>(GetController());
	if (!pPlayerController)
	{
		return;
	}

	// 入力有効化
	EnableInput(pPlayerController);

	if (InputComponent)
	{
		// 入力イベントを登録
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

		// Input Mapping Contextを登録
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> pSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pPlayerController->GetLocalPlayer()))
		{
			pSubsystem->AddMappingContext(SampleImc, 0);
		}
	}

	// パラメータ初期化
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
	// 軸情報を取得
	m_Move2D = Value.Get<FVector2D>();

	// 移動量更新
	MoveForward(m_Move2D.X);
	MoveRight(m_Move2D.Y);
}

void ASample_Character::LookAction_Triggered(const FInputActionValue& Value)
{
	// 軸情報を取得
	m_Look2D = Value.Get<FVector2D>();

	// 移動量更新
	AddControllerPitchInput(m_Look2D.Y);
	AddControllerYawInput(m_Look2D.X);
}

void ASample_Character::FireAction_Started()
{
	// 発射物を発射
	if (ClassProjectile)
	{
	    // カメラのトランスフォームを取得
	    FVector CameraLocation;
	    FRotator CameraRotation;
	    GetActorEyesViewPoint(CameraLocation, CameraRotation);

	    // MuzzleOffset がわずかにカメラ正面に対して発射物をスポーンするように設定
	    MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	    // MuzzleOffset をカメラ空間からワールド空間にトランスフォームします。
		const FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	    // 照準をわずかに上方に歪めます。
	    FRotator MuzzleRotation = CameraRotation;
	    MuzzleRotation.Pitch += 10.0f;

		if (const TObjectPtr<UWorld> pWorld = GetWorld())
	    {
	        FActorSpawnParameters SpawnParams;
	        SpawnParams.Owner = this;
	        SpawnParams.Instigator = GetInstigator();

	        // 発射物を銃口にスポーンします。
			if (const TObjectPtr<ASample_Projectile> pProjectile = pWorld->SpawnActor<ASample_Projectile>(ClassProjectile, MuzzleLocation, MuzzleRotation, SpawnParams))
	        {
	            // 発射物の初期軌道を設定します。
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

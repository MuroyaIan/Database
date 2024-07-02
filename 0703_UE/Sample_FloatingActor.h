
// インクルード部
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// インクルード終了
#include <Sample_FloatingActor.generated.h>

// 前方宣言
class UStaticMeshComponent;

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_FloatingActor : public AActor
{
	GENERATED_BODY()

public:

	// 関数宣言
	ASample_FloatingActor();                     // コンストラクタ
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// 関数宣言
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== SM仮実装 =====

public:

	//変数宣言
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshSample;	// SM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_FloatingActor")
	float FloatSpeed = 80.0f;						// Up Down Speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASample_FloatingActor")
	float RotationSpeed = 40.0f;					// Rotate Speed
};

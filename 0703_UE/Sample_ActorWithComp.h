
// インクルード部
#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

// インクルード終了
#include <Sample_ActorWithComp.generated.h>

// 前方宣言
class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UBoxComponent;

// クラス宣言
UCLASS()
class PROJECTC_5_4_API ASample_ActorWithComp : public AActor
{
	GENERATED_BODY()

public:

	// 関数宣言
	ASample_ActorWithComp();                     // コンストラクタ
	virtual void Tick(float DeltaTime) override; // Update

protected:

	// 関数宣言
	virtual void BeginPlay() override;                                       // Init
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // Uninit

	//===== コンポーネント仮実装 =====

public:

	//変数宣言
	TObjectPtr<USceneComponent> SceneSample;					// SceneComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UStaticMeshComponent> StaticMeshSample;			// StaticMeshComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UBoxComponent> ColBoxSample;						// BoxComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UParticleSystemComponent> ParticleSystemSample;	// ParticleSystemComponent

	UPROPERTY(EditAnywhere, Category = "ASample_ActorWithComp")
	TObjectPtr<UAudioComponent> AudioSample;					// AudioComponent
};

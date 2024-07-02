
// インクルード部
#include <Sample_ActorWithComp.h>
#include <Components/StaticMeshComponent.h>		// UStaticMeshComponent
#include <Components/BoxComponent.h>			// UBoxComponent
#include <Particles/ParticleSystemComponent.h>	// UParticleSystemComponent
#include <Components/AudioComponent.h>			// UBoxComponent

// クラス実装
ASample_ActorWithComp::ASample_ActorWithComp()
{
	PrimaryActorTick.bCanEverTick = true;

	// USceneComponent コンポーネント生成
	SceneSample = CreateDefaultSubobject<USceneComponent>(L"RootScene");
	check(SceneSample);
	RootComponent = SceneSample;

	// UStaticMeshComponent コンポーネント生成
	StaticMeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"MeshSample");
	check(StaticMeshSample);
	StaticMeshSample->SetupAttachment(SceneSample);

	if (TObjectPtr<USceneComponent> pSample = CastChecked<USceneComponent>(StaticMeshSample))
	{
		// UStaticMeshComponent コンポーネント生成
		ColBoxSample = CreateDefaultSubobject<UBoxComponent>(L"ColBoxSample");
		check(ColBoxSample);
		ColBoxSample->SetupAttachment(pSample);

		// UStaticMeshComponent コンポーネント生成
		ParticleSystemSample = CreateDefaultSubobject<UParticleSystemComponent>(L"EffectSample");
		check(ParticleSystemSample);
		ParticleSystemSample->SetupAttachment(pSample);

		// UStaticMeshComponent コンポーネント生成
		AudioSample = CreateDefaultSubobject<UAudioComponent>(L"AudioSample");
		check(AudioSample);
		AudioSample->SetupAttachment(pSample);
	}
}

void ASample_ActorWithComp::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASample_ActorWithComp::BeginPlay()
{
	Super::BeginPlay();
}

void ASample_ActorWithComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

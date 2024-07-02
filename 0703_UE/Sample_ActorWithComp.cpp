
// �C���N���[�h��
#include <Sample_ActorWithComp.h>
#include <Components/StaticMeshComponent.h>		// UStaticMeshComponent
#include <Components/BoxComponent.h>			// UBoxComponent
#include <Particles/ParticleSystemComponent.h>	// UParticleSystemComponent
#include <Components/AudioComponent.h>			// UBoxComponent

// �N���X����
ASample_ActorWithComp::ASample_ActorWithComp()
{
	PrimaryActorTick.bCanEverTick = true;

	// USceneComponent �R���|�[�l���g����
	SceneSample = CreateDefaultSubobject<USceneComponent>(L"RootScene");
	check(SceneSample);
	RootComponent = SceneSample;

	// UStaticMeshComponent �R���|�[�l���g����
	StaticMeshSample = CreateDefaultSubobject<UStaticMeshComponent>(L"MeshSample");
	check(StaticMeshSample);
	StaticMeshSample->SetupAttachment(SceneSample);

	if (TObjectPtr<USceneComponent> pSample = CastChecked<USceneComponent>(StaticMeshSample))
	{
		// UStaticMeshComponent �R���|�[�l���g����
		ColBoxSample = CreateDefaultSubobject<UBoxComponent>(L"ColBoxSample");
		check(ColBoxSample);
		ColBoxSample->SetupAttachment(pSample);

		// UStaticMeshComponent �R���|�[�l���g����
		ParticleSystemSample = CreateDefaultSubobject<UParticleSystemComponent>(L"EffectSample");
		check(ParticleSystemSample);
		ParticleSystemSample->SetupAttachment(pSample);

		// UStaticMeshComponent �R���|�[�l���g����
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

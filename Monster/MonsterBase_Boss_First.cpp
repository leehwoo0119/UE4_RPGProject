// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase_Boss_First.h"
#include "Particles/ParticleSystemComponent.h"

AMonsterBase_Boss_First::AMonsterBase_Boss_First()
{
	PrimaryActorTick.bCanEverTick = false;

	eyeParticle_r = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("eyeParticle_r"));
	eyeParticle_r->SetupAttachment(GetMesh(), FName("EyeParticleSocket_r"));

	eyeParticle_l = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("eyeParticle_l"));
	eyeParticle_l->SetupAttachment(GetMesh(), FName("EyeParticleSocket_l"));
}

void AMonsterBase_Boss_First::BeginPlay()
{
	Super::BeginPlay();

	eyeParticle_r->Deactivate();
	eyeParticle_l->Deactivate();
}

void AMonsterBase_Boss_First::SetEyeParticle(bool _bActive)
{
	if (!_bActive)
	{
		eyeParticle_r->Deactivate();
		eyeParticle_l->Deactivate();
	}
	else
	{
		eyeParticle_r->Activate();
		eyeParticle_l->Activate();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharWeaponSlot.h"
#include "MainChar.h"
#include "MainCharKatana.h"
#include "MainCharBow.h"
#include "MainCharArrow.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UMainCharWeaponSlot::UMainCharWeaponSlot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	katanaSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("katanaSocket"));
	katanaSocket->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	check(katanaSocket != nullptr);
}

// Called when the game starts
void UMainCharWeaponSlot::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(GetOwner());
	check(mainChar != nullptr);

	katanaSocket->AttachToComponent(
		mainChar->GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform, FName("ScabbardSocket"));

	check(mainCharKatanaBP != nullptr);
	check(mainCharBowBP != nullptr);
	check(mainCharArrowBP != nullptr);

	mainCharKatana = GetWorld()->SpawnActor<AMainCharKatana>(mainCharKatanaBP);	
	check(mainCharKatana != nullptr);
	mainCharBow = GetWorld()->SpawnActor<AMainCharBow>(mainCharBowBP);
	check(mainCharBow != nullptr);

	SetKatanaSocket("KatanaArmedSocket");
	SetBowSocket("BowUnarmedSocket");
}

void UMainCharWeaponSlot::SetKatanaSocket(FName _socketName)
{
	mainCharKatana->GetKatanaMesh()->AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(_socketName));
}

void UMainCharWeaponSlot::SetBowSocket(FName _socketName)
{
	mainCharBow->GetBowMesh()->AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(_socketName));
}

void UMainCharWeaponSlot::AttachArrowToHand()
{
	mainCharArrowToHand = GetWorld()->SpawnActor<AMainCharArrow>(mainCharArrowBP);
	check(mainCharArrowToHand != nullptr);
	mainCharArrowToHand->GetArrowMesh()->AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("ArrowSocket"));
}

void UMainCharWeaponSlot::DettachArrowToHand()
{
	if (mainCharArrowToHand && !mainCharArrowToHand->IsPendingKillPending())
	{
		mainCharArrowToHand->Destroy();
		mainCharArrowToHand = nullptr;
	}	
}

void UMainCharWeaponSlot::SpawnArrow(FTransform _transform)
{	
	check(mainCharArrowToHand != nullptr);
	mainCharArrowToHand->Destroy();
	mainCharArrowToHand = nullptr;
	mainCharArrow = GetWorld()->SpawnActor<AMainCharArrow>(mainCharArrowBP, _transform);
}


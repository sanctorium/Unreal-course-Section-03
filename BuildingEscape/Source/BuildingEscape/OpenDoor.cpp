// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	/* Getting an Actor in a Top-Down search, i.e. from the World to the Player Controler (the mind)
	to its associated Pawn (the body). Remember Pawn inherits from Actor, so its ok the below association:
	Give GetPawn() to AActor.

	In the code commented below we get the Actor Bottom-Up, i.e. from this Component to its associated
	actor -> GetOwner().
	*/

	/**** Udemy course:
	Owner = GetOwner();
	****/
	/* Removed in lecture 88, so now the doors will be open by any Actor
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	*/
	/* Getting the Z rotation of the Actor (StaticMeshActor in this case) to wich this Component
	   is associated.
	   And printing the information in the Output Log tab of the Unreal Editor.

	AActor* Owner{ GetOwner() };
	float Zrotation{ Owner->GetActorRotation().GetComponentForAxis(EAxis::Z) };
	UE_LOG(LogTemp, Warning, TEXT("The rotation for the Z axis is %f"), Zrotation);
	*/
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate (i.e. Trigger Volume) in the OpenDoor Component"), *Owner->GetName())
	}
}
/*
We are not longer openning/closing the door, i.e. setting it Z-rotation angle, in C++. Rather we now do it
in the Blueprint(ed) door class. We just generate the event (code above) and hence this code below it's
obsolete

void UOpenDoor::OpenDoor() {

	// With a Rotator. It's the same just choose one, this Rotator or the next Quaternion.
	FRotator Rotation1{ 0.f, OpenAngle, 0.f };
	Owner->SetActorRotation(Rotation1);

	// With a Quaternion.
	FRotator Rotation2{ 0.f, OpenAngle, 0.f };
	FQuat Quaternion{ Rotation2.Quaternion() };
	Owner->SetActorRotation(Quaternion);
	
	// These are absolute angle values. All "SetActorRotation()" does it's to set the values for
	// the X, Y, and Z fields of the "Rotation" subsection in the "Transform" section of the
	// "Details" window.
	
}

void UOpenDoor::CloseDoor() {

	Owner->SetActorRotation(FRotator{ 0.f, 180.f, 0.f });

}
*/
// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Polling the Trigger Volume every frame and if ActorThatOpen is in the volume then open the door(s).
	/* Removed in lecture 88, so now the doors will be open by any Actor
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
	*/
	if(GetTotalMassOfActorsOnPlate() > TriggerMass) {

		OnOpen.Broadcast();
	/* The openning/closing of the door is now do it in the Blueprint(ed) class.

		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		
	}
	if (GetWorld()->GetTimeSeconds() >= LastDoorOpenTime + DoorCloseDelay) {
		CloseDoor();
	*/
	} else {
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass{ 0.f };

	if (!PressurePlate) { return 0.f; }
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors) {
		
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Actor->GetName());

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
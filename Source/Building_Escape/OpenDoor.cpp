// Copyright MPC 2020

#include "OpenDoor.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
  Super::BeginPlay();

  InitialYaw = GetOwner()->GetActorRotation().Yaw;
  CurrentYaw = InitialYaw;
  OpenAngle = InitialYaw + OpenAngle;

  FindPressurePlate();
  FindAudioComponent();
}

void UOpenDoor::FindPressurePlate() const {
  if (!PressurePlate) {
    UE_LOG(
        LogTemp, Error,
        TEXT("%s has the OpenDoor component on it, but no PressurePlate set."),
        *GetOwner()->GetName());
  }
}

void UOpenDoor::FindAudioComponent() {
  AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
  if (!AudioComponent) {
    UE_LOG(LogTemp, Error, TEXT("%s missing Audio Component!"),
           *GetOwner()->GetName());
  }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  bool SoundHasPlay = false;
  if (TotalMassOfActors() > OUT MassToOpenDoor) {
    OpenDoor(DeltaTime);
    bOpenDoorSound = true;
    bCloseDoorSound = false;
    DoorLastOpened = GetWorld()->GetTimeSeconds();
  } else {
    if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {
      CloseDoor(DeltaTime);
      bOpenDoorSound = false;
      bCloseDoorSound = true;
    }
  }
}

void UOpenDoor::OpenDoor(float DeltaTime) {
  FRotator DoorRotation = GetOwner()->GetActorRotation();
  CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
  DoorRotation.Yaw = CurrentYaw;
  GetOwner()->SetActorRotation(DoorRotation);
  if (!AudioComponent) return;
  if (!bOpenDoorSound) AudioComponent->Play();
}

void UOpenDoor::CloseDoor(float DeltaTime) {
  FRotator DoorRotation = GetOwner()->GetActorRotation();
  CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
  DoorRotation.Yaw = CurrentYaw;
  GetOwner()->SetActorRotation(DoorRotation);
  if (!AudioComponent) return;
  if (!bCloseDoorSound) AudioComponent->Play();
}

float UOpenDoor::TotalMassOfActors() const {
  float TotalMass = 0.f;
  // get overlapping actors
  TArray<AActor*> OverlappingActors;
  if (!PressurePlate) return TotalMass;
  PressurePlate->GetOverlappingActors(OverlappingActors);

  // sum up their mass
  for (AActor* Actor : OverlappingActors) {
    TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
  }
  return TotalMass;
}

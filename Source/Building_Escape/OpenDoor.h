// Copyright MPC 2020

#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UOpenDoor();

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
  float TotalMassOfActors() const;
  void FindAudioComponent();
  void FindPressurePlate() const;

 private:
  float InitialYaw;
  float CurrentYaw;

  UPROPERTY(EditAnywhere)
  float OpenAngle = 90.f;

  float DoorLastOpened = 0.f;

  UPROPERTY(EditAnywhere)
  float DoorCloseDelay = 1.f;

  UPROPERTY(EditAnywhere)
  float DoorOpenSpeed = .8f;

  UPROPERTY(EditAnywhere)
  float DoorCloseSpeed = 2.f;

  UPROPERTY(EditAnywhere)
  ATriggerVolume* PressurePlate = nullptr;

  UPROPERTY(EditAnywhere)
  float MassToOpenDoor = 50.f;

  UPROPERTY()
  UAudioComponent* AudioComponent = nullptr;

  bool bOpenDoorSound = false;
  bool bCloseDoorSound = true;
};

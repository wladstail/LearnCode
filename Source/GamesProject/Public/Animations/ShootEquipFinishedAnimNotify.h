// Shoot Game

#pragma once

#include "CoreMinimal.h"
//#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animations/ShootAnimNotify.h"
#include "ShootEquipFinishedAnimNotify.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
//заменили класс на базовый
//class GAMESPROJECT_API UShootEquipFinishedAnimNotify : public UAnimNotify
class GAMESPROJECT_API UShootEquipFinishedAnimNotify : public UShootAnimNotify
{
    GENERATED_BODY()

//public:
//    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
//
//    FOnNotifiedSignature OnNotified;
};
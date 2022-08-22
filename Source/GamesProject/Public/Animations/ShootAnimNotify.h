// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShootAnimNotify.generated.h"

//делегат
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*); //опевещение только у меша который проигрывает анимацию

UCLASS()
class GAMESPROJECT_API UShootAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    ////функция которая показывает когда произошла анимация
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    //делегат
    FOnNotifiedSignature OnNotified;
};
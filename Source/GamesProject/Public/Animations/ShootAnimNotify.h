// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShootAnimNotify.generated.h"

//�������
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*); //���������� ������ � ���� ������� ����������� ��������

UCLASS()
class GAMESPROJECT_API UShootAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    ////������� ������� ���������� ����� ��������� ��������
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    //�������
    FOnNotifiedSignature OnNotified;
};
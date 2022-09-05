#pragma once

#include "ShootCoreTypes.generated.h"

// weapon
class AShootBaseWeapon;

//������� ������� ��������� ��� ������� �����������
//DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, AShootBaseWeapon*);  //���� ����, ����� ��������� ����� ������ �������, � ���������� �������� �������. �������� ��������� �� ������� ������. �� ���� ��� ������ Brodcast ��������, ����� ���������� ��������� �� ������, ������� ����������� �����������

//������ �������� ���������� ������
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

        //���������� ��������
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        int32 Bullets;

    //���������� ���������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))  //meta = (EditCondition = "!Infinite") �������� ����� Infinite true
        int32 Clips;

    //������� ������� ������ ��� ���
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        bool Infinite;
};

//������ ����� ������ � �������� �����������
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

        //����� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<AShootBaseWeapon> WeaponClass;

    //�������� �����������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        UAnimMontage* ReloadAnimMontage;
};

//������ ������� ������� ��� ������
USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    //��� �������� ������� ������ ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* MainIcon;  //UTexture2D - ��� ������ ��� �������. 

    //��� �������� ������ ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* CrossHairIcon;

};

// health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

//�������� ��������� �� - �� ����� ���� � ������� ������� � ��������
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);


//������
USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    //�������� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UMaterialInterface* Material;  //UMaterialInterface- ������� ����� ���������� �� ���������

    //������ ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FVector Size = FVector(10.0f); //�������� �� ���������. �������� ����� � ����� � ��������� ������ 10

    //����� ����� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float LifeTime = 5.0f;

    //����� �������� ����� ������ ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float FadeOutTime = 0.7f;  //���������� ������� ��� ������ ������������� � ��������� 

};

class UNiagaraSystem;    //���������� ��� �������

//���������� ������ � �������
USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    //��������� �� ������� �������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* NiagaraEffect;

    //���� ��������� �������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FDecalData DecalData;
};
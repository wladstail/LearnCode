#pragma once

#include "ShootCoreTypes.generated.h"

// weapon
class AShootBaseWeapon;

//делегат который оповещает что патроны закончились
//DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, AShootBaseWeapon*);  //фикс бага, когда подбираем пикап другим оружием, а предыдущее получает арсенал. Добавили указатель на базовое оружие. то есть при вызове Brodcast делегата, будет передавать указатель на оружие, которое запрашивает перезарядку

//хранит основную информацию оружия
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

        //количество патронов
        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        int32 Bullets;

    //количество магазинов
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))  //meta = (EditCondition = "!Infinite") доступен когда Infinite true
        int32 Clips;

    //конечен арсенал оружия или нет
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        bool Infinite;
};

//хранит класс оружия и анимацию перезарядки
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

        //класс оружия
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<AShootBaseWeapon> WeaponClass;

    //анимация перезарядки
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        UAnimMontage* ReloadAnimMontage;
};

//хранит виджеты прицела для оружий
USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    //Тут хранится главная иконка оружия
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* MainIcon;  //UTexture2D - тип данных для текстур. 

    //тут хранятся иконки прицелов
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* CrossHairIcon;

};

// health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

//изменили сигнатуру из - за фикса бага с красным экраном в виджетах
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);


//Декали
USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    //материал декали
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UMaterialInterface* Material;  //UMaterialInterface- базовый класс отвечающий за материалы

    //размер декали
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FVector Size = FVector(10.0f); //значение по умолчанию. Проекция будет в боксе с размерами сторон 10

    //время жизни декали
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float LifeTime = 5.0f;

    //время анимации альфа канала декали
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        float FadeOutTime = 0.7f;  //промежуток времени что декаль расстворяется и удаляется 

};

class UNiagaraSystem;    //подключили для ниагары

//Объединяет декали и ниагару
USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    //Указатель на ниагару систему
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* NiagaraEffect;

    //Наша структура декалей
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        FDecalData DecalData;
};
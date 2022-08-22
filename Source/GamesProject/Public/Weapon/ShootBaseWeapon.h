// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootCoreTypes.h"
#include "ShootBaseWeapon.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;
class UNiagaraSystem;		//создали для ниагары вспышек
class UNiagaraComponent;	//создали для ниагары вспышек

//USTRUCT(BlueprintType)
//struct FAmmoData
//{
//	GENERATED_USTRUCT_BODY()
//
//		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		int32 Bullets;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
//		int32 Clips;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		bool Infinite;
//};


UCLASS()
class GAMESPROJECT_API AShootBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AShootBaseWeapon();

	// оповещает что нужна перезарядка
	FOnClipEmptySignature OnClipEmpty;

	//Стрельба из оружия
	virtual void  StartFire();	//сделали вируатльной потому что будут наследники BaseWeapon в будущем и оружие стрелять будет по разному
	virtual void  StopFire();	//сделали вируатльной потому что будут наследники BaseWeapon в будущем и оружие стрелять будет по разному
	
	//логика замены текущей обоймы на новую
	void ChangeClip();
	bool CanReload() const;
		
	//гетер получения иконок прицела и оружия
	FWeaponUIData GetUIData() const { return UIData; }

	//создали для получения доступа к текущему арсеналу, для виджетов
	FAmmoData GetAmmoData() const { return CurrentAmmo; }

	//логика добавления магазинов и патронов
	bool TryToAddAmmo(int32 ClipsAmount);

protected:



	//меш оружия
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Compontnts")
	USkeletalMeshComponent* WeaponMesh;	//указатель на меш оружия


	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	//проперти сокета
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)*/
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;			//макс рассояние трейса, т.е. расстояние выстрела

	//перенесли в винтовку, т.к. тут не нужно
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//float DamageAmount = 10.0f;					//урон оружия

	//начальные параметры оружия
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo { 15, 10, false };

	/* перенесли
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)		//частота срабатывания при ЛКМ
	float TimeBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)		//отклонение при ЛКМ
	float BulletSpread = 1.5f;							//в градусах
	*/


	//структура иконок прицела
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")	//т.к. в секции protected создаем в паблик гетера 
	FWeaponUIData UIData;

	//устанавливается нигара вспышка дула
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* MuzzleFX;	//при спауне ниагары системы, указатель возвращается на UNiagaraComponent


	virtual void BeginPlay() override;

	//логика выстрела из оружия, сделали отдельно из-за того что выстрел будет по таймеру
	//виртуальной сделали из-за того что будем потом переопределять
	virtual void MakeShot();
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViemRotation) const;

	// bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const;	//сделали вирутальной
	FVector GetMuzzleWorldlocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd);
	/*void MakeDamage(const FHitResult& HitResult); перенесли в винтовку, т.е. тут она не нужна*/

	
	//перенесли
//private:
//	FTimerHandle ShotTimerHandle;

	//вызывается каждый раз после выстрела, уменьшает количество патронов
	void DecreaseAmmo();

	//возвращает истину когда арсенал полностью пустой(патронов и обойм нуль)
	bool IsAmmoEmpty() const;

	//возвращает истину когда текущая обойма пустая
	bool IsClipEmpty() const;
	
	//логика замены текущей обоймы на новую
	//void ChangeClip();
	
	//вывод инфы в консоль
	void LogAmmo();

	//для лучшей читаемости создали. Если текущ арсенал равен дефолтному тру
	bool IsAmmoFull() const;

	//функция спауна ниагары системы. Возвращает указатель на UNiagaraComponent
	UNiagaraComponent* SpawnMuzzleFX();
private:

	//текущий арсенал оружия
	FAmmoData CurrentAmmo;
};
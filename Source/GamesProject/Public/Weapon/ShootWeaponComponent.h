// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"
#include "ShootWeaponComponent.generated.h"

class AShootBaseWeapon;	//оружие класс

//USTRUCT(BlueprintType)
//struct FWeaponData
//{
//	GENERATED_USTRUCT_BODY()
//
//		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		TSubclassOf<AShootBaseWeapon> WeaponClass;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		UAnimMontage* ReloadAnimMontage;
//};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootWeaponComponent();


	void StartFire();	//стрельба
	void StopFire();

	//смена оружия
	void NextWeapon();

	//перезарядка
	void Reload();

	//функция обертка которая возвращает иконки оружия и прицела текущего оружия. Структуру передаем по ссылке в качестве параметра функции
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	//получение текущего арсенала
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;


	//добавляет арсенал персонажу от Puckups
	bool TryToAddAmmo(TSubclassOf<AShootBaseWeapon> WeaponType, int32 ClipsAmount);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		//TSubclassOf<AShootBaseWeapon> WeaponClass;							//выбор класса оружия
		/*TArray<TSubclassOf<AShootBaseWeapon>> WeaponClasses;*/				//выбор массив класса оружия
		TArray<FWeaponData> WeaponData;											//выбор массива структуры клааса оружия

	//сокет куда крепится оружие на спине
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		/*FName WeaponAttachPointName = "WeaponSocket";*/
		FName WeaponEquipSocketName = "WeaponSocket";

	//имя сокета к которому прикрепляем дополнительное оружие
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	//анимация смены оружия
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimMontage;

	//текущая анимация перезарядки оружия, меняет указатель на анимации при смене оружия
	UPROPERTY()
		UAnimMontage* CurrentReloadAnimMontage = nullptr;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

	//будет указатель на оружие изменяться при каждой смене оружия из массива оружия
	UPROPERTY()//пишем перед UOject, указателями всегда
		AShootBaseWeapon* CurrentWeapon = nullptr;	//переменная член класс, поэтому можно использовать в Fire()


	//void SpawnWeapons();	//спав оружия и присоединение сокета к мешу персонажа. Сокет это в анимации прикрепление оружия к кости

	//массив оружия, нужен для хранения указателей на оружие
	UPROPERTY()
		TArray<AShootBaseWeapon*> Weapons;	//указатель на базовое оружие

	//хранит индекс элемента массива оружия на который ссылается CurrentWeapon
	int32 CurrentWeaponIndex = 0;

	//когда проигрывается анимация, фикс бага быстрой смены оружия
	bool EquipAnimInProgress = false;

	//когда проигрывается анимация, фикс бага быстрой перезарядки оружия
	bool ReloadAnimInProgress = false;

	

	//спавн оружия
	void SpawnWeapons();

	//прикрепление оружия при спауне
	void AttachWeaponToSocket(AShootBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	//прикрепление оружия к руке текущего оружия
	void EquipWeapon(int32 WeaponIndex);	//параметр это индекс массива орижия который сейчас экипирован

	//проигрывает анимацию смены оружия
	void PlayAnimMontage(UAnimMontage* Animation);

	//для доступа к AnimNotify создали функцию 
	void InitAnimations();	//в ней находим нашу анимацию перезарядки AnimNotify и подписываемся на неё

	//для доступа к AnimNotify создали функцию 
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);	//биндим на делегат Notify

	//калбек для перезарядки
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	//возвращает истину когда можем стрелять
	bool CanFire() const;

	//возвращает истину когда можем сменить оружие
	bool CanEquip() const;


	bool CanReload() const;

	//калбек на наш делегат
	void OnClipEmpty(AShootBaseWeapon* AmmoWeaponEpty); //внутри добавили указатель на оружие которое необходимо перезарядить, фикс бага

	//вся логика перезарядки, если пользователь захотел или делегат OnClipEmpty
	void ChangeClip();

	/*template <typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}*/
		

};
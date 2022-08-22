// Shoot Game


#include "Weapon/ShootWeaponComponent.h"
#include "Weapon/ShootBaseWeapon.h"
#include "GameFramework/Character.h"	//указатель на персонажа 
#include "Animations/ShootEquipFinishedAnimNotify.h"	//для анимации аниммонтажа
#include "Animations/ShootReloadFinishedAnimNotify.h"	//для анимации аниммонтажа
#include "Animations/AnimUtils.h"						//для анимации аниммонтажа	

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

		UShootWeaponComponent::UShootWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	
}



	void UShootWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	/*InitAnimations();*/

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

	//создание оружия в мире
	/*SpawnWeapons();*/
	CurrentWeaponIndex = 0;

	//поиск анимации перезарядки
	InitAnimations();

	//спавн оружия всего
	SpawnWeapons();

	//прикрепление текущего оружия и задание текущему оружию номер 
	EquipWeapon(CurrentWeaponIndex);
}

	//создание оружия в мире
	void UShootWeaponComponent::SpawnWeapons()	//спавн актора и присоединение меша к нашему актору(спавн актора находится в мире игры)
{
	//if (!GetWorld()) return;	//проверка что указатель на мир не нулевой


	ACharacter* Character = Cast<ACharacter>(GetOwner());	//каст на персонажа нашего, что бы указатель получить меша
	//if (!Character) return;//если нулевой выходим из функции, если нет вызываем

	//проверка что есть персонаж и мир
	if (!Character || !GetWorld()) return;

	//массив структуры классов оружия
	//for (auto WeaponClass : WeaponClasses)
	for (auto OneWeaponData : WeaponData)	//OneWeaponData - переменная цикла
	{
		//auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass);
		auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(OneWeaponData.WeaponClass);	//OneWeaponData.WeaponClass - поле структуры
		if (!Weapon) continue;

		//спавн оружия в мире и можно указать тут же владельца
		//CurrentWeapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass); //спавн актора оружия. SpawnActor - возвращает указатель на созданный объект. поэтому можем сохранять указатель в переменной Weapon. 2 параметр указывает владельца оружия
		//if (!CurrentWeapon) return; //проверка что указатель не нулевой

		//подписывается на делегат перезарядки
		Weapon->OnClipEmpty.AddUObject(this, &UShootWeaponComponent::OnClipEmpty);

		//присваеваем для каждого оружия хозяина персонажа
		Weapon->SetOwner(Character);

		//добавляем в массив указатель, что владелец оружия персонаж через Add
		Weapons.Add(Weapon);

		//прикрепляем при спауне к ArmorySocket, а при экипировке к Equip
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);	//указатель на оружие, меш персонажа, сокет
	}
}

	//прикрепление оружия оружия к сокету
	void UShootWeaponComponent::AttachWeaponToSocket(AShootBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)//указатель на оружие, меш персонажа, сокет
{
	//проверка что они есть
	if (!Weapon || !SceneComponent) return;

	//указали тут владельца и присоединили компонент к мешу
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);//1 высчитывается трансформация присоединенного компонента к мешу по разным методам 2 симуляция физ тел
	//CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);	//присоединяем меш к нашему персонажу.1 указатель на компонент к которому ходим присоединиться 3 название сокета GetMesh - возвращает указатель на Skeleton mesh
	//CurrentWeapon->SetOwner(Character);//владельцем оружия является тот же актор, что и владелец компонента или вместо character записать GetOwner()
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

	void UShootWeaponComponent::StartFire()	//при вызове вызывается функция Fire из CurrentWeapon
	{
		//проверка что можем стрелять
		if (!CanFire()) return;	//если нулевой возвращаемся

		CurrentWeapon->StartFire();
	}

	void UShootWeaponComponent::StopFire()	//при вызове вызывается функция Fire из CurrentWeapon
	{
		if (!CurrentWeapon) return;	//если нулевой возвращаемся

		CurrentWeapon->StopFire();
	}

	//после уничтожение персонажа вызывается функция у всех компонентов персонажа
	void UShootWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
	{
		//указатель текущего оружия на нуль
		CurrentWeapon = nullptr;

		//по массиву оружия отсоединяем оружие от меша и уничтожаем его
		for (auto Weapon : Weapons)
		{
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); //KeepWorldTransform - статическая структура
			Weapon->Destroy();
		}
		
		//очищаем массив оружия 
		Weapons.Empty();

		//незабывать писать при EndPlay
		Super::EndPlay(EndPlayReason);
	}

	//прикрепление оружия к руке текущего оружия и вызываем всегда когда хотим сменить оружие, поэтому нулевым будет только при вызове на Begin Play
	void UShootWeaponComponent::EquipWeapon(int32 WeaponIndex)
	{
		//проверка, что индекс корректный
		if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
		{
			UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
			return;
		}

		//указатель на персонажа
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		
		//проверка что он есть
		if (!Character) return;

		//текущее оружие прятаем за спину, что бы при смене указателя на оружие мы знали куда девать его
		if (CurrentWeapon)
		{
			//убираем баг стрельбы при смене оружия
			CurrentWeapon->StopFire();

			//присоединяем оружие к спине
			AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
		}

		//присваиваем указателю элемент массива индекса
		CurrentWeapon = Weapons[WeaponIndex];

		//меняем анимацию перезарядки 
		// CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage; //по индексу получаем структуру из массива WeaponData и оттуда забираем анимацию перезарядки
		const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //каждый раз ищем структуру из массива WeaponData которая соответствует текущему классу оружия и оттуда забираем анимацию перезарядки. FindByPredicate - возвращает истину или ложь. Анонимная функция будет применяться к каждому элементу массива и если класс структуры является жлементом массива то вернет указатель на него
			return Data.WeaponClass == CurrentWeapon->GetClass();                                 //CurrentWeaponData - локальная переменная указателя на найденную структуру ( почитать про лямда функции )
			});

		//проверка что не нулевой укащатель
		CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

		//теперь CurrentWeapon указывает на актор который мы хотим использовать в качестве основго оружия и присоединяем его к руке персонажа
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

		//когда меняется оружие мы меняем переменную, что фикс биг на быструю смену оружия
		EquipAnimInProgress = true;

		//проигрываем анимацию смены оружия
		PlayAnimMontage(EquipAnimMontage);
	}

	//при смене оружия, изменяем индекс оружия на 1 и что бы не выходило выражение за массив индексов делим по модулю длины массива
	void UShootWeaponComponent::NextWeapon()
	{
		//проверка что EquipAnimInProgress не false, то можно дальше идти
		if (!CanEquip()) return;
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();	//Weapons.Num() - количество элементов массива

		//прикрепление оружия к руке с измененным индексом оружия
		EquipWeapon(CurrentWeaponIndex);
	}

	//проигрывание анимации перезарядки
	void UShootWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
	{
		//получаем указатель на персонажа и у него проигрываем анимацию
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character) return;

		//проверка на нуль PlayAnimMontage уже вшита в функцию

		Character->PlayAnimMontage(Animation);
	}

	//поиск нашей анимации перезарядки
	void UShootWeaponComponent::InitAnimations()
	{
		/*if (!EquipAnimMontage) return;*/
		/*auto EquipFinishedNotify = FindNotifyByClass<UShootEquipFinishedAnimNotify>(EquipAnimMontage);*/
		auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShootEquipFinishedAnimNotify>(EquipAnimMontage); //получаем указатели на EquipFinishedNotify, передаем указатель на EquipAnimMontageэ. UShootEquipFinishedAnimNotify класс интересующего notify
		
		//проверка если нашли нужный notify, то подписываем наш калбек
		if (EquipFinishedNotify)
		{
			//получаем доступ к проперти Notifies F12
			//если не нулевой достаем наш делегат OnNotified и биндим через AddUObject нашу функцию OnEquipFinished
			EquipFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnEquipFinished);
		}
		else
		{
			//не выставлен в анимации notify 
			UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}
		/*const auto NotifyEvents = EquipAnimMontage->Notifies;
		for (auto NotifyEvent : NotifyEvents)*/

		//массив оружия в котором ищем анимацию перезарядки оружия
		for (auto OneWeaponData : WeaponData)
		{
		/*	auto EquipFinishedNotify = Cast<UShootEquipFinishedAnimNotify>(NotifyEvent.Notify);	//преобразуем к нашему типу и получаем указатели на EquipFinishedNotify
			if (EquipFinishedNotify) если успешно, то этот Notifies нам и нужен
			{
				EquipFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnEquipFinished);
				break;
			}*/
			/*auto ReloadFinishedNotify = FindNotifyByClass<UShootReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
			if (!ReloadFinishedNotify) continue;*/
			//калбек на все анимации перезарядки Notify через шаблон
			auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShootReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);	//хотим найти Notify типа UShootReloadFinishedAnimNotify, анимация из текущей структуры OneWeaponData.ReloadAnimMontage
			
			//если не нашли notify для текущей анимации
			if (!ReloadFinishedNotify)
			{
				//не выставлен в анимации notify 
				UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
				checkNoEntry();
			}

			//если нашли биндим функцию для перезарядки
			ReloadFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnReloadFinished);
		}
	}

	//бинд анимации перезарядки
	void UShootWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
	{
		//получаем указатель на персонажа
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		//проверка меша персонажа с мешем, что пришел в колбеке, если меши не равны выходим
		/*if (!Character) return;*/
		if (!Character || MeshComp != Character->GetMesh()) return;

		/*if (MeshComp == Character->GetMesh())
		{
			UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
		}*/
		EquipAnimInProgress = false;	//когда проигрывается анимация фикс бага на быструю смену оружия
	}

	//проверка что можем стрелять
	bool UShootWeaponComponent::CanFire() const
	{
		/*return CurrentWeapon && !EquipAnimInProgress;*/
		return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress; //нельзя стрелять если нет оружия, играется анимация экипировки, перезарядки
	}

	//возвращает отрицание переменной EquipAnimInProgress, для фикс бага быстрой смены оружия
	bool UShootWeaponComponent::CanEquip() const
	{
		/*return !EquipAnimInProgress;*/
		return !EquipAnimInProgress && !ReloadAnimInProgress;
	}

	//перезарядка если пользователь попросит
	void UShootWeaponComponent::Reload()
	{
		//нужна ли перезарядка
		ChangeClip();
		/*if (!CanReload()) return;
		ReloadAnimInProgress = true;
		PlayAnimMontage(CurrentReloadAnimMontage);*/
	}

	//калбек для перезарядки
	void UShootWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
	{
		//получаем указатель на персонажа
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		//проверка меша персонажа с мешем, что пришел в колбеке, если меши не равны выходим
		/*if (!Character) return;*/
		if (!Character || MeshComp != Character->GetMesh()) return;

		//блокировка отключается на перезарядку
		ReloadAnimInProgress = false;
	}

	//может ли оружие быть перезаряжено
	bool UShootWeaponComponent::CanReload() const
	{
		/*return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;*/
		return CurrentWeapon             //
			&& !EquipAnimInProgress   //
			&& !ReloadAnimInProgress  //
			&& CurrentWeapon->CanReload();
	}

	//оповещение от оружия, что обойма пуста и вызывается перезарядка
	void UShootWeaponComponent::OnClipEmpty(AShootBaseWeapon* AmmoWeaponEpty)
	{
		//проверка на нулевой указатель
		if (!AmmoWeaponEpty) return;

		//проверка если указатель на текущее оружие равняется  AmmoWeaponEpty, то вкл анимацию перезарядки 
		if (CurrentWeapon == AmmoWeaponEpty)
		{
			ChangeClip();
		}
		//если текущее оружие другое
		else
		{
			//проходим по всему массиву оружия
			for (const auto Weapon : Weapons)
			{
				//если такое оружие находим
				if (Weapon == AmmoWeaponEpty)
				{
					//вызываем перезарядку данного оружия. Анимации никакие проигрываться не будут, но логика перезарядки сработает
					Weapon->ChangeClip();
				}
			}
		}
		//перенесли выше, т.к. фиксили баг ChangeClip();
	}

	//нужна ли перезарядка и её логика
	void UShootWeaponComponent::ChangeClip()
	{
		//проверка если мы не можем перезаряжаться, то выходим из функции
		if (!CanReload()) return;

		//фикс бага стрельбы при перезарядки
		CurrentWeapon->StopFire();

		//проверка что нужна перезарядка
		CurrentWeapon->ChangeClip();

		//фикс бага при перезарядки. блокировка включается
		ReloadAnimInProgress = true;

		//проигрывание анимации перезарядки
		PlayAnimMontage(CurrentReloadAnimMontage);
	}

	//получаем иконку для текущего оружия
	//функция обертка которая возвращает иконки оружия и прицела текущего оружия. Структуру передаем по ссылке в качестве параметра функции
	bool UShootWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
	{
		if (CurrentWeapon)
		{
			UIData = CurrentWeapon->GetUIData();
			return true;
		}
		return false;

		/*решение не лучшее
		//минус только в том, что надо проверять на нуль текущее оружие
		if (CurrentWeapon)
		{
			//вызываем функцию GetUIData
		}
		//иначе возвращаем пустую структуру
		*/
	}

	//получение текущего арсенала
	bool UShootWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
	{
		if (CurrentWeapon)
		{
			AmmoData = CurrentWeapon->GetAmmoData();
			return true;
		}
		return false;
	}

	//получение персонажем арсенала от Puckups
	bool UShootWeaponComponent::TryToAddAmmo(TSubclassOf<AShootBaseWeapon> WeaponType, int32 ClipsAmount)
	{
		//проходим по массиву оружия и определяем существует ли такой тип оружия
		for (const auto Weapon : Weapons)
		{
			//проверка на нуль и проверка принадлежит ли оружие к типу
			if (Weapon && Weapon->IsA(WeaponType))	//IsA(тип принимаемый) и возвращает тру если текущий объект принадлежит данному типу - UObject функция
			{

				return Weapon->TryToAddAmmo(ClipsAmount); //TryToAddAmmo находится в классе базового оружия
			}
		}

		return false;
	}

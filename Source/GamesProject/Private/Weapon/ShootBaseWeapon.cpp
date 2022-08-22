
#include "Weapon/ShootBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"	//подключили компонент меша
#include "Engine/World.h"
#include "DrawDebugHelpers.h"	//рисование debug примитивов
#include "GameFramework/Character.h"	//в нем же есть контроллер
#include "GameFramework/Controller.h" //получаем доступ к камере через класс контроллера
#include "NiagaraFunctionLibrary.h"	//создали для ниагары
#include "NiagaraComponent.h"	//создали для ниагары



DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

AShootBaseWeapon::AShootBaseWeapon()
{
	//откличили цикл тика
	PrimaryActorTick.bCanEverTick = false;	
	
	//создали компонент оружия
	WeaponMesh = CreateDefaultSubobject <USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMesh);
}


void AShootBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));
	check(WeaponMesh)	//проверка, что бы не писать в нем if, будем его использовать для получения сокета

	//присваеваем текущему арсеналу, арсенал по умолчанию
	CurrentAmmo = DefaultAmmo;
}

//начало стрельбы перенесли в дочернее оружие
void AShootBaseWeapon::StartFire()
{
	//MakeShot();	//выстрел //перенесли в дочерние 
	//GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShootBaseWeapon::MakeShot, TimeBetweenShots, true);	//таймер выстерал. 1 таймер, 2 где, 3 функцию, 4 часто выполн, 5 зацикдинность //перенесли в дочерние 
}

//конец стрельбы перенесли в дочернее оружие
void AShootBaseWeapon::StopFire()
{
	// GetWorldTimerManager().ClearTimer(ShotTimerHandle);	//скинули счётчик таймера ////перенесли в дочерние 
}

//логика выстрела перенесли в дочернее оружие
void AShootBaseWeapon::MakeShot()		//попадание трейсом
{
//			if (!GetWorld()) return;	//проверка что мир не нулевой
///*
//	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - владелец компонента, актора возвращает
//	if (!Player) return;	//проверка есть ли владелец
//
//	const auto Controller = Player->GetController<APlayerController>();	//получаем контроллер персонажа
//	if (!Controller) return;	//проверка на нулевой контроллер
//*/
//
//			FVector TraceStart, TraceEnd;
//			if (!GetTraceDate(TraceStart, TraceEnd)) return;
//	/*
//		FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
//		FRotator ViemRotation;	//направление вектора через неё можно будет получить
//
//		GetPlayerViewPoint(ViewLocation, ViemRotation); //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
//		/*
//		const auto Controller = GetPlayerController();	//присвоили Controller значение что выходит из функции
//		if (!Controller) return;	//если указатель нулевой то выходим из функции
//
//
//		FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
//		FRotator ViemRotation;	//направление вектора через неё можно будет получить
//
//		Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
//		*/		/* продолжение первого коммента
//
//		//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//получаем трансформацию сокета через создание локальной переменоой. Получить Сокет - GetSocketTransform(имя сокета, система координат)
//
//		const FVector TraceStart = ViewLocation;//переменные траектории выстрела
//		//const FVector TraceStart = SocketTransform.GetLocation();//переменные траектории выстрела. переписали что б выстрел по камере был а не по дулу
//		const FVector ShootDirector = ViemRotation.Vector();//вектор направления Vector() - работает так же как GetRotation().GetForwardVector(). Направление кватерниона
//		//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//вектор направления выстрел GetForwardVector - возвращает вектор поворота по оси Х
//		const FVector TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector единичный векто поэтому домножили. Конечная точка линии вектора
//
//		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию
//	*/
//
//			FHitResult HitResult;
//			MakeHit(HitResult, TraceStart, TraceEnd);
//	/*
//		FCollisionQueryParams CollisionParams;
//		CollisionParams.AddIgnoredActor(GetOwner());	//защищает актор от попадания
//
//
//		FHitResult HitResult;	//возвращает булеан true или false
//		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//получает информацию от нашего отрезка с первым пересекшимся объектом. ECollisionChannel - с помощью него фильтрация объектов на сцене, работать будет только с теми кто блокирует наш канал, другие игнорируются
//	*/
//			if (HitResult.bBlockingHit)	//проверка было ли пересечении, функция в FHitResult называется bBlockingHit. Если во что то попали true
//	{
//		//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры
//
//				//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры
//
//		MakeDamage(HitResult);	//получение урона при блоке трейсом персонажа
//		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры
//		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f, 0, 3.0f);	//рисует сферу в конце отрезка. HitResult.ImpactPoint - точка пересечения
//
//		UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s"), *HitResult.BoneName.ToString());	//показывает часть тела в которую попали
//	}
//			else
//	{		//если никуда не попали
//		/*
//		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
//		*/
//		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
//	}

}

//проверка что нашим персонажем управляем мы
APlayerController* AShootBaseWeapon::GetPlayerController() const //создали функцию которая будет возвращать указатель на PlayerController
{
	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - владелец компонента, актора возвращает
	if (!Player) return nullptr;	//проверка есть ли игрок, если нету возвращает нульпоинтер, т.е. котроллера не существует, а если существует то

	return Player->GetController<APlayerController>();	//возвращает указатель контроллера
}

//возвращает позицию и ориентацию в пространстве камеры
bool AShootBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViemRotation) const
{
	const auto Controller = GetPlayerController();	//присвоили Controller значение что выходит из функции
	if (!Controller) return false;	//если указатель нулевой то выходим вывод ложь

	Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
	return true;
}

//получение позиции сокета
FVector AShootBaseWeapon::GetMuzzleWorldlocation() const	//возвращает положение сокета
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);	//получаем позицию сокета без использования GetSocketTransform
}

//получение векторов начала и конца логической линии
bool AShootBaseWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const //функции для получения векторов начала и конца
{
	FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
	FRotator ViemRotation;	//направление вектора через неё можно будет получить

	if(!GetPlayerViewPoint(ViewLocation, ViemRotation)) return false; //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
	
	TraceStart = ViewLocation;//переменные траектории выстрела
	//const FVector TraceStart = SocketTransform.GetLocation();//переменные траектории выстрела. переписали что б выстрел по камере был а не по дулу

	//const auto HalfRad = FMath::DegreesToRadians(BulletSpread);	//переводит в градусы
	//FMath::VRandCone убрав её ниже будет точное направление вектора, а так будет конусообразное тип с отклонениями и передает только половину угла
	const FVector ShootDirector = ViemRotation.Vector();//вектор направления Vector() - работает так же как GetRotation().GetForwardVector(). Направление кватерниона

	//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//вектор направления выстрел GetForwardVector - возвращает вектор поворота по оси Х
	TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector единичный векто поэтому домножили. Конечная точка линии вектора
	return true;
}

//пересечение с логической линией
void AShootBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd)	//делает trace
{
	if (!GetWorld()) return;	//проверка потому что её могут вызвать из наследников класса, т.к. находится в секции protected, в Private проверку не надо было бы делать
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());	//защищает актор от попадания

	//по умолчанию физический материал не передается в HitResult, поэтому указываем его явно. Не передается из-за оптимизации
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//получает информацию от нашего отрезка с первым пересекшимся объектом. ECollisionChannel - с помощью него фильтрация объектов на сцене, работать будет только с теми кто блокирует наш канал, другие игнорируются
}

/*
//нанесение урона
void AShootBaseWeapon::MakeDamage(const FHitResult& HitResult) 
{
	//возвращает указатель на актора при успешном прохождении с колизией
	const auto DamageActor = HitResult.GetActor();		//возвращает указатель на актора при успешном прохождении с колизией
	if (!DamageActor) return;	//если нет дамага

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);	//получение урона

}
*/

//вызывается каждый раз после выстрела, уменьшает количество патронов
void AShootBaseWeapon::DecreaseAmmo()
{
	//проверка что пуль нуль
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}

	//уменьшаем количество патронов
	CurrentAmmo.Bullets--;
	
	//лог стрельбы
	//LogAmmo();

	//проверка что нужна перезарядка
	if (IsClipEmpty() && !IsAmmoEmpty())	//обойма должна быть пуста и наш арсенал не пустой
	{
		//смена обоймы
		/*ChangeClip();*/
		//фикс бага стрельбы после перезарядки
		StopFire();
		OnClipEmpty.Broadcast(this/*фикс бага. указатель на текущий истанс*/);	//оповещение если нужна перезарядка
	}
}

//возвращает истину когда арсенал полностью пустой(патронов и обойм нуль)
bool AShootBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();	//арсенал должен быть конечен и количество обойм = 0 и текущая обойма пустая
}

//возвращает истину когда текущая обойма пустая
bool AShootBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

//логика замены текущей обоймы на новую
void AShootBaseWeapon::ChangeClip()
{
	//устанавливает количество текущий патронов на по умолчанию
	//CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	
	//проверка что не бесконечный боезапас
	if (!CurrentAmmo.Infinite)
	{
		//проверка что обойма пуста
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
			return;
		}

		//уменьшаем количество обойм
		CurrentAmmo.Clips--;
	}

	//устанавливает количество текущий патронов на по умолчанию
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("------ Change Clip ------"));
}

void AShootBaseWeapon::LogAmmo()
{
	//строка в консоле
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool AShootBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets&& CurrentAmmo.Clips > 0;
}

//текущий арсенал полон
bool AShootBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips &&
		CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

//логика добавления магазинов и патронов
bool AShootBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	//условие при котором не можем добавить
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0.0f) return false;	//бесконечный запас или полный арсенал или магазинов больше нуля

	//условие при котором не можем добавить, если патров и магазинов 0
	if (IsAmmoEmpty())
	{
		//полностью заполняем боезапас магазинов
		CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1); //т.к. дефолтное значение может быть больше, чем может быть у нас боезпас, устанавливаем границу. ограничиваем магазин ClipsAmount, магазином по умолчанию DefaultAmmo.Clips. 0 - нижняя граница. Добавили + 1, потому что вызываем перезарядку(тип текущая обойма и те что есть)
		
		//вызываем делегат перезарядки
		OnClipEmpty.Broadcast(this/*фикс бага, указатель на текущий истанс*/);
	}
	//если текущее значение обойм меньше бойм по умолчанию
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
	//создали локальную переменную т.к.нужно проверить сможем ли заполнить и магазин и патроны
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;//NextClipsAmount - текущее значение магазинов и значение магазинов переданную в функцию
		
	//проверка если по умолчанию и NextClipsAmount больше нуля, то можем присвоить магазину, значение переменной  NextClipsAmount
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)	//количество магазинов не перевалит за максимум
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		//проверка если по умолчанию и NextClipsAmount меньше нуля, то есть 1 доп обойма с которой, можем пополнить боезапас
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	//если текущая обойма равня по умолчанию. попадаем сюда т.к. больше вариантов нет
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets; //боймы полны, но патроны нет, 
	}

	//взяли Puckup
	return true;
}

//возвращает указатель на ниагару компонент
UNiagaraComponent* AShootBaseWeapon::SpawnMuzzleFX()
{
	//SpawnSystemAttached - niagara будет трансформироваться взависимости от родителя к которому присоединились. 
	return UNiagaraFunctionLibrary::SpawnSystemAttached
	(
		MuzzleFX,// 1. указатель на ниагару систему
		WeaponMesh, //2 компонент к которому приатачим.
		MuzzleSocketName,// 3.позиция 
		FVector::ZeroVector,	//4,5 параметры трансформации
		FRotator::ZeroRotator,	//
		EAttachLocation::SnapToTarget,//присоединение к оружию
		true	//автоуничтожение
	);
}
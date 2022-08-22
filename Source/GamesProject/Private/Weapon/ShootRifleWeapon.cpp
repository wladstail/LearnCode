// Shoot Game


#include "Weapon/ShootRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"	//рисование debug примитивов
#include "Weapon/Components/ShootWeaponFXComponent.h"	//для ниагары 


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)


//конструктор нашей винтовки
AShootRifleWeapon::AShootRifleWeapon()
{	
	//компонент ниагары создаем для винтовки
	WeaponFXComponent = CreateDefaultSubobject<UShootWeaponFXComponent>("WeaponFXComponent");
}

//добавили BeginPlay, что бы проверить что эффект ниагары для винтовки выставлен
void AShootRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	//проверка что существует ниагары эффект
	check(WeaponFXComponent);
}

//начало выстрела
void AShootRifleWeapon::StartFire()
{

		//MakeShot();	//выстрел
		//таймер запускается и вызывается функция попадания
		GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShootRifleWeapon::MakeShot, TimeBetweenShots, true);	//таймер выстерал. 1 таймер, 2 где, 3 функцию, 4 частота выполнения, 5 зацикдинность
		MakeShot();
}

//конец выстрела
void AShootRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);	//скинули счётчик таймера
}

//попадание логической линией
void AShootRifleWeapon::MakeShot()		//попадание трейсом
{
	//if (!GetWorld()) return;	//проверка что мир не нулевой
	if (!GetWorld() || IsAmmoEmpty()) //проверка что мир не нулевой или арсенал пустой
	{
		//фикс бага с таймером после конца арсенала
		StopFire();
		return;
	}
/*
	const auto Player = Cast<ACharacter>(GetOwner());	//GetOwner - владелец компонента, актора возвращает
	if (!Player) return;	//проверка есть ли владелец

	const auto Controller = Player->GetController<APlayerController>();	//получаем контроллер персонажа
	if (!Controller) return;	//проверка на нулевой контроллер
*/

	FVector TraceStart, TraceEnd;
	/*if (!GetTraceDate(TraceStart, TraceEnd)) return;*/
	if (!GetTraceDate(TraceStart, TraceEnd))
	{
		//фикс бага с таймером после конца арсенала
		StopFire();
		return;
	}
	/*
		FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
		FRotator ViemRotation;	//направление вектора через неё можно будет получить

		GetPlayerViewPoint(ViewLocation, ViemRotation); //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
		/*
		const auto Controller = GetPlayerController();	//присвоили Controller значение что выходит из функции
		if (!Controller) return;	//если указатель нулевой то выходим из функции


		FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
		FRotator ViemRotation;	//направление вектора через неё можно будет получить

		Controller->GetPlayerViewPoint(ViewLocation, ViemRotation); //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры
		*/		/* продолжение первого коммента

		//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//получаем трансформацию сокета через создание локальной переменоой. Получить Сокет - GetSocketTransform(имя сокета, система координат)

		const FVector TraceStart = ViewLocation;//переменные траектории выстрела
		//const FVector TraceStart = SocketTransform.GetLocation();//переменные траектории выстрела. переписали что б выстрел по камере был а не по дулу
		const FVector ShootDirector = ViemRotation.Vector();//вектор направления Vector() - работает так же как GetRotation().GetForwardVector(). Направление кватерниона
		//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//вектор направления выстрел GetForwardVector - возвращает вектор поворота по оси Х
		const FVector TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector единичный векто поэтому домножили. Конечная точка линии вектора

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию
	*/

	//пересечение с объектом нашей линии
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	/*
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());	//защищает актор от попадания


		FHitResult HitResult;	//возвращает булеан true или false
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);	//получает информацию от нашего отрезка с первым пересекшимся объектом. ECollisionChannel - с помощью него фильтрация объектов на сцене, работать будет только с теми кто блокирует наш канал, другие игнорируются
	*/

	//проверка, если попали получаем урон, рисуем линию от нашего сокета и сферу пересечения, иначе линия сквозная
	if (HitResult.bBlockingHit)	//проверка было ли пересечении, функция в FHitResult называется bBlockingHit. Если во что то попали true
	{
		//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры

				//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры

		MakeDamage(HitResult);	//получение урона при блоке трейсом персонажа
		/*сделали из - за ниагры  DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);	//рисуем линию, но начало уже из координат нашего сокета а не камеры
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f, 0, 3.0f);	//рисует сферу в конце отрезка. HitResult.ImpactPoint - точка пересечения
		*/

		//включение ниагры при пересечении с объектом
		WeaponFXComponent->PlayImpactFX(HitResult);

		//UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s"), *HitResult.BoneName.ToString());	//показывает часть тела в которую попали
	}
	else
	{		//если никуда не попали
		/*
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
		*/
		DrawDebugLine(GetWorld(), GetMuzzleWorldlocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
	}

	//уменьшаем количество патронов при выстреле
	DecreaseAmmo();
}

//получает вектор логической линии, начало и конец
bool AShootRifleWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const //функции для получения векторов начала и конца
{
	FVector ViewLocation;	//присвоили тип, заглушки на наши параметры ниже. положение камеры
	FRotator ViemRotation;	//направление вектора через неё можно будет получить

	if (!GetPlayerViewPoint(ViewLocation, ViemRotation)) return false; //возвращает позицию и ориентацию в пространсве камеры у неё есть доступ к PlayerCameraManager, который глобальный класс и из него можно получить указатель на камеру персонажа. 1 положение камеры 2 вращение камеры

	//начало логической линии
	TraceStart = ViewLocation;//переменные траектории выстрела
	//const FVector TraceStart = SocketTransform.GetLocation();//переменные траектории выстрела. переписали что б выстрел по камере был а не по дулу

	//разброс оружия
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);	//переводит в градусы
	//FMath::VRandCone убрав её ниже будет точное направление вектора, а так будет конусообразное тип с отклонениями и передает только половину угла

	//получение направления выстрела
	const FVector ShootDirector = FMath::VRandCone(ViemRotation.Vector(), HalfRad);//вектор направления Vector() - работает так же как GetRotation().GetForwardVector(). Направление кватерниона

	//конец логической линии
	//const FVector ShootDirector = SocketTransform.GetRotation().GetForwardVector();//вектор направления выстрел GetForwardVector - возвращает вектор поворота по оси Х
	TraceEnd = TraceStart + ShootDirector * TraceMaxDistance;	//ShootDirector единичный вектор поэтому домножили. Конечная точка линии вектора
	return true;
}

void AShootRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

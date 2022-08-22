


#include "Player/ShootBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"					//инпукт вкл
#include "GameFramework/SpringArmComponent.h"			//вращение камеры вкл
#include "Components/Shoot_CharacterMovementComponent.h" //компонент увел скорости
#include "Components/ShootHealtsComponent.h"			//компонент хп
#include "Components/TextRenderComponent.h"
#include "Weapon/ShootWeaponComponent.h"		//оружие компонент
#include "Components/CapsuleComponent.h"	//коллизия
#include "GameFramework/Controller.h"		//контроллер спектр


//#include "Weapon/ShootBaseWeapon.h"			//оружие перенесли в компонент оружия

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


//Создали компоненты персонажа
AShootBaseCharacter::AShootBaseCharacter(const FObjectInitializer& ObjInit):Super(ObjInit.SetDefaultSubobjectClass<UShoot_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) //SetDefaultSubobjectClass<класс объекта который хотим заменить>
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject <USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());	  					    //присоединили к корневому
	SpringArmComponent->bUsePawnControlRotation = true;								//вкл вращение через контроллер

		//для изменения разворота текста хп
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);				//сдвиг компонента на координату


	CameraComponent = CreateDefaultSubobject <UCameraComponent>("CameraComponent"); //по умолчанию создали компонент
	CameraComponent->SetupAttachment(SpringArmComponent);							 //GetRootComponent возвращает указатель на корней компонент актора
	//в деталях галочку поставить в Use Pawn Control Rotation что бы поворот был камеры у персонажа



	HealtsComponent = CreateDefaultSubobject <UShootHealtsComponent>("HealtsComponent");	//комопнент хп, он логический присоединять не обязательно

	HealtsTextComponent = CreateDefaultSubobject <UTextRenderComponent>("HealtsTextComponent");
	HealtsTextComponent->SetupAttachment(GetRootComponent());

	//вкл текст только над чужими игроками
	HealtsTextComponent->SetOwnerNoSee(true);	//вкл текст над персонажем, но только не нашем


	WeaponComponent = CreateDefaultSubobject <UShootWeaponComponent>("WeaponComponent");		//компонент оружия

}

//Хп персонажа и взаимодействия от него
void AShootBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//генерирует только в дебаг средах 
	check(HealtsComponent);		//если нулевой компонент сгенерирует ошибку
	check(HealtsTextComponent);
	check(GetCharacterMovement());
	check(GetMesh());	//проверка что есть меш у персонажа

	//срабатывание вначале игры
	OnHealtsChanged(HealtsComponent->GetHealts(), 0.0f); //получить в начале игры 100 хп и указали это, для фикса что в начале будет 0 хп. второй аргумент добавили из-за фикса красного экрана в виджетах

	//делегат срабатываюий при смерти персонажа
	HealtsComponent->OnDeath.AddUObject(this, &AShootBaseCharacter::OnDeath);	//в компоненте находим делегат смерти подписываем персонажа к нему.т.к. он будет ток в С++ то функцию используем AddObject
	
	//делегат меняющий текст при изменении здоровья
	HealtsComponent->OnHeatlsChanged.AddUObject(this, &AShootBaseCharacter::OnHealtsChanged); //меняет текст хп только когда изменились они, а не каждый тик
	//OnTakeAnyDamage.AddDynamic(this, &AShootBaseCharacter::OnTakeAnyDamageHandle);	//забиндили функцию дамага

	//для получения урона падения с высоты
	LandedDelegate.AddDynamic(this, &AShootBaseCharacter::OnGroundLanded);//1 мы 2 функция в котором будем обрабатывать событие. Сигнатуры смотреть в Landed


	
	//SpawnWeapons();		//спавн оружия перенсли в компонент 
}

//Текст хп персонажа срабатывает при вызове делегата при изменении хп
void AShootBaseCharacter::OnHealtsChanged(float Healts, float HealthDelta)
{
	HealtsTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Healts)));	//вывод текста хп
}

void AShootBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const auto Healts = HealtsComponent->GetHealts();	//каждый тик получает наше хп

	// HealtsTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Healts)));	//вывод текста хп




	//TakeDamage(0.1f, FDamageEvent(), Controller, this);	//урон, какой дамаг по типу, указатель на контроллер нанесшего, указатель что наносит урон
	

}

//движение персонажа, а так же стрельба
void AShootBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShootBaseCharacter::MoveForward);				//забиндили клавиши
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootBaseCharacter::RightForward);

	PlayerInputComponent->BindAxis("LookUp", this, &AShootBaseCharacter::AddControllerPitchInput);						//забиндили мышь вместо this, &AShootBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &AShootBaseCharacter::AddControllerYawInput);				//вместо  this, & AShootBaseCharacter::TurnAround);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShootBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShootBaseCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShootBaseCharacter::StopRun);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShootWeaponComponent::StartFire);	//вызываем уже не персонажа а компонент
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShootWeaponComponent::StopFire);	//вызываем уже не персонажа а компонент

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShootWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShootWeaponComponent::Reload);
}

//движения векторов персонажа
void AShootBaseCharacter::MoveForward(float Amount)
{
	RunCharacter = Amount > 0.0f;							//проверка бежит ли персонаж
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);		//GetActorForwardVector,GetActorRightVector,GetActorUpVector это как x,y,z

}	   
void AShootBaseCharacter::RightForward(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);

}
void AShootBaseCharacter::Run()
{
	RunKeyActiv = true;


}
void AShootBaseCharacter::StopRun()
{
	RunKeyActiv = false;
}
bool AShootBaseCharacter::isRuning() const
{
	return RunKeyActiv && RunCharacter && !GetVelocity().IsZero();	//возвращает значение, если все True то True, иначе False
}

//Анимация персонажа при поворотах
float AShootBaseCharacter::GetMovementDirection() const	//для анимации при поворотах персонажа
{
	if (GetVelocity().IsZero()) return 0.0f; //если она возвращает True, мы возвращаем нуль
	const auto VelocityNormal = GetVelocity().GetSafeNormal();	//auto атоматическое определение типа. GetSafeNormal - возвращает нормаль
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));		//угол между нашим форвард вектором и вектором скорости
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);			//векторное произведение векторов. Ортогональный вектор	
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);					//Сделали потому что при движении назад вектор тоже будет нуль как и вперед
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);		// FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}
/*
void AShootBaseCharacter::LookUp(float Amount)
{

	AddControllerPitchInput(Amount);			//т.к. ничего не возвращает и принимает только один параметр флоат, делаем на прямую

}
void AShootBaseCharacter::TurnAround(float Amount)
{

	AddControllerYawInput(Amount);					//Вращение контроллера

}
*/

//Смерть персонажа и её анимация и убрать коллизию после смерти
void AShootBaseCharacter::OnDeath() 
{

	UE_LOG(BaseCharacterLog, Display, TEXT("Player %sis Dead"), *GetName());

	//проигрывание анимации из кода( в блупринтах задать анимацию )
	//заменили на RagDool PlayAnimMontage(DeathAnimMontage);	//вызываем функцию монтажа смерти

	//выключение передвижения
	GetCharacterMovement()->DisableMovement();//отключает все движения

	//уничтожение персонажа с таймером
	SetLifeSpan(LifeSpan); // уничтожение персонажа через 5 секунд
						   
	//отключает управление после смерти
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);	//контроль после смерти у спектра
	}

	//что бы мы могли проходить через коллизию
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	//получили колизию, сделали её игнорируемой

	//фикс бага стрелять после смерти( таймер обнуляем этим ) 
	WeaponComponent->StopFire();

	//включает для коллизии меша симуляцию физики
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//получаем указатель на скелетал меш персонажа->влючает коллизии компонента

	//включает симуляцию физики на меш
	GetMesh()->SetSimulatePhysics(true); //получаем указатель на скелетал меш персонаж->вкл симуляцию физики
}

//Урон персонажа при высоте
void AShootBaseCharacter::OnGroundLanded(const FHitResult& Hit)	//урон с высоты
{
	//скорость падения
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;//принимает FallVelosity вектор компонента скорости.можно было получить и через GetVelocity().Z 
	UE_LOG(BaseCharacterLog, Display, TEXT("on Landed: %f"), FallVelocityZ);	//выводит значение скорости приземления

	//выходим из функции если урон меньше заданного интервала
	if (FallVelocityZ < LandedDamageVelocity.X) return; //если значение max скорости меньше интервала то выходим. При падении вектор скорости противоположен поэтому *(-1)

	//нанесение урона
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);//1 входной отрезок 2 выходной отрезок 3 скорость с которой упали. Получим число из отрезка в процентном соотношении на отрезке LandedDamageVelocity
	UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);	//передаем урон
};

/*перенесли в оружие
void AShootBaseCharacter::SpawnWeapons()	//спавн актора и присоединение меша к нашему актору(спавн актора находится в мире игры)
{
	if (!GetWorld()) return;	//проверка что указатель на мир не нулевой

	const auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass);	//спавн актора оружия. SpawnActor - возвращает указатель на созданный объект. поэтому можем сохранять указатель в переменной Weapon
	if (Weapon)	//проверка что объект создался
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);//1 высчитывается трансформация присоединенного компонента к мешу по разным методам 2 симуляция физ тел
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");	//присоединяем меш к нашему персонажу.1 указатель на компонент к которому ходим присоединиться 3 название сокета GetMesh - возвращает указатель на Skeleton mesh
	}
}
*/
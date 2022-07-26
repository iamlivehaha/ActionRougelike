// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* AI_Controller = OwnerComp.GetAIOwner();
			if (ensure(AI_Controller))
			{
				APawn* AIPawn = AI_Controller->GetPawn();
				if (ensure(AIPawn))
				{
					//optimization FVector::DistSquared
					float TargetDistance = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
					bool bWithinRange = TargetDistance < SenseRange;

					bool bLoS = false;
					if (bWithinRange)
					{
						bLoS = AI_Controller->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange&& bLoS);
				}
			}
		}
	}
}

#pragma once

#include "CrowdState.generated.h"

UENUM(BlueprintType)
enum class ECrowdState : uint8 {
	StaticForm,
	FollowingPlayer,
	MovingToTarget,
	SlowingDown,
	ConsumingPlayer,
	Reforming,
	AbsorbDisintegratedActor
};

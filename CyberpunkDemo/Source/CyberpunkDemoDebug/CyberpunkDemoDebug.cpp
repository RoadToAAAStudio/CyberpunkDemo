#include "CyberpunkDemoDebug.h"
#include "Modules/ModuleManager.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategory_BasicEnemy.h"
#endif // WITH_GAMEPLAY_DEBUGGER
 
IMPLEMENT_MODULE(FCyberpunkDemoDebugModule, CyberpunkDemoDebug);
 
void FCyberpunkDemoDebugModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("CyberpunkDemoDebug module has started!"));
#if WITH_GAMEPLAY_DEBUGGER
    IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
    GameplayDebuggerModule.RegisterCategory("BasicEnemy", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_BasicEnemy::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);
    GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FCyberpunkDemoDebugModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("CyberpunkDemoDebug module has shut down!"));
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("BasicEnemy");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif // WITH_GAMEPLAY_DEBUGGER
}

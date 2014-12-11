# UE4_TurnBased

Things (mostly base clases for blueprints) that might come in handy for making a turn-based game in Unreal Engine 4

## Install
 * Place the content under `<YourProject>/Source`
 * Include `UE4_TurnBased` in the call to `OutExtraModuleNames.AddRange` in `<YourProject>.Target.cs`
 * Include `UE4_TurnBased` in the call to `OutExtraModuleNames.AddRange` in `<YourProject>Editor.Target.cs`
 * Add the following to `Config/DefaultEngine.ini`:
```
[UnrealEd.EditorEngine]
+EditPackages=UE4_TurnBased

[Launch]
Module=UE4_TurnBased

[/Script/Engine.UObjectPackages]
+NativePackages=UE4_TurnBased
```
  * Add the follwing entry to `Modules` in `<YourProject>.uproject`:
```
                {
                        "Name": "UE4_TurnBased",
                        "Type": "Runtime",
                        "LoadingPhase": "Default"
                }
```

 * Right click on `<YourProject>.uproject` and select `Generate Visual Studio project files`


Documentation for modules: https://docs.unrealengine.com/latest/INT/Programming/Modules/Gameplay/index.html

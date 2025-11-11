
# 虚拟实验室
## 项目介绍
这个项目是作为我的毕业设计和当作一次UE C++练手。游戏内容如项目名称而言，就是一个模拟实验操作的游戏，我打算在游戏中设计一个实验，让玩家进行实验步骤操作。如果后续还有时间就再加多一个实验关卡。

## 代码功能实现
### **VirtualLabPlayerController**
我在这里实现的功能有：

 **1. 绑定增强输入**
 
 - VirtualLabPlayerController.h
 ```
 protected:
	/*输入映射上下文*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
```
头文件中设置传入上下文引用

- VirtualLabPlayerController,cpp
```
if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
{
	if(InputMapping.IsValid()||InputMapping.LoadSynchronous())
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	}
}
```
先获取本地玩家的增强输入子系统，再根据传入的上下文引用来绑定到玩家增强输入子系统中。

### PlayerCharacter
在这里我实现的功能有：
**1. 初始化第一人称相机**

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTM0Nzc0MTc2MSwyODk0MTE1Nl19
-->
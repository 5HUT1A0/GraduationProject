
# 虚拟实验室
## 项目介绍
这个项目是作为我的毕业设计和当作一次UE C++练手。游戏内容如项目名称而言，就是一个模拟实验操作的游戏，我打算在游戏中设计一个实验，让玩家进行实验步骤操作。如果后续还有时间就再加多一个实验关卡。

## 代码功能实现
### **VirtualLabPlayerController**
我在这里实现的功能有

 **1. 绑定增强输入**
 
 - VirtualLabPlayerController.h
 ```
 protected:
	/*输入映射上下文*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
```


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTMyMzc3NjEzMSwyODk0MTE1Nl19
-->
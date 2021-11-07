#pragma once

#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"
#include "IInputDevice.h"
#include "DIDeviceInfo.h"
#include "DISettings.h"

class DIRECTINPUT_API FDIEngineDevice : public IInputDevice // , IWindowsMessageHandler
{
public:

	FDIEngineDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	virtual ~FDIEngineDevice();

	/** Tick the interface (e.g. check for new controllers) */
	virtual void Tick( float DeltaTime ) override;

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents() override;

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler( const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler ) override;

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec( UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar ) override;

	/**
	 * IForceFeedbackSystem pass through functions
	 */
	virtual void SetChannelValue (int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues (int32 ControllerId, const FForceFeedbackValues &values) override;
	virtual bool SupportsForceFeedback(int32 ControllerId) override;

	/**
	 * Pass though functions for light color
	 */
	virtual void SetLightColor(int32 ControllerId, FColor Color) override;
	virtual void ResetLightColor(int32 ControllerId) override;

	/**
	* Sets a property for a given controller id.
	* Will be ignored for devices which don't support the property.
	*
	* @param ControllerId the id of the controller whose property is to be applied
	* @param Property Base class pointer to property that will be applied
	*/
	virtual void SetDeviceProperty(int32 ControllerId, const FInputDeviceProperty* Property) override;

	/** If this device supports a haptic interface, implement this, and inherit the IHapticDevice interface */
	virtual class IHapticDevice* GetHapticDevice() override;

	virtual bool IsGamepadAttached() const override;

	// CUSTOM

	void RequestUpdateConfiguration();
	void SetForceFeedback(const FString& DeviceTag, float Constant, float Damper);

protected:

	void InitConfiguration();
	void UpdateConfiguration();
	void SetupBindings(struct DIDeviceImpl* Dev);
	void RemoveBindings(struct DIDeviceImpl* Dev);
	void Rebind(class UInputSettings* Settings, const FKey& Key, const FName& NewTarget, bool IsAxis);
	struct DIDeviceImpl* GetDevice(const FString& Uid);

	void* EngineWindow = nullptr;
	TSharedRef< FGenericApplicationMessageHandler > MessageHandler;

	TArray<FDIDeviceInfo> EnumeratedDevices;
	TArray<struct DIDeviceImpl*> ActiveDevices;
	TMap<FName, FName> InputMapper;

	FDIDeviceConfiguration DefaultDriving;
	bool UpdateConfigurationFlag = false;
};

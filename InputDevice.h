#pragma once


class InputDevice
{
public:
								InputDevice();

	void						setDeviceID(int ID);
	void						setIsKeyboard(bool flag);

	int							getDeviceID();
	bool						getIsKeyboard();

private:
	int							mDeviceID;
	bool						isKeyboard;

};


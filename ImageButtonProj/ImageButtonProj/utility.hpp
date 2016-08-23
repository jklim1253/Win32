#pragma once

template<typename T>
class Singleton {
protected :
	Singleton() {}
	virtual ~Singleton() {}
public :
	static T& GetReference() {
		static T instance;
		return instance;
	}
};
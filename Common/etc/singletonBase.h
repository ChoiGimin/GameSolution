#pragma once

namespace common
{
	template <typename T>
	class singletonBase
	{
	protected:
		static T* singleton; //싱글톤 인스턴스 선언
		singletonBase(void) {};
		~singletonBase(void) {};

	public:
		static T* getSingleton(void); //인스턴스 얻는다
		void releaseSingleton(void); //인스턴스 해제해 준다
		//(선언했으면 반드시 해제..)
	};

	//싱글톤 초기화
	template <typename T>
	T* singletonBase<T>::singleton = 0;

	//싱글톤 값 가져오기
	template <typename T>
	T* singletonBase<T>::getSingleton(void)
	{
		if (!singleton) singleton = new T;
		return singleton;
	}

	//싱글톤 메모리 해제
	template <typename T>
	void singletonBase<T>::releaseSingleton(void)
	{
		if (singleton)
		{
			delete singleton;
			singleton = 0;
		}
	}
}
